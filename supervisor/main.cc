
#include "nmi.h"

#define IOCTL_SCAN_KERNEL                                                      \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_GET_PROCESSORS_NUMBER                                            \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_NEITHER, FILE_ANY_ACCESS)

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
    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        irp->IoStatus.Status = status;
        irp->IoStatus.Information = 0;
        break;
    }

    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return status;
}

__forceinline void *get_current_prcb()
{
    return (void *)__readgsqword(FIELD_OFFSET(KPCR, CurrentPrcb));
}

void initialize_svac() { nmi::setup_nmi(); }

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

    initialize_svac();

    return status;
}
