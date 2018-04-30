/*
 * Util.h
 *
 *  Created on: Apr 26, 2018
 *      Author: curiosul
 *
 *  Collection of some util functions used all around the project!
 *  Is can be easily reused as there are no dependencies.
 */

#ifndef SOURCES_UTIL_H_
#define SOURCES_UTIL_H_

#include <stdint.h>

/// Insert a delay in ms - this is not accurate and value 3390 was obtained empirically
void delayMs(int ms)
{
	for(int i = 0; i < ms; i++)
	{
		//1ms delay
		for(int j=0; j < 3390; j++)
			__asm("nop");
	}
}

/// Map a given value from an interval to another (integers)
long mapint(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// Map a given value from an interval to another (floats)
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// Function used to convert analog value to a voltage. Unit is VOLT
float analog2Volts(uint16_t val)
{
	//TODO: Implement
	float convertedVal = 13.37;
	return convertedVal;
}

/// Convert analog measurement in degrees (Cellsius)
float analog2Cellsius(uint16_t val)
{
	//TODO: Implement
	float convertedVal = 13.37;
	return convertedVal;
}

#endif /* SOURCES_UTIL_H_ */
