#pragma once

#include "utils.h"

namespace system_threads
{
extern _LIST_ENTRY *g_list_head;
extern _LIST_ENTRY *g_last_entry;

typedef struct _invalid_system_thread {
    HANDLE pid;
    HANDLE tid;

    PETHREAD thread;

    bool unlinked_system_process;
    bool unlinked_cid_table;
    bool jmp_at_start_address;
    bool invalid_system_thread_bit;

    bool invalid_start_address;
    ULONG64 start_address;
    void *start_address_dump;
    size_t sa_dump_size;

    bool invalid_win32_start_address;
    ULONG64 win32_start_address;
    void *win32_start_address_dump;
    size_t win32_sa_dump_size;

    _LIST_ENTRY entry;
} invalid_system_thread, *pinvalid_system_thread;

bool validate_system_thread(_ETHREAD *thread, _LIST_ENTRY *head);
ULONG64 scan_system_process_threads();

}