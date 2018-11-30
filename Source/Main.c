////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************ BEGIN FILE **********************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:    		Main.c
//
// Description: The main objective is to create a CPU cooler from 
//							a TEC. We want to allow nerds the ability to change what temperature their CPU runs at in order
//							to optimize performance. To do this we need a wireless comm port, a TEC fixed on heatsink, 
//							thermistor						
//
//							MCU = STM32F030K6T6. 4KB RAM. 32KB	Flash.		
//
// Author:			John Petrilli
// Date:				28.Feb.2016
// IP:					
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************* INCLUDE HEADER FILES HERE **********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stm32f0xx.h>									// Header file for ST cortex M0 value line devices
#include "core_cm0.h"										// Header for ARM Cortex M0 Architecture
#include "system_stm32f0xx.h"						// Header file for ST startup
#include <RTL.h>												// Header file for real-time library
#include <math.h>												// Math header file for log
#include "string.h"											// For UART string fxns
#include "stdio.h"											// For print fxns such as sprintf()							
#include "stdlib.h"											// For c standard fxns like abs()
#include "ADC.h"												// Header file for ADC reading
#include "Communication.h"							// Header file for comm led
#include "UART.h"												// Header file for send data to pc over rs232
#include "InitIO.h"											// Header file for configuring IO port/pins
#include "IRQn.h"												// Header file for interrupts
#include "WatchDog.h"										// Header file for independent watchdog timer
#include "PID.h"												// Header file for PID loop task
#include "Flash.h"											// Header file to config flash mem
#include "ClockConfig.h"								// Header file to get clock started
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************* END HEADER FILES ***************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************* PREPROCESSOR DIRECTIVES HERE *******************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FW										0x006								//Define FW version
#define TEMP_ERROR_SUM_MAX		255									//Max Temp Error for PID
#define FLASH_PAGE_TEMP				0x08007C00					//Page to flash for temp set point
#define FLASH_PAGE_BT					0x08007800					//Page to flash for BT init
#define KiEnable 						  2500								//Value to enable integral for PID
#define Kp										5										//Proportional value for PID
#define Ki										0										//Proportional value for PID
#define Kd										0										//Proportional value for PID
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************** END PREPROCESSOR DIRECTIVES ******************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************** NAME OS TASKS HERE **********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OS_TID		t_Communication;												//Name Communication OS task 
OS_TID		t_PID;																	//Name Communication OS task 
OS_TID		t_UART;																	//Name UART OS task
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************** END NAME OS TASKS *********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************** GLOBAL VARIABLES HERE *******************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t		 idx;																	//Loop counter for locking GPIO registers
int 				 iTemperature = 0;										//Temperature measured from thermistor in °C
int          iTemperatureError = 0;								//Temperature error can be positive or negative
int					 iTempErrorSum = 0;										//Hold sum of temp errors
int					 iPID = 0;														//Sum of PID 
int					 iLastTemp = 25000;										//The previous temperature. Initialize at 25C
int 				 PID_Duty = 0;												//PWM Duty cycle
unsigned int P = 0;																//Proportional term for PID
unsigned int I = 0;																//Integral term for PID
unsigned int D = 0;																//Derivative term for PID
unsigned int uiTemperatureSetPt;									//Temperature set point in milli degrees
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************** END GLOBAL VARIABLES ********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************* PUT FXNS HERE **********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitBluetooth(void){
	int  BT_Init;																//Variable to keep track of BT initializtion
	char BT_ARR[20];														//Array to hold chars to send for init process
	
	BT_Init = ReadFromFlash(FLASH_PAGE_BT);			//Read if BT has already been initialized
	
	//Check if module has been initialized
	if(BT_Init < 0x0000FFFF){
		SetCommMode();														//Put in BT in standard comm mode
	}
	else{
		SetATmode();																//Put the BT module in AT mode
		USART1->CR2 |= USART_CR2_SWAP;						//Swap the Tx/Rx pins for initialization	
		// Send values to init the BT
		sprintf(BT_ARR, "AT+ROLE=0\r\n");						//Set to Slave
		PrintBT(BT_ARR);
		sprintf(BT_ARR, "AT+CMODE=1\r\n");					//Allow master to connect to any address
		PrintBT(BT_ARR);
		sprintf(BT_ARR, "AT+NAME=CPU Cooler\r\n");	//BT Name
		PrintBT(BT_ARR);														//Send name
		sprintf(BT_ARR, "AT+PSWD=1714\r\n");				//BT password
		PrintBT(BT_ARR);														//Send password
		//Put BT module in communication mode
		SetCommMode();															//Put in comm mode
		//Finish the Init Process
		USART1->CR2 &= ~USART_CR2_SWAP;							//Regular Tx/Rx pins
		BT_Init = 1;																//Set BT Init 
		WriteToFlash(BT_Init, FLASH_PAGE_BT); 			//Write to flash that BT was taken care of	
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************** END FXNS *************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************* OS TASKS HERE ************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Task 1 'tCommunication': switches the LED on and off. Visual check for bogged down uC.											
__task void tCommunication (void){
	Communication_On();					//Turn on LED for 2seconds on startup/reset
	os_dly_wait(200);						//Used this to test watchdog, kind of like it so I kept it in
	
	for(;;){
		Communication_On();				// Turn on comm led
		os_dly_wait(10);					// Wait 10ms
		Communication_Off();			// Turn off comm led
		os_dly_wait(90);					// Wait 90ms	
	}
}

//Task 2 'tPID': Keeps the TEC at a constant temp
__task void tPID (void){
	int Rt;							//Resistance from the voltage divider
	int Vout;						//Output voltage from the ADC
	
	uiTemperatureSetPt = 25000; 														//Set point to 25C
//	uiTemperatureSetPt = ReadFromFlash(FLASH_PAGE_TEMP); //Grab set point from flash
//	if(uiTemperatureSetPt < 25000 || uiTemperatureSetPt > 65000){
//		uiTemperatureSetPt = 26000; 											//If set point is not in range set to 26C
//	}
	for(;;){
		//First thing we want to do is get the temperature of the TEC block
		Vout = ADC_Channel_0();									//Value from voltage divider (V)
		Rt = (Vout*10000)/(3300 - Vout);				//Back calculate the resistance of thermistor
		iTemperature = FindTemp(Rt);						//Use lookup table and interpolation to calc. temp
		
		//Next let's check that the temp is reasonable and get the PWM duty cycle from PID loop
		if(iTemperature >= -40000 && iTemperature <= 125000){		//Make sure temp is within range 
			iTemperatureError = iTemperature - uiTemperatureSetPt;//Get temp error 
			if(abs(iTemperatureError) < KiEnable){								//Run integral if temp error is less than ki
				iTempErrorSum += iTemperatureError;									//Sum temp error
			}
			else{
				iTempErrorSum = 0;																	//Clear the summing container									
			}
			if(iTempErrorSum > 100000){														//Prevent against overflow
				iTempErrorSum = 100000;
			}
			if(iTempErrorSum < 100000){														//Prevent against underflow
				iTempErrorSum = -100000;
			}
			P = (Kp*iTemperatureError)>>10;												//Calculate porportional value
			I = (Ki*iTempErrorSum) >> 5;													//Calculate integral value
			D = (Kd*(iTemperature - iLastTemp)) >> 5;							//Calculate derivative value
			iLastTemp = iTemperature;															//Save previous temperature
			PID_Duty = (P + I + D);																//Calculate PID sum
			
			if(PID_Duty < 0){																			
				Set_PWM_Duty(0);																		//If temp is under set point don't bother
			}
			else if(PID_Duty >= 100){
				Set_PWM_Duty(100);																	//If PWM should be > 100% then set to 100%
			}
			else{
				Set_PWM_Duty(PID_Duty);															//Otherwise set to duty cycle	percentage. (ie 50 = 50%)
			}
		}
		else{
			Set_PWM_Duty(0);																			//Else ensure that TEC is turned off so damage does not occur.
		}		
		os_dly_wait(25);																				// Run this task ever 10ms
	}
}

//Task 3 'tUART': Keeps the TEC at a constant temp
__task void tUART (void){
	static char  Tx[20];																						//Transmit buffer
	static int  ibuffer;																						//buffer for recieve input

	for(;;){
		os_evt_wait_or(0x0001,0xffff);																//Wait for go signal from interrupt that string has been received
		if(!strncmp((char*)RX, "connect", 7)){
			sprintf(Tx, "Connected Serially \r\n");											//Format string to send
			PrintBT(Tx);																								//Print to terminal
		}	
		if(!strncmp((char*)RX, "status", 6)){
			sprintf(Tx, "FW: %d\r\n", FW);															//Format string to send
			PrintBT(Tx);																								//Print to terminal
			sprintf(Tx, "TempSetPt: %d\r\n", uiTemperatureSetPt);				//Format string to send
			PrintBT(Tx);																								//Print to terminal
			sprintf(Tx, "Temp: %d\r\n", iTemperature);									//Format string to send
			PrintBT(Tx);																								//Print to terminal
		}
		else if(!strncmp((char*)RX, "settemp", 7)){
			sprintf(Tx, "Enter Temp in milli-celcius: \r\n");						//Format string to send
			PrintBT(Tx);																								//Print string to terminal
			os_evt_wait_or(0x0001, 0xffff);															//Wait for event from USART1 ISR
			if(strncmp((char*)RX,"\r",1)){
				ibuffer = atoi((char*)RX);																//Convert char array into int, store to buffer
				if(ibuffer < 25000 || ibuffer > 65000){										//Make sure buffer is in range
					sprintf(Tx, "Temp must be between 25000 & 65000\r\n");	//If not in range format error message
					PrintBT(Tx);																						//Send error message
				}
				else{
					uiTemperatureSetPt = ibuffer;														//Set DAC value to user input
					WriteToFlash(ibuffer,FLASH_PAGE_TEMP);									//Write new value to flash
					sprintf(Tx, "Temperature set to: %d mC\r\n", ibuffer);	//Format string to send
					PrintBT(Tx);																						//Print over RS232
				}
			}
		}
		else if(!strncmp((char*)RX, "temp?", 5)){
			sprintf(Tx, "Processor temperature is: %d mC\r\n", iTemperature);	//Current temperature
			PrintBT(Tx);																											//Send string
		}
		else if(!strncmp((char*)RX, "tempset?", 7)){												
			sprintf(Tx, "Temperature set point is: %d mC\r\n", uiTemperatureSetPt); //Current set point
			PrintBT(Tx);																														//Send string
		}			
	}	
}

//Task 5 'init': Initialize the tasks and delete init task
__task void init (void){
	t_Communication = os_tsk_create(tCommunication, 252);		//Create task for comm led
	t_UART = os_tsk_create(tUART, 253);											//Create task for UART
	t_PID = os_tsk_create(tPID, 254);												//Create task for PID loop
  os_tsk_delete_self();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************* END OS TASKS ***********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(void){
  SystemClockConfig();              				//Configure the HSI clock to start up uC
//	InitWatchDog();														//Initialize the independent watchdog
	EnableFlashSecurity();										//Enable read protection before initializing uC
	InitCommunication();											//Initialize communication led
	InitADC();																//Initialize analog to digital converter
	InitPWM();																//Initialize initialize the PWM
	InitUART();																//Initialize UART peripherial
	InitIO();																	//Initialize remaining pins and lock config registers
	//InitBluetooth();													//Initialize the bluetooth module
	for(idx=0; idx<16; idx++){								//For loop to execute 16 times to lock out 32 IO registers
		LockGPIOA(idx);													//Lock GPIOA configuration registers
		LockGPIOB(idx);													//Lock GPIOB configuration registers
	}
  os_sys_init (init);               				//Init RTOS tasks and start the RTX kernel
	while(1);																	//Code should never reach this loop
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************* END FILE Main.c **********************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
