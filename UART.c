#include "UART.h"

void UART2_Init(void) {
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;					// (1a) Enable the USART2 clock in the peripheral clock register
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;							// (1b) Clear USART2 Clock source selection
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;						// (1b) Select the system clock as the USART2 clock source in the peripheral indpendent clock config register (01)
}

void UART2_GPIO_Init(void) {	// [TODO]: MODIFY TO PA2/PA3
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;								
	
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
	
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFSEL3);														// Clear alternate function for PB6 and PB7
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2);	// Set PB6 to AF7 (USART1 Tx)
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2);	// Set PB7 to AF7 (USART1 Rx)
	
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);	// Set PB6 and PB7 to very high speed
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);						// Set both pins to push-pull output type
	
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);						// Clear pull up/pull down mode for PB6 and PB7
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0);				// Set both to pull up (01)
}

void USART_Init(USART_TypeDef* USARTx) {
	USARTx->CR1 &= ~USART_CR1_UE;					// Disable USART before configuration
	
	USARTx->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);		// (3a) Set word length to 8 bits
	USARTx->CR1 &= ~USART_CR1_OVER8;									// (3a) Set oversampling mode to oversample by 16
	USARTx->CR2 &= ~USART_CR2_STOP;										// (3a) Set number of stop bits to 1 (00)
	
	USARTx->BRR = 8333;																// (3b) Set baud rate to 9600
	
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);			// (3c) Enable transmit and receive
	USARTx->CR1 |= USART_CR1_UE;											// (3d) Enable USART
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
