/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32wlxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN Private defines */
//DHT11 values structure
typedef union
{
	uint32_t getResult;

	struct
	{
		uint8_t decT;
		uint8_t intT;
		uint8_t decRH;
		uint8_t intRH;

	}Values;

}DHT11_t;

//Particle sensor values structure
typedef union
{
	uint8_t getResult[11];

	struct
	{
		uint8_t B0;//0xFE
		uint8_t B1;//0xFE
		uint8_t B2;//0xA5
		uint8_t B3;//0x02
		uint8_t B4;//0x00
		uint8_t B5;//DF11
		uint8_t B6;//DF12 -> pm1.0 value
		uint8_t B7;//DF21
		uint8_t B8;//DF22 -> pm2.5 value
		uint8_t B9;//DF31
		uint8_t B10;//DF32 -> pm10 value
		uint8_t B11;//[CS]


	}Values;

}IH_PMC_t;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
