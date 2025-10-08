#ifndef __Alarm_H
#define __Alarm_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"


typedef struct
{
	uint32_t Pattern;
	uint32_t AlarmCount;
	uint8_t BeepOnOff;
	uint8_t TimePeriod;
  
} Message;


// Alarm Message Definition
#define MEDIUM_BEEP_X1 0x000000FF
#define MEDIUM_BEEP_X2 0x00FF00FF
#define LONG_BEEP 0x0000FFFF
#define VERY_LONG_BEEP 0xFFFFFFFF
#define EMERGENCY_BEEP 0x0F0F0F0F
#define PULSE_TRAIN_BEEP 0x33333333
#define SHORT_BEEP_X1 0x00000003
#define SHORT_BEEP_X2 0x00000033
#define SHORT_BEEP_X3 0x00000333
#define SHORT_BEEP_X4 0x00003333
#define DELAYED_SHORT_BEEP_X1 0xC0000000
#define DELAYED_SHORT_BEEP_X2 0xCC000000
#define VERYSHORT_BEEP_X1 0x00000001
#define BEEP_ON 0x01
#define BEEP_OFF 0x00
#define MESSAGE_QUEUE_SIZE 32

extern void Alarm(uint32_t, uint8_t, uint8_t, uint8_t);
extern void Update_Alarm(void);

#ifdef __cplusplus
}
#endif
#endif /*__Alarm_H */
