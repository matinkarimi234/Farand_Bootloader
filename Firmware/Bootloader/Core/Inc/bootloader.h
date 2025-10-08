#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  bool stayInBootloader;
  bool bootFlagRequested;
  bool applicationValid;
  bool applicationLaunchRequested;
} BootloaderStatus_t;

void Bootloader_StartupCheck(void);
bool Bootloader_ShouldStayInBootloader(void);
bool Bootloader_WasRequestedByFlag(void);
void Bootloader_ClearRequestFlagIfNeeded(void);
void Bootloader_StartUsb(void);
void Bootloader_SetStayInBootloader(bool stayActive);
void Bootloader_RequestApplicationStart(void);
void Bootloader_HandleTasks(void);
BootloaderStatus_t Bootloader_GetStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* BOOTLOADER_H */