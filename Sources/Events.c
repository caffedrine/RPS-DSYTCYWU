/* ###################################################################
**     Filename    : Events.c
**     Project     : battery-charger
**     Processor   : SKEAZN64MLH2
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-04-26, 17:12, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include "Common.h"
#include "Errors.h"

// Defined in main
error_status_t curr_err_status;

/// ADC Values - Values are not changed on main() so let's make them volatile
extern uint16_t analog_truck_12v_voltage;
extern uint16_t analog_truck_24v_voltage;
extern uint16_t analog_external_pwr_source_voltage;
extern uint16_t analog_pwm_fb;
extern uint16_t	analog_v_battery_micro_voltage;
extern uint16_t analog_battery_current_amps;	/// Battery current
extern uint16_t analog_NTC_mos_temp;			/// PCB Temperature
extern uint16_t analog_NTC_batt1_temp;
extern uint16_t analog_NTC_batt2_temp;

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKE02Z64QH2]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  ADC_OnEnd (module Events)
**
**     Component   :  ADC [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void ADC_OnEnd(void)
{
	/// The values are handled on main loop

	if( ADC_GetChanValue16(ADC_TRUCK_12V_CHANNEL, &analog_truck_12v_voltage) == ERR_OK);
	if( ADC_GetChanValue16(ADC_TRUCK_24V_CHANNEL, &analog_truck_24v_voltage) == ERR_OK);
	if( ADC_GetChanValue16(ADC_EXTERNAL_PWR_SOURCE_CHANNEL, &analog_external_pwr_source_voltage) == ERR_OK);
	if( ADC_GetChanValue16(ADC_PWM_FB_CHANNEL, &analog_pwm_fb) == ERR_OK);
	if( ADC_GetChanValue16(ADC_V_BATTERY_MICRO_CHANNEL, &analog_v_battery_micro_voltage) == ERR_OK);
	if( ADC_GetChanValue16(ADC_BATTERY_CURRENT_CHANNEL, &analog_battery_current_amps) == ERR_OK);
	if( ADC_GetChanValue16(ADC_NTC_MOS_CHANNEL, &analog_NTC_mos_temp) == ERR_OK);
	if( ADC_GetChanValue16(ADC_NTC_BATT_1_CHANNEL, &analog_NTC_batt1_temp) == ERR_OK);
	if( ADC_GetChanValue16(ADC_NTC_BATT_2_CHANNEL, &analog_NTC_batt2_temp) == ERR_OK);
}

/*
** ===================================================================
**     Event       :  ErrorsSignaling_OnInterrupt (module Events)
**
**     Component   :  ErrorsSignaling [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ErrorsSignaling_OnInterrupt(void)
{

}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
