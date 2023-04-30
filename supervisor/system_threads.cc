#include "sytem_threads.h"

#include "big_pools.h"
#include "stack_walk.h"

namespace system_threads {

_LIST_ENTRY *g_list_head{};
_LIST_ENTRY *g_last_entry{};

bool validate_system_thread(_ETHREAD *thread, _LIST_ENTRY *head)
{
    if ((ULONG64)thread == (ULONG64)KeGetCurrentThread())
        return false;

    invalid_system_thread *ist =
        reinterpret_cast<invalid_system_thread *>(ExAllocatePoolZero(
            NonPagedPool, sizeof(invalid_system_thread), ' tsi'));
    if (!ist)
        return false;

    HANDLE tid = PsGetThreadId((PETHREAD)thread);
    HANDLE pid = PsGetProcessId(PsGetThreadProcess((PETHREAD)thread));

    ist->pid = pid;
    ist->tid = tid;

    ULONG64 start_address =
        *reinterpret_cast<ULONG64 *>(reinterpret_cast<ULONG64>(thread) + 0x450);
    ULONG64 win32_start_address =
        *reinterpret_cast<ULONG64 *>(reinterpret_cast<ULONG64>(thread) + 0x4d0);
    bool is_system_thread = *reinterpret_cast<ULONG32 *>(
                                reinterpret_cast<ULONG64>(thread) + 0x074) &
                            0x400;

    // check start addresses
    if (!utils::in_valid_module(start_address)) {
        ist->invalid_start_address = true;
        ist->start_address = start_address;
    }

    if (!utils::in_valid_module(win32_start_address)) {
        ist->invalid_win32_start_address = true;
        ist->win32_start_address = win32_start_address;
    }

    big_pools::bigpool_inf bp_inf{};
    big_pools::bigpool_inf bp_inf_win32{};

    // TODO: check if address is inside any big pools
    auto check_in_bigpool = [&](ULONG64 address) -> big_pools::bigpool_inf {
        for (size_t i = 0; i < big_pools::g_bigpool_info->Count; i++) {
            auto bigpool = big_pools::g_bigpool_info->AllocatedInfo[i];

            if (address >= (bigpool.VirtualAddress & 0xFFFFFFFFFFFFFFFE) &&
                address < ((bigpool.VirtualAddress & 0xFFFFFFFFFFFFFFFE) +
                           bigpool.SizeInBytes)) {

                return {reinterpret_cast<void *>(bigpool.VirtualAddress &
                                                 0xFFFFFFFFFFFFFFFE),
                        bigpool.SizeInBytes, reinterpret_cast<void *>(address)};
            }
        }
        return {0, 0, 0};
    };

    if (big_pools::g_bigpool_info) {
        if (start_address == win32_start_address &&
            ist->invalid_start_address) {
            bp_inf = check_in_bigpool(start_address);
            ist->start_address_dump = bp_inf.start_addr;
            ist->sa_dump_size = bp_inf.size;
        }

        if (ist->invalid_start_address) {
            bp_inf = check_in_bigpool(start_address);
            ist->start_address_dump = bp_inf.start_addr;
            ist->sa_dump_size = bp_inf.size;
        }
        if (ist->invalid_win32_start_address) {
            bp_inf_win32 = check_in_bigpool(win32_start_address);
            ist->win32_start_address_dump = bp_inf_win32.start_addr;
            ist->win32_sa_dump_size = bp_inf_win32.size;
        }
    }

    // Did someone mendle with MiscFlags indicating being a system thread?
    if (!is_system_thread) {
        ist->invalid_system_thread_bit = true;
    }

    // TODO: check jmp shellcode start address

    // TODO: Check if thread was unlinked from CID table

    // TODO: Check if thread was unlinked from system process thread list

    // TODO: Stackwalk thread

    if (ist->unlinked_cid_table || ist->unlinked_cid_table ||
        ist->jmp_at_start_address || ist->invalid_system_thread_bit ||
        ist->invalid_start_address || ist->invalid_win32_start_address) {
        ist->thread = reinterpret_cast<PETHREAD>(thread);

        stack_walk::test_get_context((PETHREAD)thread);

        InsertTailList(head, &ist->entry);
        return true;
    }
    else
        ExFreePool(ist);

    return false;
}

ULONG64 scan_system_process_threads()
{
    ULONG64 n{};

    if (!g_list_head)
        g_list_head = reinterpret_cast<_LIST_ENTRY *>(
            ExAllocatePoolZero(NonPagedPool, sizeof(_LIST_ENTRY), 'tsil'));
    else {
        // Clear list
        for (auto entry = g_list_head->Flink; entry != g_list_head;) {
            auto ist = CONTAINING_RECORD(entry, invalid_system_thread, entry);
            entry = entry->Flink;
            ExFreePool(ist);
        }
    }

    if (!g_list_head)
        return 0;

    InitializeListHead(g_list_head);

    PEPROCESS process{};
    auto status = PsLookupProcessByProcessId((HANDLE)4, &process);
    if (!NT_SUCCESS(status)) {
        KdLog("Failed PsLookupProcessByProcessId %d\n", status);
        return 0;
    }

    _LIST_ENTRY *thread_list_head = reinterpret_cast<_LIST_ENTRY *>(
        (reinterpret_cast<ULONG64>(process) + get_offset(thread_list_head)));

    big_pools::query_bigpools();

    for (auto entry = thread_list_head->Flink; entry != thread_list_head;
         entry = entry->Flink) {
        _ETHREAD *thread = reinterpret_cast<_ETHREAD *>(
            reinterpret_cast<ULONG64>(entry) - 0x4e8);

        if (validate_system_thread(thread, g_list_head))
            n++;
    }

    ObDereferenceObject(process);

    g_last_entry = g_list_head->Flink;

    return n;
}
} // namespace system_threads