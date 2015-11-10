#ifndef _NON_DRIVER_H_
#define _NON_DRIVER_H_

#define DRVIER_A_GUID \
     {\
       0xfe1236ab, 0x32e8, 0x5a9b, {0x32, 0x45, 0xCD, 0x1F, 0x69, 0x45, 0x23, 0xB5}\
     }

typedef struct{
  USB_WWID_DEVICE_PATH       vPath;
  EFI_DEVICE_PATH_PROTOCOL   EndPath;
}vPath_T;

typedef struct {
  EFI_HANDLE                        Handle;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL    vConIn;
  EFI_EVENT                         Timer;
}_Virtual_HID_DEV;

typedef _Virtual_HID_DEV  VHidDev;
#endif
