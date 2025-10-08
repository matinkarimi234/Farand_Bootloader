#include "stm32f1xx_hal.h"
typedef struct
{
	double Start_Point;
	double Destination_Point;
	double Delta;
	double Center_Point;
	double Current_Point;
	double Home_Position;
	double Step_Length;
	double Upper_Limit;
	double Lower_Limit;
	
	int	Axis_Polarity;
  int	Opto_Interrupter ;
	int	Opto_Interrupter_Prev;
	int Opto_Interrupter_Raw;
	int Ones_Counter;
	int Zeros_Counter;
	int Go_To_Start_Up_State_Flag;
	int Motion_Complete_Flag;
	int Go_Home_Flag ;
	int Go_To_Center_Position_Flag;
	int Go_To_User_Position_Flag;
	int Opto_interrupter_Error_Flag;
	int Go_To_Center_Error_Flag;
	int Go_To_User_Position_Error_Flag;
	int Position_Reached_Flag;
	int State;
	int Out_of_Range_Error;
	
	
	int8_t Direction;
	int8_t determine_Direction;
	uint32_t Pulse_Count;
	uint32_t Pulse_Counter;
	int Time_Out;
	int Slow_Down_Counter;
	int Slow_Down_Factor;
	int Step_Dir;
	
	double mm_Per_Rotation;
	double Stepper_Motor_Resolution;
	
}Axis_TypeDef;

extern Axis_TypeDef X_Axis;
extern Axis_TypeDef Y_Axis;
extern Axis_TypeDef Z_Axis;

extern void Initialize_Axis_Parameters(void);

extern void Activate_Drivers(void);
extern void Move_Stepper_Motor_One_Step(Axis_TypeDef* axis_ptr);
extern void Determine_PulseCount_And_DIR_For_Going_To_User_Position(Axis_TypeDef* axis_ptr);
extern void Apply_Filter_On_Opto_Interrupter_Signal(Axis_TypeDef* axis_ptr);
extern void Update_State_Machine(Axis_TypeDef* axis_ptr);

extern int Read_Opto_Interrupter_Pin(Axis_TypeDef* axis_ptr);
extern int Read_DIR_Pin(Axis_TypeDef* axis_ptr);
extern int Read_Pulse_Value(Axis_TypeDef* axis_ptr);
void Set_Direction(Axis_TypeDef* axis_ptr, int direction);
extern void Generate_Pulse(Axis_TypeDef* axis_ptr);
extern void Set_New_Home_Position(Axis_TypeDef* axis_ptr);
extern void Set_Home_Positions(void);
extern void Read_Home_Position_From_Flash_Memory(void);


extern void Update_Position(Axis_TypeDef* axis_ptr);
extern void Clear_All_Buffers(void);
extern void Check_the_Range(Axis_TypeDef* axis_ptr);

extern int32_t X_int;
extern int32_t Y_int;
extern int32_t Z_int;

#define START_UP_STATE 0
#define GO_HOME_STATE 1
#define GO_TO_CENTER_POSITION 2
#define GO_TO_USER_POSITION_STATE 3
#define IDLE_STATE 4
#define ERROR_STATE 5
#define GET_OUT_OF_LIMIT_RANGE 6
#define RETREAT_FROM_HOME_STATE 7
#define FIND_HOME_STATE 8



