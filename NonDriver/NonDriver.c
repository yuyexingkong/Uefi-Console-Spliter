#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Protocol/SimpleTextIn.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/DevicePath.h>
#include "NonDriver.h"

EFI_GUID  VTextInProtocolGuid = EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID;
EFI_GUID gEfiConsoleInDeviceGuid = { 0xD3B36F2B, 0xD551, 0x11D4, { 0x9A, 0x46, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D }};
VHidDev   *gVirHidDev;
EFI_KEY   Buffer[10];
UINT8     Keyflag;

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
  EFI_DEVICE_PATH  vPath[2];


  
  vPath[0].Type = 0x03;
  vPath[0].SubType = 0x0f;
  vPath[0].Length[0] = 0x04;

  vPath[1].Type = 0x7F;
  vPath[1].SubType = 0xFF;
  vPath[1].Length[0] = 0x04;

  DEBUG((EFI_D_INFO, "Pedroa NonDriver Test Start\n"));

  CpuBreakpoint();
  Status = gBS->AllocatePool (
                 EfiBootServicesData,
                 sizeof (VHidDev),
                 (void **)&gVirHidDev
                 );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "allocate pool %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  gVirHidDev->vPath = (EFI_DEVICE_PATH *)&vPath;

  gVirHidDev->vConIn.Reset = NULL;
  gVirHidDev->vConIn.ReadKeyStroke = vReadKeyStroke;


  gVirHidDev->Handle = NULL;

  Status = gBS->InstallProtocolInterface (
                  &gVirHidDev->Handle,
                  &VTextInProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &gVirHidDev->vConIn
                  );
  if (EFI_ERROR(Status))
    DEBUG((EFI_D_INFO, "Install protocol Status = %r\n", Status));

  Status = gBS->InstallProtocolInterface (
                  &gVirHidDev->Handle,
                  &gEfiConsoleInDeviceGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  if (EFI_ERROR(Status))
   DEBUG((EFI_D_INFO, "Install PATH protocol Status = %r\n", Status));

  return Status;
}
