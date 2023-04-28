#include "nmi.h"

namespace nmi {
_KAFFINITY_EX *nmi_affinity{0};
void *nmi_callback_handle{0};
volatile LONG nmi_fired{0};
bool nmi_stop{0};
HANDLE nmi_t_handle{0};
pstack_trace nmi_stack_traces{0};
bool nmi_initialized{0};

BOOLEAN nmi_callback(PVOID context, BOOLEAN handled)
{
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(handled);

    void **stack_trace = reinterpret_cast<void **>(
        ExAllocatePoolZero(NonPagedPool, 0x1000, 'thrd'));

    if (stack_trace) {
        __try {
            USHORT capturedFrames =
                RtlCaptureStackBackTrace(0, 0x1000 / 8, stack_trace, NULL);

            ULONG i = KeGetCurrentProcessorNumber();

            if (nmi_stack_traces && nmi_stack_traces[i].stack_trace == 0) {
                nmi_stack_traces[i].stack_trace = stack_trace;
                nmi_stack_traces[i].captured_frames = capturedFrames;
            }
        }
        __except (1) {
            KdLog("NmiCallback exception on processor %d\n",
                  KeGetCurrentProcessorNumber());
        }
    }

    InterlockedDecrement(&nmi_fired);
    return TRUE;
}

void send_nmi(int processor)
{
    KeInitializeAffinityEx(nmi_affinity);
    KeAddProcessorAffinityEx(nmi_affinity, processor);

    HalSendNMI(nmi_affinity);
}

void process_nmi(pnmi_info nmii)
{
    ULONG n_processors = KeQueryActiveProcessorCountEx(0);

    nmi_fired = n_processors;
    for (ULONG i = 0; i < n_processors; i++) {
        send_nmi(i);
        utils::sleep(100);
    }

    LARGE_INTEGER start_time{};
    LARGE_INTEGER end_time{};
    ULONG start_seconds, end_seconds;
    KeQuerySystemTime(&start_time);

    for (;;) {
        if (nmi_fired == 0)
            break;

        KeQuerySystemTime(&end_time);

        RtlTimeToSecondsSince1970(&start_time, &start_seconds);
        RtlTimeToSecondsSince1970(&end_time, &end_seconds);

        // timeout after 10 seconds
        if (end_seconds - start_seconds > 10)
            break;

        utils::sleep(1);
    }

    nmii->n_timeouts = nmi_fired;

    if (!nmi_stack_traces) {
        nmii->freed_stack_traces_allocation = true;
        return;
    }

    // process stack frame of each processor
    for (ULONG i = 0; i < n_processors; i++) {

        void **stack_trace = nmi_stack_traces[i].stack_trace;
        USHORT captured_frames = nmi_stack_traces[i].captured_frames;

        nmii->stack_infos[i].n_captured_frames = captured_frames;

        if (stack_trace == 0 || !MmIsAddressValid(stack_trace)) {
            nmii->stack_infos[i].invalid_stack_trace_pointer = true;
            continue;
        }

        int counter = 0;
        for (int j = 0; j < captured_frames; j++) {
            void *frame_value = stack_trace[j];

            if (reinterpret_cast<UINT64>(frame_value) < 0xFFFF000000000000)
                continue;

            if (!utils::in_valid_module(
                    reinterpret_cast<UINT64>(frame_value))) {
                nmii->stack_infos[i].invalid_frames[counter] = frame_value;
                counter++;
            }
        }

        nmii->stack_infos[i].n_invalid_frames = counter;

        ExFreePool(stack_trace);
        nmi_stack_traces[i].stack_trace = 0;
        nmi_stack_traces[i].captured_frames = 0;
    }
}

nmi_status setup_nmi()
{
    // Allocate space for a stack_trace for each logical core
    nmi_stack_traces = reinterpret_cast<pstack_trace>(ExAllocatePoolZero(
        NonPagedPool, sizeof(stack_trace) * KeQueryActiveProcessorCountEx(0),
        ' imn'));
    if (!nmi_stack_traces)
        return FAIL_ALLOCATION_STACK;

    nmi_affinity = reinterpret_cast<_KAFFINITY_EX *>(
        ExAllocatePoolZero(NonPagedPool, sizeof(_KAFFINITY_EX), ' imn'));
    if (!nmi_affinity) {
        ExFreePool(nmi_stack_traces);
        return FAIL_ALLOCATION_AFFINITY;
    }

    nmi_callback_handle = KeRegisterNmiCallback(nmi_callback, nullptr);
    if (!nmi_callback_handle) {
        ExFreePool(nmi_stack_traces);
        ExFreePool(nmi_affinity);
        return FAILED_REGISTER;
    }

    nmi_initialized = true;
    return SUCCESS;
}

nmi_status clean()
{
    nmi_stop = true;

    //
    // Wait for nmi system thread to stop
    //
    if (nmi_t_handle) {
        _ETHREAD *thread{};
        ObReferenceObjectByHandle(nmi_t_handle, THREAD_ALL_ACCESS, NULL,
                                  KernelMode,
                                  reinterpret_cast<void **>(&thread), NULL);
        if (thread) {
            KeWaitForSingleObject(thread, Executive, KernelMode, FALSE, NULL);
            ObDereferenceObject(thread);
        }
    }

    if (nmi_callback_handle)
        KeDeregisterNmiCallback(nmi_callback_handle);
    if (nmi_affinity)
        ExFreePool(nmi_affinity);
    if (nmi_stack_traces)
        ExFreePool(nmi_stack_traces);

    return SUCCESS;
}
} // namespace nmi
