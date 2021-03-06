////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************** BEGIN WatchDog.c ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		WatchDog.c
// Description: WD timer is used to detect malfuction in software and cause a system reset
// Author:			John Petrilli & Jack Haller
// Date:				24.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>
#include "WatchDog.h"										// WD Files

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** INSERT FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Configure the WD Timer
void InitWatchDog(void){
	RCC->APB2ENR |= RCC_APB2ENR_DBGMCUEN;						//Enable dbg clock
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP; //Stop the counter in debug mode
	
	RCC->CSR |= RCC_CSR_LSIRDY;						// Turn on low speed oscillator
	IWDG->KR  = 0xCCCC;										// Start the timer
	IWDG->KR  = 0x5555;										// Enable access to write registers 
	IWDG->PR  = 0x02;											// Prescaler divider to 256
	IWDG->RLR = 0xFFF;										// Set down counter. Watchdog needs to be kicked every 6.4ms
}

// Resets the watchdog timer. Place this in the idle loop
void KickWatchDog(void){	
	IWDG->KR = 0xAAAA;										// Reset Reload register
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************** END FUNCTIONS **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************* END WatchDog.c *********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
