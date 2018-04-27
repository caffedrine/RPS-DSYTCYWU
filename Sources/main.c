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
#include "DebugConsole.h"
#include "IO1.h"
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
///////////////////////////////////////////////
/// Global variables
///////////////////////////////////////////////
extern error_t curr_err_status;


// Global ADC values - updated from interrupt routine
uint16_t analog_truck_12v_voltage;
uint16_t analog_truck_24v_voltage;
uint16_t analog_external_pwr_source_voltage;
uint16_t analog_pwm_fb;
uint16_t analog_v_battery_micro_voltage;
uint16_t analog_battery_current_amps;		/// Battery current
uint16_t analog_NTC_mos_temp;				/// PCB Temperature
uint16_t analog_NTC_batt1_temp;
uint16_t analog_NTC_batt2_temp;

charging_method_t chargingMethod = charge_NONE;

///////////////////////////////////////////////
/// Functions
///////////////////////////////////////////////

void handle_24V_truck_charging(void);
void handle_12V_truck_charging(void);
void handle_24V_external_charging(void);

void converterSetVout(float voltage);
charging_method_t detectPowerSupply();

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	converterSetVout(0);		// !!! Make sure converter is not functional when switching power relay

	// Wait a a second for ADC channels to read all the voltages and stuff
	delayMs(1000);

	#ifdef DEBUG
		// If debug is enabled, detect the power supply and then print a pretty message!
		chargingMethod = detectPowerSupply();
		printf("Power source detection: ");

		switch(chargingMethod)
		{
			case charge_TRUCK_12V:
			{
				printf("12VDC from TRUCK\r\n");
			}break;

			case charge_TRUCK_24V:
			{
				printf("24VDC from TRUCK\r\n");
			}break;

			case charge_EXTERNAL:
			{
				printf("ETERNAL POWER SUPPLY\r\n");
			}break;

			case charge_NONE:
			{
				printf("NONE DETECTED\r\n");
			}break;

			case charge_ERR_MULTIPLE:
			{
				printf("!!!WARNING: Multiple power sources detected!");
			}break;

			default:
			{
				printf("ERR_EXCEPTION - This is bad...");
			}break;
		}
	#endif

	for (;;)
	{
		// Detecting the power source and it's kind
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

charging_method_t detectPowerSupply()
{
	// this function will return first one which is found to be valid

	charging_method_t detected = charge_NONE;	// Variable used to store detected method
	float voltage;								// Used to store voltage received from power sources

	// Check if power supply is 12V truck
	voltage = analog2Volts( analog_truck_12v_voltage );	// the ADC measurement is converted to a more human readable format
	if( voltage >= TRUCK_12V_MIN_VOLTAGE_ALLOWED && voltage <= TRUCK_12V_MAX_VOLTAGE_ALLOWED )
	{
		detected = charge_TRUCK_12V;
	}

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

void converterSetVout(float voltage)
{
	//TODO: Implement
}



/* END main */
/*!
 ** @}
 */



