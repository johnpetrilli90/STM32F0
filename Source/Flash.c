////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************** BEGIN Flash.c ****************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		Flash.c
// Description: Used to save and initialize temperature set point
// Author:			John Petrilli & Jack Haller
// Date:				22.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>
#include "Flash.h"													//Flash Files

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** INSERT FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Write variables to non-volatile memory
void WriteToFlash(uint16_t Value, int Flash_Page){
	//Unlock Flash
	while ((FLASH->SR & FLASH_SR_BSY) != 0);	//Wait until flash not busy
	if ((FLASH->CR & FLASH_CR_LOCK) != 0){		//If flash is locked, do unlk seq.
		FLASH->KEYR = FLASH_FKEY1; 							//Unlock code 1
		FLASH->KEYR = FLASH_FKEY2;							//Unlock code 2
	}
	
	//Clear Flags
	FLASH->SR |= FLASH_SR_EOP;								//Clear end of operation flag
	FLASH->SR |= FLASH_SR_WRPRTERR;						//Clear write protect error flag
	FLASH->SR |= FLASH_SR_PGERR;							//Clear programming error
		
	//Erase Page before writing
	FLASH->CR |= FLASH_CR_PER; 								//Enable flash page erase
	FLASH->AR = Flash_Page; 									//Set page to erase
	FLASH->CR |= FLASH_CR_STRT;								//Start erase
	while ((FLASH->SR & FLASH_SR_BSY) != 0);	//Wait until flash no busy
	if ((FLASH->SR & FLASH_SR_EOP) != 0){			//If flash finished operation
		FLASH->SR |= FLASH_SR_EOP;							//Clear flag
	}
	FLASH->CR &= ~FLASH_CR_PER; 							//Disable page erase
	
	//Write to Page
	FLASH->CR |= FLASH_CR_PG;									//Write 1 to PG (programming bit)
	//*pPage = Temperature;										//Write to flash page 
	 *(__IO uint16_t*)(Flash_Page) = Value; 	//Change temp set point in flash
	while ((FLASH->SR & FLASH_SR_BSY) != 0);	//Wait until bus is not busy
	if ((FLASH->SR & FLASH_SR_EOP) != 0){			//Check if flash is completed
		FLASH->SR |= FLASH_SR_EOP;							//Clear flag is flash is complete
	}
	FLASH->CR &= ~FLASH_CR_PG;								//Clear programming bit to disable write to flash
	FLASH->CR |= FLASH_CR_LOCK;								//Lock the flash
}

//Read values non-volatile values from flash
unsigned int ReadFromFlash(int Flash_Page){
	return *((uint16_t *) Flash_Page); 				//Read temp from flash and return
}

//Enable read protection on flash (Level 1)
//Changed to level 2 if OB->RDP = 0xCC. If you change to level 2
//you can't go back. Chip is protected, no dbg & no flashing
//If you go back to level 0 (OB->RDP = 0xCC) then the flash 
//memory is completely wiped
void EnableFlashSecurity(void){
	//Enable flash clock
	RCC->AHBENR |= RCC_AHBENR_FLITFEN;
	
	//Unlock option bytes block access
  FLASH->OPTKEYR = FLASH_OPTKEY1;						//Unlock option bytes for modification
  FLASH->OPTKEYR = FLASH_OPTKEY2;						//Send keys to unlock option bytes
	
	//Apply read protection level 1
	FLASH->CR |= FLASH_CR_OPTER;							//Erase value in option byte
	FLASH->CR |= FLASH_CR_STRT;								//Begin erase
	while ((FLASH->SR & FLASH_SR_BSY));				//Wait until bus is not busy	
	FLASH->CR &= ~FLASH_CR_OPTER;							//Disable option byte erasing
	FLASH->CR |= FLASH_CR_OPTPG;							//Enable option byte programming
	OB->RDP = 0x99;														//Program option byte read protection
	while ((FLASH->SR & FLASH_SR_BSY));				//Wait until bus is not busy	
	FLASH->CR &= ~FLASH_CR_OPTPG;							//Disable option byte programming
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************** END FUNCTIONS **************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
