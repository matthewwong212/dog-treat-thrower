#ifndef __STM32L476G_DISCOVERY_MOTOR_H
#define __STM32L476G_DISCOVERY_MOTOR_H

#include "stm32l476xx.h"

void motorsInit(void);
void releaseTrigger(void);
void holdTrigger(void);
void slackTension(void);
void pullTension(void);
void delay(int times);
void fire(void);
void resetArm(void);

#endif
