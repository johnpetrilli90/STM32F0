////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************** BEGIN UART.c ****************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		UART.c
// Description: USART peripherial is used to communicate over Bluetooth SPP
// 							PA2  => USART1 Tx : No Remap
// 							PA3  => USART1 Rx : No Remap
// Author:			John Petrilli & Jack Haller
// Date:				22.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>
#include "UART.h"													//UART Files

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** DEFINE VARIABLES HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char RX[50];															//Receive buffer for RS232
char *pRx;																//Pointer to shift through Tx command
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************** END VARIABLE DEFINITION ***********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** INSERT FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function is used to init the uart
void InitUART(void){
// Enable Clocks
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;			//Enable USART1 peripheral clk
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;				//Enable Port A Clk for USART1

// Configure PA2 (Tx) and PA3 (Rx)
	GPIOA->AFR[0]  |=  0x01 << (2 * 4);				//AFRL, PA2, UART1
	GPIOA->AFR[0]  |=  0x01 << (3 * 4);				//AFRL, PA3, UART1
	GPIOA->MODER   &= ~0x000000F0;						//Clear PA2 & PA3
	GPIOA->MODER   |=  GPIO_MODER_MODER2_1;		//Alternate Fxn mode
	GPIOA->MODER   |=  GPIO_MODER_MODER3_1;		//Alternate Fxn mode
	GPIOA->OTYPER  &= ~GPIO_OTYPER_OT_2;			//Output Push/Pull
	GPIOA->OTYPER  &= ~GPIO_OTYPER_OT_3;			//Output Push/Pull
	GPIOA->OSPEEDR |=  GPIO_OSPEEDR_OSPEEDR2;	//PA2 high speed
	GPIOA->OSPEEDR |=  GPIO_OSPEEDR_OSPEEDR3;	//PA3 high speed
	GPIOA->PUPDR   |=  GPIO_PUPDR_PUPDR2_0;		//PA2 Pull up
	GPIOA->PUPDR   |=  GPIO_PUPDR_PUPDR3_0;		//PA3 Pull up

// Configure UART1
	USART1->CR1 &= ~USART_CR1_UE;							//Disable UART for configuring
	USART1->CR1 &= ~USART_CR1_M;							//Data length = 8bits
	NVIC_EnableIRQ(USART1_IRQn);							//Enable USART1 IRQ
	USART1->CR1 |= USART_CR1_RXNEIE;					//Rx interrupt enable
	USART1->CR1 &= ~USART_CR1_PCE;						//No parity control
	USART1->CR1 &= ~USART_CR2_STOP;						//1 stop bit
	USART1->CR1 &= ~USART_CR3_CTSE;						//No hardware flow control
	USART1->CR1 &= ~USART_CR3_RTSE;						//No hardware flow control
	USART1->BRR = (SystemCoreClock/38400);		//Baud Rate of 115.2Kbps
	USART1->CR1 |= USART_CR1_RE;							//Recieve enable
	USART1->CR1 |= USART_CR1_TE;							//Transmit enable
	USART1->CR1 |= USART_CR1_UE;							//USART enable
	pRx = &RX[0];															//Make sure pRx is pointed at first position of buffer
}	

//Set baud rate for typical communication (AT mode 1)
void SetBaud9600(void){
	USART1->BRR = (SystemCoreClock/9600);			//Baud Rate of 9600
}

//Set baud rate for setting up BT module (AT mode 2)
void SetBaud38400(void){
	USART1->BRR = (SystemCoreClock/38400);		//Baud Rate of 38400
}

//This function prints to the comm port 
void PrintBT(char* pString){	
	USART1->CR1 |= USART_CR1_TE;							//Set transmit enable bit, prepare for sending data
	while(*pString){													//Print char's as long as not a null char
		USART1->TDR = *pString;									//Put char into the data register
		while(!(USART1->ISR & USART_ISR_TXE));	//Wait for char to go to shift register
		pString++;															//Increment pointer one space in memory
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************** END FUNCTIONS **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
