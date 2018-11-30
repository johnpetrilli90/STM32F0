////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************** BEGIN PID.c ***************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		PID.c
// Description: This file is initializes and sets the duty cycle for
//							a PWM output which is controlled by the PID loop.
//
// Author:			John Petrilli
// Date:				22.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>
#include "PID.h"																						//PID.h header file

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** INSERT FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitPWM(void){
		// Configure alternate fucntion PWM output
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;                      //Enable GPIOA clock
    GPIOA->MODER |= GPIO_MODER_MODER8_1;                    //Pin A8 mode = Alt Function
    GPIOA->AFR[1] |= 0x02;                                	//Pin A8 alt function = AF2 (TIM1)
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8;               //Pin A8 speed = 50MHz

    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;                     //Enable clock
    TIM1->PSC = 23;                                         //Prescaler = 23 (1 tick per half microsecond)
    TIM1->ARR = 19999;                                      //Auto-reload = PWM frequency in half microseconds = 10ms period
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;     //OC1M = 110 for PWM Mode 1 output on ch1
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;                         //Output 1 preload enable
    TIM1->CR1 |= TIM_CR1_ARPE;                              //Auto-reload preload enable
    TIM1->CCER |= TIM_CCER_CC1E;                            //Enable output for ch1
    TIM1->CCR1 = 1800;                                      //CCR1 = Duty cycle in half microseconds = high for 900us
    TIM1->EGR |= TIM_EGR_UG;                                //Force update
		TIM1->BDTR |= TIM_BDTR_MOE;															//Main output enable
    TIM1->CR1 |= TIM_CR1_CEN;                               //Enable counter
}

void Set_PWM_Duty(int DutyCycle){
	//The prescaler for PWM is set the 1 tick for every 500ns
	//The PWM frequency is set to 100Hz
	//If duty cycle variable = 2000 then the actual PWM D.C. is 10%
	//100% duty cycle is 19999 (same as TIM->ARR)
	DutyCycle *= 200;																					//Convert 0-100% input to correlate to PWM set up
	TIM1->CCR1 = DutyCycle;																		//Set duty cycle
	TIM1->EGR |= TIM_EGR_UG;                             			//Force PWM update		
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************** END FUNCTIONS **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************* END PID.c **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
