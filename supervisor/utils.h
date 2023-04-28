#pragma once

#include <ntifs.h>
#include <ntddk.h>

#include <intrin.h>
#include <stdlib.h>

#include "definitions.h"
#include "offsets.h"

#define KdLog(...)                                                             \
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[SVAC]: " __VA_ARGS__)

#define rva(addr, instr_size, rva_size)                                        \
    (void *)(*(INT32 *)(addr + instr_size - rva_size) + addr + instr_size)

#define findUEXFunction(module_name, func_name)                                \
    utils::find_function(module_name, utils::ptrn##func_name)

namespace utils {

struct func_pattern {
    char *pattern;
    char *mask;
    bool adifo;
    size_t instruction_size;
    size_t rva_size;
    void *func_ptr;
};

extern func_pattern ptrnPsSuspendThread;
extern func_pattern ptrnPspGetContextThreadInternal;
extern func_pattern ptrnPsResumeThread;
extern func_pattern ptrnKeSuspendThread;
extern func_pattern ptrnKeResumeThread;

bool in_valid_module(UINT64 addr);
void sleep(UINT32 milliseconds);
void get_kernel_module_base(const char *module_name, void **base,
                            size_t *m_size);
void *find_pattern_raw(const char *start, const char *end, const char *pattern,
                       const char *mask);
bool pattern_at(const char *current, const char *pattern, const char *mask);
void *find_function(char *module_name, func_pattern pattern);
} // namespace utils
