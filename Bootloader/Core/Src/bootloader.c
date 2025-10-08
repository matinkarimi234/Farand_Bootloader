#include "bootloader.h"

#include "main.h"
#include "usb_device.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"

#define APP_START_ADDRESS             0x08004000U
#define APP_MAX_ADDRESS               0x08020000U
#define BOOT_FLAG_ADDRESS             0x0800FC00U
#define BOOT_FLAG_REQUEST_VALUE       0x00000001U
#define APPLICATION_MAX_STACK_ADDRESS (SRAM_BASE + 0x00010000U)

typedef void (*ApplicationEntryPoint)(void);

static bool s_bootloaderRequested = false;
static bool s_stayInBootloader = true;
static bool s_applicationLaunchRequested = false;

static bool Bootloader_IsRequestFlagSet(void);
static void Bootloader_ClearRequestFlag(void);
static bool Bootloader_IsApplicationValid(void);
static void Bootloader_JumpToApplication(void);

void Bootloader_StartupCheck(void)
{
  s_bootloaderRequested = Bootloader_IsRequestFlagSet();
  s_stayInBootloader = s_bootloaderRequested;
  s_applicationLaunchRequested = false;

  if (!s_stayInBootloader)
  {
    if (Bootloader_IsApplicationValid())
    {
      Bootloader_JumpToApplication();
    }
    else
    {
      s_stayInBootloader = true;
    }
  }
}

bool Bootloader_ShouldStayInBootloader(void)
{
  return s_stayInBootloader;
}

bool Bootloader_WasRequestedByFlag(void)
{
  return s_bootloaderRequested;
}

void Bootloader_ClearRequestFlagIfNeeded(void)
{
  if (s_bootloaderRequested)
  {
    Bootloader_ClearRequestFlag();
    s_bootloaderRequested = false;
  }
}

void Bootloader_StartUsb(void)
{
  MX_USB_DEVICE_Init();
}

void Bootloader_SetStayInBootloader(bool stayActive)
{
  s_stayInBootloader = stayActive;
}

void Bootloader_RequestApplicationStart(void)
{
  s_applicationLaunchRequested = true;
  s_stayInBootloader = false;
}

void Bootloader_HandleTasks(void)
{
  if (s_applicationLaunchRequested)
  {
    if (Bootloader_IsApplicationValid())
    {
      Bootloader_JumpToApplication();
    }
  }
}

BootloaderStatus_t Bootloader_GetStatus(void)
{
  BootloaderStatus_t status;

  status.stayInBootloader = s_stayInBootloader;
  status.bootFlagRequested = s_bootloaderRequested;
  status.applicationValid = Bootloader_IsApplicationValid();
  status.applicationLaunchRequested = s_applicationLaunchRequested;

  return status;
}

static bool Bootloader_IsRequestFlagSet(void)
{
  return (*(__IO uint32_t *)BOOT_FLAG_ADDRESS) == BOOT_FLAG_REQUEST_VALUE;
}

static void Bootloader_ClearRequestFlag(void)
{
  FLASH_EraseInitTypeDef eraseConfig = {0};
  uint32_t pageError = 0U;

  HAL_FLASH_Unlock();

  eraseConfig.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseConfig.PageAddress = BOOT_FLAG_ADDRESS;
  eraseConfig.NbPages = 1U;

  if (HAL_FLASHEx_Erase(&eraseConfig, &pageError) != HAL_OK)
  {
    HAL_FLASH_Lock();
    return;
  }

  HAL_FLASH_Lock();
}

static bool Bootloader_IsApplicationValid(void)
{
  uint32_t appStackPointer = *(__IO uint32_t *)APP_START_ADDRESS;
  uint32_t appResetHandler = *(__IO uint32_t *)(APP_START_ADDRESS + 4U);

  if ((appStackPointer == 0x00000000U) || (appStackPointer == 0xFFFFFFFFU))
  {
    return false;
  }

  if ((appResetHandler == 0x00000000U) || (appResetHandler == 0xFFFFFFFFU))
  {
    return false;
  }

  if ((appStackPointer < SRAM_BASE) || (appStackPointer > APPLICATION_MAX_STACK_ADDRESS))
  {
    return false;
  }

  if ((appResetHandler < APP_START_ADDRESS) || (appResetHandler > APP_MAX_ADDRESS))
  {
    return false;
  }

  return true;
}

static void Bootloader_JumpToApplication(void)
{
  uint32_t appStackPointer = *(__IO uint32_t *)APP_START_ADDRESS;
  uint32_t appResetHandler = *(__IO uint32_t *)(APP_START_ADDRESS + 4U);
  ApplicationEntryPoint appEntry = (ApplicationEntryPoint)appResetHandler;

  __disable_irq();

  HAL_RCC_DeInit();
  HAL_DeInit();

  SysTick->CTRL = 0U;
  SysTick->LOAD = 0U;
  SysTick->VAL = 0U;

  SCB->VTOR = APP_START_ADDRESS;

  __set_MSP(appStackPointer);

  appEntry();
}