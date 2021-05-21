#include "motorControl.h"
#include "LED.h"

void motorsInit(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	// Enable Port A
	
	// Trigger motor init
	GPIOA->MODER &= ~GPIO_MODER_MODE5;		// clear mode
	GPIOA->MODER |= GPIO_MODER_MODE5_1;	// Set as alternate function
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;		// Clear afsel0 field
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;	// Set to AF1 (TIM2_CH1)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;		// Set PA0 to no pull up, no pull down
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;		// Set to push-pull output type
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;	// Set to high output speed
	
	// Tensioner motor init
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	GPIOA->MODER |= GPIO_MODER_MODE1_1;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL1_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1;
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;	// Enable timer 2
	TIM2->PSC = 79;				// If Sysclock is 80MHz
	TIM2->CR1 |= TIM_CR1_ARPE;		// enable auto reload preload
	TIM2->ARR = 0x4E20;						// Set period to 20ms
	TIM2->CCR1 = 1100;						// Channel 1 ~1.1ms period on, 5% duty cycle, ARM RELEASED
	TIM2->CCR2 = 355;						// Channel 2 (Tensioner)
	
	TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);		// Clear output control mode
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);		// Set channel 1 and 2 to PWM mode 1 (0110)
	TIM2->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);				// Enable output compare preload
	TIM2->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);		// Enable the output in the capture/compare mode register
	
	TIM2->BDTR |= TIM_BDTR_OSSR;				// Set the bit for off-state selection for run mode
	TIM2->BDTR |= TIM_BDTR_MOE;					// Set the bit for main output enable
	TIM2->EGR |= TIM_EGR_UG;				// enable update generation
	TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_CC2IE); 		// Enable interrupt
	TIM2->DIER |= TIM_DIER_UIE;		// enable update interrupt
	TIM2->SR &= ~TIM_SR_UIF;		// Clear update interrupt flag
	TIM2->CR1 &= ~TIM_CR1_DIR;		// set to upcounting mode
	TIM2->CR1 |= TIM_CR1_CEN;		// enable the counter
}

void delay(int times) {
	for (int i = 0; i < times; i++) {
		for (int k = 0; k < 50000; ++k);
	}
}

void releaseTrigger(void) {
	TIM2->CCR1 = 1100;						// Channel 1 = ~1.1ms period on, 5% duty cycle, ARM RELEASED
}

void holdTrigger(void) {
	TIM2->CCR1 = 2100;
}

void slackTension(void) {
	TIM2->CCR2 = 355;
}

void pullTension(void) {
	TIM2->CCR2 = 2600;
}

void resetArm(void) {
	Yellow_LED_On();
	releaseTrigger();
	slackTension();
	delay(500);			// Delays to allow arm to fall, test values
	holdTrigger();
	delay(400);
	Yellow_LED_Off();
	Green_LED_On();
}

void fire(void) {
	Green_LED_Off();
	Yellow_LED_On();
	pullTension();
	delay(640);
	releaseTrigger();
	delay(100);
	//resetArm();
}




