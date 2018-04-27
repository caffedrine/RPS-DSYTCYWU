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
#include "LedGreen.h"
#include "LedRed.h"
#include "LedYellow.h"

void handle_err_Floating()
{
	// In floating mode, green lead must be turned on all the time
	LedGreen_SetVal();
}

void handle_err_FastCharge()
{
	// Means battery is charging in PHASE1 - so the green led must blink
	LedGreen_NegVal();
}

void handle_err_SlowCharge()
{
	// Means that battery is charging in PHASE2 - so the led also must blink
	LedGreen_NegVal();
}

void handle_err_ShortCircuit()
{
	// Just turn off
}

void handle_err_HotTemp()
{
	LedGreen_NegVal();
	LedYellow_NegVal();
	LedRed_NegVal();
}

void handle_err_BrokenConnection()
{
	LedYellow_NegVal();
}

void handle_err_DamagedBattery()
{
	LedYellow_NegVal();
}

void handle_err_Nightmare()
{
	// Exception. This code shall not be reached
	LedGreen_NegVal();
	LedYellow_NegVal();
	LedRed_NegVal();
}

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

#endif /* SOURCES_ERRORS_H_ */
