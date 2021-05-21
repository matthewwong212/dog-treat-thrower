#ifndef __STM32L476G_DISCOVERY_NUNCHUCK_H
#define __STM32L476G_DISCOVERY_NUNCHUCK_H

#include "stm32l476xx.h"
#include "I2C.h"

void Nunchuck_Init(void);
void Nunchuck_Get_Data(uint8_t *dataAdd);
int Nunchuck_Z_Button(void);
int Nunchuck_C_Button(void);

#endif
