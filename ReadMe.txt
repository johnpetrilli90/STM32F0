TEC Controlled CPU Cooler
- John Petrilli
- 22.Apr.2016
					  
Firmware project is intended to be used in conjunction with our CPU cooler project
----------------------------------------------------------------------------------------
Version 0x001
	- This code is being developed on the STM32F030 Nucleo board from STmicro
	- Salvaged as much code as possible from attiny85
	- Communication task: Blinks led at 1Hz
	- Temperature task: Gets ADC reading, calculates thermistor resistance, runs steinhart-hart eq
	- PID task: used same logic from attiny, just converted PID to 32bit with a scaling conversion
	- UART task: Two commands "connect" and "settemp" one returns ok if serial connection is 
				 established. The other allows the user to change temp set point
Version 0x002
	- Code still be developed on nucleo kit. 
	- UART task: Added 2 commands "temperature" & "tempsetpoint". One returns the current temp
				 the other returns the temp set point for TEC
	- Flash.c: Added the capability to flash any page. You now have to pass address you want
			   to flash to fxn 'WriteToFlash()'
	- BlueTooth task: Added this task to initialize the HC05 with a custom name and password.
					  Logic seems correct, this is untested as of now.
Version 0x003
	- Edited the IRQn.c file for initializing the bluetooth task. Added bt flag and code to 
	  process Rx irqs. 
	- Cleaned up code a little

Version 0x004
	- Transferred code from nucleo development kit over to custom PCB with STM32F030K6T6
	- The RTOS stack size in 'RTX_Conf_CM.c' was too large for the stack on this uC
	  reduced from 200 to 64. 
	- Removed round robin task switching
	- This uC couldn't handle doing natural log calculations, so a lookup table was created
	  for the thermistor values and interpolation is used to calculate the temperature
	- ADC algorithm for back calculating was changed to take it easy on the processor and
	  changed from actual values to milli-values (was float now int) to relieve the processor
	- Removed temperature task. Added temperature calcuation to PID task.
	- PID task: Removed old algorithm and attempeted to implement one similar to the NML.
	            still needs work and needs to be tuned.
	- Implemented the independent watchdog
	- Lock GPIO registers after intialization
	- Initialize unused GPIO registers to floating input in 'InitIO.c'
	- Added read protection to flash for security
	- Changed everything from actual values to milli-values. 
	
Version 0x005
	- Renamed target to "CPU_Cooler_v005" just ot give visual on screen what version of firmware is
	  open.
	- PID algorithm was tested with a temperatuer sensor andm manually changing the temperature of the sensor, PWM response
	  was observed to react correctly on scope. Just waiting for adhesive to arrive so further testing can be done on a completed unit.
	  
Version 0x006
	- Modified hardware to give us the option to turn on/off BT module (PB7)
	- Added fxns 'SetATmode' and 'SetCommMode' for the BT initialization
	- Added fxn 'delay' for BT init
	- Added fxn 'SetBaud9600' and 'SetBaud38400' for BT init
	- Remove Bluetooth task and turned it into an init fxn. This made a few things easier, like locking the GPIO registers
	- Remove ability to go to BT task from IRQ
		
----------------------------------------------------------------------------------------
To Do:
- Get BT Working

----------------------------------------------------------------------------------------
In Progress:

----------------------------------------------------------------------------------------
Complete:
- Lock flash/set security bit
- Independent WatchDog configured
- Lock registers after config
- UART functioning properly 
- PID tune
----------------------------------------------------------------------------------------
