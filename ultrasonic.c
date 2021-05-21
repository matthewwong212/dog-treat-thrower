#include "ultrasonic.h"


// On PA0, TIM5_CH1
void Input_Capture_Setup() {	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;				// (1a) Enable the clock for GPIO Port B
	GPIOA->MODER &= ~GPIO_MODER_MODE0;					// (1b) Clear mode 6
	GPIOA->MODER |= GPIO_MODER_MODE0_1;					// (1b) Set mode 6[1] to 1, resulting in mode 6 = 10 (alternate function)
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0;					// (1b) Clear AFSEL6 field
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1;				// (1b) Configure PB6 to be used as alternate function TIM4_CH1 (AF2)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;					// (1c) Set PB6 to no pull-up, no pull-down
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;				// (2) Enable timer 4
	TIM5->PSC = 79;															// (3) If Sysclock is 80MHz
	TIM5->ARR = 0xFFFF;													// (4) Set the auto reload value to its maximum value
	TIM5->CR1 |= TIM_CR1_ARPE;									// (4) Enable auto reload preload
	
	TIM5->CCMR1 &= ~TIM_CCMR1_CC1S;							// (5) Clear the input capture mode bits
	TIM5->CCMR1 |= TIM_CCMR1_CC1S_0;						// (5) Set the input capture mode such that the input capture is mapped to timer input 1 (01)
	TIM5->CCER |= TIM_CCER_CC1P;								// (6) Set bits to capture both rising and falling edges
	TIM5->CCER |= TIM_CCER_CC1NP;								// (6) Set bits to capture both rising and falling edges
	TIM5->CCER |= TIM_CCER_CC1E;								// (6) Enable capturing
	
	TIM5->DIER |= TIM_DIER_CC1IE;								// (7) Enable interrupt for channel 1
	TIM5->DIER |= TIM_DIER_CC1DE;								// (7) In the DMA/Interrupt register, enable DMA requests
	TIM5->DIER |= TIM_DIER_UDE;									// (7) In the DMA/Interrupt register, enable DMA requests
	TIM5->DIER |= TIM_DIER_UIE;									// (7) Enable update interrupt
	
	TIM5->EGR |= TIM_EGR_UG;										// (8) Enable update generation in the event generation register
	TIM5->SR &= ~TIM_SR_UIF;										// (9) Clear the update interrupt flag
	TIM5->SR &= ~TIM_SR_CC1IF;									// clear channel 1 interrupt flag
	TIM5->CR1 &= ~TIM_CR1_DIR;									// (10) Set the direction of the counter (upcounter)(0)
	TIM5->CR1 |= TIM_CR1_CEN;										// (10) Enable the counter
	
	NVIC_EnableIRQ(TIM5_IRQn);									// (11) Enable interrupt TIM4_IRQn
	NVIC_SetPriority(TIM5_IRQn, 2);							// (11) Set priority to 2
}

void Sensor_Trigger_Setup() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;				// (1a) Enable the clock for GPIO Port E
	GPIOE->MODER &= ~GPIO_MODER_MODE11;					// (1b) Clear Mode 11
	GPIOE->MODER |= GPIO_MODER_MODE11_1;				// (1b) Set Mode 11 [1] to 1, resulting in Mode 11 = 10 (Alternate function)
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL11;				// (1b) Clear AFSEL11 field
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL11_0;				// (1b) Configure PE11 to be used as alternative function TIM1_CH2 (AF1)
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD11;					// (1c) Set PE11 to no pull-up, no pull-down (0)
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT11;					// (1d) Set output type of PE11 to push-pull (0)
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED11;		// (1e) Set PE11 to high output speed (11)
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;					// (2) Enable Timer 1 in RCC_APB2ENR
	TIM1->PSC = 15;															// (3) Set the prescaler to 15
	TIM1->CR1 |= TIM_CR1_ARPE;									// (4) Enable auto reload preload in the control register
	TIM1->ARR = 0xFFFF;													// (4) Set the auto reload value to its maximum value
	TIM1->CCR2 = 10;														// (5) Set CCR2 to trigger sensor (10 microseconds)
	
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;														// (6) Clear Output Control Mode
	TIM1->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);			// (6) Set Output control mode to PWM mode 1 (0110)
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;														// (6) Enable output compare preload
	TIM1->CCER |= TIM_CCER_CC2E;															// (7) Enable the output in the capture/compare mode register

	TIM1->BDTR |= TIM_BDTR_OSSR;															// (8) In BDTR, set the bit for off-state selection for run mode
	TIM1->BDTR |= TIM_BDTR_MOE;																// (8) In BDTR, set the bit for main output enable
	TIM1->EGR |= TIM_EGR_UG;																	// (9) Enable update generation in the event generation register
	TIM1->DIER |= TIM_DIER_CC2IE;	
	TIM1->DIER |= TIM_DIER_UIE;																// (10) Enable update interrupt
	TIM1->SR &= ~TIM_SR_UIF;																	// (10) Clear the update interrupt flag
	TIM1->CR1 &= ~TIM_CR1_DIR;													  		// (11) Set the direction of the counter (upcounting)
	TIM1->CR1 |= TIM_CR1_CEN;																	// (11) Enable the counter
}
