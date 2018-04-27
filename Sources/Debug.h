/*
 * Debug.h
 *
 *  Created on: Apr 27, 2018
 *      Author: curiosul
 */

#ifndef SOURCES_DEBUG_H_
#define SOURCES_DEBUG_H_

#include <stdio.h>
#include "Common.h"

void printPowerSource(charging_method_t chargingMethod)
{
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
}


#endif /* SOURCES_DEBUG_H_ */
