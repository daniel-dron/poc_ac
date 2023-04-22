#pragma once
#pragma warning(push)

#pragma warning(disable : 4201)
#pragma warning(disable : 4471)
// 0x1 bytes (sizeof)
union _KWAIT_STATUS_REGISTER {
    UCHAR Flags;        // 0x0
    UCHAR State : 3;    // 0x0
    UCHAR Affinity : 1; // 0x0
    UCHAR Priority : 1; // 0x0
    UCHAR Apc : 1;      // 0x0
    UCHAR UserApc : 1;  // 0x0
    UCHAR Alert : 1;    // 0x0
};

// 0x8 bytes (sizeof)
union _KERNEL_SHADOW_STACK_LIMIT {
    ULONGLONG AllFields;                // 0x0
    ULONGLONG ShadowStackType : 3;      // 0x0
    ULONGLONG Unused : 9;               // 0x0
    ULONGLONG ShadowStackLimitPfn : 52; // 0x0
};

// 0x8 bytes (sizeof)
union _PS_CLIENT_SECURITY_CONTEXT {
    ULONGLONG ImpersonationData;      // 0x0
    VOID *ImpersonationToken;         // 0x0
    ULONGLONG ImpersonationLevel : 2; // 0x0
    ULONGLONG EffectiveOnly : 1;      // 0x0
};

// 0x8 bytes (sizeof)
struct _EX_PUSH_LOCK {
    union {
        struct {
            ULONGLONG Locked : 1;         // 0x0
            ULONGLONG Waiting : 1;        // 0x0
            ULONGLONG Waking : 1;         // 0x0
            ULONGLONG MultipleShared : 1; // 0x0
            ULONGLONG Shared : 60;        // 0x0
        };
        ULONGLONG Value; // 0x0
        VOID *Ptr;       // 0x0
    };
};

// 0x18 bytes (sizeof)
struct _PS_PROPERTY_SET {
    struct _LIST_ENTRY ListHead; // 0x0
    ULONGLONG Lock;              // 0x10
};

// 0x10 bytes (sizeof)
struct _KLOCK_ENTRY_LOCK_STATE {
    union {
        struct {
            ULONGLONG CrossThreadReleasable : 1; // 0x0
            ULONGLONG Busy : 1;                  // 0x0
            ULONGLONG Reserved : 61;             // 0x0
            ULONGLONG InTree : 1;                // 0x0
        };
        VOID *LockState; // 0x0
    };
    union {
        VOID *SessionState; // 0x8
        struct {
            ULONG SessionId;  // 0x8
            ULONG SessionPad; // 0xc
        };
    };
};

// 0x10 bytes (sizeof)
struct _RTL_RB_TREE {
    struct _RTL_BALANCED_NODE *Root; // 0x0
    union {
        UCHAR Encoded : 1;              // 0x8
        struct _RTL_BALANCED_NODE *Min; // 0x8
    };
};

// 0x8 bytes (sizeof)
union _KLOCK_ENTRY_BOOST_BITMAP {
    ULONGLONG AllFields; // 0x0
    struct {
        ULONG AllBoosts;    // 0x0
        ULONG WaiterCounts; // 0x4
    };
    ULONG CpuBoostsBitmap : 30; // 0x0
    ULONG IoBoost : 1;          // 0x0
    struct {
        ULONG IoQoSBoost : 1;                  // 0x0
        ULONG IoNormalPriorityWaiterCount : 8; // 0x4
    };
    ULONG IoQoSWaiterCount : 7; // 0x4
};

// 0x60 bytes (sizeof)
struct _KLOCK_ENTRY {
    union {
        struct _KLOCK_ENTRY_LOCK_STATE LockState; // 0x0
        VOID *volatile LockUnsafe;                // 0x0
        struct {
            volatile UCHAR CrossThreadReleasableAndBusyByte; // 0x0
            UCHAR Reserved[6];                               // 0x1
            volatile UCHAR InTreeByte;                       // 0x7
            union {
                VOID *SessionState; // 0x8
                struct {
                    ULONG SessionId;  // 0x8
                    ULONG SessionPad; // 0xc
                };
            };
        };
    };
    union {
        ULONG EntryFlags; // 0x10
        struct {
            UCHAR EntryIndex;   // 0x10
            UCHAR WaitingByte;  // 0x11
            UCHAR AcquiredByte; // 0x12
            union {
                UCHAR CrossThreadFlags; // 0x13
                struct {
                    UCHAR HeadNodeBit : 1;   // 0x13
                    UCHAR IoPriorityBit : 1; // 0x13
                    UCHAR IoQoSWaiter : 1;   // 0x13
                    UCHAR Spare1 : 5;        // 0x13
                };
            };
        };
        struct {
            ULONG StaticState : 8; // 0x10
            ULONG AllFlags : 24;   // 0x10
        };
    };
    ULONG SpareFlags;                   // 0x14
    struct _RTL_BALANCED_NODE TreeNode; // 0x18
    union {
        struct {
            struct _RTL_RB_TREE OwnerTree;  // 0x30
            struct _RTL_RB_TREE WaiterTree; // 0x40
        };
        CHAR CpuPriorityKey; // 0x30
    };
    ULONGLONG EntryLock;                         // 0x50
    union _KLOCK_ENTRY_BOOST_BITMAP BoostBitmap; // 0x58
};

// 0x480 bytes (sizeof)
struct _KTHREAD {
    struct _DISPATCHER_HEADER Header;                    // 0x0
    VOID *SListFaultAddress;                             // 0x18
    ULONGLONG QuantumTarget;                             // 0x20
    VOID *InitialStack;                                  // 0x28
    VOID *volatile StackLimit;                           // 0x30
    VOID *StackBase;                                     // 0x38
    ULONGLONG ThreadLock;                                // 0x40
    volatile ULONGLONG CycleTime;                        // 0x48
    ULONG CurrentRunTime;                                // 0x50
    ULONG ExpectedRunTime;                               // 0x54
    VOID *KernelStack;                                   // 0x58
    struct _XSAVE_FORMAT *StateSaveArea;                 // 0x60
    struct _KSCHEDULING_GROUP *volatile SchedulingGroup; // 0x68
    union _KWAIT_STATUS_REGISTER WaitRegister;           // 0x70
    volatile UCHAR Running;                              // 0x71
    UCHAR Alerted[2];                                    // 0x72
    union {
        struct {
            ULONG AutoBoostActive : 1;         // 0x74
            ULONG ReadyTransition : 1;         // 0x74
            ULONG WaitNext : 1;                // 0x74
            ULONG SystemAffinityActive : 1;    // 0x74
            ULONG Alertable : 1;               // 0x74
            ULONG UserStackWalkActive : 1;     // 0x74
            ULONG ApcInterruptRequest : 1;     // 0x74
            ULONG QuantumEndMigrate : 1;       // 0x74
            ULONG SecureThread : 1;            // 0x74
            ULONG TimerActive : 1;             // 0x74
            ULONG SystemThread : 1;            // 0x74
            ULONG ProcessDetachActive : 1;     // 0x74
            ULONG CalloutActive : 1;           // 0x74
            ULONG ScbReadyQueue : 1;           // 0x74
            ULONG ApcQueueable : 1;            // 0x74
            ULONG ReservedStackInUse : 1;      // 0x74
            ULONG Spare : 1;                   // 0x74
            ULONG TimerSuspended : 1;          // 0x74
            ULONG SuspendedWaitMode : 1;       // 0x74
            ULONG SuspendSchedulerApcWait : 1; // 0x74
            ULONG CetUserShadowStack : 1;      // 0x74
            ULONG BypassProcessFreeze : 1;     // 0x74
            ULONG CetKernelShadowStack : 1;    // 0x74
            ULONG StateSaveAreaDecoupled : 1;  // 0x74
            ULONG Reserved : 8;                // 0x74
        };
        LONG MiscFlags; // 0x74
    };
    union {
        struct {
            ULONG UserIdealProcessorFixed : 1;         // 0x78
            ULONG IsolationWidth : 1;                  // 0x78
            ULONG AutoAlignment : 1;                   // 0x78
            ULONG DisableBoost : 1;                    // 0x78
            ULONG AlertedByThreadId : 1;               // 0x78
            ULONG QuantumDonation : 1;                 // 0x78
            ULONG EnableStackSwap : 1;                 // 0x78
            ULONG GuiThread : 1;                       // 0x78
            ULONG DisableQuantum : 1;                  // 0x78
            ULONG ChargeOnlySchedulingGroup : 1;       // 0x78
            ULONG DeferPreemption : 1;                 // 0x78
            ULONG QueueDeferPreemption : 1;            // 0x78
            ULONG ForceDeferSchedule : 1;              // 0x78
            ULONG SharedReadyQueueAffinity : 1;        // 0x78
            ULONG FreezeCount : 1;                     // 0x78
            ULONG TerminationApcRequest : 1;           // 0x78
            ULONG AutoBoostEntriesExhausted : 1;       // 0x78
            ULONG KernelStackResident : 1;             // 0x78
            ULONG TerminateRequestReason : 2;          // 0x78
            ULONG ProcessStackCountDecremented : 1;    // 0x78
            ULONG RestrictedGuiThread : 1;             // 0x78
            ULONG VpBackingThread : 1;                 // 0x78
            ULONG EtwStackTraceCrimsonApcDisabled : 1; // 0x78
            ULONG EtwStackTraceApcInserted : 8;        // 0x78
        };
        volatile LONG ThreadFlags; // 0x78
    };
    volatile UCHAR Tag;                      // 0x7c
    UCHAR SystemHeteroCpuPolicy;             // 0x7d
    UCHAR UserHeteroCpuPolicy : 7;           // 0x7e
    UCHAR ExplicitSystemHeteroCpuPolicy : 1; // 0x7e
    union {
        struct {
            UCHAR RunningNonRetpolineCode : 1; // 0x7f
            UCHAR SpecCtrlSpare : 7;           // 0x7f
        };
        UCHAR SpecCtrl; // 0x7f
    };
    ULONG SystemCallNumber;         // 0x80
    ULONG ReadyTime;                // 0x84
    VOID *FirstArgument;            // 0x88
    struct _KTRAP_FRAME *TrapFrame; // 0x90
    union {
        struct _KAPC_STATE ApcState; // 0x98
        struct {
            UCHAR ApcStateFill[43];   // 0x98
            CHAR Priority;            // 0xc3
            ULONG UserIdealProcessor; // 0xc4
        };
    };
    volatile LONGLONG WaitStatus;       // 0xc8
    struct _KWAIT_BLOCK *WaitBlockList; // 0xd0
    union {
        struct _LIST_ENTRY WaitListEntry;        // 0xd8
        struct _SINGLE_LIST_ENTRY SwapListEntry; // 0xd8
    };
    struct _DISPATCHER_HEADER *volatile Queue; // 0xe8
    VOID *Teb;                                 // 0xf0
    ULONGLONG RelativeTimerBias;               // 0xf8
    struct _KTIMER Timer;                      // 0x100
    union {
        struct _KWAIT_BLOCK WaitBlock[4]; // 0x140
        struct {
            UCHAR WaitBlockFill4[20]; // 0x140
            ULONG ContextSwitches;    // 0x154
        };
        struct {
            UCHAR WaitBlockFill5[68]; // 0x140
            volatile UCHAR State;     // 0x184
            CHAR Spare13;             // 0x185
            UCHAR WaitIrql;           // 0x186
            CHAR WaitMode;            // 0x187
        };
        struct {
            UCHAR WaitBlockFill6[116]; // 0x140
            ULONG WaitTime;            // 0x1b4
        };
        struct {
            UCHAR WaitBlockFill7[164]; // 0x140
            union {
                struct {
                    SHORT KernelApcDisable;  // 0x1e4
                    SHORT SpecialApcDisable; // 0x1e6
                };
                ULONG CombinedApcDisable; // 0x1e4
            };
        };
        struct {
            UCHAR WaitBlockFill8[40];                 // 0x140
            struct _KTHREAD_COUNTERS *ThreadCounters; // 0x168
        };
        struct {
            UCHAR WaitBlockFill9[88];        // 0x140
            struct _XSTATE_SAVE *XStateSave; // 0x198
        };
        struct {
            UCHAR WaitBlockFill10[136]; // 0x140
            VOID *volatile Win32Thread; // 0x1c8
        };
        struct {
            UCHAR WaitBlockFill11[176]; // 0x140
            ULONGLONG Spare18;          // 0x1f0
            ULONGLONG Spare19;          // 0x1f8
        };
    };
    union {
        volatile LONG ThreadFlags2; // 0x200
        struct {
            ULONG BamQosLevel : 8;           // 0x200
            ULONG ThreadFlags2Reserved : 24; // 0x200
        };
    };
    UCHAR HgsFeedbackClass;            // 0x204
    UCHAR Spare23[3];                  // 0x205
    struct _LIST_ENTRY QueueListEntry; // 0x208
    union {
        volatile ULONG NextProcessor; // 0x218
        struct {
            ULONG NextProcessorNumber : 31; // 0x218
            ULONG SharedReadyQueue : 1;     // 0x218
        };
    };
    LONG QueuePriority;                 // 0x21c
    struct _KPROCESS *Process;          // 0x220
    struct _KAFFINITY_EX *UserAffinity; // 0x228
    USHORT UserAffinityPrimaryGroup;    // 0x230
    CHAR PreviousMode;                  // 0x232
    CHAR BasePriority;                  // 0x233
    union {
        CHAR PriorityDecrement; // 0x234
        struct {
            UCHAR ForegroundBoost : 4; // 0x234
            UCHAR UnusualBoost : 4;    // 0x234
        };
    };
    UCHAR Preempted;                // 0x235
    UCHAR AdjustReason;             // 0x236
    CHAR AdjustIncrement;           // 0x237
    ULONGLONG AffinityVersion;      // 0x238
    struct _KAFFINITY_EX *Affinity; // 0x240
    USHORT AffinityPrimaryGroup;    // 0x248
    UCHAR ApcStateIndex;            // 0x24a
    UCHAR WaitBlockCount;           // 0x24b
    ULONG IdealProcessor;           // 0x24c
    ULONGLONG NpxState;             // 0x250
    union {
        struct _KAPC_STATE SavedApcState; // 0x258
        struct {
            UCHAR SavedApcStateFill[43]; // 0x258
            UCHAR WaitReason;            // 0x283
            CHAR SuspendCount;           // 0x284
            CHAR Saturation;             // 0x285
            USHORT SListFaultCount;      // 0x286
        };
    };
    union {
        struct _KAPC SchedulerApc; // 0x288
        struct {
            UCHAR SchedulerApcFill1[3]; // 0x288
            UCHAR QuantumReset;         // 0x28b
        };
        struct {
            UCHAR SchedulerApcFill2[4]; // 0x288
            ULONG KernelTime;           // 0x28c
        };
        struct {
            UCHAR SchedulerApcFill3[64];      // 0x288
            struct _KPRCB *volatile WaitPrcb; // 0x2c8
        };
        struct {
            UCHAR SchedulerApcFill4[72]; // 0x288
            VOID *LegoData;              // 0x2d0
        };
        struct {
            UCHAR SchedulerApcFill5[83]; // 0x288
            UCHAR CallbackNestingLevel;  // 0x2db
            ULONG UserTime;              // 0x2dc
        };
    };
    struct _KEVENT SuspendEvent;        // 0x2e0
    struct _LIST_ENTRY ThreadListEntry; // 0x2f8
    struct _LIST_ENTRY MutantListHead;  // 0x308
    UCHAR AbEntrySummary;               // 0x318
    UCHAR AbWaitEntryCount;             // 0x319
    union {
        UCHAR FreezeFlags; // 0x31a
        struct {
            UCHAR FreezeCount2 : 1; // 0x31a
            UCHAR FreezeNormal : 1; // 0x31a
            UCHAR FreezeDeep : 1;   // 0x31a
        };
    };
    CHAR SystemPriority;                            // 0x31b
    ULONG SecureThreadCookie;                       // 0x31c
    VOID *Spare22;                                  // 0x320
    struct _SINGLE_LIST_ENTRY PropagateBoostsEntry; // 0x328
    struct _SINGLE_LIST_ENTRY IoSelfBoostsEntry;    // 0x330
    UCHAR PriorityFloorCounts[32];                  // 0x338
    ULONG PriorityFloorSummary;                     // 0x358
    volatile LONG AbCompletedIoBoostCount;          // 0x35c
    volatile LONG AbCompletedIoQoSBoostCount;       // 0x360
    volatile SHORT KeReferenceCount;                // 0x364
    UCHAR AbOrphanedEntrySummary;                   // 0x366
    UCHAR AbOwnedEntryCount;                        // 0x367
    ULONG ForegroundLossTime;                       // 0x368
    union {
        struct _LIST_ENTRY GlobalForegroundListEntry; // 0x370
        struct {
            struct _SINGLE_LIST_ENTRY ForegroundDpcStackListEntry; // 0x370
            ULONGLONG InGlobalForegroundList;                      // 0x378
        };
    };
    LONGLONG ReadOperationCount;     // 0x380
    LONGLONG WriteOperationCount;    // 0x388
    LONGLONG OtherOperationCount;    // 0x390
    LONGLONG ReadTransferCount;      // 0x398
    LONGLONG WriteTransferCount;     // 0x3a0
    LONGLONG OtherTransferCount;     // 0x3a8
    struct _KSCB *QueuedScb;         // 0x3b0
    volatile ULONG ThreadTimerDelay; // 0x3b8
    union {
        volatile LONG ThreadFlags3; // 0x3bc
        struct {
            ULONG ThreadFlags3Reserved : 8;   // 0x3bc
            ULONG PpmPolicy : 3;              // 0x3bc
            ULONG ThreadFlags3Reserved2 : 21; // 0x3bc
        };
    };
    ULONGLONG TracingPrivate[1];          // 0x3c0
    VOID *SchedulerAssist;                // 0x3c8
    VOID *volatile AbWaitObject;          // 0x3d0
    ULONG ReservedPreviousReadyTimeValue; // 0x3d8
    ULONGLONG KernelWaitTime;             // 0x3e0
    ULONGLONG UserWaitTime;               // 0x3e8
    union {
        struct _LIST_ENTRY GlobalUpdateVpThreadPriorityListEntry; // 0x3f0
        struct {
            struct _SINGLE_LIST_ENTRY
                UpdateVpThreadPriorityDpcStackListEntry;  // 0x3f0
            ULONGLONG InGlobalUpdateVpThreadPriorityList; // 0x3f8
        };
    };
    LONG SchedulerAssistPriorityFloor;                       // 0x400
    LONG RealtimePriorityFloor;                              // 0x404
    VOID *KernelShadowStack;                                 // 0x408
    VOID *KernelShadowStackInitial;                          // 0x410
    VOID *KernelShadowStackBase;                             // 0x418
    union _KERNEL_SHADOW_STACK_LIMIT KernelShadowStackLimit; // 0x420
    ULONGLONG ExtendedFeatureDisableMask;                    // 0x428
    ULONGLONG HgsFeedbackStartTime;                          // 0x430
    ULONGLONG HgsFeedbackCycles;                             // 0x438
    ULONG HgsInvalidFeedbackCount;                           // 0x440
    ULONG HgsLowerPerfClassFeedbackCount;                    // 0x444
    ULONG HgsHigherPerfClassFeedbackCount;                   // 0x448
    ULONG Spare27;                                           // 0x44c
    struct _SINGLE_LIST_ENTRY SystemAffinityTokenListHead;   // 0x450
    VOID *IptSaveArea;                                       // 0x458
    UCHAR ResourceIndex;                                     // 0x460
    UCHAR CoreIsolationReasons;                              // 0x461
    UCHAR BamQosLevelFromAssistPage;                         // 0x462
    UCHAR Spare31[1];                                        // 0x463
    ULONG Spare32;                                           // 0x464
    ULONGLONG EndPadding[3];                                 // 0x468
};

// 0x900 bytes (sizeof)
struct _ETHREAD {
    struct _KTHREAD Tcb;             // 0x0
    union _LARGE_INTEGER CreateTime; // 0x480
    union {
        union _LARGE_INTEGER ExitTime;     // 0x488
        struct _LIST_ENTRY KeyedWaitChain; // 0x488
    };
    union {
        struct _LIST_ENTRY PostBlockList; // 0x498
        struct {
            VOID *ForwardLinkShadow; // 0x498
            VOID *StartAddress;      // 0x4a0
        };
    };
    union {
        struct _TERMINATION_PORT *TerminationPort; // 0x4a8
        struct _ETHREAD *ReaperLink;               // 0x4a8
        VOID *KeyedWaitValue;                      // 0x4a8
    };
    ULONGLONG ActiveTimerListLock;          // 0x4b0
    struct _LIST_ENTRY ActiveTimerListHead; // 0x4b8
    struct _CLIENT_ID Cid;                  // 0x4c8
    union {
        struct _KSEMAPHORE KeyedWaitSemaphore; // 0x4d8
        struct _KSEMAPHORE AlpcWaitSemaphore;  // 0x4d8
    };
    union _PS_CLIENT_SECURITY_CONTEXT ClientSecurity; // 0x4f8
    struct _LIST_ENTRY IrpList;                       // 0x500
    ULONGLONG TopLevelIrp;                            // 0x510
    struct _DEVICE_OBJECT *DeviceToVerify;            // 0x518
    VOID *Win32StartAddress;                          // 0x520
    VOID *ChargeOnlySession;                          // 0x528
    VOID *LegacyPowerObject;                          // 0x530
    struct _LIST_ENTRY ThreadListEntry;               // 0x538
    struct _EX_RUNDOWN_REF RundownProtect;            // 0x548
    struct _EX_PUSH_LOCK ThreadLock;                  // 0x550
    ULONG ReadClusterSize;                            // 0x558
    volatile LONG MmLockOrdering;                     // 0x55c
    union {
        ULONG CrossThreadFlags; // 0x560
        struct {
            ULONG Terminated : 1;               // 0x560
            ULONG ThreadInserted : 1;           // 0x560
            ULONG HideFromDebugger : 1;         // 0x560
            ULONG ActiveImpersonationInfo : 1;  // 0x560
            ULONG HardErrorsAreDisabled : 1;    // 0x560
            ULONG BreakOnTermination : 1;       // 0x560
            ULONG SkipCreationMsg : 1;          // 0x560
            ULONG SkipTerminationMsg : 1;       // 0x560
            ULONG CopyTokenOnOpen : 1;          // 0x560
            ULONG ThreadIoPriority : 3;         // 0x560
            ULONG ThreadPagePriority : 3;       // 0x560
            ULONG RundownFail : 1;              // 0x560
            ULONG UmsForceQueueTermination : 1; // 0x560
            ULONG IndirectCpuSets : 1;          // 0x560
            ULONG DisableDynamicCodeOptOut : 1; // 0x560
            ULONG ExplicitCaseSensitivity : 1;  // 0x560
            ULONG PicoNotifyExit : 1;           // 0x560
            ULONG DbgWerUserReportActive : 1;   // 0x560
            ULONG ForcedSelfTrimActive : 1;     // 0x560
            ULONG SamplingCoverage : 1;         // 0x560
            ULONG ReservedCrossThreadFlags : 8; // 0x560
        };
    };
    union {
        ULONG SameThreadPassiveFlags; // 0x564
        struct {
            ULONG ActiveExWorker : 1;                  // 0x564
            ULONG MemoryMaker : 1;                     // 0x564
            ULONG StoreLockThread : 2;                 // 0x564
            ULONG ClonedThread : 1;                    // 0x564
            ULONG KeyedEventInUse : 1;                 // 0x564
            ULONG SelfTerminate : 1;                   // 0x564
            ULONG RespectIoPriority : 1;               // 0x564
            ULONG ActivePageLists : 1;                 // 0x564
            ULONG SecureContext : 1;                   // 0x564
            ULONG ZeroPageThread : 1;                  // 0x564
            ULONG WorkloadClass : 1;                   // 0x564
            ULONG GenerateDumpOnBadHandleAccess : 1;   // 0x564
            ULONG ReservedSameThreadPassiveFlags : 19; // 0x564
        };
    };
    union {
        ULONG SameThreadApcFlags; // 0x568
        struct {
            UCHAR OwnsProcessAddressSpaceExclusive : 1;    // 0x568
            UCHAR OwnsProcessAddressSpaceShared : 1;       // 0x568
            UCHAR HardFaultBehavior : 1;                   // 0x568
            volatile UCHAR StartAddressInvalid : 1;        // 0x568
            UCHAR EtwCalloutActive : 1;                    // 0x568
            UCHAR SuppressSymbolLoad : 1;                  // 0x568
            UCHAR Prefetching : 1;                         // 0x568
            UCHAR OwnsVadExclusive : 1;                    // 0x568
            UCHAR SystemPagePriorityActive : 1;            // 0x569
            UCHAR SystemPagePriority : 3;                  // 0x569
            UCHAR AllowUserWritesToExecutableMemory : 1;   // 0x569
            UCHAR AllowKernelWritesToExecutableMemory : 1; // 0x569
            UCHAR OwnsVadShared : 1;                       // 0x569
            UCHAR SessionAttachActive : 1;                 // 0x569
            UCHAR PasidMsrValid : 1;                       // 0x56a
        };
    };
    UCHAR CacheManagerActive;          // 0x56c
    UCHAR DisablePageFaultClustering;  // 0x56d
    UCHAR ActiveFaultCount;            // 0x56e
    UCHAR LockOrderState;              // 0x56f
    ULONG PerformanceCountLowReserved; // 0x570
    LONG PerformanceCountHighReserved; // 0x574
    ULONGLONG AlpcMessageId;           // 0x578
    union {
        VOID *AlpcMessage;             // 0x580
        ULONG AlpcReceiveAttributeSet; // 0x580
    };
    struct _LIST_ENTRY AlpcWaitListEntry;             // 0x588
    LONG ExitStatus;                                  // 0x598
    ULONG CacheManagerCount;                          // 0x59c
    ULONG IoBoostCount;                               // 0x5a0
    ULONG IoQoSBoostCount;                            // 0x5a4
    ULONG IoQoSThrottleCount;                         // 0x5a8
    ULONG KernelStackReference;                       // 0x5ac
    struct _LIST_ENTRY BoostList;                     // 0x5b0
    struct _LIST_ENTRY DeboostList;                   // 0x5c0
    ULONGLONG BoostListLock;                          // 0x5d0
    ULONGLONG IrpListLock;                            // 0x5d8
    VOID *ReservedForSynchTracking;                   // 0x5e0
    struct _SINGLE_LIST_ENTRY CmCallbackListHead;     // 0x5e8
    struct _GUID *ActivityId;                         // 0x5f0
    struct _SINGLE_LIST_ENTRY SeLearningModeListHead; // 0x5f8
    VOID *VerifierContext;                            // 0x600
    VOID *AdjustedClientToken;                        // 0x608
    VOID *WorkOnBehalfThread;                         // 0x610
    struct _PS_PROPERTY_SET PropertySet;              // 0x618
    VOID *PicoContext;                                // 0x630
    ULONGLONG UserFsBase;                             // 0x638
    ULONGLONG UserGsBase;                             // 0x640
    struct _THREAD_ENERGY_VALUES *EnergyValues;       // 0x648
    union {
        ULONGLONG SelectedCpuSets;          // 0x650
        ULONGLONG *SelectedCpuSetsIndirect; // 0x650
    };
    struct _EJOB *Silo;                            // 0x658
    struct _UNICODE_STRING *ThreadName;            // 0x660
    struct _CONTEXT *SetContextState;              // 0x668
    UCHAR LastSoftParkElectionQos;                 // 0x670
    UCHAR LastSoftParkElectionWorkloadType;        // 0x671
    UCHAR LastSoftParkElectionRunningType;         // 0x672
    UCHAR Spare1;                                  // 0x673
    ULONG HeapData;                                // 0x674
    struct _LIST_ENTRY OwnerEntryListHead;         // 0x678
    ULONGLONG DisownedOwnerEntryListLock;          // 0x688
    struct _LIST_ENTRY DisownedOwnerEntryListHead; // 0x690
    struct _KLOCK_ENTRY LockEntries[6];            // 0x6a0
    VOID *CmThreadInfo;                            // 0x8e0
    VOID *FlsData;                                 // 0x8e8
    ULONG LastExpectedRunTime;                     // 0x8f0
    ULONG LastSoftParkElectionRunTime;             // 0x8f4
    ULONGLONG LastSoftParkElectionGeneration;      // 0x8f8
};

// 0x4 bytes (sizeof)
union _KPRCBFLAG {
    volatile LONG PrcbFlags;         // 0x0
    ULONG BamQosLevel : 8;           // 0x0
    ULONG PendingQosUpdate : 2;      // 0x0
    ULONG CacheIsolationEnabled : 1; // 0x0
    ULONG TracepointActive : 1;      // 0x0
    ULONG LongDpcRunning : 1;        // 0x0
    ULONG PrcbFlagsReserved : 19;    // 0x0
};

// 0x10 bytes (sizeof)
struct _KDESCRIPTOR {
    USHORT Pad[3]; // 0x0
    USHORT Limit;  // 0x6
    VOID *Base;    // 0x8
};

// 0xf0 bytes (sizeof)
struct _KSPECIAL_REGISTERS {
    ULONGLONG Cr0;                  // 0x0
    ULONGLONG Cr2;                  // 0x8
    ULONGLONG Cr3;                  // 0x10
    ULONGLONG Cr4;                  // 0x18
    ULONGLONG KernelDr0;            // 0x20
    ULONGLONG KernelDr1;            // 0x28
    ULONGLONG KernelDr2;            // 0x30
    ULONGLONG KernelDr3;            // 0x38
    ULONGLONG KernelDr6;            // 0x40
    ULONGLONG KernelDr7;            // 0x48
    struct _KDESCRIPTOR Gdtr;       // 0x50
    struct _KDESCRIPTOR Idtr;       // 0x60
    USHORT Tr;                      // 0x70
    USHORT Ldtr;                    // 0x72
    ULONG MxCsr;                    // 0x74
    ULONGLONG DebugControl;         // 0x78
    ULONGLONG LastBranchToRip;      // 0x80
    ULONGLONG LastBranchFromRip;    // 0x88
    ULONGLONG LastExceptionToRip;   // 0x90
    ULONGLONG LastExceptionFromRip; // 0x98
    ULONGLONG Cr8;                  // 0xa0
    ULONGLONG MsrGsBase;            // 0xa8
    ULONGLONG MsrGsSwap;            // 0xb0
    ULONGLONG MsrStar;              // 0xb8
    ULONGLONG MsrLStar;             // 0xc0
    ULONGLONG MsrCStar;             // 0xc8
    ULONGLONG MsrSyscallMask;       // 0xd0
    ULONGLONG Xcr0;                 // 0xd8
    ULONGLONG MsrFsBase;            // 0xe0
    ULONGLONG SpecialPadding0;      // 0xe8
};

// 0x5c0 bytes (sizeof)
struct _KPROCESSOR_STATE {
    struct _KSPECIAL_REGISTERS SpecialRegisters; // 0x0
    struct _CONTEXT ContextFrame;                // 0xf0
};

// 0x10 bytes (sizeof)
struct _PP_LOOKASIDE_LIST {
    struct _GENERAL_LOOKASIDE *P; // 0x0
    struct _GENERAL_LOOKASIDE *L; // 0x8
};

// 0x108 bytes (sizeof)
struct _KAFFINITY_EX {
    USHORT Count;   // 0x0
    USHORT Size;    // 0x2
    ULONG Reserved; // 0x4
    union {
        ULONGLONG Bitmap[1];        // 0x8
        ULONGLONG StaticBitmap[32]; // 0x8
    };
};

// 0x420 bytes (sizeof)
struct _KSTATIC_AFFINITY_BLOCK {
    union {
        struct _KAFFINITY_EX KeFlushTbAffinity;     // 0x0
        struct _KAFFINITY_EX KeFlushWbAffinity;     // 0x0
        struct _KAFFINITY_EX KeSyncContextAffinity; // 0x0
    };
    struct _KAFFINITY_EX KeFlushTbDeepIdleAffinity; // 0x108
    struct _KAFFINITY_EX KeIpiSendAffinity;         // 0x210
    struct _KAFFINITY_EX KeIpiSendIpiSet;           // 0x318
};

// 0x110 bytes (sizeof)
struct _KSOFTWARE_INTERRUPT_BATCH {
    UCHAR Level;                                 // 0x0
    UCHAR TargetType;                            // 0x1
    UCHAR ReservedBatchInProgress;               // 0x2
    UCHAR Spare;                                 // 0x3
    ULONG SingleTargetIndex;                     // 0x4
    struct _KAFFINITY_EX MultipleTargetAffinity; // 0x8
};

// 0x10 bytes (sizeof)
struct _KDPC_LIST {
    struct _SINGLE_LIST_ENTRY ListHead;   // 0x0
    struct _SINGLE_LIST_ENTRY *LastEntry; // 0x8
};

// 0x30 bytes (sizeof)
struct _KDPC_DATA {
    struct _KDPC_LIST DpcList;        // 0x0
    ULONGLONG DpcLock;                // 0x10
    volatile LONG DpcQueueDepth;      // 0x18
    ULONG DpcCount;                   // 0x1c
    struct _KDPC *volatile ActiveDpc; // 0x20
    ULONG LongDpcPresent;             // 0x28
    ULONG Padding;                    // 0x2c
};

// 0x20 bytes (sizeof)
struct _KTIMER_TABLE_ENTRY {
    ULONGLONG Lock;             // 0x0
    struct _LIST_ENTRY Entry;   // 0x8
    union _ULARGE_INTEGER Time; // 0x18
};

// 0x18 bytes (sizeof)
struct _KTIMER_TABLE_STATE {
    ULONGLONG LastTimerExpiration[2]; // 0x0
    ULONG LastTimerHand[2];           // 0x10
};

// 0x4218 bytes (sizeof)
struct _KTIMER_TABLE {
    struct _KTIMER *TimerExpiry[64];                 // 0x0
    struct _KTIMER_TABLE_ENTRY TimerEntries[2][256]; // 0x200
    struct _KTIMER_TABLE_STATE TableState;           // 0x4200
};

// 0x6 bytes (sizeof)
struct _PROC_IDLE_POLICY {
    UCHAR PromotePercent;     // 0x0
    UCHAR DemotePercent;      // 0x1
    UCHAR PromotePercentBase; // 0x2
    UCHAR DemotePercentBase;  // 0x3
    UCHAR AllowScaling;       // 0x4
    UCHAR ForceLightIdle;     // 0x5
};

// 0x4 bytes (sizeof)
union _PPM_IDLE_SYNCHRONIZATION_STATE {
    LONG AsLong;        // 0x0
    LONG RefCount : 24; // 0x0
    ULONG State : 8;    // 0x0
};

// 0x90 bytes (sizeof)
struct _PROC_FEEDBACK {
    ULONGLONG Lock;                             // 0x0
    ULONGLONG CyclesLast;                       // 0x8
    ULONGLONG CyclesActive;                     // 0x10
    struct _PROC_FEEDBACK_COUNTER *Counters[2]; // 0x18
    ULONGLONG LastUpdateTime;                   // 0x28
    ULONGLONG UnscaledTime;                     // 0x30
    volatile LONGLONG UnaccountedTime;          // 0x38
    ULONGLONG ScaledTime[2];                    // 0x40
    ULONGLONG UnaccountedKernelTime;            // 0x50
    ULONGLONG PerformanceScaledKernelTime;      // 0x58
    ULONG UserTimeLast;                         // 0x60
    ULONG KernelTimeLast;                       // 0x64
    ULONGLONG IdleGenerationNumberLast;         // 0x68
    ULONGLONG HvActiveTimeLast;                 // 0x70
    ULONGLONG StallCyclesLast;                  // 0x78
    ULONGLONG StallTime;                        // 0x80
    UCHAR KernelTimesIndex;                     // 0x88
    UCHAR CounterDiscardsIdleTime;              // 0x89
    UCHAR CounterReadOptimize;                  // 0x8a
};

// 0x20 bytes (sizeof)
struct _PPM_FFH_THROTTLE_STATE_INFO {
    UCHAR EnableLogging;                   // 0x0
    ULONG MismatchCount;                   // 0x4
    UCHAR Initialized;                     // 0x8
    ULONGLONG LastValue;                   // 0x10
    union _LARGE_INTEGER LastLogTickCount; // 0x18
};

// 0x10 bytes (sizeof)
struct _PROC_IDLE_SNAP {
    ULONGLONG Time; // 0x0
    ULONGLONG Idle; // 0x8
};

// 0x48 bytes (sizeof)
struct _PROC_PERF_CHECK_CONTEXT {
    struct _PROC_PERF_DOMAIN *Domain;         // 0x0
    struct _PROC_PERF_CONSTRAINT *Constraint; // 0x8
    struct _PROC_PERF_CHECK *PerfCheck;       // 0x10
    struct _PROC_PERF_LOAD *Load;             // 0x18
    struct _PROC_PERF_HISTORY *PerfHistory;   // 0x20
    ULONG Utility;                            // 0x28
    ULONG AffinitizedUtility;                 // 0x2c
    ULONG MediaUtility;                       // 0x30
    ULONG ImportantUtility;                   // 0x34
    ULONG IdealUtility;                       // 0x38
    USHORT LatestAffinitizedPercent;          // 0x3c
    USHORT AveragePerformancePercent;         // 0x3e
    ULONG RelativePerformance;                // 0x40
    UCHAR NtProcessor;                        // 0x44
};

// 0x230 bytes (sizeof)
struct _PROCESSOR_POWER_STATE {
    struct _PPM_IDLE_STATES *IdleStates;          // 0x0
    struct _PROC_IDLE_ACCOUNTING *IdleAccounting; // 0x8
    ULONGLONG IdleTimeLast;                       // 0x10
    ULONGLONG IdleTimeTotal;                      // 0x18
    volatile ULONGLONG IdleSequenceNumber;        // 0x20
    ULONGLONG IdleTimeEntry;                      // 0x28
    union {
        ULONGLONG IdleTimeExpiration;   // 0x30
        volatile LONGLONG IdleWakeTime; // 0x30
    };
    UCHAR NonInterruptibleTransition;                         // 0x38
    UCHAR PepWokenTransition;                                 // 0x39
    UCHAR HvTargetState;                                      // 0x3a
    UCHAR SoftParked;                                         // 0x3b
    ULONG TargetIdleState;                                    // 0x3c
    struct _PROC_IDLE_POLICY IdlePolicy;                      // 0x40
    union _PPM_IDLE_SYNCHRONIZATION_STATE Synchronization;    // 0x48
    struct _PROC_FEEDBACK PerfFeedback;                       // 0x50
    enum _PROC_HYPERVISOR_STATE Hypervisor;                   // 0xe0
    ULONG LastSysTime;                                        // 0xe4
    ULONGLONG WmiDispatchPtr;                                 // 0xe8
    LONG WmiInterfaceEnabled;                                 // 0xf0
    struct _PPM_FFH_THROTTLE_STATE_INFO FFHThrottleStateInfo; // 0xf8
    struct _KDPC PerfActionDpc;                               // 0x118
    volatile LONG PerfActionMask;                             // 0x158
    struct _PROC_IDLE_SNAP HvIdleCheck;                       // 0x160
    struct _PROC_PERF_CHECK_CONTEXT CheckContext;             // 0x170
    struct _PPM_CONCURRENCY_ACCOUNTING *Concurrency;          // 0x1b8
    struct _PPM_CONCURRENCY_ACCOUNTING *ClassConcurrency;     // 0x1c0
    UCHAR ArchitecturalEfficiencyClass;                       // 0x1c8
    UCHAR PerformanceSchedulingClass;                         // 0x1c9
    UCHAR EfficiencySchedulingClass;                          // 0x1ca
    UCHAR EarlyBootArchitecturalEfficiencyClass;              // 0x1cb
    UCHAR Parked;                                             // 0x1cc
    UCHAR LongPriorQosPeriod;                                 // 0x1cd
    union {
        ULONGLONG SnapTimeLast;   // 0x1d0
        ULONGLONG EnergyConsumed; // 0x1d0
    };
    ULONGLONG ActiveTime;                                               // 0x1d8
    ULONGLONG TotalTime;                                                // 0x1e0
    struct _POP_FX_DEVICE *FxDevice;                                    // 0x1e8
    ULONGLONG LastQosTranstionTsc;                                      // 0x1f0
    ULONGLONG QosTransitionHysteresis;                                  // 0x1f8
    enum _KHETERO_CPU_QOS RequestedQosClass;                            // 0x200
    enum _KHETERO_CPU_QOS ResolvedQosClass;                             // 0x204
    USHORT QosEquivalencyMask;                                          // 0x208
    USHORT HwFeedbackTableOffset;                                       // 0x20a
    UCHAR HwFeedbackParkHint;                                           // 0x20c
    UCHAR HeteroCoreType;                                               // 0x20d
    USHORT HwFeedbackTableIndex;                                        // 0x20e
    struct _KHETRO_HWFEEDBACK_TYPE *HwFeedbackClassList;                // 0x210
    struct _PROCESSOR_CYCLES_WORKLOAD_CLASS *EeCyclesWorkloadClassList; // 0x218
    struct _PROCESSOR_CYCLES_WORKLOAD_CLASS
        *PerfCyclesWorkloadClassList; // 0x220
    UCHAR NotUsed;                    // 0x228
};

// 0xb8 bytes (sizeof)
struct _SYNCH_COUNTERS {
    ULONG SpinLockAcquireCount;                                  // 0x0
    ULONG SpinLockContentionCount;                               // 0x4
    ULONG SpinLockSpinCount;                                     // 0x8
    ULONG IpiSendRequestBroadcastCount;                          // 0xc
    ULONG IpiSendRequestRoutineCount;                            // 0x10
    ULONG IpiSendSoftwareInterruptCount;                         // 0x14
    ULONG ExInitializeResourceCount;                             // 0x18
    ULONG ExReInitializeResourceCount;                           // 0x1c
    ULONG ExDeleteResourceCount;                                 // 0x20
    ULONG ExecutiveResourceAcquiresCount;                        // 0x24
    ULONG ExecutiveResourceContentionsCount;                     // 0x28
    ULONG ExecutiveResourceReleaseExclusiveCount;                // 0x2c
    ULONG ExecutiveResourceReleaseSharedCount;                   // 0x30
    ULONG ExecutiveResourceConvertsCount;                        // 0x34
    ULONG ExAcqResExclusiveAttempts;                             // 0x38
    ULONG ExAcqResExclusiveAcquiresExclusive;                    // 0x3c
    ULONG ExAcqResExclusiveAcquiresExclusiveRecursive;           // 0x40
    ULONG ExAcqResExclusiveWaits;                                // 0x44
    ULONG ExAcqResExclusiveNotAcquires;                          // 0x48
    ULONG ExAcqResSharedAttempts;                                // 0x4c
    ULONG ExAcqResSharedAcquiresExclusive;                       // 0x50
    ULONG ExAcqResSharedAcquiresShared;                          // 0x54
    ULONG ExAcqResSharedAcquiresSharedRecursive;                 // 0x58
    ULONG ExAcqResSharedWaits;                                   // 0x5c
    ULONG ExAcqResSharedNotAcquires;                             // 0x60
    ULONG ExAcqResSharedStarveExclusiveAttempts;                 // 0x64
    ULONG ExAcqResSharedStarveExclusiveAcquiresExclusive;        // 0x68
    ULONG ExAcqResSharedStarveExclusiveAcquiresShared;           // 0x6c
    ULONG ExAcqResSharedStarveExclusiveAcquiresSharedRecursive;  // 0x70
    ULONG ExAcqResSharedStarveExclusiveWaits;                    // 0x74
    ULONG ExAcqResSharedStarveExclusiveNotAcquires;              // 0x78
    ULONG ExAcqResSharedWaitForExclusiveAttempts;                // 0x7c
    ULONG ExAcqResSharedWaitForExclusiveAcquiresExclusive;       // 0x80
    ULONG ExAcqResSharedWaitForExclusiveAcquiresShared;          // 0x84
    ULONG ExAcqResSharedWaitForExclusiveAcquiresSharedRecursive; // 0x88
    ULONG ExAcqResSharedWaitForExclusiveWaits;                   // 0x8c
    ULONG ExAcqResSharedWaitForExclusiveNotAcquires;             // 0x90
    ULONG ExSetResOwnerPointerExclusive;                         // 0x94
    ULONG ExSetResOwnerPointerSharedNew;                         // 0x98
    ULONG ExSetResOwnerPointerSharedOld;                         // 0x9c
    ULONG ExTryToAcqExclusiveAttempts;                           // 0xa0
    ULONG ExTryToAcqExclusiveAcquires;                           // 0xa4
    ULONG ExBoostExclusiveOwner;                                 // 0xa8
    ULONG ExBoostSharedOwners;                                   // 0xac
    ULONG ExEtwSynchTrackingNotificationsCount;                  // 0xb0
    ULONG ExEtwSynchTrackingNotificationsAccountedCount;         // 0xb4
};

// 0x10 bytes (sizeof)
struct _FILESYSTEM_DISK_COUNTERS {
    ULONGLONG FsBytesRead;    // 0x0
    ULONGLONG FsBytesWritten; // 0x8
};

// 0x158 bytes (sizeof)
struct _KENTROPY_TIMING_STATE {
    ULONG EntropyCount;          // 0x0
    ULONG Buffer[64];            // 0x4
    struct _KDPC Dpc;            // 0x108
    ULONG LastDeliveredBuffer;   // 0x148
    VOID *ReservedRawDataBuffer; // 0x150
};

// 0x1 bytes (sizeof)
union _KPRIORITY_STATE {
    UCHAR Priority : 7;       // 0x0
    UCHAR IsolationWidth : 1; // 0x0
    UCHAR AllFields;          // 0x0
};

// 0x1 bytes (sizeof)
union _KHETERO_STATE {
    UCHAR Qos : 3;           // 0x0
    UCHAR WorkloadClass : 3; // 0x0
    UCHAR RunningType : 1;   // 0x0
    UCHAR AllFields;         // 0x0
};

// 0x380 bytes (sizeof)
struct _KSHARED_READY_QUEUE_HETERO_STATISTICS {
    ULONGLONG ExpectedRuntimeByClass[7][2][8]; // 0x0
};

// 0x680 bytes (sizeof)
struct _KSHARED_READY_QUEUE {
    ULONGLONG Lock;                                // 0x0
    ULONG ReadySummary;                            // 0x8
    struct _LIST_ENTRY ReadyListHead[32];          // 0x10
    union _KPRIORITY_STATE RunningSummary[64];     // 0x210
    union _KHETERO_STATE HeteroRunningSummary[64]; // 0x250
    UCHAR Span;                                    // 0x290
    UCHAR LowProcIndex;                            // 0x291
    UCHAR QueueIndex;                              // 0x292
    UCHAR NormalPriorityQueueIndex;                // 0x293
    UCHAR ProcCount;                               // 0x294
    UCHAR ScanOwner;                               // 0x295
    UCHAR Spare1[2];                               // 0x296
    ULONGLONG Affinity;                            // 0x298
    ULONG ReadyThreadCount;                        // 0x2a0
    UCHAR SoftParkElectionScheduled;               // 0x2a4
    UCHAR Spare2[3];                               // 0x2a5
    ULONGLONG ReadyQueueExpectedRunTime;           // 0x2a8
    ULONGLONG SoftParkElectionGeneration;          // 0x2b0
    ULONGLONG SoftParkElectionRunTime;             // 0x2b8
    struct _KDPC SoftParkElectionDpc;              // 0x2c0
    struct _KSHARED_READY_QUEUE_HETERO_STATISTICS
        SoftParkElectionHeteroStats; // 0x300
};

// 0xbf00 bytes (sizeof)
struct _KPRCB {
    ULONG MxCsr;                    // 0x0
    UCHAR LegacyNumber;             // 0x4
    UCHAR ReservedMustBeZero;       // 0x5
    UCHAR InterruptRequest;         // 0x6
    UCHAR IdleHalt;                 // 0x7
    struct _KTHREAD *CurrentThread; // 0x8
    struct _KTHREAD *NextThread;    // 0x10
    struct _KTHREAD *IdleThread;    // 0x18
    UCHAR NestingLevel;             // 0x20
    UCHAR ClockOwner;               // 0x21
    union {
        UCHAR PendingTickFlags; // 0x22
        struct {
            UCHAR PendingTick : 1;       // 0x22
            UCHAR PendingBackupTick : 1; // 0x22
        };
    };
    UCHAR IdleState;                       // 0x23
    ULONG Number;                          // 0x24
    ULONGLONG RspBase;                     // 0x28
    ULONGLONG PrcbLock;                    // 0x30
    union _KPRIORITY_STATE *PriorityState; // 0x38
    CHAR CpuType;                          // 0x40
    CHAR CpuID;                            // 0x41
    union {
        USHORT CpuStep; // 0x42
        struct {
            UCHAR CpuStepping; // 0x42
            UCHAR CpuModel;    // 0x43
        };
    };
    ULONG MHz;                                    // 0x44
    ULONGLONG HalReserved[8];                     // 0x48
    USHORT MinorVersion;                          // 0x88
    USHORT MajorVersion;                          // 0x8a
    UCHAR BuildType;                              // 0x8c
    UCHAR CpuVendor;                              // 0x8d
    UCHAR LegacyCoresPerPhysicalProcessor;        // 0x8e
    UCHAR LegacyLogicalProcessorsPerCore;         // 0x8f
    ULONGLONG TscFrequency;                       // 0x90
    struct _KPRCB_TRACEPOINT_LOG *TracepointLog;  // 0x98
    ULONG CoresPerPhysicalProcessor;              // 0xa0
    ULONG LogicalProcessorsPerCore;               // 0xa4
    ULONGLONG PrcbPad04[3];                       // 0xa8
    struct _KSCHEDULER_SUBNODE *SchedulerSubNode; // 0xc0
    ULONGLONG GroupSetMember;                     // 0xc8
    UCHAR Group;                                  // 0xd0
    UCHAR GroupIndex;                             // 0xd1
    UCHAR PrcbPad05[2];                           // 0xd2
    ULONG InitialApicId;                          // 0xd4
    ULONG ScbOffset;                              // 0xd8
    ULONG ApicMask;                               // 0xdc
    VOID *AcpiReserved;                           // 0xe0
    ULONG CFlushSize;                             // 0xe8
    union _KPRCBFLAG PrcbFlags;                   // 0xec
    union {
        struct {
            ULONGLONG TrappedSecurityDomain; // 0xf0
            union {
                UCHAR BpbState; // 0xf8
                struct {
                    UCHAR BpbCpuIdle : 1;             // 0xf8
                    UCHAR BpbFlushRsbOnTrap : 1;      // 0xf8
                    UCHAR BpbIbpbOnReturn : 1;        // 0xf8
                    UCHAR BpbIbpbOnTrap : 1;          // 0xf8
                    UCHAR BpbIbpbOnRetpolineExit : 1; // 0xf8
                    UCHAR BpbStateReserved : 3;       // 0xf8
                };
            };
            union {
                UCHAR BpbFeatures; // 0xf9
                struct {
                    UCHAR BpbClearOnIdle : 1;      // 0xf9
                    UCHAR BpbEnabled : 1;          // 0xf9
                    UCHAR BpbSmep : 1;             // 0xf9
                    UCHAR BpbFeaturesReserved : 5; // 0xf9
                };
            };
            UCHAR BpbCurrentSpecCtrl;    // 0xfa
            UCHAR BpbKernelSpecCtrl;     // 0xfb
            UCHAR BpbNmiSpecCtrl;        // 0xfc
            UCHAR BpbUserSpecCtrl;       // 0xfd
            volatile SHORT PairRegister; // 0xfe
        };
        ULONGLONG PrcbPad11[2]; // 0xf0
    };
    struct _KPROCESSOR_STATE ProcessorState;            // 0x100
    struct _XSAVE_AREA_HEADER *ExtendedSupervisorState; // 0x6c0
    ULONG ProcessorSignature;                           // 0x6c8
    ULONG ProcessorFlags;                               // 0x6cc
    union {
        struct {
            UCHAR BpbRetpolineExitSpecCtrl;        // 0x6d0
            UCHAR BpbTrappedRetpolineExitSpecCtrl; // 0x6d1
            union {
                UCHAR BpbTrappedBpbState; // 0x6d2
                struct {
                    UCHAR BpbTrappedCpuIdle : 1;             // 0x6d2
                    UCHAR BpbTrappedFlushRsbOnTrap : 1;      // 0x6d2
                    UCHAR BpbTrappedIbpbOnReturn : 1;        // 0x6d2
                    UCHAR BpbTrappedIbpbOnTrap : 1;          // 0x6d2
                    UCHAR BpbTrappedIbpbOnRetpolineExit : 1; // 0x6d2
                    UCHAR BpbtrappedBpbStateReserved : 3;    // 0x6d2
                };
            };
            union {
                UCHAR BpbRetpolineState; // 0x6d3
                struct {
                    UCHAR BpbRunningNonRetpolineCode : 1; // 0x6d3
                    UCHAR BpbIndirectCallsSafe : 1;       // 0x6d3
                    UCHAR BpbRetpolineEnabled : 1;        // 0x6d3
                    UCHAR BpbRetpolineStateReserved : 5;  // 0x6d3
                };
            };
            ULONG PrcbPad12b; // 0x6d4
        };
        ULONGLONG PrcbPad12a; // 0x6d0
    };
    ULONGLONG PrcbPad12[3];                                       // 0x6d8
    struct _KSPIN_LOCK_QUEUE LockQueue[17];                       // 0x6f0
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];                // 0x800
    struct _GENERAL_LOOKASIDE_POOL PPNxPagedLookasideList[32];    // 0x900
    struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];     // 0x1500
    struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];      // 0x2100
    ULONGLONG MsrIa32TsxCtrl;                                     // 0x2d00
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;              // 0x2d08
    volatile LONG MmPageFaultCount;                               // 0x2d10
    volatile LONG MmCopyOnWriteCount;                             // 0x2d14
    volatile LONG MmTransitionCount;                              // 0x2d18
    volatile LONG MmDemandZeroCount;                              // 0x2d1c
    volatile LONG MmPageReadCount;                                // 0x2d20
    volatile LONG MmPageReadIoCount;                              // 0x2d24
    volatile LONG MmDirtyPagesWriteCount;                         // 0x2d28
    volatile LONG MmDirtyWriteIoCount;                            // 0x2d2c
    volatile LONG MmMappedPagesWriteCount;                        // 0x2d30
    volatile LONG MmMappedWriteIoCount;                           // 0x2d34
    ULONG KeSystemCalls;                                          // 0x2d38
    ULONG KeContextSwitches;                                      // 0x2d3c
    ULONG PrcbPad40;                                              // 0x2d40
    ULONG CcFastReadNoWait;                                       // 0x2d44
    ULONG CcFastReadWait;                                         // 0x2d48
    ULONG CcFastReadNotPossible;                                  // 0x2d4c
    ULONG CcCopyReadNoWait;                                       // 0x2d50
    ULONG CcCopyReadWait;                                         // 0x2d54
    ULONG CcCopyReadNoWaitMiss;                                   // 0x2d58
    volatile LONG IoReadOperationCount;                           // 0x2d5c
    volatile LONG IoWriteOperationCount;                          // 0x2d60
    volatile LONG IoOtherOperationCount;                          // 0x2d64
    union _LARGE_INTEGER IoReadTransferCount;                     // 0x2d68
    union _LARGE_INTEGER IoWriteTransferCount;                    // 0x2d70
    union _LARGE_INTEGER IoOtherTransferCount;                    // 0x2d78
    volatile LONG PacketBarrier;                                  // 0x2d80
    volatile LONG TargetCount;                                    // 0x2d84
    volatile ULONG IpiFrozen;                                     // 0x2d88
    ULONG PrcbPad30;                                              // 0x2d8c
    VOID *IsrDpcStats;                                            // 0x2d90
    ULONG DeviceInterrupts;                                       // 0x2d98
    LONG LookasideIrpFloat;                                       // 0x2d9c
    ULONG InterruptLastCount;                                     // 0x2da0
    ULONG InterruptRate;                                          // 0x2da4
    ULONGLONG PrcbPad31;                                          // 0x2da8
    struct _KPRCB *PairPrcb;                                      // 0x2db0
    struct _KSTATIC_AFFINITY_BLOCK StaticAffinity;                // 0x2db8
    struct _KSOFTWARE_INTERRUPT_BATCH DeferredDispatchInterrupts; // 0x31d8
    ULONGLONG PrcbPad35[3];                                       // 0x32e8
    union _SLIST_HEADER InterruptObjectPool;                      // 0x3300
    struct _RTL_HASH_TABLE *DpcRuntimeHistoryHashTable;           // 0x3310
    struct _KDPC *DpcRuntimeHistoryHashTableCleanupDpc;           // 0x3318
    VOID (*CurrentDpcRoutine)
    (struct _KDPC *arg1, VOID *arg2, VOID *arg3, VOID *arg4); // 0x3320
    ULONGLONG CurrentDpcRuntimeHistoryCached;                 // 0x3328
    ULONGLONG CurrentDpcStartTime;                            // 0x3330
    struct _KTHREAD *DpcDelegateThread;                       // 0x3338
    struct _KDPC_DATA DpcData[2];                             // 0x3340
    VOID *DpcStack;                                           // 0x33a0
    LONG MaximumDpcQueueDepth;                                // 0x33a8
    ULONG DpcRequestRate;                                     // 0x33ac
    ULONG MinimumDpcRate;                                     // 0x33b0
    ULONG DpcLastCount;                                       // 0x33b4
    UCHAR ThreadDpcEnable;                                    // 0x33b8
    volatile UCHAR QuantumEnd;                                // 0x33b9
    volatile UCHAR DpcRoutineActive;                          // 0x33ba
    volatile UCHAR IdleSchedule;                              // 0x33bb
    union {
        volatile LONG DpcRequestSummary; // 0x33bc
        SHORT DpcRequestSlot[2];         // 0x33bc
        struct {
            SHORT NormalDpcState; // 0x33bc
            SHORT ThreadDpcState; // 0x33be
        };
        struct {
            ULONG DpcNormalProcessingActive : 1;       // 0x33bc
            ULONG DpcNormalProcessingRequested : 1;    // 0x33bc
            ULONG DpcNormalThreadSignal : 1;           // 0x33bc
            ULONG DpcNormalTimerExpiration : 1;        // 0x33bc
            ULONG DpcNormalDpcPresent : 1;             // 0x33bc
            ULONG DpcNormalLocalInterrupt : 1;         // 0x33bc
            ULONG DpcNormalPriorityAntiStarvation : 1; // 0x33bc
            ULONG DpcNormalSwapToDpcDelegate : 1;      // 0x33bc
            ULONG DpcNormalSpare : 8;                  // 0x33bc
            ULONG DpcThreadActive : 1;                 // 0x33bc
            ULONG DpcThreadRequested : 1;              // 0x33bc
            ULONG DpcThreadSpare : 14;                 // 0x33bc
        };
    };
    ULONG LastTick;                    // 0x33c0
    ULONG ClockInterrupts;             // 0x33c4
    ULONG ReadyScanTick;               // 0x33c8
    ULONG SingleDpcSoftTimeLimitTicks; // 0x33cc
    struct _KSINGLE_DPC_SOFT_TIMEOUT_EVENT_INFO
        *SingleDpcSoftTimeoutEventInfo;    // 0x33d0
    ULONG CumulativeDpcSoftTimeLimitTicks; // 0x33d8
    ULONG DpcWatchdogProfileBufferSize;    // 0x33dc
    ULONG PrcbPad93[8];                    // 0x33e0
    VOID *InterruptObject[256];            // 0x3400
    struct _KTIMER_TABLE TimerTable;       // 0x3c00
    ULONG PrcbPad92[10];                   // 0x7e18
    struct _KGATE DpcGate;                 // 0x7e40
    VOID *PrcbPad52;                       // 0x7e58
    struct _KDPC CallDpc;                  // 0x7e60
    LONG ClockKeepAlive;                   // 0x7ea0
    UCHAR PrcbPad60[2];                    // 0x7ea4
    union {
        struct {
            UCHAR NmiActive; // 0x7ea6
            UCHAR MceActive; // 0x7ea7
        };
        USHORT CombinedNmiMceActive; // 0x7ea6
    };
    LONG DpcWatchdogPeriodTicks;                         // 0x7ea8
    LONG DpcWatchdogCount;                               // 0x7eac
    volatile LONG KeSpinLockOrdering;                    // 0x7eb0
    ULONG DpcWatchdogProfileCumulativeDpcThresholdTicks; // 0x7eb4
    VOID *CachedPtes;                                    // 0x7eb8
    struct _LIST_ENTRY WaitListHead;                     // 0x7ec0
    ULONGLONG WaitLock;                                  // 0x7ed0
    ULONG ReadySummary;                                  // 0x7ed8
    LONG AffinitizedSelectionMask;                       // 0x7edc
    ULONG QueueIndex;                                    // 0x7ee0
    ULONG NormalPriorityQueueIndex;                      // 0x7ee4
    ULONG NormalPriorityReadyScanTick;                   // 0x7ee8
    ULONG DpcWatchdogSequenceNumber;                     // 0x7eec
    struct _KDPC TimerExpirationDpc;                     // 0x7ef0
    struct _RTL_RB_TREE ScbQueue;                        // 0x7f30
    struct _LIST_ENTRY DispatcherReadyListHead[32];      // 0x7f40
    ULONG InterruptCount;                                // 0x8140
    ULONG KernelTime;                                    // 0x8144
    ULONG UserTime;                                      // 0x8148
    ULONG DpcTime;                                       // 0x814c
    ULONG InterruptTime;                                 // 0x8150
    ULONG AdjustDpcThreshold;                            // 0x8154
    UCHAR DebuggerSavedIRQL;                             // 0x8158
    UCHAR GroupSchedulingOverQuota;                      // 0x8159
    volatile UCHAR DeepSleep;                            // 0x815a
    UCHAR PrcbPad80;                                     // 0x815b
    ULONG DpcTimeCount;                                  // 0x815c
    ULONG DpcTimeLimitTicks;                             // 0x8160
    ULONG PeriodicCount;                                 // 0x8164
    ULONG PeriodicBias;                                  // 0x8168
    ULONG AvailableTime;                                 // 0x816c
    ULONG KeExceptionDispatchCount;                      // 0x8170
    ULONG ReadyThreadCount;                              // 0x8174
    ULONGLONG ReadyQueueExpectedRunTime;                 // 0x8178
    ULONGLONG StartCycles;                               // 0x8180
    ULONGLONG TaggedCycles[4];                           // 0x8188
    ULONGLONG AffinitizedCycles;                         // 0x81a8
    ULONGLONG *CyclesByThreadType;                       // 0x81b0
    ULONG CpuCycleScalingFactor;                         // 0x81b8
    USHORT PerformanceScoreByClass[8];                   // 0x81bc
    USHORT EfficiencyScoreByClass[8];                    // 0x81cc
    ULONG PrcbPad83[25];                                 // 0x81dc
    ULONG DpcWatchdogProfileSingleDpcThresholdTicks;     // 0x8240
    LONG PrcbPad82;                                      // 0x8244
    VOID *volatile CachedStack;                          // 0x8248
    ULONG PageColor;                                     // 0x8250
    ULONG NodeColor;                                     // 0x8254
    ULONG NodeShiftedColor;                              // 0x8258
    ULONG SecondaryColorMask;                            // 0x825c
    UCHAR PrcbPad81[5];                                  // 0x8260
    UCHAR SystemWorkKickInProgress;                      // 0x8265
    UCHAR ExceptionStackActive;                          // 0x8266
    UCHAR TbFlushListActive;                             // 0x8267
    VOID *ExceptionStack;                                // 0x8268
    volatile LONGLONG MmSpinLockOrdering;                // 0x8270
    ULONGLONG CycleTime;                                 // 0x8278
    ULONGLONG Cycles[4][2];                              // 0x8280
    ULONG CcFastMdlReadNoWait;                           // 0x82c0
    ULONG CcFastMdlReadWait;                             // 0x82c4
    ULONG CcFastMdlReadNotPossible;                      // 0x82c8
    ULONG CcMapDataNoWait;                               // 0x82cc
    ULONG CcMapDataWait;                                 // 0x82d0
    ULONG CcPinMappedDataCount;                          // 0x82d4
    ULONG CcPinReadNoWait;                               // 0x82d8
    ULONG CcPinReadWait;                                 // 0x82dc
    ULONG CcMdlReadNoWait;                               // 0x82e0
    ULONG CcMdlReadWait;                                 // 0x82e4
    ULONG CcLazyWriteHotSpots;                           // 0x82e8
    ULONG CcLazyWriteIos;                                // 0x82ec
    ULONG CcLazyWritePages;                              // 0x82f0
    ULONG CcDataFlushes;                                 // 0x82f4
    ULONG CcDataPages;                                   // 0x82f8
    ULONG CcLostDelayedWrites;                           // 0x82fc
    ULONG CcFastReadResourceMiss;                        // 0x8300
    ULONG CcCopyReadWaitMiss;                            // 0x8304
    ULONG CcFastMdlReadResourceMiss;                     // 0x8308
    ULONG CcMapDataNoWaitMiss;                           // 0x830c
    ULONG CcMapDataWaitMiss;                             // 0x8310
    ULONG CcPinReadNoWaitMiss;                           // 0x8314
    ULONG CcPinReadWaitMiss;                             // 0x8318
    ULONG CcMdlReadNoWaitMiss;                           // 0x831c
    ULONG CcMdlReadWaitMiss;                             // 0x8320
    ULONG CcReadAheadIos;                                // 0x8324
    volatile LONG MmCacheTransitionCount;                // 0x8328
    volatile LONG MmCacheReadCount;                      // 0x832c
    volatile LONG MmCacheIoCount;                        // 0x8330
    ULONG PrcbPad91;                                     // 0x8334
    VOID *MmInternal;                                    // 0x8338
    struct _PROCESSOR_POWER_STATE PowerState;            // 0x8340
    ULONGLONG PrcbPad96[2];                              // 0x8570
    VOID *PrcbPad90;                                     // 0x8580
    struct _LIST_ENTRY ScbList;                          // 0x8588
    struct _KDPC ForceIdleDpc;                           // 0x8598
    struct _KDPC DpcWatchdogDpc;                         // 0x85d8
    ULONGLONG PrcbPad98[8];                              // 0x8618
    struct _CACHE_DESCRIPTOR Cache[5];                   // 0x8658
    ULONG CacheCount;                                    // 0x8694
    volatile ULONG CachedCommit;                         // 0x8698
    volatile ULONG CachedResidentAvailable;              // 0x869c
    VOID *WheaInfo;                                      // 0x86a0
    VOID *EtwSupport;                                    // 0x86a8
    VOID *ExSaPageArray;                                 // 0x86b0
    ULONG KeAlignmentFixupCount;                         // 0x86b8
    ULONG PrcbPad95;                                     // 0x86bc
    union _SLIST_HEADER HypercallPageList;               // 0x86c0
    ULONGLONG *StatisticsPage;                           // 0x86d0
    ULONGLONG GenerationTarget;                          // 0x86d8
    ULONGLONG PrcbPad85[4];                              // 0x86e0
    VOID *HypercallCachedPages;                          // 0x8700
    VOID *VirtualApicAssist;                             // 0x8708
    struct _KAFFINITY_EX PackageProcessorSet;            // 0x8710
    union {
        struct {
            ULONG ProcessorId; // 0x8818
            ULONG CoreId;      // 0x881c
            ULONG ModuleId;    // 0x8820
            ULONG DieId;       // 0x8824
            ULONG PackageId;   // 0x8828
        };
        ULONG TopologyId[5]; // 0x8818
    };
    ULONG NodeRelativeTopologyIndex[5];            // 0x882c
    ULONGLONG SharedReadyQueueMask;                // 0x8840
    struct _KSHARED_READY_QUEUE *SharedReadyQueue; // 0x8848
    ULONG SharedQueueScanOwner;                    // 0x8850
    ULONG ScanSiblingIndex;                        // 0x8854
    struct _KCORE_CONTROL_BLOCK *CoreControlBlock; // 0x8858
    ULONGLONG CoreProcessorSet;                    // 0x8860
    ULONGLONG ScanSiblingMask;                     // 0x8868
    ULONGLONG LLCMask;                             // 0x8870
    ULONGLONG GroupModuleProcessorSet;             // 0x8878
    struct _KTHREAD *SmtIsolationThread;           // 0x8880
    ULONGLONG PrcbPad97[2];                        // 0x8888
    struct _PROCESSOR_PROFILE_CONTROL_AREA
        *ProcessorProfileControlArea;                 // 0x8898
    VOID *ProfileEventIndexAddress;                   // 0x88a0
    VOID **DpcWatchdogProfile;                        // 0x88a8
    VOID **DpcWatchdogProfileCurrentEmptyCapture;     // 0x88b0
    VOID *SchedulerAssist;                            // 0x88b8
    struct _SYNCH_COUNTERS SynchCounters;             // 0x88c0
    ULONGLONG PrcbPad94;                              // 0x8978
    struct _FILESYSTEM_DISK_COUNTERS FsCounters;      // 0x8980
    UCHAR VendorString[13];                           // 0x8990
    UCHAR PrcbPad100[3];                              // 0x899d
    ULONGLONG FeatureBits;                            // 0x89a0
    union _LARGE_INTEGER UpdateSignature;             // 0x89a8
    ULONGLONG PteBitCache;                            // 0x89b0
    ULONG PteBitOffset;                               // 0x89b8
    ULONG PrcbPad105;                                 // 0x89bc
    struct _CONTEXT *Context;                         // 0x89c0
    ULONG ContextFlagsInit;                           // 0x89c8
    ULONG PrcbPad115;                                 // 0x89cc
    struct _XSAVE_AREA *ExtendedState;                // 0x89d0
    VOID *IsrStack;                                   // 0x89d8
    struct _KENTROPY_TIMING_STATE EntropyTimingState; // 0x89e0
    struct {
        ULONG UpdateCycle; // 0x8b38
        union {
            SHORT PairLocal; // 0x8b3c
            struct {
                UCHAR PairLocalLow;            // 0x8b3c
                UCHAR PairLocalForceStibp : 1; // 0x8b3d
                UCHAR Reserved : 4;            // 0x8b3d
                UCHAR Frozen : 1;              // 0x8b3d
                UCHAR ForceUntrusted : 1;      // 0x8b3d
                UCHAR SynchIpi : 1;            // 0x8b3d
            };
        };
        union {
            SHORT PairRemote; // 0x8b3e
            struct {
                UCHAR PairRemoteLow; // 0x8b3e
                UCHAR Reserved2;     // 0x8b3f
            };
        };
        UCHAR Trace[24];                             // 0x8b40
        ULONGLONG LocalDomain;                       // 0x8b58
        ULONGLONG RemoteDomain;                      // 0x8b60
        struct _KTHREAD *Thread;                     // 0x8b68
    } StibpPairingTrace;                             // 0x8b38
    struct _SINGLE_LIST_ENTRY AbSelfIoBoostsList;    // 0x8b70
    struct _SINGLE_LIST_ENTRY AbPropagateBoostsList; // 0x8b78
    struct _KDPC AbDpc;                              // 0x8b80
};
#pragma warning(pop)

// 0x4 bytes (sizeof)
enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation = 0,
    SystemProcessorInformation = 1,
    SystemPerformanceInformation = 2,
    SystemTimeOfDayInformation = 3,
    SystemPathInformation = 4,
    SystemProcessInformation = 5,
    SystemCallCountInformation = 6,
    SystemDeviceInformation = 7,
    SystemProcessorPerformanceInformation = 8,
    SystemFlagsInformation = 9,
    SystemCallTimeInformation = 10,
    SystemModuleInformation = 11,
    SystemLocksInformation = 12,
    SystemStackTraceInformation = 13,
    SystemPagedPoolInformation = 14,
    SystemNonPagedPoolInformation = 15,
    SystemHandleInformation = 16,
    SystemObjectInformation = 17,
    SystemPageFileInformation = 18,
    SystemVdmInstemulInformation = 19,
    SystemVdmBopInformation = 20,
    SystemFileCacheInformation = 21,
    SystemPoolTagInformation = 22,
    SystemInterruptInformation = 23,
    SystemDpcBehaviorInformation = 24,
    SystemFullMemoryInformation = 25,
    SystemLoadGdiDriverInformation = 26,
    SystemUnloadGdiDriverInformation = 27,
    SystemTimeAdjustmentInformation = 28,
    SystemSummaryMemoryInformation = 29,
    SystemMirrorMemoryInformation = 30,
    SystemPerformanceTraceInformation = 31,
    SystemObsolete0 = 32,
    SystemExceptionInformation = 33,
    SystemCrashDumpStateInformation = 34,
    SystemKernelDebuggerInformation = 35,
    SystemContextSwitchInformation = 36,
    SystemRegistryQuotaInformation = 37,
    SystemExtendServiceTableInformation = 38,
    SystemPrioritySeperation = 39,
    SystemVerifierAddDriverInformation = 40,
    SystemVerifierRemoveDriverInformation = 41,
    SystemProcessorIdleInformation = 42,
    SystemLegacyDriverInformation = 43,
    SystemCurrentTimeZoneInformation = 44,
    SystemLookasideInformation = 45,
    SystemTimeSlipNotification = 46,
    SystemSessionCreate = 47,
    SystemSessionDetach = 48,
    SystemSessionInformation = 49,
    SystemRangeStartInformation = 50,
    SystemVerifierInformation = 51,
    SystemVerifierThunkExtend = 52,
    SystemSessionProcessInformation = 53,
    SystemLoadGdiDriverInSystemSpace = 54,
    SystemNumaProcessorMap = 55,
    SystemPrefetcherInformation = 56,
    SystemExtendedProcessInformation = 57,
    SystemRecommendedSharedDataAlignment = 58,
    SystemComPlusPackage = 59,
    SystemNumaAvailableMemory = 60,
    SystemProcessorPowerInformation = 61,
    SystemEmulationBasicInformation = 62,
    SystemEmulationProcessorInformation = 63,
    SystemExtendedHandleInformation = 64,
    SystemLostDelayedWriteInformation = 65,
    SystemBigPoolInformation = 66,
    SystemSessionPoolTagInformation = 67,
    SystemSessionMappedViewInformation = 68,
    SystemHotpatchInformation = 69,
    SystemObjectSecurityMode = 70,
    SystemWatchdogTimerHandler = 71,
    SystemWatchdogTimerInformation = 72,
    SystemLogicalProcessorInformation = 73,
    SystemWow64SharedInformationObsolete = 74,
    SystemRegisterFirmwareTableInformationHandler = 75,
    SystemFirmwareTableInformation = 76,
    SystemModuleInformationEx = 77,
    SystemVerifierTriageInformation = 78,
    SystemSuperfetchInformation = 79,
    SystemMemoryListInformation = 80,
    SystemFileCacheInformationEx = 81,
    SystemThreadPriorityClientIdInformation = 82,
    SystemProcessorIdleCycleTimeInformation = 83,
    SystemVerifierCancellationInformation = 84,
    SystemProcessorPowerInformationEx = 85,
    SystemRefTraceInformation = 86,
    SystemSpecialPoolInformation = 87,
    SystemProcessIdInformation = 88,
    SystemErrorPortInformation = 89,
    SystemBootEnvironmentInformation = 90,
    SystemHypervisorInformation = 91,
    SystemVerifierInformationEx = 92,
    SystemTimeZoneInformation = 93,
    SystemImageFileExecutionOptionsInformation = 94,
    SystemCoverageInformation = 95,
    SystemPrefetchPatchInformation = 96,
    SystemVerifierFaultsInformation = 97,
    SystemSystemPartitionInformation = 98,
    SystemSystemDiskInformation = 99,
    SystemProcessorPerformanceDistribution = 100,
    SystemNumaProximityNodeInformation = 101,
    SystemDynamicTimeZoneInformation = 102,
    SystemCodeIntegrityInformation = 103,
    SystemProcessorMicrocodeUpdateInformation = 104,
    SystemProcessorBrandString = 105,
    SystemVirtualAddressInformation = 106,
    SystemLogicalProcessorAndGroupInformation = 107,
    SystemProcessorCycleTimeInformation = 108,
    SystemStoreInformation = 109,
    SystemRegistryAppendString = 110,
    SystemAitSamplingValue = 111,
    SystemVhdBootInformation = 112,
    SystemCpuQuotaInformation = 113,
    SystemNativeBasicInformation = 114,
    SystemErrorPortTimeouts = 115,
    SystemLowPriorityIoInformation = 116,
    SystemBootEntropyInformation = 117,
    SystemVerifierCountersInformation = 118,
    SystemPagedPoolInformationEx = 119,
    SystemSystemPtesInformationEx = 120,
    SystemNodeDistanceInformation = 121,
    SystemAcpiAuditInformation = 122,
    SystemBasicPerformanceInformation = 123,
    SystemQueryPerformanceCounterInformation = 124,
    SystemSessionBigPoolInformation = 125,
    SystemBootGraphicsInformation = 126,
    SystemScrubPhysicalMemoryInformation = 127,
    SystemBadPageInformation = 128,
    SystemProcessorProfileControlArea = 129,
    SystemCombinePhysicalMemoryInformation = 130,
    SystemEntropyInterruptTimingInformation = 131,
    SystemConsoleInformation = 132,
    SystemPlatformBinaryInformation = 133,
    SystemPolicyInformation = 134,
    SystemHypervisorProcessorCountInformation = 135,
    SystemDeviceDataInformation = 136,
    SystemDeviceDataEnumerationInformation = 137,
    SystemMemoryTopologyInformation = 138,
    SystemMemoryChannelInformation = 139,
    SystemBootLogoInformation = 140,
    SystemProcessorPerformanceInformationEx = 141,
    SystemCriticalProcessErrorLogInformation = 142,
    SystemSecureBootPolicyInformation = 143,
    SystemPageFileInformationEx = 144,
    SystemSecureBootInformation = 145,
    SystemEntropyInterruptTimingRawInformation = 146,
    SystemPortableWorkspaceEfiLauncherInformation = 147,
    SystemFullProcessInformation = 148,
    SystemKernelDebuggerInformationEx = 149,
    SystemBootMetadataInformation = 150,
    SystemSoftRebootInformation = 151,
    SystemElamCertificateInformation = 152,
    SystemOfflineDumpConfigInformation = 153,
    SystemProcessorFeaturesInformation = 154,
    SystemRegistryReconciliationInformation = 155,
    SystemEdidInformation = 156,
    SystemManufacturingInformation = 157,
    SystemEnergyEstimationConfigInformation = 158,
    SystemHypervisorDetailInformation = 159,
    SystemProcessorCycleStatsInformation = 160,
    SystemVmGenerationCountInformation = 161,
    SystemTrustedPlatformModuleInformation = 162,
    SystemKernelDebuggerFlags = 163,
    SystemCodeIntegrityPolicyInformation = 164,
    SystemIsolatedUserModeInformation = 165,
    SystemHardwareSecurityTestInterfaceResultsInformation = 166,
    SystemSingleModuleInformation = 167,
    SystemAllowedCpuSetsInformation = 168,
    SystemVsmProtectionInformation = 169,
    SystemInterruptCpuSetsInformation = 170,
    SystemSecureBootPolicyFullInformation = 171,
    SystemCodeIntegrityPolicyFullInformation = 172,
    SystemAffinitizedInterruptProcessorInformation = 173,
    SystemRootSiloInformation = 174,
    SystemCpuSetInformation = 175,
    SystemCpuSetTagInformation = 176,
    SystemWin32WerStartCallout = 177,
    SystemSecureKernelProfileInformation = 178,
    SystemCodeIntegrityPlatformManifestInformation = 179,
    SystemInterruptSteeringInformation = 180,
    SystemSupportedProcessorArchitectures = 181,
    SystemMemoryUsageInformation = 182,
    SystemCodeIntegrityCertificateInformation = 183,
    SystemPhysicalMemoryInformation = 184,
    SystemControlFlowTransition = 185,
    SystemKernelDebuggingAllowed = 186,
    SystemActivityModerationExeState = 187,
    SystemActivityModerationUserSettings = 188,
    SystemCodeIntegrityPoliciesFullInformation = 189,
    SystemCodeIntegrityUnlockInformation = 190,
    SystemIntegrityQuotaInformation = 191,
    SystemFlushInformation = 192,
    SystemProcessorIdleMaskInformation = 193,
    SystemSecureDumpEncryptionInformation = 194,
    SystemWriteConstraintInformation = 195,
    SystemKernelVaShadowInformation = 196,
    SystemHypervisorSharedPageInformation = 197,
    SystemFirmwareBootPerformanceInformation = 198,
    SystemCodeIntegrityVerificationInformation = 199,
    SystemFirmwarePartitionInformation = 200,
    SystemSpeculationControlInformation = 201,
    SystemDmaGuardPolicyInformation = 202,
    SystemEnclaveLaunchControlInformation = 203,
    SystemWorkloadAllowedCpuSetsInformation = 204,
    SystemCodeIntegrityUnlockModeInformation = 205,
    SystemLeapSecondInformation = 206,
    SystemFlags2Information = 207,
    SystemSecurityModelInformation = 208,
    SystemCodeIntegritySyntheticCacheInformation = 209,
    SystemFeatureConfigurationInformation = 210,
    SystemFeatureConfigurationSectionInformation = 211,
    SystemFeatureUsageSubscriptionInformation = 212,
    SystemSecureSpeculationControlInformation = 213,
    SystemSpacesBootInformation = 214,
    SystemFwRamdiskInformation = 215,
    SystemWheaIpmiHardwareInformation = 216,
    SystemDifSetRuleClassInformation = 217,
    SystemDifClearRuleClassInformation = 218,
    SystemDifApplyPluginVerificationOnDriver = 219,
    SystemDifRemovePluginVerificationOnDriver = 220,
    SystemShadowStackInformation = 221,
    SystemBuildVersionInformation = 222,
    SystemPoolLimitInformation = 223,
    SystemCodeIntegrityAddDynamicStore = 224,
    SystemCodeIntegrityClearDynamicStores = 225,
    SystemPoolZeroingInformation = 227,
    MaxSystemInfoClass = 228
};

typedef struct _RTL_PROCESS_MODULE_INFORMATION {
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES {
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

extern "C" NTSTATUS
ZwQuerySystemInformation(_SYSTEM_INFORMATION_CLASS SystemInformationClass,
                         PVOID SystemInformation, ULONG SystemInformationLength,
                         ULONG *ReturnLength);
