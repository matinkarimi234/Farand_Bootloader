#ifndef BOOT_COMM_H
#define BOOT_COMM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void BootComm_Init(void);
void BootComm_Task(void);
void BootComm_OnPacketReceived(uint8_t *buffer, uint32_t length);
void Service_Input_Commands(uint8_t *command_Bytes);
void BootComm_ReportStatus(void);
void Report_To_PC(void);
void Clear_All_Buffers(void);
void Code_0x1A(uint8_t *Buf, uint32_t Len);

#ifdef __cplusplus
}
#endif

#endif /* BOOT_COMM_H */