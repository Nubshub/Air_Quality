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

#define IH_PCM_002_ADDR 0x08
#define START_CMD 0x1000100500F6
#define STOP_CMD 0x100104
#define INIT_CMD 0xFF



void SysClockConfig(void);

void UART2_Init(void);

void I2C1_Init(void);

void delay_ms(uint32_t delay);



#endif /* INC_MAIN_H_ */
