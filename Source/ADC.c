////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************** BEGIN ADC.c ***************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		ADC.c
// Description: 12 bit conversion, 3.3 Vref so scaling factor is...
//							voltage = 3300/0xFFF x adc reading (mV)
//							
//							Also included is FindTemp() & Interpolate() fxn for
//							getting value of temp from adc
//
// Author:			John Petrilli & Jack Haller
// Date:				22.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>
#include "ADC.h"								// ADC.h header file

//This lookup table is used to determine the temperature of the CPU
const int TempLookUp[34] = {197388,149395,114345,88381,68915,54166,42889,34196,27445,
														22165,18010,14720,12099,10000,8309,6939,5824,4911,4160,3539,
														3024,2593,2233,1929,1673,1455,1270,1112,976,860,759,673,598,532};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** INSERT FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This function finds the two values in the lookup table that the measured resistance falls between
extern int FindTemp(int Radc){
	int 	index = 0,							//array index of lookup table
				found = 0;							//found position in array
	
	if(Radc <= 532){							//check for value under -40C
		found = 1;									//set flag
		index = 33;									//set index to lowest resistance value
	}
	else if(Radc >= 197388){			//check for value over 125C
		found = 1;									//set flag
		index = -1;									//set index to negative for interpolation fxn to parse
	}
	while(!found){								//stay in loop until flag is set
		if(index >= 33){						//if index reached end of array then exit loop
			index = 33;								//set index to end of array
			found = 1;								//set flag
		}
		else if(Radc >= TempLookUp[index] && Radc < TempLookUp[index-1]){
			found = 1;								//set flag
		}
		else{
			index++;									//increment position in array
		}			
	}
	
	return Interpolate(index, Radc);	//return interpolated value
}

//This fxn calculates the temperature by interpolation of the look up table from the 
//thermistor datasheet. Interpolate between two values 5C apart.
extern int Interpolate(int i, int Radc){
	int 		Ltemp,								//low temperature
					Mtemp,								//calculated temperature
					Htemp;								//high temperature

	if(i == -1){
		return 125000;							//If index is negative return 125C
	}
	else if(i == 33){
		return -40000;							//If index is at end of array return -40C
	}
	else{													//else calculate interpolated temperature
		Htemp = (5*i - 40)*1000;		//calculate known temp based off of index position
		Ltemp = Htemp - 5000;				//subtract 5C from the high temp to get low temp
		Mtemp = Ltemp +((TempLookUp[i-1] - Radc)*(Htemp - Ltemp))/(TempLookUp[i-1] - TempLookUp[i]); //interpolate
		return Mtemp;								//return temperature
	}
}	


void InitADC(void){
	// Select the HSI14 clock as source
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;				// Enable adc clock
	RCC->CR2 		 |= RCC_CR2_HSI14ON; 					// Turn on clock for adc
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;				// Enable Port A Clk for analog input
	while (!(RCC->CR2 & RCC_CR2_HSI14RDY));		// Wait for clock to turn on
	
	// Configure GPIO PA0 for analog input
	GPIOA->MODER |= GPIO_MODER_MODER0;				// Set mode to analog
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;				// No pullup
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1;				// No pulldown
	
	// Configure clock, channel selction, sample speed
	ADC1->CFGR1  &= ~ADC_CFGR1_SCANDIR;				// Toggle scan direction
	ADC1->CFGR2  &= ~ADC_CFGR2_CKMODE;				// Select HSI clock for adc
	ADC1->CHSELR |= ADC_CHSELR_CHSEL0; 				// Select channel 0 to be converted (PA0)
	ADC1->SMPR   |= ADC_SMPR_SMP_0; 					// Select sampling mode
	ADC1->SMPR	 |= ADC_SMPR_SMP_1; 					// ob111
	ADC1->SMPR   |= ADC_SMPR_SMP_2; 					// 239.d ADC clk to be greater than 17.1us

	// Calibrate the ADC
	if ((ADC1->CR & ADC_CR_ADEN) != 0) 				// Check to make sure adc is off
		ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);		// Turn adc off
	ADC1->CR |= ADC_CR_ADCAL; 								// Turn on calibration mode
	while ((ADC1->CR & ADC_CR_ADCAL) != 0);		// Wait until calibration is complete
}

//ADC Channel 0; PA0
int ADC_Channel_0(void){
	int ADC_Value = 0;												// Store adc value
	uint8_t i;																// Loop counter
	
	ADC1->CR |= ADC_CR_ADEN;									// Turn on the ADC
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0); // Wait until ADC is ready
	for (i=0; i < 4; i++){										// Take 4 samples
		ADC1->CR |= ADC_CR_ADSTART;							// Start ADC conversion
		while (!(ADC1->ISR & ADC_ISR_EOC));			// Wait for end of conversion
		ADC_Value += ADC1->DR;									// Save ADC samples
	}
	
	return ((ADC_Value*3300)/0xFFF) >> 2;			// Multiply by 1000 to make millivolt, then shift operator to divide by 4
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************** END FUNCTIONS **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************* END ADC.c **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
