////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************** BEGIN UART.h ****************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		UART.h
// Description: Header file and function definition for UART.c
// Author:			John Petrilli & Jack Haller
// Date:				23.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stm32f0xx.h>	
#include <RTL.h>								//Real time library

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** DEFINE VARIABLES HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern char RX[50];							//Receive buffer for RS232
extern char *pRx;								//Pointer for RS232 buffer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************** END VARIABLE DEFINITION ***********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** DEFINE FUNCTIONS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void InitUART(void);			//USART Initialization fxn
extern void PrintBT(char*);			//USART1 Print Fxn
extern void SetBaud9600(void);  //Set baud to 9600 for AT Mode 1 (typical operation)
extern void SetBaud38400(void);	//Set baud to 38400 for AT Mode 2 (BT module set up)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************** END FUNCTION DEFINITION ***********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************* END UART.h ***************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////