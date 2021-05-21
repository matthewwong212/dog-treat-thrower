/*
 * ECE 153B
 *
 * Name(s): Matthew Wong
 * Section: Wednesday 7:00PM
 * Final Project
 * COM4 For Bluetooth
 */
 
#include <stdio.h> 
#include <string.h>
#include "stm32l476xx.h"
#include "motorControl.h"
#include "LED.h"
#include "I2C.h"
#include "nunchuck.h"
#include "ultrasonic.h"
#include "SysClock.h"
#include "UART.h"

uint32_t volatile currentValue = 0xFFFFFFFF;
uint32_t volatile lastValue = 0xFFFFFFFF;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;		// in count units (1 us per count)
int readyToFire = 0;
int autoEnable = 0;
int fireCount = 0;
char message[8];
char countStr[10];

void UART_Init(void) {
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
}

// Ultrasonic Sensor Distance Reading Handler
void TIM5_IRQHandler(void) {
	if (TIM5->SR & TIM_SR_UIF) {	// overflow
		TIM5->SR &= ~TIM_SR_UIF;
		if (lastValue != 0xFFFFFFFF) {
			overflowCount++;
		}
	} else if (TIM5->SR & TIM_SR_CC1IF) {	// interrupt from channel 1 
		if (lastValue == 0xFFFFFFFF) {	// last value needs to be set
			lastValue = TIM5->CCR1;
		} else {	// falling edge
			currentValue = TIM5->CCR1;
			timeInterval = currentValue - lastValue + (0x1 + 0xFFFF) * overflowCount;
			overflowCount = 0;
			lastValue = 0xFFFFFFFF;
			currentValue = 0xFFFFFFFF;
		}
	}
	NVIC_ClearPendingIRQ(TIM5_IRQn);
}

void fireSequence(void) {
	readyToFire = 0;
	fireCount++;
	printf("Firing\r\n");
	sprintf(countStr, "%d", fireCount);
	printf("Fire Count: ");
	printf("%s", countStr);
	printf("\r\n");
	fire();
	printf("Resetting...\r\n");
	resetArm();
	readyToFire = 1;
	printf("Ready to fire.\r\n");
}

int main(void) {	
	// Initialize peripherals
	System_Clock_Init();
	Input_Capture_Setup();
	Sensor_Trigger_Setup();
	LED_Init();	
	I2C_GPIO_Init();
	I2C_Initialization();
	Nunchuck_Init();
	UART_Init();
	motorsInit();
	
	// Initial setup and arm reset
	resetArm();
	readyToFire = 1;
	
	float distance;
	while(1) {
		// Toggle auto function
		if (Nunchuck_C_Button() == 1) {
			if (autoEnable == 1) {
				autoEnable = 0;
				Red_LED_Off();
				printf("Sensor OFF\r\n");
			} else {
				autoEnable = 1;
				Red_LED_On();
				printf("Sensor ON\r\n");
			}
			while (Nunchuck_C_Button() == 1);
		}
		
		// Fire button
		if (Nunchuck_Z_Button() == 1) {
			if (readyToFire == 1) {
				fireSequence();
			}
			while (Nunchuck_Z_Button() == 1);
		}
		
		if (timeInterval < 37500) {
			distance = timeInterval / 58.0;
		}

		if ((distance < 10) && readyToFire == 1 && autoEnable == 1) {
			fireSequence();
		}
		
		// Some delay
		for(int i = 0; i < 50000; ++i); 
	}
	
}
