
#include "utils.h"

namespace utils {
pmod_info g_modules{};
size_t g_number_of_modules{};

bool in_valid_module(UINT64 addr)
{
    if (!g_modules) {
        ULONG size{};
        ZwQuerySystemInformation(SystemModuleInformation, nullptr, 0, &size);

        if (!size) {
            return false;
        }

        _RTL_PROCESS_MODULES *modules =
            reinterpret_cast<_RTL_PROCESS_MODULES *>(
                ExAllocatePoolZero(NonPagedPool, size, 'RMT'));
        if (!modules) {
            return false;
        }

        ZwQuerySystemInformation(SystemModuleInformation, modules, size, &size);

        g_modules = reinterpret_cast<pmod_info>(ExAllocatePoolZero(
            NonPagedPool, sizeof(mod_info) * modules->NumberOfModules, ' mvi'));

        KdLog("Loaded %ld modules!\n", modules->NumberOfModules);
        g_number_of_modules = modules->NumberOfModules;
        for (ULONG i = 0; i < modules->NumberOfModules; i++) {
            g_modules[i] = mod_info{
                reinterpret_cast<UINT64>(modules->Modules[i].ImageBase),
                modules->Modules[i].ImageSize};
            KdLog("%s - [0x%p - 0x%p]\n", modules->Modules[i].FullPathName,
                  g_modules[i].base, g_modules[i].size);
        }

        ExFreePool(modules);

        KdLog("Initialized g_modules\n");
    }

    bool found = false;

    for (ULONG i = 0; i < g_number_of_modules; i++) {
        if (addr >= g_modules[i].base &&
            (addr + 0xFFF) <= (g_modules[i].base + g_modules[i].size)) {
            found = true;
            break;
        }
    }

    return found;
}

void sleep(UINT32 milliseconds)
{
    UINT64 ms = milliseconds;
    ms = (ms * 1000) * 10;
    ms = ms * -1;
    KeDelayExecutionThread(KernelMode, 0, (PLARGE_INTEGER)&ms);
}
} // namespace utils
