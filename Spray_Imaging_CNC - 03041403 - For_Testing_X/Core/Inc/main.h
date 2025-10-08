/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PC_Communication.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "Alarm.h"
#include "motor_Controller.h"
#include "stdio.h"
#include "string.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_1_Pin GPIO_PIN_13
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_14
#define LED_2_GPIO_Port GPIOC
#define Z_Driver_ENA_Pin GPIO_PIN_0
#define Z_Driver_ENA_GPIO_Port GPIOA
#define Z_Driver_OPTO_Pin GPIO_PIN_1
#define Z_Driver_OPTO_GPIO_Port GPIOA
#define Z_Driver_DIR_Pin GPIO_PIN_2
#define Z_Driver_DIR_GPIO_Port GPIOA
#define Z_Driver_Pulse_Pin GPIO_PIN_3
#define Z_Driver_Pulse_GPIO_Port GPIOA
#define Y_Driver_ENA_Pin GPIO_PIN_4
#define Y_Driver_ENA_GPIO_Port GPIOA
#define Y_Driver_OPTO_Pin GPIO_PIN_5
#define Y_Driver_OPTO_GPIO_Port GPIOA
#define Y_Driver_DIR_Pin GPIO_PIN_6
#define Y_Driver_DIR_GPIO_Port GPIOA
#define Y_Driver_Pulse_Pin GPIO_PIN_7
#define Y_Driver_Pulse_GPIO_Port GPIOA
#define X_Driver_ENA_Pin GPIO_PIN_0
#define X_Driver_ENA_GPIO_Port GPIOB
#define X_Driver_OPTO_Pin GPIO_PIN_1
#define X_Driver_OPTO_GPIO_Port GPIOB
#define X_Driver_DIR_Pin GPIO_PIN_10
#define X_Driver_DIR_GPIO_Port GPIOB
#define X_Driver_Pulse_Pin GPIO_PIN_11
#define X_Driver_Pulse_GPIO_Port GPIOB
#define Push_Button_Input_Pin GPIO_PIN_15
#define Push_Button_Input_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_8
#define Buzzer_GPIO_Port GPIOA
#define USART_uC_TX_Pin GPIO_PIN_9
#define USART_uC_TX_GPIO_Port GPIOA
#define USART_uC_RX_Pin GPIO_PIN_10
#define USART_uC_RX_GPIO_Port GPIOA
#define Z_Opto_Interrupter_Pin GPIO_PIN_15
#define Z_Opto_Interrupter_GPIO_Port GPIOA
#define X_Opto_Interrupter_Pin GPIO_PIN_3
#define X_Opto_Interrupter_GPIO_Port GPIOB
#define Y_Opto_Interrupter_Pin GPIO_PIN_4
#define Y_Opto_Interrupter_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
