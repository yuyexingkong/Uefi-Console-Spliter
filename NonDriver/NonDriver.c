#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Protocol/SimpleTextIn.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/DevicePath.h>
#include <Library/DevicePathLib.h>
#include "NonDriver.h"

VHidDev   *gVirHidDev;
EFI_KEY   Buffer[10];
UINT8     Keyflag;

VOID
EFIAPI
CallBack (
    IN  EFI_EVENT  Event,
    IN  VOID       *Context
    )
{
  gBS->SignalEvent (Event);
}

STATIC vPath_T gvHidDevPath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_WWID_DP,
      {
        (UINT8) (sizeof (USB_WWID_DEVICE_PATH)),
        (UINT8) (sizeof (USB_WWID_DEVICE_PATH) >> 8)
      }
    },
    0x01,
    0xaa,
    0xbb
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),   
      (UINT8) (END_DEVICE_PATH_LENGTH >> 8)   
    }
  }

};

EFI_STATUS
EFIAPI
vReadKeyStroke (
  IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL    *This,
  OUT EFI_INPUT_KEY                     *Key
  )
{
  Key->UnicodeChar = L'A';
  return EFI_SUCCESS;    
}

EFI_STATUS
EFIAPI
TestNonDriver (
    IN EFI_HANDLE          ImageHandle,
    IN EFI_SYSTEM_TABLE    *SystemTable
    )
{
  
  EFI_STATUS       Status;

  DEBUG((EFI_D_INFO, "Pedroa NonDriver Test Start\n"));

  Status = gBS->AllocatePool (
                 EfiBootServicesData,
                 sizeof (VHidDev),
                 (void **)&gVirHidDev
                 );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "allocate pool %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }


  gVirHidDev->vConIn.Reset = NULL;
  gVirHidDev->vConIn.ReadKeyStroke = vReadKeyStroke;
  gVirHidDev->Handle = NULL;

  Status = gBS->CreateEvent (
                   EVT_NOTIFY_WAIT,
                   TPL_NOTIFY,
                   CallBack,
                   NULL,
                   &gVirHidDev->vConIn.WaitForKey
                   );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Create Event %r\n", Status));
    return Status;
  }

  Status = gBS->InstallMultipleProtocolInterfaces(
                  &gVirHidDev->Handle,
                  &gEfiDevicePathProtocolGuid,
                  &gvHidDevPath,
                  &gEfiSimpleTextInProtocolGuid,
                  &gVirHidDev->vConIn,
                  NULL
                  );
  
  return EFI_SUCCESS;
}
