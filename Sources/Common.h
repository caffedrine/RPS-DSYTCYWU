/*
 * Common.h
 *
 *  Created on: Apr 26, 2018
 *      Author: curiosul
 */

#ifndef SOURCES_COMMON_H_
#define SOURCES_COMMON_H_

/// Firmware version
#define	major	0x01
#define minor	0x01
#define release	0x00
#define	build	0x00

/// Toggle debugging mode
#define DEBUG			/// By default UART0 (PTB0-PTB1) is used

#define BATTERY_TYPE	12V		// 24V

// 12V AGM Battery
#define BATTERY_12V_MIN_CHARGING_VOLTAGE	(14.10f)
#define BATTERY_12V_MAX_CHARGING_VOLTAGE	(14.70f)
#define BATTERY_12V_FLOATING_VOLTAGE		(13.50f)

// 24V AGM Battery
#define BATTERY_24V_MIN_CHARGING_VOLTAGE	(28.20f)
#define	BATTERY_24V_MAX_CHARGING_VOLTAGE	(29.40f)
#define BTTERY_24V_FLOATING_VOLTAGE			(27.00f)

// Truck 12V supply configuration
#define	TRUCK_12V_MIN_VOLTAGE_ALLOWED		(10.50f)
#define TRUCK_12V_MAX_VOLTAGE_ALLOWED		(15.50f)

// Truck 24V supply configuration
#define	TRUCK_24V_MIN_VOLTAGE_ALLOWED		(21.00f)
#define TRUCK_24V_MAX_VOLTAGE_ALLOWED		(29.50f)

// External power supply configuration
#define	EXTERNAL_MIN_VOLTAGE_ALLOWED		(21.00f)
#define	EXTERNAL_MAX_VOLTAGE_ALLOWED		(29.50f)

// ADC Channels used
#define ADC_TRUCK_12V_CHANNEL		1
#define ADC_TRUCK_24V_CHANNEL		2
#define ADC_EXTERNAL_PWR_SOURCE_CHANNEL	6
#define ADC_PWM_FB_CHANNEL			12
#define ADC_V_BATTERY_MICRO_CHANNEL	13
#define ADC_BATTERY_CURRENT_CHANNEL	14
#define ADC_NTC_MOS_CHANNEL			15
#define ADC_NTC_BATT_1_CHANNEL		8
#define ADC_NTC_BATT_2_CHANNEL		9

// Leds for signaling
#define LED_RED		PTE0
#define LED_YELLOW	PTG0
#define LED_GREEN	PTE1

typedef enum charging_method
{
	charge_TRUCK_12V,
	charge_TRUCK_24V,
	charge_EXTERNAL,
	charge_NONE,
	charge_ERR_MULTIPLE				// In case multiple power sources are detected
}charging_method_t;

typedef enum error
{
	err_Floating,
	err_FastCharge,
	err_SlowCharge,
	err_ShortCircuit,
	err_HotTemp,
	err_BrokenConnection,
	err_DamagedBattery,

}error_status_t;

#endif /* SOURCES_COMMON_H_ */
