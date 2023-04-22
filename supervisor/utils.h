#pragma once

#include <ntifs.h>
#include <ntddk.h>

#include <intrin.h>
#include <stdlib.h>

#include "definitions.h"

#define KdLog(...)                                                             \
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[SVAC]: " __VA_ARGS__)

namespace utils {
bool in_valid_module(UINT64 addr);
void sleep(UINT32 milliseconds);
} // namespace utils
