////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************** BEGIN InitIO.c ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		InitIO.c
// Description: This file configures the PA4 Key input/output
//							We also configure all other unused GPIOs to floating input
//							We implement functions to lock the GPIO config registers
//
// Author:			John Petrilli & Jack Haller
// Date:				22.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>
#include "InitIO.h"													//Header file for InitIO	
#include "UART.h"														//UART header for setting baud rate

#define		PA4		4
#define 	PB7		7

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** INSERT FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitIO(void){
	//Enable Clocks
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;				//Enable Port A Clk for unused pins
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;				//Enable Port B Clk for unused pins
	
	/*************************************************************************************
	* Port A: 
	*		PA0  - ADC
	*		PA2  - TX
	*		PA3  - RX
	*		PA4  - KEY
	*		PA8  - PWM
	*		PA12 - Comm LED
	*		PA13 - SWDIO
	*		PA14 - SWDCLK
	*
	*	Port B:
	*		PB7  - PMOS On/Off
	*************************************************************************************/
	
	//PA4 - General Purpose Output. Pullup
	//Used for putting the BT module into AT command mode
	GPIOA->MODER |= GPIO_MODER_MODER4_0;			//Set to general purpose output
	GPIOA->MODER &= ~GPIO_MODER_MODER4_1;			//Set to general purpose output
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;				//Set push/pull output
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR4;	//High speed output
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_0;			//Yes pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4_1;			//No pull down
	GPIOA->BSRR |= GPIO_BSRR_BR_4;						//Set GPIOB Pin 7 low initially
	
	//PB7 - General Purpose Output. Pullup
	//Used for turning the BT on/off
	GPIOB->MODER |= GPIO_MODER_MODER7_0;			//Set to general purpose output
	GPIOB->MODER &= ~GPIO_MODER_MODER7_1;			//Set to general purpose output
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_7;				//Set push/pull output
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR7;	//High speed output
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;			//Yes pull up
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_1;			//Yes pull down
	GPIOB->BSRR |= GPIO_BSRR_BS_7;						//Set GPIOA Pin 4 high initially
	
	//PA1 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER1_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER1_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1_1;		//No pull down
	//PA5 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER5_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER5_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5_1;		//No pull down
	//PA6 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER6_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER6_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6_1;		//No pull down
	//PA7 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER7_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER7_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7_1;		//No pull down
	//PA9 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER9_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER9_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9_1;		//No pull down
	//PA10 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER10_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER10_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10_1;		//No pull down
	//PA11 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER11_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER11_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR11_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR11_1;		//No pull down
	//PA15 - Input Floating
	GPIOA->MODER &= ~GPIO_MODER_MODER15_0;	//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER15_1;	//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR15_0;	//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR15_1;	//No pull down
	//PB0 - Input Floating
	GPIOB->MODER &= ~GPIO_MODER_MODER0_0;		//Set to floating input
	GPIOB->MODER &= ~GPIO_MODER_MODER0_1;		//Set to floating input
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0_0;		//No pull up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0_1;		//No pull down
	//PB1 - Input Floating
	GPIOB->MODER &= ~GPIO_MODER_MODER1_0;		//Set to floating input
	GPIOB->MODER &= ~GPIO_MODER_MODER1_1;		//Set to floating input
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR1_0;		//No pull up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR1_1;		//No pull down
	//PB2 - Input Floating
	GPIOB->MODER &= ~GPIO_MODER_MODER2_0;		//Set to floating input
	GPIOB->MODER &= ~GPIO_MODER_MODER2_1;		//Set to floating input
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR2_0;		//No pull up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR2_1;		//No pull down
	//PB3 - Input Floating
	GPIOB->MODER &= ~GPIO_MODER_MODER3_0;		//Set to floating input
	GPIOB->MODER &= ~GPIO_MODER_MODER3_1;		//Set to floating input
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3_0;		//No pull up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3_1;		//No pull down
	//PB4 - Input Floating
	GPIOB->MODER &= ~GPIO_MODER_MODER4_0;		//Set to floating input
	GPIOB->MODER &= ~GPIO_MODER_MODER4_1;		//Set to floating input
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR4_0;		//No pull up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR4_1;		//No pull down
	//PB5 - Input Floating
	GPIOB->MODER &= ~GPIO_MODER_MODER5_0;		//Set to floating input
	GPIOB->MODER &= ~GPIO_MODER_MODER5_1;		//Set to floating input
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR5_0;		//No pull up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR5_1;		//No pull down
	//PB6 - Input Floating
	GPIOB->MODER &= ~GPIO_MODER_MODER6_0;		//Set to floating input
	GPIOB->MODER &= ~GPIO_MODER_MODER6_1;		//Set to floating input
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6_0;		//No pull up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6_1;		//No pull down
}

//Lock GPIOA config registers
void LockGPIOA(uint16_t Lock){
	Lock = GPIO_LCKR_LCK0 << Lock;					//Configure correct Port Pin 
	GPIOA->LCKR = GPIO_LCKR_LCKK + Lock; 		//First write sequence to lock reg
	GPIOA->LCKR = Lock; 										//Second write sequence to lock reg
	GPIOA->LCKR = GPIO_LCKR_LCKK + Lock;		//Third write sequence to lock reg
	GPIOA->LCKR; 														//Last sequence to lock reg
}

//Lock GPIOA config registers
void LockGPIOB(uint16_t Lock){
	Lock = GPIO_LCKR_LCK0 << Lock;					//Configure correct Port Pin 
	GPIOB->LCKR = GPIO_LCKR_LCKK + Lock; 		//First write sequence to lock reg
	GPIOB->LCKR = Lock; 										//Second write sequence to lock reg
	GPIOB->LCKR = GPIO_LCKR_LCKK + Lock;		//Third write sequence to lock reg
	GPIOB->LCKR; 														//Last sequence to lock reg
}

//Set to normal operation
void SetATmode(void){
	SetBaud38400();													//Set baud rate to correct value
	GPIOA->BSRR |= GPIO_BSRR_BS_4;					//Set GPIOB Pin 4
	Delay(9600000);													//Delay for about 200ms
	GPIOB->BSRR |= GPIO_BSRR_BR_7;					//Set GPIOB Pin 7
}

//Set to BT initialization
void SetCommMode(void){
	SetBaud9600();													//Set baud rate to correct value	
	GPIOA->MODER &= ~GPIO_MODER_MODER4_0;		//Set to floating input
	GPIOA->MODER &= ~GPIO_MODER_MODER4_1;		//Set to floating input
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4_0;		//No pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4_1;		//No pull down
	GPIOB->BSRR |= GPIO_BSRR_BS_7;					//Turn off power to BT
	Delay(9600000);													//Delay about 200ms
	GPIOB->BSRR |= GPIO_BSRR_BR_7;					//Restore power to BT	
}

//Used for delay during BT setup
void Delay(int x){
	while(x){															//delay x number of clock cycles
		x--;																//Each clk cyc. is ~21ns
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************** END FUNCTIONS **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************** END InitIO.c ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
