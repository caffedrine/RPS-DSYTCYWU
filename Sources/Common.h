/*
 * Common.h
 *
 *  Created on: Apr 26, 2018
 *      Author: curiosul
 *
 *  Common global defines and datatypes required by most of the modules.
 *  This is also the configuration file when all params can be changed.
 */

#ifndef SOURCES_COMMON_H_
#define SOURCES_COMMON_H_

#include "gpio.h"

/// Firmware version
#define	major	0x01
#define minor	0x01
#define release	0x00
#define	build	0x00

//	   ____ ___  _   _ _____ ___ ____ ____
//	  / ___/ _ \| \ | |  ___|_ _/ ___/ ___|
//	 | |  | | | |  \| | |_   | | |  _\___ \
//	 | |__| |_| | |\  |  _|  | | |_| |___) |
//	  \____\___/|_| \_|_|   |___\____|____/
//
/// Toggle debugging mode - by default UART0 (PTB0-PTB1) is used
#define DEBUG			///

#define BATTERY_TYPE	12		// 24

////////////////// Battery configs //////////////////
// 12V AGM Battery
#define BATTERY_12V_ABSORPTION_CHARGING_VOLTAGE		(14.70f)
#define BATTERY_12V_MIN_BULK_CHARGING_VOLTAGE		(12.80f)
#define BATTERY_12V_MAX_BULK_CHARGING_VOLTAGE		(14.70f)
#define BATTERY_12V_FLOATING_VOLTAGE				(13.50f)

// 24V AGM Battery
#define BATTERY_24V_ABSORPTION_CHARGING_VOLTAGE		(29.40f)
#define BATTERY_24V_MIN_BULK_CHARGING_VOLTAGE		(26.00f)
#define	BATTERY_24V_MAX_BULK_CHARGING_VOLTAGE		(29.40f)
#define BATTERY_24V_FLOATING_VOLTAGE				(27.00f)


////////////////// Power Supply //////////////////
/////////////////Check LM5175RHFR Manual//////////

// Truck 12V supply configuration
#define	TRUCK_12V_MIN_VOLTAGE_ALLOWED		(12.50f)
#define TRUCK_12V_MAX_VOLTAGE_ALLOWED		(15.50f)

// Truck 24V supply configuration
#define	TRUCK_24V_MIN_VOLTAGE_ALLOWED		(21.00f)
#define TRUCK_24V_MAX_VOLTAGE_ALLOWED		(29.50f)

// External power supply configuration
#define	EXTERNAL_MIN_VOLTAGE_ALLOWED		(21.00f)
#define	EXTERNAL_MAX_VOLTAGE_ALLOWED		(29.50f)

////////////////// High level ports map //////////////////
// ADC Channels used
#define ADC_TRUCK_12V_CHANNEL			1
#define ADC_TRUCK_24V_CHANNEL			2
#define ADC_EXTERNAL_PWR_SOURCE_CHANNEL	6
#define ADC_PWM_FB_CHANNEL				12
#define ADC_V_BATTERY_MICRO_CHANNEL		13
#define ADC_BATTERY_CURRENT_CHANNEL		14
#define ADC_NTC_MOS_CHANNEL				15
#define ADC_NTC_BATT_1_CHANNEL			8
#define ADC_NTC_BATT_2_CHANNEL			9

// Leds for signaling
#define LED_RED							PTE0
#define LED_YELLOW						PTG0
#define LED_GREEN						PTE1

// LM5175RHFR connections

#define LM5175_EN_UVLO					PTB4
#define LM5175_SS						PTE6
#define LM5175_DITH						PTE5
#define LM5175_DCM						PTB5
#define LM5175_DCMH						PTC3
#define LM5175_CCM						PTC2
#define LM5175_VOUT_OK					PTG2

// Bluetooth module
#define BT_RESET						PTD5
#define BT_BOOT							PTD4
#define BT_RTS							PTD3
#define BT_TX							PTD6
#define BT_POWER						PTD0

// RS-485 configs
#define RS485_TX						PTC7
#define RS485_RX						PTC6
#define RS485_EN						PTF1
#define	RS485

// Others
#define POWER_AUX						PTH7
#define TRIKKLE_OK						PTD1
#define POWER_14V						PTH0

//
//	  ____        _           _
//	 |  _ \  __ _| |_ __ _   | |_ _   _ _ __   ___  ___
//	 | | | |/ _` | __/ _` |  | __| | | | '_ \ / _ \/ __|
//	 | |_| | (_| | || (_| |  | |_| |_| | |_) |  __/\__ \
//	 |____/ \__,_|\__\__,_|   \__|\__, | .__/ \___||___/
//								  |___/|_|

typedef enum battery_type
{
	battery_12V,
	battery_24V,
	battery_none,
	battery_error
}battery_type_t;

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
	errOK_Bulk,
	errOK_Absorption,
	errOK_Floating,
	err_ShortCircuit,
	err_HotTemp,
	err_BrokenConnection,
	err_DamagedBattery,
}charger_status_t;

#endif /* SOURCES_COMMON_H_ */
