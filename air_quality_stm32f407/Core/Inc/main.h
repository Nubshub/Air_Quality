/*
 * main.h
 *
 *  Created on: Nov 15, 2023
 *      Author: admin
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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


void SysClockConfig(void);

void RTC_AlarmConfig(void);
void RTC_read(void);
void RTC_Init(void);

void UART2_Init(void);
void UART3_Init(void);

void TIM6_Init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t delay);

void PD11_Init_Output(void);
void PD11_Init_Input(void);
void DHT11_start(void);
uint8_t Check_Response(void);
void DHT11_read(void);

void Particle_sensor_values(void);
void Temp_humidity_values(void);



#endif /* INC_MAIN_H_ */
