#include "main.h"

extern void Service_Input_Commands(uint8_t* command_Bytes);
extern void Report_To_PC(void);
extern void Code_0x1A(uint8_t* Buf, uint32_t Len);
extern void Service_PC_Rx(uint8_t* rx_Bytes);
extern uint8_t uart_PC_Rx_Buf[8];
extern uint8_t uart_PC_Tx_Buf[360];	
extern uint8_t report_Flag;
extern uint8_t command_Bytes;
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
