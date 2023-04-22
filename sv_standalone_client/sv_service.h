#pragma once
#include <string>
#include <Windows.h>

#define IOCTL_SCAN_KERNEL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_GET_PROCESSORS_NUMBER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_NEITHER, FILE_ANY_ACCESS)

class sv_service {
public:
    typedef struct _stack_info_per_processor
    {
        bool invalid_stack_trace_pointer;
        ULONG n_captured_frames;
        ULONG n_invalid_frames;
        void* invalid_frames[10];
    } stack_ipp, pstack_ipp;

    typedef struct _nmi_info {
        ULONG n_processors;
        ULONG n_timeouts;
        bool freed_stack_traces_allocation;
        stack_ipp stack_infos[1];
    } nmi_info, * pnmi_info;

    const std::wstring driver_file_name;
    const std::wstring device_name;
    const std::wstring driver_path;

    HANDLE _handle;

    sv_service()
        : driver_file_name(L"svac.sys"), device_name(L"\\\\.\\svac"),
        driver_path(L"C:\\Users\\droon\\Desktop\\sv\\svac.sys") {}

    ~sv_service() {}
    bool create_service();
    bool load();
    bool unload();

    bool dispatch_kernel_detections();
    ULONG get_number_of_processors();
    bool hearbeat();
};

/***

1. Check if file exists
3. Create service
4. Start service

***/
