
#include "utils.h"

namespace utils {

bool in_valid_module(UINT64 addr)
{
    ULONG size{};
    ZwQuerySystemInformation(SystemModuleInformation, nullptr, 0, &size);

    if (!size) {
        return false;
    }

    _RTL_PROCESS_MODULES *modules = reinterpret_cast<_RTL_PROCESS_MODULES *>(
        ExAllocatePoolZero(NonPagedPool, size, 'RMT'));
    if (!modules) {
        return false;
    }

    ZwQuerySystemInformation(SystemModuleInformation, modules, size, &size);

    bool found = false;
    for (ULONG i = 0; i < modules->NumberOfModules; i++) {
        if (addr >= reinterpret_cast<UINT64>(modules->Modules[i].ImageBase) &&
            (addr + 0xFFF) <=
                (reinterpret_cast<UINT64>(modules->Modules[i].ImageBase) +
                 modules->Modules[i].ImageSize)) {
            found = true;
            break;
        }
    }

    ExFreePool(modules);

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
