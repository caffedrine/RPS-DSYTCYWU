/* ###################################################################
 **     Filename    : main.c
 **     Project     : battery-charger
 **     Processor   : SKEAZN64MLH2
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2018-04-26, 17:12, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "ADC.h"
#include "AdcLdd1.h"
#ifdef DEBUG
	#include "DebugConsole.h"
	#include "IO1.h"
#endif
#include "ErrorsSignaling.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "LedYellow.h"
#include "BitIoLdd1.h"
#include "LedGreen.h"
#include "BitIoLdd2.h"
#include "LedRed.h"
#include "BitIoLdd3.h"

/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Common.h"
#include "Util.h"
#include "gpio.h"
#ifdef DEBUG
	#include "Debug.h"
#endif

//	   ____ _       _           _
//	  / ___| | ___ | |__   __ _| |___
//	 | |  _| |/ _ \| '_ \ / _` | / __|
//	 | |_| | | (_) | |_) | (_| | \__ \
//	  \____|_|\___/|_.__/ \__,_|_|___/
//
charger_status_t curr_status;							/// Store current status of the charger

// Global ADC values - updated from interrupt routine
uint16_t analog_truck_12v_voltage;						/// Store current read voltage of truck power supply from 12V pin
uint16_t analog_truck_24v_voltage;						/// Store current read voltage of truck power supply from 24V pin
uint16_t analog_external_pwr_source_voltage;			/// External power source voltage read
uint16_t analog_pwm_fb;
uint16_t analog_v_battery_micro_voltage;				/// Battery voltage read on uC
uint16_t analog_battery_current_amps;					/// Battery current read
uint16_t analog_NTC_mos_temp;							/// PCB Temperature read
uint16_t analog_NTC_batt1_temp;							/// Battery temperature 1
uint16_t analog_NTC_batt2_temp;							/// Battery temperature 2

charging_method_t chargingMethod = charge_NONE;			/// Store current selected/detected charging method

//	 ____            _                 _   _
//	|  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___
//	| | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
//	| |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
//	|____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
//
void handle_24V_truck_charging(void);
void handle_12V_truck_charging(void);
void handle_24V_external_charging(void);

void lm5175SetVout(float voltage);
void lm5175SetIout(float current);
void lm5175SetEnable(bool enable);

charging_method_t detectPowerSupply();

//	 ____        __ _       _ _   _
//	|  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  ___
//	| | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \/ __|
//	| |_| |  __/  _| | | | | | |_| | (_) | | | \__ \
//	|____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/
//
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	lm5175SetVout(0);		// !!! Make sure converter is not functional when switching power relay
	lm5175SetIout(0);		// !!! Set output current to 0

	for (;;)
	{
#ifdef DEBUG
		// Check whether power supply was changed every time
		if( detectPowerSupply() != chargingMethod )	// if new detected power supply != than the previously detected
		{
			printf("POWER SUPPLY CHANGE DETECTED!\r\n");
		}
#endif

		// Detect the power source and it's kind and store it
		chargingMethod = detectPowerSupply();

		switch(chargingMethod)
		{
			case charge_TRUCK_12V:
			{
				handle_12V_truck_charging();
			}break;

			case charge_TRUCK_24V:
			{
				handle_24V_truck_charging();
			}break;

			case charge_EXTERNAL:
			{
				handle_24V_external_charging();
			}break;

			case charge_NONE:
			{
				// Take a breath and try again
				delayMs(200);	// Wait for ADC port to be filled up
			}break;

			case charge_ERR_MULTIPLE:
			{
				// Multiple power sources detected at the same time
				printPowerSource(chargingMethod);	// print error
			}break;
		}
	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/


//	  ____                    __                  _   _
//	 / ___|___  _ __ ___     / _|_   _ _ __   ___| |_(_) ___  _ __  ___
//	| |   / _ \| '__/ _ \   | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//	| |__| (_) | | |  __/   |  _| |_| | | | | (__| |_| | (_) | | | \__ \
//	 \____\___/|_|  \___|   |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//
void handle_12V_truck_charging(void)
{
	//TODO: Implement
}

void handle_24V_truck_charging(void)
{
	//TODO: Implement
}

void handle_24V_external_charging(void)
{
	//TODO: Implement
}

//	 _____                 _   _
//	|  ___|   _ _ __   ___| |_(_) ___  _ __  ___
//	| |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//	|  _|| |_| | | | | (__| |_| | (_) | | | \__ \
//	|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//
charging_method_t detectPowerSupply()
{
	// this function will return first one which is found to be valid

	charging_method_t detected = charge_NONE;	// Variable used to store detected method
	float voltage;								// Used to store voltage received from power sources

#if(BATTERY_TYPE==12)
	// Check if power supply is 12V truck
	voltage = analog2Volts( analog_truck_12v_voltage );	// the ADC measurement is converted to a more human readable format
	if( voltage >= TRUCK_12V_MIN_VOLTAGE_ALLOWED && voltage <= TRUCK_12V_MAX_VOLTAGE_ALLOWED )
	{
		if(detected != charge_NONE)
		{
			detected = charge_ERR_MULTIPLE;
			return detected;
		}
		else
		{
			detected = charge_TRUCK_12V;
		}
	}
#elif(BATTERY_TYPE==24)
	// Check if power supply is 24V truck
	voltage = analog2Volts( analog_truck_24v_voltage );
	if( voltage >= TRUCK_24V_MIN_VOLTAGE_ALLOWED && voltage <= TRUCK_24V_MAX_VOLTAGE_ALLOWED)
	{
		// Check whether there was detected another source previously
		// In this case, two power sources are detected and this is bad
		if(detected != charge_NONE)
		{
			detected = charge_ERR_MULTIPLE;
			return detected;
		}
		else
		{
			detected = charge_TRUCK_24V;
		}
	}
#endif
	// Finally check if the power supply is external
	voltage = analog2Volts( analog_external_pwr_source_voltage );
	if(voltage >= EXTERNAL_MIN_VOLTAGE_ALLOWED && voltage <= EXTERNAL_MAX_VOLTAGE_ALLOWED)
	{
		// Check whether there was detected another source previously
		// In this case, two power sources are detected and this is bad
		if(detected != charge_NONE)
		{
			detected = charge_ERR_MULTIPLE;
			return detected;
		}
		else
		{
			detected = charge_EXTERNAL;
		}
	}

	return detected;
}

void lm5175SetVout(float voltage)
{
	//TODO: Implement
}

void lm5175SetIout(float current)
{
	///TODO: Implement
}

/// Low level eneble/disable LM5175 driver
void lm5175SetEnable(bool enable)
{
	/// This is a critical pin! Must be sure at any moment (even before initialization) that
	/// it can be securely used!

	// Set as output
	GPIOB_BASE_PTR->PDDR |= (1 << LM5175_EN_UVLO);

	// Write logic value to it's output
	if(enable == 1)
		PTB->PSOR |= 1 << LM5175_EN_UVLO;
	else
		PTB->PCOR |= 1 << LM5175_EN_UVLO;
}


/* END main */
/*!
 ** @}
 */



