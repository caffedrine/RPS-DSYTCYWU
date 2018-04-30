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

//  _                    _     __                  _   _
// | |    ___   ___ __ _| |   / _|_   _ _ __   ___| |_(_) ___  _ __  ___
// | |   / _ \ / __/ _` | |  | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |__| (_) | (_| (_| | |  |  _| |_| | | | | (__| |_| | (_) | | | \__ \
// |_____\___/ \___\__,_|_|  |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//

void handle_errOK_Floating()
{
	// In floating mode, green lead must be turned on all the time
	LedGreen_SetVal();
}

void handle_errOK_Absorption()
{
	// Means battery is charging in PHASE1 - so the green led must blink
	LedGreen_NegVal();
}

void handle_errOK_Bulk()
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


//	  ____ _       _           _     __                  _   _
//	 / ___| | ___ | |__   __ _| |   / _|_   _ _ __   ___| |_(_) ___  _ __  ___
//	| |  _| |/ _ \| '_ \ / _` | |  | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//	| |_| | | (_) | |_) | (_| | |  |  _| |_| | | | | (__| |_| | (_) | | | \__ \
//	 \____|_|\___/|_.__/ \__,_|_|  |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

// This function is executed from timer every 800ms
void handle_error(charger_status_t err_type)
{
	switch(err_type)
	{
		case errOK_Floating:
		{
			handle_errOK_Floating();
		}break;

		case errOK_Absorption:
		{
			handle_errOK_Absorption();
		}break;

		case errOK_Bulk:
		{
			handle_errOK_Bulk();
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
