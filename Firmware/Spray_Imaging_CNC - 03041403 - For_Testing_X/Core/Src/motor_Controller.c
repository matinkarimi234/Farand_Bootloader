#include "main.h"

Axis_TypeDef X_Axis;
Axis_TypeDef Y_Axis;
Axis_TypeDef Z_Axis;
		
void Initialize_Axis_Parameters(void);
void Activate_Drivers(void);
void Move_Stepper_Motor_One_Step(Axis_TypeDef* axis_ptr);
void Determine_PulseCount_And_DIR_For_Going_To_User_Position(Axis_TypeDef* axis_ptr);
void Apply_Filter_On_Opto_Interrupter_Signal(Axis_TypeDef* axis_ptr);
void Update_State_Machine(Axis_TypeDef* axis_ptr);
void Set_New_Home_Position(Axis_TypeDef* axis_ptrv);

int Read_Opto_Interrupter_Pin(Axis_TypeDef* axis_ptr);
int Read_DIR_Pin(Axis_TypeDef* axis_ptr);
int Read_Pulse_Value(Axis_TypeDef* axis_ptr);
void Set_Direction(Axis_TypeDef* axis_ptr, int direction);
void Generate_Pulse(Axis_TypeDef* axis_ptr);
void Update_Position(Axis_TypeDef* axis_ptr);
void Clear_All_Buffers(void);
void Set_Home_Positions(void);
void Read_Home_Position_From_Flash_Memory(void);
void Check_the_Range(Axis_TypeDef* axis_ptr);

int32_t X_int;
int32_t Y_int;
int32_t Z_int;

void Determine_PulseCount_And_DIR_For_Going_To_User_Position(Axis_TypeDef* axis_ptr)
{
	
		axis_ptr -> Delta = axis_ptr -> Destination_Point - axis_ptr -> Current_Point;
			
		if((axis_ptr -> Delta) < 0)
		{
			axis_ptr -> Direction = -1;
			axis_ptr -> Delta = - axis_ptr -> Delta;
		}
		else
		{
			axis_ptr -> Direction = 1;
		}

		axis_ptr -> Direction = (axis_ptr -> Direction) * (axis_ptr -> Axis_Polarity);
		axis_ptr -> determine_Direction = axis_ptr -> Direction;
		
		axis_ptr -> Pulse_Count = (uint32_t) ((axis_ptr -> Delta * axis_ptr -> Stepper_Motor_Resolution) / axis_ptr -> mm_Per_Rotation);
		
		axis_ptr -> Slow_Down_Factor = 75;
}

void Check_the_Range(Axis_TypeDef* axis_ptr)
{
	if(axis_ptr -> Destination_Point >= axis_ptr -> Upper_Limit || axis_ptr -> Destination_Point <= axis_ptr -> Lower_Limit)
	{
		axis_ptr -> Out_of_Range_Error = 1;
	}
}



void Apply_Filter_On_Opto_Interrupter_Signal(Axis_TypeDef* axis_ptr)
{
	//This method is used for filtering the noises that can effect the value of opto-interrupter
	if(axis_ptr -> Opto_Interrupter_Raw == 1)
	{
		axis_ptr -> Ones_Counter ++;
	}
	
	if(axis_ptr -> Opto_Interrupter_Raw == 0)
	{
		axis_ptr -> Ones_Counter --;
	}
	
	if( axis_ptr -> Ones_Counter <= -3 ) 
	{ 
		axis_ptr -> Ones_Counter = 0;
		axis_ptr -> Opto_Interrupter = 0;	
	}
	
	if( axis_ptr -> Ones_Counter >= 3 ) 
	{ 
		axis_ptr -> Ones_Counter = 0;
		axis_ptr -> Opto_Interrupter = 1;	
	}
}


void Initialize_Axis_Parameters()
{
	X_Axis.mm_Per_Rotation  = 4;
	X_Axis.Stepper_Motor_Resolution  = 400;
	X_Axis.Home_Position = +250;
  X_Axis.Axis_Polarity = 1;
	X_Axis.Upper_Limit = +251;
	X_Axis.Lower_Limit = -251;
	
	Y_Axis.mm_Per_Rotation  = 5;
	Y_Axis.Stepper_Motor_Resolution  = 400;
	Y_Axis.Home_Position = +268;
	Y_Axis.Axis_Polarity = 1;
	Y_Axis.Upper_Limit = 269;
	Y_Axis.Lower_Limit = -269;

	
	Z_Axis.mm_Per_Rotation  = 4;
	Z_Axis.Stepper_Motor_Resolution  = 400;
	Z_Axis.Home_Position = -1;
	Z_Axis.Axis_Polarity = -1;
	Z_Axis.Upper_Limit = 2;
	Z_Axis.Lower_Limit = -2;
} 


void Update_Position(Axis_TypeDef* axis_ptr)
{
//	if(axis_ptr -> Current_Point < axis_ptr -> Upper_Limit && axis_ptr -> Current_Point > axis_ptr -> Lower_Limit)
	//{

		if(axis_ptr -> Axis_Polarity == 1 )
		{
			if(axis_ptr -> Direction == 1)
			{
				axis_ptr -> Current_Point += axis_ptr -> mm_Per_Rotation/axis_ptr -> Stepper_Motor_Resolution ;
			}
			else
			{
				axis_ptr -> Current_Point -= axis_ptr -> mm_Per_Rotation/axis_ptr -> Stepper_Motor_Resolution ;
			}
		}
		else
		{
			if(axis_ptr -> Direction == 1)
			{
				axis_ptr -> Current_Point -= axis_ptr -> mm_Per_Rotation/axis_ptr -> Stepper_Motor_Resolution ;
			}
			else
			{
				axis_ptr -> Current_Point += axis_ptr -> mm_Per_Rotation/axis_ptr -> Stepper_Motor_Resolution ;
			}
		}
	//}
	//else
	//{
		//axis_ptr -> Motion_Complete_Flag = 1;
	  //axis_ptr -> Out_of_Range_Error = 1;
	//}
}


void Activate_Drivers()
{
	HAL_GPIO_WritePin(X_Driver_ENA_GPIO_Port,X_Driver_ENA_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(X_Driver_OPTO_GPIO_Port,X_Driver_OPTO_Pin,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(Y_Driver_ENA_GPIO_Port,Y_Driver_ENA_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Y_Driver_OPTO_GPIO_Port,Y_Driver_OPTO_Pin,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(Z_Driver_ENA_GPIO_Port,Z_Driver_ENA_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Z_Driver_OPTO_GPIO_Port,Z_Driver_OPTO_Pin,GPIO_PIN_SET);

}

void Update_Slow_Down_Factor(Axis_TypeDef* axis_ptr)
{
	int d1 = axis_ptr -> Pulse_Counter - 0;
	int d2 = axis_ptr -> Pulse_Count -axis_ptr -> Pulse_Counter;
	int factor;
	
	if( d1 <= 50 || d2 <= 50)
	{
		factor = 75; 
	}
	else if( d1 <= 100 || d2 <= 100)
	{
		factor = 50; 
	}
	else if( d1 <= 200 || d2 <= 200)
	{
		factor = 33; 
	}
	else if( d1 <= 400 || d2 <= 400)
	{
		factor = 22; 
	}
	else if( d1 <= 800 || d2 <= 800)
	{
		factor = 15; 
	}
	else 
	{
		factor = 10; 
	}
	
	axis_ptr -> Slow_Down_Factor = factor;
}

void Move_Stepper_Motor_One_Step(Axis_TypeDef* axis_ptr)
{
	if(axis_ptr -> Direction == 1)
	{
		Set_Direction(axis_ptr, 1 );
	}
	else
	{
		Set_Direction(axis_ptr, -1);
	}
	
	axis_ptr -> Slow_Down_Counter ++;
	if(axis_ptr -> Slow_Down_Counter == axis_ptr -> Slow_Down_Factor) //This slows down stepper motor pulse frquency to half
	{
		axis_ptr -> Slow_Down_Counter = 0;
		
		Generate_Pulse(axis_ptr);
		
		if(Read_Pulse_Value(axis_ptr) == 0)
		{
			//Update_X_Position();
			Update_Position(axis_ptr);
			Update_Slow_Down_Factor(axis_ptr);
			axis_ptr -> Pulse_Counter++;
			if(axis_ptr -> Pulse_Counter >= axis_ptr -> Pulse_Count )
			{
				 axis_ptr -> Pulse_Counter = 0;
				 axis_ptr -> Motion_Complete_Flag = 1;
			}
		}
	}
}

void Set_Home_Positions(void)
{
	X_Axis.Home_Position = X_Axis.Home_Position - X_Axis.Current_Point;
	Y_Axis.Home_Position = Y_Axis.Home_Position - Y_Axis.Current_Point;
	Z_Axis.Home_Position = Z_Axis.Home_Position - Z_Axis.Current_Point;
	
	//Unlock the flash memory
	HAL_FLASH_Unlock();
	
 //Erase the flash memory
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t page_Error = 0;

	uint32_t address = 0x08008000; //Address to start erasing
	//uint32_t page_Number = (address - FLASH_BASE)/FLASH_PAGE_SIZE;
	
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address; //Address of the page to erase
	EraseInitStruct.NbPages = 1; //Number of pages to erase
	
	if(HAL_FLASHEx_Erase(&EraseInitStruct, &page_Error) != HAL_OK)
	{
		Alarm(LONG_BEEP, 1 , 32, 1);
	}
	
	//Program the flash memory
	int32_t x_Home = (int32_t)(100 * X_Axis.Home_Position);
	if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, x_Home) != HAL_OK)
	{
		Alarm(LONG_BEEP, 1 , 32, 1);
	}
	
	int32_t y_Home = (int32_t)(100 * Y_Axis.Home_Position);
	if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + 4, y_Home) != HAL_OK)
	{
		Alarm(LONG_BEEP, 1 , 32, 1);
	}

	
	int32_t z_Home = (int32_t)(100 * Z_Axis.Home_Position);
	if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + 8, z_Home) != HAL_OK)
	{
		Alarm(LONG_BEEP, 1 , 32, 1);
	}
	
  //lock the flash memory
	HAL_FLASH_Lock();
	
	//Verify the data
	if(*(uint32_t*)address != x_Home)
	{
		Alarm(LONG_BEEP, 1 , 32, 1);
	}

	if(*(uint32_t*)(address + 4) != y_Home)
	{
		Alarm(LONG_BEEP, 1 , 32, 1);
	}


	if(*(uint32_t*)(address + 8) != z_Home)
	{
		Alarm(LONG_BEEP, 1 , 32, 1);
	}
}


void Read_Home_Position_From_Flash_Memory(void)
{
  uint32_t address = 0x08008000; //Address to start erasing
	
	X_Axis.Home_Position = (int32_t)(*(uint32_t*)address) * 0.01 ;
	Y_Axis.Home_Position = (int32_t)(*(uint32_t*)(address + 4)) * 0.01 ;
	Z_Axis.Home_Position = (int32_t)(*(uint32_t*)(address + 8)) * 0.01 ;
}


int Read_Opto_Interrupter_Pin(Axis_TypeDef* axis_ptr)
{
	int pin_Vlaue = 0;
	if( axis_ptr == &X_Axis)
	{
		pin_Vlaue = HAL_GPIO_ReadPin(X_Opto_Interrupter_GPIO_Port,X_Opto_Interrupter_Pin);
	}
	
	if( axis_ptr == &Y_Axis)
	{
		pin_Vlaue = HAL_GPIO_ReadPin(Y_Opto_Interrupter_GPIO_Port,Y_Opto_Interrupter_Pin);
	}

	
	if( axis_ptr == &Z_Axis)
	{
		pin_Vlaue = HAL_GPIO_ReadPin(Z_Opto_Interrupter_GPIO_Port,Z_Opto_Interrupter_Pin);
	}

	return pin_Vlaue;
}

int Read_DIR_Pin(Axis_TypeDef* axis_ptr)
{
	int direction = 0;
	if( axis_ptr == &X_Axis)
	{
		direction = (HAL_GPIO_ReadPin(X_Driver_DIR_GPIO_Port,X_Driver_DIR_Pin) );
	}
	
	if( axis_ptr == &Y_Axis)
	{
		direction = (HAL_GPIO_ReadPin(Y_Driver_DIR_GPIO_Port,Y_Driver_DIR_Pin) );
	}

	
	if( axis_ptr == &Z_Axis)
	{
		direction = HAL_GPIO_ReadPin(Z_Driver_DIR_GPIO_Port,Z_Driver_DIR_Pin);
	}

	return direction;
}

int Read_Pulse_Value(Axis_TypeDef* axis_ptr)
{
	int pin_Vlaue = 0;
	if( axis_ptr == &X_Axis)
	{
		pin_Vlaue = HAL_GPIO_ReadPin(X_Driver_Pulse_GPIO_Port,X_Driver_Pulse_Pin);
	}
	
	if( axis_ptr == &Y_Axis)
	{
		pin_Vlaue = HAL_GPIO_ReadPin(Y_Driver_Pulse_GPIO_Port,Y_Driver_Pulse_Pin);
	}

	
	if( axis_ptr == &Z_Axis)
	{
		pin_Vlaue = HAL_GPIO_ReadPin(Z_Driver_Pulse_GPIO_Port,Z_Driver_Pulse_Pin);
	}

	return pin_Vlaue;
}


void Set_Direction(Axis_TypeDef* axis_ptr, int direction)
{
	if( axis_ptr == &X_Axis)
	{
		if(direction == -1)
		{
			HAL_GPIO_WritePin(X_Driver_DIR_GPIO_Port,X_Driver_DIR_Pin,GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(X_Driver_DIR_GPIO_Port,X_Driver_DIR_Pin,GPIO_PIN_SET);
		}
	}
	
	if( axis_ptr == &Y_Axis)
	{
		if(direction == -1)
		{
			HAL_GPIO_WritePin(Y_Driver_DIR_GPIO_Port,Y_Driver_DIR_Pin,GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(Y_Driver_DIR_GPIO_Port,Y_Driver_DIR_Pin,GPIO_PIN_SET);
		}
	}

	
	if( axis_ptr == &Z_Axis)
	{
		if(direction == 1)
		{
			HAL_GPIO_WritePin(Z_Driver_DIR_GPIO_Port,Z_Driver_DIR_Pin,GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(Z_Driver_DIR_GPIO_Port,Z_Driver_DIR_Pin,GPIO_PIN_SET);
		}
	}
}


void Generate_Pulse(Axis_TypeDef* axis_ptr)
{
	if( axis_ptr == &X_Axis)
	{
		HAL_GPIO_TogglePin(X_Driver_Pulse_GPIO_Port,X_Driver_Pulse_Pin);
	}
	
	if( axis_ptr == &Y_Axis)
	{
		HAL_GPIO_TogglePin(Y_Driver_Pulse_GPIO_Port,Y_Driver_Pulse_Pin);
	}

	if( axis_ptr == &Z_Axis)
	{
		HAL_GPIO_TogglePin(Z_Driver_Pulse_GPIO_Port,Z_Driver_Pulse_Pin);
	}
}


void Update_State_Machine(Axis_TypeDef* axis_ptr)
{
	switch(axis_ptr -> State)
	{
		case START_UP_STATE:
			  //When the CNC starts to work it remains in this state until Go_Home_Flag =1
			  // Then if it was in limit range the moment that it starts up, it will move 1cm to the opposite direction, otherwise it will move towards opto-interrupters.
		
				if(axis_ptr -> Go_Home_Flag == 1)
				{
					axis_ptr -> Go_Home_Flag = 0;
					
					if( Read_Opto_Interrupter_Pin(axis_ptr) == 1 ) 
					{ 
						axis_ptr -> Current_Point = axis_ptr -> Home_Position;
						axis_ptr -> Destination_Point = axis_ptr -> Home_Position - 2 * axis_ptr -> Axis_Polarity ; //Go to the extreme left
						axis_ptr -> Pulse_Count = 0;
						axis_ptr -> Pulse_Counter = 0;
						Determine_PulseCount_And_DIR_For_Going_To_User_Position(axis_ptr);
						axis_ptr -> Opto_Interrupter_Prev = 0;
						axis_ptr -> Opto_Interrupter = 0;
						axis_ptr -> State = RETREAT_FROM_HOME_STATE;	
					}
					else
					{
						axis_ptr -> Destination_Point =  1000 * axis_ptr -> Axis_Polarity  ; //Go to the extreme left
						Determine_PulseCount_And_DIR_For_Going_To_User_Position(axis_ptr);
						axis_ptr -> State = GO_HOME_STATE;	
					}
				}
				break;
				
		case GO_HOME_STATE:
			
				axis_ptr -> Opto_Interrupter_Prev = axis_ptr -> Opto_Interrupter;
				axis_ptr -> Opto_Interrupter_Raw = Read_Opto_Interrupter_Pin(axis_ptr);
		
		    Apply_Filter_On_Opto_Interrupter_Signal(axis_ptr);
		
				if( axis_ptr -> Opto_Interrupter_Prev == 0 && axis_ptr -> Opto_Interrupter == 0 ) 
				{ 
					//Generate stepper motor pulses while the opto-interrupter is not excited
					Move_Stepper_Motor_One_Step(axis_ptr);
				}
				
				if( axis_ptr -> Opto_Interrupter_Prev == 0 && axis_ptr -> Opto_Interrupter == 1 ) 
				{ 
					//If Opto-interrupter rising edge is detected stop moving and change state to IDLE
					axis_ptr -> Current_Point = axis_ptr -> Home_Position;
					axis_ptr -> Destination_Point = axis_ptr -> Home_Position - 2 * axis_ptr -> Axis_Polarity  ; //Go to the extreme left
					axis_ptr -> Pulse_Count = 0;
					axis_ptr -> Pulse_Counter = 0;
			    Determine_PulseCount_And_DIR_For_Going_To_User_Position(axis_ptr);
					axis_ptr -> Opto_Interrupter_Prev = 0;
					axis_ptr -> Opto_Interrupter = 0;
					axis_ptr -> State = RETREAT_FROM_HOME_STATE;	
				}
				
				//Time Out
				axis_ptr -> Time_Out++;
				if( axis_ptr -> Time_Out == 5000000 ) // 1000 Sec
				{
					axis_ptr -> Time_Out = 0;	
					axis_ptr -> Opto_interrupter_Error_Flag = 1;
					axis_ptr -> State = ERROR_STATE;				

				}
				
				//For Emergency Stop
			  if(axis_ptr -> Motion_Complete_Flag == 1)
				{
					axis_ptr -> Motion_Complete_Flag = 0;
					axis_ptr -> State = IDLE_STATE;
				}

			break;
				
		case GO_TO_CENTER_POSITION:
					Move_Stepper_Motor_One_Step(axis_ptr);
		
					//Time Out
					axis_ptr -> Time_Out++;
					if( axis_ptr -> Time_Out == 5000000 ) // 1000 Sec
					{
						axis_ptr -> Time_Out = 0;	
						axis_ptr -> Go_To_Center_Error_Flag = 1;
						axis_ptr -> State = ERROR_STATE;			

					}
					
					//For Emergency Stop
					if(axis_ptr -> Motion_Complete_Flag == 1)
					{
						axis_ptr -> Motion_Complete_Flag = 0;
						axis_ptr -> State = IDLE_STATE;	
					}
					
					


			break;
				
	 case GO_TO_USER_POSITION_STATE:
		 
	      Move_Stepper_Motor_One_Step(axis_ptr);
	 
				//Time Out
				axis_ptr -> Time_Out++;
				if( axis_ptr -> Time_Out == 500000000 ) // 1000 Sec
				{
					axis_ptr -> Time_Out = 0;	
					axis_ptr -> Go_To_User_Position_Error_Flag = 1;
					axis_ptr -> State = ERROR_STATE;			
					//Alarm(PULSE_TRAIN_BEEP, 1 , 32, 2);

				}
				
				//For Emergency Stop
				if(axis_ptr -> Motion_Complete_Flag == 1)
				{
					axis_ptr -> Motion_Complete_Flag = 0;
					axis_ptr -> State = IDLE_STATE;	
					//Alarm(PULSE_TRAIN_BEEP, 1 , 32, 1);
				}
				
				if(axis_ptr -> Out_of_Range_Error == 1)
				{ 
					//axis_ptr -> Out_of_Range_Error = 0;
					axis_ptr -> State = ERROR_STATE;	
				}


			break;
				
		case GET_OUT_OF_LIMIT_RANGE:
			    //This state happens when the CNC is in limit range as it starts to work and it will move 1cm to the opposite direction
					Set_Direction(axis_ptr,1);
					Generate_Pulse(axis_ptr);
					axis_ptr -> Pulse_Counter++;
					if(axis_ptr -> Pulse_Counter == 1000)
					{
						 axis_ptr -> Pulse_Counter = 0;
						 axis_ptr -> State = GO_HOME_STATE;	
					}
					
					//For Emergency Stop
					if(axis_ptr -> Motion_Complete_Flag == 1)
					{
						axis_ptr -> Motion_Complete_Flag = 0;
						axis_ptr -> State = IDLE_STATE;	
					}

			break;

	 case IDLE_STATE:
		    
	      if(axis_ptr -> Go_Home_Flag == 1)
				{ 
					axis_ptr -> Go_Home_Flag = 0;
					axis_ptr -> Destination_Point = 1000 * axis_ptr -> Axis_Polarity  ;
					Determine_PulseCount_And_DIR_For_Going_To_User_Position(axis_ptr);
					axis_ptr -> State = GO_HOME_STATE;		
					
				}

				if(axis_ptr -> Go_To_User_Position_Flag == 1)
				{ 
					axis_ptr -> Go_To_User_Position_Flag = 0;
					Check_the_Range(axis_ptr);
					Determine_PulseCount_And_DIR_For_Going_To_User_Position(axis_ptr);
					
					axis_ptr -> State = GO_TO_USER_POSITION_STATE;		
				}
				
				if(axis_ptr -> Go_To_Center_Position_Flag == 1)
				{
					axis_ptr -> Go_To_Center_Position_Flag = 0;
					axis_ptr -> Destination_Point = 0;
					Determine_PulseCount_And_DIR_For_Going_To_User_Position(axis_ptr);
					
					axis_ptr -> State = GO_TO_CENTER_POSITION;		
				}		
				
				
			break;
				
	 case ERROR_STATE:
		 if(axis_ptr -> Motion_Complete_Flag == 1)
			{
				axis_ptr -> Motion_Complete_Flag = 0;
				axis_ptr -> Out_of_Range_Error = 0;
				axis_ptr -> State = IDLE_STATE;	
			}
		    
	    break;
			
		case RETREAT_FROM_HOME_STATE:
			Move_Stepper_Motor_One_Step(axis_ptr);
		  
		
		  if(axis_ptr -> Motion_Complete_Flag == 1)
		  {
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);;
		  	axis_ptr -> Motion_Complete_Flag = 0;
				axis_ptr -> Destination_Point = axis_ptr -> Home_Position + 1 * axis_ptr -> Axis_Polarity  ; //
				axis_ptr -> Pulse_Count = 0;
				axis_ptr -> Pulse_Counter = 0;
				Determine_PulseCount_And_DIR_For_Going_To_User_Position(axis_ptr);
		  	axis_ptr -> State = FIND_HOME_STATE;	
		  }
			break;
			
		case FIND_HOME_STATE:
			//axis_ptr -> Direction = - axis_ptr -> Direction;
			Move_Stepper_Motor_One_Step(axis_ptr);
		  
		
			axis_ptr -> Opto_Interrupter_Prev = axis_ptr -> Opto_Interrupter;
			axis_ptr -> Opto_Interrupter_Raw =  Read_Opto_Interrupter_Pin(axis_ptr);
	
			Apply_Filter_On_Opto_Interrupter_Signal(axis_ptr);

		
			if(axis_ptr -> Opto_Interrupter_Prev == 0 && axis_ptr -> Opto_Interrupter == 1)
			{
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);;
				axis_ptr -> Pulse_Count = 0;
				axis_ptr -> Pulse_Counter = 0;
				axis_ptr -> Opto_Interrupter_Prev = 0;
				axis_ptr -> Opto_Interrupter = 0;
				axis_ptr -> Current_Point = axis_ptr -> Home_Position;
				axis_ptr -> State = IDLE_STATE;
				
			}
			
		  if(axis_ptr -> Motion_Complete_Flag == 1)
		  {
				//Alarm(SHORT_BEEP_X1, 1 , 8, 1);;
				axis_ptr -> Motion_Complete_Flag = 0;
				axis_ptr -> Pulse_Count = 0;
				axis_ptr -> Pulse_Counter = 0;
				axis_ptr -> Opto_Interrupter_Prev = 0;
				axis_ptr -> Opto_Interrupter = 0;
				axis_ptr -> State = IDLE_STATE;
			}
			break;
	}
}



