#pragma once

#include "utils.h"

namespace nmi {
typedef struct _stack_trace {
    void **stack_trace;
    USHORT captured_frames;
} stack_trace, *pstack_trace;

typedef struct _stack_info_per_processor {
    bool invalid_stack_trace_pointer;
    ULONG n_captured_frames;
    ULONG n_invalid_frames;
    void *invalid_frames[10];
} stack_ipp, pstack_ipp;

typedef struct _nmi_info {
    ULONG n_processors;
    ULONG n_timeouts;
    bool freed_stack_traces_allocation;
    stack_ipp stack_infos[1];
} nmi_info, *pnmi_info;

enum nmi_status {
    SUCCESS,
    FAILED_REGISTER,
    FAIL_ALLOCATION_AFFINITY,
    FAIL_ALLOCATION_STACK,
    FAILED_CREATE_NMI_THREAD
};

extern _KAFFINITY_EX *nmi_affinity;
extern void *nmi_callback_handle;
extern volatile LONG nmi_fired;
extern bool nmi_stop;
extern HANDLE nmi_t_handle;
extern bool nmi_initialized;

extern pstack_trace nmi_stack_traces;

extern "C" VOID KeInitializeAffinityEx(_KAFFINITY_EX *affinity);
extern "C" VOID KeAddProcessorAffinityEx(_KAFFINITY_EX *affinity, INT num);
extern "C" VOID HalSendNMI(_KAFFINITY_EX *affinity);

BOOLEAN nmi_callback(PVOID context, BOOLEAN handled);
nmi_status setup_nmi();
void send_nmi(int processor);
nmi_status clean();
void process_nmi(pnmi_info nmii);
} // namespace nmi
