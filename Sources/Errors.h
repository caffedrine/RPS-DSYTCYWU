/*
 * Errors.h
 *
 *  Created on: Apr 27, 2018
 *      Author: curiosul
 *
 *  Description: This file contains functions used to handle all error
 *  signaling interfaces. In this case, LEDS are used and probably debug
 *  console.
 */

#ifndef SOURCES_ERRORS_H_
#define SOURCES_ERRORS_H_

#include "Common.h"

// This function is executed from timer every 800ms
void handle_error(error_status_t err_type)
{
	switch(err_type)
	{
		case err_Floating:
		{
			handle_err_Floating();
		}break;

		case err_FastCharge:
		{
			handle_err_FastCharge();
		}break;

		case err_SlowCharge:
		{
			handle_err_SlowCharge();
		}break;

		case err_ShortCircuit:
		{
			handle_err_ShortCircuit();
		}break;

		case err_HotTemp:
		{
			handle_err_HotTemp();
		}break;

		case err_BrokenConnection:
		{
			handle_err_BrokenConnection();
		}break;

		case err_DamagedBattery:
		{
			handle_err_DamagedBattery();
		}break;

		default:
		{
			handle_err_Nightmare();
		}break;
	}
}

void handle_err_Floating()
{

}

void handle_err_FastCharge()
{

}

void handle_err_SlowCharge()
{

}

void handle_err_ShortCircuit()
{

}

void handle_err_HotTemp()
{

}

void handle_err_BrokenConnection()
{

}

void handle_err_damagedBattery()
{

}

void handle_err_Nightmare()
{

}


#endif /* SOURCES_ERRORS_H_ */
