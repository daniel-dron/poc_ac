#include "sv_service.h"

#include "log.h"

#include <experimental/filesystem>
#include <filesystem>

bool sv_service::create_service()
{
    SC_HANDLE scm_handle =
        OpenSCManager(nullptr, SERVICES_ACTIVE_DATABASE, GENERIC_ALL);
    if (!scm_handle)
    {
        log_error("open service manager failed: %d\n", GetLastError());
        return false;
    }

    SC_HANDLE service_handle = CreateService(
        scm_handle, L"svac", L"Supervisor Anti-Cheat", SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        driver_path.c_str(), NULL, NULL, NULL, NULL, NULL);
    if (!service_handle)
    {
        CloseHandle(scm_handle);
        log_error("create service failed: %d\n", GetLastError());
        return false;
    }

    CloseHandle(scm_handle);

    return StartService(service_handle, 0, nullptr);
}

bool sv_service::load()
{
    namespace filesystem = std::experimental::filesystem::v1;

    //
    // Check if driver file exists
    //
    if (!filesystem::exists(driver_path))
    {
        log_error("Could not find svac.sys driver file. Aborting process.\n");
        return false;
    }

    //
    // Create service and start it
    //
    if (!create_service())
    {
        log_error("Failed to load service svac.\n");
        return false;
    }

    //
    // Open a handle to the service
    //
    _handle = CreateFile(L"\\\\.\\svac", GENERIC_ALL, FILE_SHARE_WRITE, nullptr,
                         OPEN_EXISTING, 0, nullptr);
    if (_handle == INVALID_HANDLE_VALUE)
    {
        log_error(
            "Could not open handle to device driver. Aborting process. %d\n",
            GetLastError());
        return false;
    }

    return true;
}

bool sv_service::unload() { return true; }

ULONG sv_service::get_number_of_processors()
{
    ULONG n_processors{};
    DWORD returned{};
    DeviceIoControl(_handle, IOCTL_GET_PROCESSORS_NUMBER, &n_processors,
                    sizeof(n_processors), nullptr, 0, &returned, nullptr);

    return n_processors;
}

bool sv_service::dispatch_kernel_detections()
{
    ULONG n = get_number_of_processors();
    SIZE_T allocation_size = sizeof(_nmi_info) + (sizeof(stack_ipp) * (n - 1));
    pnmi_info nmi_info = reinterpret_cast<pnmi_info>(
        VirtualAlloc(0, allocation_size, MEM_COMMIT, PAGE_READWRITE));
    if(!nmi_info)
        return false;

    DWORD returned{};
    DeviceIoControl(_handle, IOCTL_SCAN_KERNEL, nmi_info, allocation_size,
                    nullptr, 0, &returned, nullptr);

    if (nmi_info->freed_stack_traces_allocation)
        log_error("Stack Trace Allocation was freed!\n");

    if (nmi_info->n_timeouts > 0)
        log_error("There were %d NMI timeouts!\n", nmi_info->n_timeouts);

    for (int i = 0; i < 4; i++)
    {
        if (nmi_info->stack_infos[i].n_captured_frames == 0 ||
            nmi_info->stack_infos[i].n_invalid_frames > 0)
        {
            log_error("Found invalid thread running on processor %d\n", i);
            log_error("Captured stack frames: %d\n",
                      nmi_info->stack_infos[i].n_captured_frames);
            log_error("Number of frames outside of valid modules: %d\n",
                      nmi_info->stack_infos[i].n_invalid_frames);
            for (int j = 0; j < nmi_info->stack_infos[i].n_invalid_frames; j++)
                log_error("\t[0x%x]: 0x%p\n", j,
                          nmi_info->stack_infos[i].invalid_frames[j]);
        }
    }

    return true;
}

bool sv_service::hearbeat() { return true; }
