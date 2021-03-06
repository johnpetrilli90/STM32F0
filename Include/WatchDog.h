////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************** BEGIN WatchDog.h **********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: 		    WatchDog.h
// Description: Header file and function definition for WatchDog.c					
// Author:			John Petrilli & Jack Haller
// Date:				24.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** DEFINE FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void InitWatchDog(void);					// Fxn to intialze the WD peripheral
extern void KickWatchDog(void);					// Fxn to reset WD timer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************** END FUNCTION DEFINITION ***********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************* END WatchDog.h *********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
