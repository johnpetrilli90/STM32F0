////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************ BEGIN Flash.h *******************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		Flash.h
// Description: Header file and function definition for Flash.c					
// Author:			John Petrilli & Jack Haller
// Date:				22.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** DEFINE FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void WriteToFlash(uint16_t,int);					//Used to flash new temperature set point
extern unsigned int ReadFromFlash(int);					//Used to initalize temperature set point
extern void EnableFlashSecurity(void);					//Implement Level 1 protection on Flash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************** END FUNCTION DEFINITION ***********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************** END Flash.h ******************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////