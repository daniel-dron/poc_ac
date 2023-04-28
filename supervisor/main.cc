
#include "nmi.h"
#include "sytem_threads.h"

#define IOCTL_SCAN_KERNEL                                                      \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_GET_PROCESSORS_NUMBER                                            \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_GET_INVALID_THREADS                                              \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_GET_NEXT_INVALID_THREAD                                          \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_NEITHER, FILE_ANY_ACCESS)

NTSTATUS dispatch_create_close(PDEVICE_OBJECT device_object, PIRP irp)
{
    UNREFERENCED_PARAMETER(device_object);

    NTSTATUS status = STATUS_SUCCESS;

    irp->IoStatus.Status = status;
    irp->IoStatus.Information = 0;
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS device_control(PDEVICE_OBJECT device_object, PIRP irp)
{
    UNREFERENCED_PARAMETER(device_object);

    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION io_stack;
    nmi::_nmi_info *nmi_info;
    ULONG *n_processors;
    ULONG64 n_invalid_threads{};
    ULONG64 *n_invalid_threads_output{};

    //
    system_threads::pinvalid_system_thread ist{};

    io_stack = IoGetCurrentIrpStackLocation(irp);

    switch (io_stack->Parameters.DeviceIoControl.IoControlCode) {
    case IOCTL_SCAN_KERNEL:
        nmi_info = reinterpret_cast<nmi::_nmi_info *>(
            io_stack->Parameters.DeviceIoControl.Type3InputBuffer);

        if (nmi::nmi_initialized &&
            io_stack->Parameters.DeviceIoControl.InputBufferLength >=
                sizeof(nmi::_nmi_info)) {
            nmi_info->n_processors = KeQueryActiveProcessorCountEx(0);
            nmi::process_nmi(nmi_info);

            irp->IoStatus.Status = status;
            irp->IoStatus.Information = sizeof(nmi::_nmi_info);
        }
        else
            status = STATUS_INVALID_PARAMETER;

        break;
    case IOCTL_GET_PROCESSORS_NUMBER:
        n_processors = reinterpret_cast<ULONG *>(
            io_stack->Parameters.DeviceIoControl.Type3InputBuffer);
        if (MmIsAddressValid(n_processors))
            *n_processors = KeQueryActiveProcessorCountEx(0);

        irp->IoStatus.Status = status;
        irp->IoStatus.Information = 0;
        break;
    case IOCTL_GET_INVALID_THREADS:
        n_invalid_threads = system_threads::scan_system_process_threads();

        status = n_invalid_threads > 0 ? STATUS_SUCCESS : STATUS_INTERNAL_ERROR;

        n_invalid_threads_output = reinterpret_cast<ULONG64 *>(
            io_stack->Parameters.DeviceIoControl.Type3InputBuffer);

        KdLog("invalid threads %lld\n", n_invalid_threads);
        KdLog("output address 0x%p\n", n_invalid_threads_output);
        KdLog("sizeof output address %lld\n",
              io_stack->Parameters.DeviceIoControl.InputBufferLength);

        if (n_invalid_threads_output &&
            io_stack->Parameters.DeviceIoControl.InputBufferLength >=
                sizeof(ULONG64))
            *n_invalid_threads_output = n_invalid_threads;

        irp->IoStatus.Status = status;
        irp->IoStatus.Information = 0;
        break;
    case IOCTL_GET_NEXT_INVALID_THREAD:

        ist = reinterpret_cast<system_threads::pinvalid_system_thread>(
            io_stack->Parameters.DeviceIoControl.Type3InputBuffer);
        if (system_threads::g_last_entry && MmIsAddressValid(ist) &&
            io_stack->Parameters.DeviceIoControl.InputBufferLength >=
                sizeof(system_threads::invalid_system_thread)) {

            // If we are currently at the head, skip this one
            if (system_threads::g_last_entry == system_threads::g_list_head)
                system_threads::g_last_entry =
                    system_threads::g_list_head->Flink;

            system_threads::pinvalid_system_thread invalid_st =
                CONTAINING_RECORD(system_threads::g_last_entry,
                                  system_threads::invalid_system_thread, entry);

            memmove(ist, invalid_st,
                    sizeof(system_threads::invalid_system_thread));
            
            system_threads::g_last_entry = system_threads::g_last_entry->Flink;
        }

        irp->IoStatus.Status = status;
        irp->IoStatus.Information = 0;
        break;
    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        irp->IoStatus.Status = status;
        irp->IoStatus.Information = 0;
        break;
    }

    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return status;
}

void initialize_svac()
{
    system_threads::scan_system_process_threads();

    // nmi::setup_nmi();
}

void driver_unload(PDRIVER_OBJECT driver_object)
{
    UNICODE_STRING sym_link_name = RTL_CONSTANT_STRING(L"\\??\\svac");

    nmi::clean();

    IoDeleteSymbolicLink(&sym_link_name);
    IoDeleteDevice(driver_object->DeviceObject);

    KdLog("Unloaded driver!\n");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object,
                                PUNICODE_STRING registry_path)
{
    UNREFERENCED_PARAMETER(registry_path);

    NTSTATUS status;
    PDEVICE_OBJECT device_object;
    UNICODE_STRING device_name = RTL_CONSTANT_STRING(L"\\Device\\svac");
    UNICODE_STRING sym_link_name = RTL_CONSTANT_STRING(L"\\??\\svac");

    driver_object->DriverUnload = driver_unload;

    status = IoCreateDevice(driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN,
                            0, FALSE, &device_object);
    if (!NT_SUCCESS(status)) {
        KdLog("Failed to create device object!\nReturned status = %d\n",
              status);
        return status;
    }

    status = IoCreateSymbolicLink(&sym_link_name, &device_name);
    if (!NT_SUCCESS(status)) {
        KdLog("Failed to create symbolic link!\nReturned status = %d\n",
              status);
        if (device_object)
            IoDeleteDevice(device_object);
        return status;
    }

    driver_object->MajorFunction[IRP_MJ_CREATE] =
        driver_object->MajorFunction[IRP_MJ_CLOSE] = dispatch_create_close;

    driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = device_control;

    // initialize_svac();

    return status;
}
