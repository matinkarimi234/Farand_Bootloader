#include "main.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"

#define PC_CMD_ENTER_BOOTLOADER       0xF0U
#define BOOT_FLAG_ADDRESS             0x0800FC00U
#define BOOT_FLAG_REQUEST_VALUE       0x00000001U


uint32_t com_Counter = 0;
uint8_t command_Bytes;

void Service_Input_Commands(uint8_t* command_Bytes);
static HAL_StatusTypeDef Bootloader_SetRequestFlag(void);
static void Bootloader_RequestFromApplication(void);

void Code_0x1A(uint8_t* Buf, uint32_t Len);


void Service_Input_Commands(uint8_t* command_Bytes)
{	
	if(command_Bytes[29] == 0xAA && command_Bytes[30] == 0xAA)
	{
		switch (command_Bytes[0])
		{
			case 0x01: //No operation just for checking communication
      
			 
				break;	
			
			case 0x02: // X Go Home
				X_Axis.Go_Home_Flag = 1;
			
	  		Alarm(SHORT_BEEP_X1, 1 , 8, 1);
			
			  break;
			
			case 0x03: // X Go to center
			  X_Axis.Go_To_Center_Position_Flag = 1;
			
        Alarm(SHORT_BEEP_X1, 1 , 8, 1);	
		  	break;	
			
			case 0x04: // X Go to 
				X_Axis.Destination_Point = 0.01 * (int32_t)((command_Bytes[4] << 24 ) + (command_Bytes[3] << 16 ) + (command_Bytes[2] << 8 ) + command_Bytes[1]);
				
			  if(X_Axis.State == IDLE_STATE)
				{
					X_Axis.Go_To_User_Position_Flag = 1;
				}

				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	
			
			
			case 0x05: // X Step
				X_Axis.Step_Length = 0.01 * (int32_t)((command_Bytes[4] << 24 ) + (command_Bytes[3] << 16 ) + (command_Bytes[2] << 8 ) + command_Bytes[1]);
				X_Axis.Destination_Point = X_Axis.Current_Point + X_Axis.Step_Length;
				X_Axis.Go_To_User_Position_Flag = 1;
		 		
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	
			
			case 0x06: // X Clear errors
			  X_Axis.Opto_interrupter_Error_Flag = 0;
		  	X_Axis.Go_To_Center_Error_Flag = 0;
			  X_Axis.Go_To_User_Position_Error_Flag = 0;
			  X_Axis.Out_of_Range_Error = 0;
			  X_Axis.Motion_Complete_Flag = 1;

				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	
			
		 	case 0xA2: // Y Go Home
				Y_Axis.Go_Home_Flag = 1;
			  
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	

						
			case 0xA3: // Y Go to center
				Y_Axis.Go_To_Center_Position_Flag = 1;
			
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);				
				break;	

									
			case 0xA4: // Y Go to 
				Y_Axis.Destination_Point = 0.01 * (int32_t)((command_Bytes[4] << 24 ) + (command_Bytes[3] << 16 ) + (command_Bytes[2] << 8 ) + command_Bytes[1]);
				Y_Axis.Go_To_User_Position_Flag = 1;
			
			  Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				
				break;	
				
			case 0xA5: // Y Step
				Y_Axis.Step_Length = 0.01 * (int32_t)((command_Bytes[4] << 24 ) + (command_Bytes[3] << 16 ) + (command_Bytes[2] << 8 ) + command_Bytes[1]);
				Y_Axis.Destination_Point = Y_Axis.Current_Point + Y_Axis.Step_Length;
				Y_Axis.Go_To_User_Position_Flag = 1;

				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	

								
			case 0xA6: // Y Clear errors
				Y_Axis.Opto_interrupter_Error_Flag = 0;
			  Y_Axis.Go_To_Center_Error_Flag = 0;
			  Y_Axis.Go_To_User_Position_Error_Flag = 0;
			  Y_Axis.Motion_Complete_Flag = 1;
		  	Y_Axis.Out_of_Range_Error = 0;
		 	  
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	

												
			case 0xB2: // Z Go Home
				Z_Axis.Go_Home_Flag = 1;
			
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);				 
				break;	
			
			case 0xB3: // Z Go to center
				Z_Axis.Go_To_Center_Position_Flag = 1;
				
			  Alarm(SHORT_BEEP_X1, 1 , 8, 1);				
				break;	

									
			case 0xB4: // Z Go to 
				Z_Axis.Destination_Point = 0.01 * (int32_t)((command_Bytes[4] << 24 ) + (command_Bytes[3] << 16 ) + (command_Bytes[2] << 8 ) + command_Bytes[1]);
				Z_Axis.Go_To_User_Position_Flag = 1;
			
			  Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	
				
			case 0xB5: // Z Step
				Z_Axis.Step_Length = 0.01 * (int32_t)((command_Bytes[4] << 24 ) + (command_Bytes[3] << 16 ) + (command_Bytes[2] << 8 ) + command_Bytes[1]);
				Z_Axis.Destination_Point = Z_Axis.Current_Point + Z_Axis.Step_Length;
				Z_Axis.Go_To_User_Position_Flag = 1;

				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	

								
			case 0xB6: // Z Clear errors
				Z_Axis.Opto_interrupter_Error_Flag = 0;
			  Z_Axis.Go_To_Center_Error_Flag = 0;
			  Z_Axis.Go_To_User_Position_Error_Flag = 0;
			  Z_Axis.Motion_Complete_Flag = 1;
			  Z_Axis.Out_of_Range_Error = 0;
				
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
			 break;	
			
			case 0xC2: // X Y Z Go to
			 X_Axis.Destination_Point = 0.01 * (int32_t)((command_Bytes[4] << 24 ) + (command_Bytes[3] << 16 ) + (command_Bytes[2] << 8 ) + command_Bytes[1]);	
			 Y_Axis.Destination_Point = 0.01 * (int32_t)((command_Bytes[8] << 24 ) + (command_Bytes[7] << 16 ) + (command_Bytes[6] << 8 ) + command_Bytes[5]);
			 Z_Axis.Destination_Point = 0.01 * (int32_t)((command_Bytes[12] << 24 ) + (command_Bytes[11] << 16 ) + (command_Bytes[10] << 8 ) + command_Bytes[9]);
			
			 X_Axis.Go_To_User_Position_Flag = 1;
			 Y_Axis.Go_To_User_Position_Flag = 1;
			 Z_Axis.Go_To_User_Position_Flag = 1;
			
			 Alarm(SHORT_BEEP_X1, 1 , 8, 1);
			 break;	
			
			case 0xC7: // Emergency Stop	
				X_Axis.Motion_Complete_Flag = 1;
				Y_Axis.Motion_Complete_Flag = 1;
				Z_Axis.Motion_Complete_Flag = 1;
				
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	
			
			case 0x07: // X Y Z Go Home	
				X_Axis.Go_Home_Flag = 1;
				Y_Axis.Go_Home_Flag = 1;
				Z_Axis.Go_Home_Flag = 1;
				
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	
			
			case 0x08: // X Y Z Go To Center
				X_Axis.Go_To_Center_Position_Flag = 1;
				Y_Axis.Go_To_Center_Position_Flag = 1;
				Z_Axis.Go_To_Center_Position_Flag = 1;
				
				Alarm(SHORT_BEEP_X1, 1 , 8, 1);
				break;	
			
			case 0x09: //Set New Center
			  Set_Home_Positions();

				Alarm(LONG_BEEP, 1 , 32, 1);
				break;	
			
			case PC_CMD_ENTER_BOOTLOADER: // Enter bootloader
				Bootloader_RequestFromApplication();
				break;




		}	
  }		
}

void Report_To_PC(void)
{
	//Clear_All_Buffers();
	com_Counter++;

	tx_buff[0]= 0x00;  //0	Reserved for coding
	tx_buff[1]= X_Axis.Direction; //1
	tx_buff[2]= com_Counter;  //2
	tx_buff[3]= 0x00;
	tx_buff[4]= (uint8_t)((X_Axis.Pulse_Count & 0x00FF) >> 0);
	tx_buff[5]= (uint8_t)((X_Axis.Pulse_Count & 0xFF00) >> 8);
	tx_buff[6]= (uint8_t)((X_Axis.Pulse_Counter & 0x00FF) >> 0);
	tx_buff[7]= (uint8_t)((X_Axis.Pulse_Counter & 0xFF00) >> 8);
	tx_buff[8]= 0x00;  //8 Reserved for coding
	tx_buff[9]= (X_Axis.Out_of_Range_Error << 3) + (X_Axis.Opto_interrupter_Error_Flag << 2) + (X_Axis.Go_To_Center_Error_Flag << 1) + (X_Axis.Go_To_User_Position_Error_Flag << 0);  //9  X Error byte
	tx_buff[10]= (Y_Axis.Out_of_Range_Error << 3) + (Y_Axis.Opto_interrupter_Error_Flag << 2) + (Y_Axis.Go_To_Center_Error_Flag << 1) + (Y_Axis.Go_To_User_Position_Error_Flag << 0);; //10 y Error byte
	tx_buff[11]= (Z_Axis.Out_of_Range_Error << 3) + (Z_Axis.Opto_interrupter_Error_Flag << 2) + (Z_Axis.Go_To_Center_Error_Flag << 1) + (Z_Axis.Go_To_User_Position_Error_Flag << 0); //11 Z Error byte
	tx_buff[12]= X_Axis.State; //12
	tx_buff[13]= Y_Axis.State; //Y_Axis_State; //13
	tx_buff[14]= Z_Axis.State; //Z_Axis_State; //14
	tx_buff[15]= X_Axis.State + Y_Axis.State + Z_Axis.State; //15
  tx_buff[16]= 0x00; //16	Reserved for coding
	
	//X_Axis.Current_Point = 20;
	float x_round;
	if(X_Axis.Current_Point < 0)
	{
		x_round = -0.5f;
	}
	else
	{
		x_round = + 0.5f;
	}
	X_int = (int32_t)(100 * X_Axis.Current_Point + x_round);
	tx_buff[17]= (uint8_t)((X_int & 0x000000FF) >> 0);
	tx_buff[18]= (uint8_t)((X_int & 0x0000FF00) >> 8);
	tx_buff[19]= (uint8_t)((X_int & 0x00FF0000) >> 16);
	tx_buff[20]= (uint8_t)((X_int & 0xFF000000) >> 24);
	
	float y_round;
	if(Y_Axis.Current_Point < 0)
	{
		y_round = -0.5f;
	}
	else
	{
		y_round = + 0.5f;
	}
	Y_int = (int32_t)(100 * Y_Axis.Current_Point + y_round);
	tx_buff[21]= (uint8_t)((Y_int & 0x000000FF) >> 0);
	tx_buff[22]= (uint8_t)((Y_int & 0x0000FF00) >> 8);
	tx_buff[23]= (uint8_t)((Y_int & 0x00FF0000) >> 16);
	tx_buff[24]= 0x00;  //24	Reserved for coding
	tx_buff[25]= (uint8_t)((Y_int & 0xFF000000) >> 24);
	
	float z_round;
	if(Z_Axis.Current_Point < 0)
	{
		z_round = -0.5f;
	}
	else
	{
		z_round = + 0.5f;
	}
	Z_int = (int32_t)(100 * Z_Axis.Current_Point + z_round);
  tx_buff[26]= (uint8_t)((Z_int & 0x000000FF) >> 0);
	tx_buff[27]= (uint8_t)((Z_int & 0x0000FF00) >> 8);
	tx_buff[28]= (uint8_t)((Z_int & 0x00FF0000) >> 16);
	tx_buff[29]= (uint8_t)((Z_int & 0xFF000000) >> 24);
	
	tx_buff[30]= (uint8_t)((Y_Axis.Pulse_Count & 0x00FF) >> 0);
	tx_buff[31]= (uint8_t)((Y_Axis.Pulse_Count & 0xFF00) >> 8);
	tx_buff[32]= 0x00;  //32	Reserved for coding
	tx_buff[33]= (uint8_t)((Y_Axis.Pulse_Counter & 0x00FF) >> 0);
	tx_buff[34]= (uint8_t)((Y_Axis.Pulse_Counter & 0xFF00) >> 8);
	
	tx_buff[35]= (uint8_t)((Z_Axis.Pulse_Count & 0x00FF) >> 0);
	tx_buff[36]= (uint8_t)((Z_Axis.Pulse_Count & 0xFF00) >> 8);
	tx_buff[37]= (uint8_t)((Z_Axis.Pulse_Counter & 0x00FF) >> 0);
	tx_buff[38]= (uint8_t)((Z_Axis.Pulse_Counter & 0xFF00) >> 8);
	tx_buff[39] = Y_Axis.Direction;
	tx_buff[40] = 0x00; //40	Reserved for coding
	tx_buff[41] = Z_Axis.Direction;
	tx_buff[42] = (int8_t)Z_Axis.Destination_Point;
	tx_buff[43] = Z_Axis.determine_Direction;
	tx_buff[44] = X_Axis.Motion_Complete_Flag;
	tx_buff[45] = Y_Axis.Motion_Complete_Flag;
	tx_buff[46] = Z_Axis.Motion_Complete_Flag;
	tx_buff[47] = X_Axis.Out_of_Range_Error;
	tx_buff[49] = Y_Axis.Out_of_Range_Error;
	tx_buff[50] = Z_Axis.Out_of_Range_Error;
	tx_buff[51] = 0x55;
	tx_buff[52] = 0x55;
	
	uint32_t cs_tx = 0;
	
	for(int i = 0; i < 55; i++)
	{
		cs_tx += tx_buff[i];
	}
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
  tx_buff[55] = cs_tx;
	
	Code_0x1A(tx_buff,56);
	CDC_Transmit_FS(tx_buff,56);
	 
	//HAL_UART_Transmit_IT(&huart1, tx_buff, 56); 
}

void Clear_All_Buffers()
{
	for(int i = 0; i<56; i ++)
	{
		tx_buff[i] = 0x00;
	}
}

static HAL_StatusTypeDef Bootloader_SetRequestFlag(void)
{
	FLASH_EraseInitTypeDef eraseConfig = {0};
	uint32_t pageError = 0U;
	HAL_StatusTypeDef status;

	HAL_FLASH_Unlock();

	eraseConfig.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseConfig.PageAddress = BOOT_FLAG_ADDRESS;
	eraseConfig.NbPages = 1U;

	status = HAL_FLASHEx_Erase(&eraseConfig, &pageError);

	if(status == HAL_OK)
	{
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, BOOT_FLAG_ADDRESS, BOOT_FLAG_REQUEST_VALUE);
	}

	HAL_FLASH_Lock();

	return status;
}

static void Bootloader_RequestFromApplication(void)
{
	if(Bootloader_SetRequestFlag() == HAL_OK)
	{
		__disable_irq();
		NVIC_SystemReset();
	}
}


	
void Code_0x1A(uint8_t* Buf, uint32_t Len)
{
	int  Code_Byte = 0;
	int  codeIndex = 0;
	uint32_t  n = 0;
	uint32_t  loopCount = Len/8;
	int nx8 = 0 ;
	
	
// 		This coding sets a Code Byte in the form abc0 defg with the following dataPack sequence
//		CODE_0_6	| code_byte6	| code_byte5	| code_byte4	|	0	| code_byte3 |	code_byte2 |	code_byte1 |	code_byte0		0x1A never happens									
//		byte0										
//		byte1										
//		byte2										
//		byte3										
//		byte4										
//		byte5										
//		byte6	
	
	
	// Code 0x1A for normal command data (8 bytes) -----------------------------------------------------------------------------
	for( n = 0 ; n < loopCount ; n++)
	{
		nx8 = n * 8 ;
		
		Buf[nx8] = 0;
		for(codeIndex = 0 ; codeIndex < 8 ; codeIndex++)
		{
			if(Buf[nx8 + codeIndex + 1] == 0x1A)
			{
				
				Buf[nx8] |= 0x01 << codeIndex ;
				Buf[nx8+ codeIndex + 1] = 0xA1 ; //  this is replaced to 0x1A to help finding this code

			}
		}

		 //Shift high nibble of code byte one bit to left to clear bit position 4 to 0, this avoids occurence of 0x1A in code byte itself
		 
		 Code_Byte = (0xF0 & Buf[nx8]) << 1;
		 Buf[nx8] = (0x0F & Buf[nx8]) | Code_Byte;
	}
}
