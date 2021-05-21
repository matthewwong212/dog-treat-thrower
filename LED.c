#include "LED.h"

int greenOn = 0;
int redOn = 0;
int yellowOn = 0;

void LED_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;	// Enable ports b and c
	GPIOB->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);		// Clear mode for PB2/PB3
	GPIOB->MODER |= (GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);		// Set to general purpose output mode
	GPIOE->MODER &= ~GPIO_MODER_MODE12;
	GPIOE->MODER |= GPIO_MODER_MODE12_0;
}

// RED ON PB2
void Red_LED_On(void) {
	GPIOB->BSRR &= ~GPIO_BSRR_BR2;	// clear reset
	GPIOB->BSRR |= GPIO_BSRR_BS2;		// set
	redOn = 1;
}

void Red_LED_Off(void) {
	//GPIOB->BSRR &= ~GPIO_BSRR_BS2;	// clear set
	GPIOB->BSRR |= GPIO_BSRR_BR2;		// reset
	redOn = 0;
}

// GREEN ON PB3
void Green_LED_On(void) {
	GPIOB->BSRR &= ~GPIO_BSRR_BR3;
	GPIOB->BSRR |= GPIO_BSRR_BS3;
	greenOn = 1;
}

void Green_LED_Off(void) {
	//GPIOB->BSRR &= ~GPIO_BSRR_BS3;
	GPIOB->BSRR |= GPIO_BSRR_BR3;
	greenOn = 0;
}

void Yellow_LED_On(void) {
	GPIOE->BSRR &= ~GPIO_BSRR_BR12;
	GPIOE->BSRR |= GPIO_BSRR_BS12;
	yellowOn = 1;
}

void Yellow_LED_Off(void) {
	GPIOE->BSRR |= GPIO_BSRR_BR12;
	yellowOn = 0;
}

void Yellow_LED_Toggle(void) {
	if (yellowOn == 1) {
		Yellow_LED_Off();
	} else {
		Yellow_LED_On();
	}
}

void Green_LED_Toggle(void) {
	if (greenOn == 1) {
		Green_LED_Off();
	} else {
		Green_LED_On();
	}
}

void Red_LED_Toggle(void) {
	if (redOn == 1) {
		Red_LED_Off();
	} else {
		Red_LED_On();
	}
}
