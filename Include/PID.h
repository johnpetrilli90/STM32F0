////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************ BEGIN PID.h *********************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		PID.h
// Description: Header file and function definition for PID.c					
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
extern void InitPWM(void);						//Init the PWM on PA8
extern void Set_PWM_Duty(int);				//Set the Duty cycle on PWM
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************** END FUNCTION DEFINITION ***********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************** END PID.h ********************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
