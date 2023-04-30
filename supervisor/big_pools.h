#pragma once

#include "utils.h"

namespace big_pools
{
typedef struct _bigpool_info {
    void *start_addr;
    size_t size;
    void *thread_start_addr;
} bigpool_inf, pbigpool_inf;

PSYSTEM_BIGPOOL_INFORMATION g_bigpool_info{};

void query_bigpools()
{
    SYSTEM_BIGPOOL_INFORMATION _bpi{};

    ULONG size{};
    NTSTATUS status = ZwQuerySystemInformation(SystemBigPoolInformation, &_bpi,
                                               sizeof(_bpi), &size);
    PSYSTEM_BIGPOOL_INFORMATION bigpool_info =
        reinterpret_cast<PSYSTEM_BIGPOOL_INFORMATION>(
            ExAllocatePoolZero(NonPagedPool, size, 'loop'));

    status = ZwQuerySystemInformation(SystemBigPoolInformation, bigpool_info,
                                      size, &size);

    if (g_bigpool_info && MmIsAddressValid(g_bigpool_info))
        ExFreePool(g_bigpool_info);

    g_bigpool_info = bigpool_info;
}
}