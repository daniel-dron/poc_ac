
#include "utils.h"

namespace utils {

func_pattern ptrnPsSuspendThread{
    (char *)"\xE8\x00\x00\x00\x00\x85\xC0\x78\x10\xBE\x00\x00\x00\x00",
    (char *)"x????xxxxx????",
    true,
    5,
    4,
    nullptr};
func_pattern ptrnPspGetContextThreadInternal{
    (char *)"\xE8\x00\x00\x00\x00\x8B\xF0\x85\xC0\x79\x48",
    (char *)"x????xxxxxx",
    true,
    5,
    4,
    nullptr};
func_pattern ptrnPsResumeThread{
    (char *)"\xE8\x00\x00\x00\x00\x8B\x43\x4C\xA8\x08",
    (char *)"x????xxxxx",
    true,
    5,
    4,
    nullptr};

func_pattern ptrnKeSuspendThread{
    (char *)"\xE8\x00\x00\x00\x00\x89\x44\x24\x20\x33\xDB",
    (char *)"x????xxxxxx",
    true,
    5,
    4,
    nullptr};

func_pattern ptrnKeResumeThread{
    (char *)"\xE8\x00\x00\x00\x00\x65\x48\x8B\x14\x25\x00\x00\x00\x00\x8B\xF0"
            "\x83\xF8\x01",
    (char *)"x????xxxxx????xxxxx",
    true,
    5,
    4,
    nullptr};

void *find_function(char *module_name, func_pattern pattern)
{
    if (pattern.func_ptr)
        return pattern.func_ptr;

    void *base;
    size_t size;
    utils::get_kernel_module_base(module_name, &base, &size);
    if (!base || !size)
        return nullptr;

    ULONG64 result = (ULONG64)utils::find_pattern_raw(
        (char *)base, (char *)((ULONG64)base + size), pattern.pattern,
        pattern.mask);

    pattern.func_ptr = (pattern.adifo) ? rva(result, pattern.instruction_size,
                                             pattern.rva_size)
                                       : (void *)result;

    return pattern.func_ptr;
}

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

void get_kernel_module_base(const char *module_name, void **base,
                            size_t *m_size)
{
    ULONG size{};
    ZwQuerySystemInformation(SystemModuleInformation, nullptr, 0, &size);

    if (!size) {
        return;
    }

    _RTL_PROCESS_MODULES *modules = reinterpret_cast<_RTL_PROCESS_MODULES *>(
        ExAllocatePoolZero(NonPagedPool, size, 'RMT'));
    if (!modules) {
        return;
    }

    ZwQuerySystemInformation(SystemModuleInformation, modules, size, &size);

    for (ULONG i = 0; i < modules->NumberOfModules; i++) {
        if (strstr((const char *)modules->Modules[i].FullPathName,
                   module_name) != NULL) {
            *base = modules->Modules[i].ImageBase;
            *m_size = modules->Modules[i].ImageSize;
        }
    }
}

void *find_pattern_raw(const char *start, const char *end, const char *pattern,
                       const char *mask)
{
    if (start == end || end <= start)
        return nullptr;

    char *current = const_cast<char *>(start);
    while (current != end) {
        if (pattern_at(current, pattern, mask))
            return current;

        current++;
    }

    return nullptr;
}
bool pattern_at(const char *current, const char *pattern, const char *mask)
{
    for (; *mask; ++mask, ++current, ++pattern)
        if (*mask == 'x' && *current != *pattern)
            return 0;

    return (*mask) == 0;
}
} // namespace utils
