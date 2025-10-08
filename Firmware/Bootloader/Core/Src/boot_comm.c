#include "boot_comm.h"

#include <stdbool.h>
#include <string.h>

#include "bootloader.h"
#include "main.h"
#include "usbd_cdc_if.h"

#define BOOT_COMM_FRAME_LENGTH      56U
#define BOOT_COMM_MAGIC_INDEX_0     29U
#define BOOT_COMM_MAGIC_INDEX_1     30U
#define BOOT_COMM_MAGIC_VALUE       0xAAU
#define BOOT_COMM_STATUS_INTERVAL   500U

#define BOOT_COMM_CMD_NOP           0x01U
#define BOOT_COMM_CMD_STATUS        0x10U
#define BOOT_COMM_CMD_START_APP     0x20U
#define BOOT_COMM_CMD_STAY_ACTIVE   0x21U
#define BOOT_COMM_CMD_CLEAR_FLAG    0x22U

static uint8_t tx_buff[BOOT_COMM_FRAME_LENGTH];
static uint32_t com_Counter = 0U;
static uint32_t lastStatusTick = 0U;

static void BootComm_ClearBuffer(void);
static void BootComm_FinalizeAndSend(uint32_t length);
static bool BootComm_IsFrameValid(const uint8_t *buffer, uint32_t length);
static void BootComm_HandleCommand(uint8_t *command_Bytes);
static void BootComm_SendAck(uint8_t commandCode, uint8_t resultCode);
static void BootComm_SendError(uint8_t commandCode, uint8_t errorCode);
static uint32_t BootComm_ComputeChecksum(const uint8_t *buffer, uint32_t length);

void BootComm_Init(void)
{
  BootComm_ClearBuffer();
  com_Counter = 0U;
  lastStatusTick = HAL_GetTick();
  Report_To_PC();
}

void BootComm_Task(void)
{
  uint32_t now = HAL_GetTick();
  if ((now - lastStatusTick) >= BOOT_COMM_STATUS_INTERVAL)
  {
    Report_To_PC();
    lastStatusTick = now;
  }
}

void BootComm_OnPacketReceived(uint8_t *buffer, uint32_t length)
{
  if (length < BOOT_COMM_FRAME_LENGTH)
  {
    return;
  }

  if (!BootComm_IsFrameValid(buffer, length))
  {
    return;
  }

  Service_Input_Commands(buffer);
}

void Service_Input_Commands(uint8_t *command_Bytes)
{
  BootComm_HandleCommand(command_Bytes);
}

void BootComm_ReportStatus(void)
{
  Report_To_PC();
}

void Report_To_PC(void)
{
  BootloaderStatus_t status = Bootloader_GetStatus();

  BootComm_ClearBuffer();
  com_Counter++;

  tx_buff[0] = BOOT_COMM_CMD_STATUS;
  tx_buff[1] = status.stayInBootloader ? 1U : 0U;
  tx_buff[2] = status.applicationValid ? 1U : 0U;
  tx_buff[3] = status.bootFlagRequested ? 1U : 0U;
  tx_buff[4] = status.applicationLaunchRequested ? 1U : 0U;
  tx_buff[5] = (uint8_t)(com_Counter & 0xFFU);
  tx_buff[6] = (uint8_t)((com_Counter >> 8) & 0xFFU);
  tx_buff[7] = (uint8_t)((com_Counter >> 16) & 0xFFU);
  tx_buff[8] = (uint8_t)((com_Counter >> 24) & 0xFFU);
  tx_buff[BOOT_COMM_MAGIC_INDEX_0] = BOOT_COMM_MAGIC_VALUE;
  tx_buff[BOOT_COMM_MAGIC_INDEX_1] = BOOT_COMM_MAGIC_VALUE;
  tx_buff[51] = 0x55U;
  tx_buff[52] = 0x55U;
  tx_buff[BOOT_COMM_FRAME_LENGTH - 1U] = 0U;

  BootComm_FinalizeAndSend(BOOT_COMM_FRAME_LENGTH);
}

void Clear_All_Buffers(void)
{
  BootComm_ClearBuffer();
}

void Code_0x1A(uint8_t *Buf, uint32_t Len)
{
  uint32_t loopCount = Len / 8U;

  for (uint32_t n = 0U; n < loopCount; n++)
  {
    uint32_t baseIndex = n * 8U;
    Buf[baseIndex] = 0U;
    for (uint32_t codeIndex = 0U; codeIndex < 8U; codeIndex++)
    {
      if (Buf[baseIndex + codeIndex + 1U] == 0x1AU)
      {
        Buf[baseIndex] |= (uint8_t)(0x01U << codeIndex);
        Buf[baseIndex + codeIndex + 1U] = 0xA1U;
      }
    }

    uint8_t codeByte = (uint8_t)((Buf[baseIndex] & 0xF0U) << 1);
    Buf[baseIndex] = (uint8_t)((Buf[baseIndex] & 0x0FU) | codeByte);
  }
}

static void BootComm_ClearBuffer(void)
{
  memset(tx_buff, 0, sizeof(tx_buff));
}

static void BootComm_FinalizeAndSend(uint32_t length)
{
  if (length > BOOT_COMM_FRAME_LENGTH)
  {
    length = BOOT_COMM_FRAME_LENGTH;
  }

  uint32_t checksum = BootComm_ComputeChecksum(tx_buff, length - 1U);
  tx_buff[length - 1U] = (uint8_t)(checksum & 0xFFU);

  Code_0x1A(tx_buff, length);

  while (CDC_Transmit_FS(tx_buff, length) == USBD_BUSY)
  {
  }
}

static bool BootComm_IsFrameValid(const uint8_t *buffer, uint32_t length)
{
  if (length < BOOT_COMM_FRAME_LENGTH)
  {
    return false;
  }

  if ((buffer[BOOT_COMM_MAGIC_INDEX_0] != BOOT_COMM_MAGIC_VALUE) ||
      (buffer[BOOT_COMM_MAGIC_INDEX_1] != BOOT_COMM_MAGIC_VALUE))
  {
    return false;
  }

  uint32_t expectedChecksum = BootComm_ComputeChecksum(buffer, length - 1U);
  return ((uint8_t)(expectedChecksum & 0xFFU)) == buffer[length - 1U];
}

static void BootComm_HandleCommand(uint8_t *command_Bytes)
{
  switch (command_Bytes[0])
  {
    case BOOT_COMM_CMD_NOP:
      BootComm_SendAck(command_Bytes[0], 0x00U);
      break;

    case BOOT_COMM_CMD_STATUS:
      Report_To_PC();
      break;

    case BOOT_COMM_CMD_START_APP:
      Bootloader_RequestApplicationStart();
      BootComm_SendAck(command_Bytes[0], 0x00U);
      break;

    case BOOT_COMM_CMD_STAY_ACTIVE:
      Bootloader_SetStayInBootloader(true);
      BootComm_SendAck(command_Bytes[0], 0x00U);
      break;

    case BOOT_COMM_CMD_CLEAR_FLAG:
      Bootloader_ClearRequestFlagIfNeeded();
      BootComm_SendAck(command_Bytes[0], 0x00U);
      break;

    default:
      BootComm_SendError(command_Bytes[0], 0xFFU);
      break;
  }
}

static void BootComm_SendAck(uint8_t commandCode, uint8_t resultCode)
{
  BootComm_ClearBuffer();
  tx_buff[0] = commandCode;
  tx_buff[1] = resultCode;
  tx_buff[BOOT_COMM_MAGIC_INDEX_0] = BOOT_COMM_MAGIC_VALUE;
  tx_buff[BOOT_COMM_MAGIC_INDEX_1] = BOOT_COMM_MAGIC_VALUE;
  tx_buff[51] = 0x55U;
  tx_buff[52] = 0x55U;
  BootComm_FinalizeAndSend(BOOT_COMM_FRAME_LENGTH);
}

static void BootComm_SendError(uint8_t commandCode, uint8_t errorCode)
{
  BootComm_ClearBuffer();
  tx_buff[0] = commandCode;
  tx_buff[1] = errorCode;
  tx_buff[2] = 0xEEU;
  tx_buff[BOOT_COMM_MAGIC_INDEX_0] = BOOT_COMM_MAGIC_VALUE;
  tx_buff[BOOT_COMM_MAGIC_INDEX_1] = BOOT_COMM_MAGIC_VALUE;
  tx_buff[51] = 0x55U;
  tx_buff[52] = 0x55U;
  BootComm_FinalizeAndSend(BOOT_COMM_FRAME_LENGTH);
}

static uint32_t BootComm_ComputeChecksum(const uint8_t *buffer, uint32_t length)
{
  uint32_t checksum = 0U;
  for (uint32_t i = 0U; i < length; i++)
  {
    checksum += buffer[i];
  }
  return checksum;
}