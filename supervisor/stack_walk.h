#pragma once

#include "utils.h"

namespace stack_walk {
void test_get_context(PETHREAD thread)
{
    auto KeSuspendThread =
        (__int64(__fastcall *)(PETHREAD thread))findUEXFunction(
            (char *)"ntoskrnl.exe", KeSuspendThread);
    auto KeResumeThread =
        (__int64(__fastcall *)(PETHREAD thread))findUEXFunction(
            (char *)"ntoskrnl.exe", KeResumeThread);
    UNREFERENCED_PARAMETER(KeSuspendThread);
    UNREFERENCED_PARAMETER(KeResumeThread);

    auto PsSuspendThread =
        (__int64(__fastcall *)(PETHREAD thread, DWORD32 * a2))
            findUEXFunction((char *)"ntoskrnl.exe", PsSuspendThread);
    auto PsResumeThread = (__int64(__fastcall *)(PETHREAD thread, DWORD32 * a2))
        findUEXFunction((char *)"ntoskrnl.exe", PsResumeThread);
    UNREFERENCED_PARAMETER(PsSuspendThread);
    UNREFERENCED_PARAMETER(PsResumeThread);


    auto PspGetContextThreadInternal =
        (__int64(__fastcall *)(PETHREAD thread, CONTEXT * ctx, KPROCESSOR_MODE,
                               KPROCESSOR_MODE, KPROCESSOR_MODE))
            findUEXFunction((char *)"ntoskrnl.exe",
                            PspGetContextThreadInternal);

    KdLog("Context thread 0x%p\n", thread);

    KeSuspendThread(thread);

    CONTEXT Ctx = {0};
    Ctx.ContextFlags = CONTEXT_ALL;
    auto result2 = PspGetContextThreadInternal(thread, &Ctx, KernelMode,
                                               KernelMode, 1);

    KeResumeThread(thread);
    
    KdLog("Result 2: %lld\n", result2);
    KdLog("Ctx : 0x%p\n", &Ctx);
}
} // namespace stack_walk