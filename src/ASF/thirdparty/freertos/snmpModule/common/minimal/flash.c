/*
    FreeRTOS V7.0.0 - Copyright (C) 2011 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/**
 * This version of flash .c is for use on systems that have limited stack space
 * and no display facilities.  The complete version can be found in the
 * Demo/Common/Full directory.
 *
 * Three tasks are created, each of which flash an LED at a different rate.  The first
 * LED flashes every 200ms, the second every 400ms, the third every 600ms.
 *
 * The LED flash tasks provide instant visual feedback.  They show that the scheduler
 * is still operational.
 *
 */


#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo program include files. */
#include "partest.h"
#include "flash.h"
#include "flashc.h"
#include "usart.h"
#define ledSTACK_SIZE		configMINIMAL_STACK_SIZE
#define ledNUMBER_OF_LEDS	( 3 )
#define ledFLASH_RATE_BASE	( ( portTickType ) 333 )

/* Variable used by the created tasks to calculate the LED number to use, and
the rate at which they should flash the LED. */
static volatile unsigned portBASE_TYPE uxFlashTaskNumber = 0;

/* The task that is created three times. */
static portTASK_FUNCTION_PROTO( vLEDFlashTask, pvParameters );

/*-----------------------------------------------------------*/

void vStartLEDFlashTasks( unsigned portBASE_TYPE uxPriority )
{
signed portBASE_TYPE xLEDTask;

	/* Create the three tasks. */
	//for( xLEDTask = 0; xLEDTask < ledNUMBER_OF_LEDS; ++xLEDTask )
	//{
		/* Spawn the task. */
		//xTaskCreate( vLEDFlashTask, ( signed char * ) "LEDx", ledSTACK_SIZE, NULL, uxPriority, ( xTaskHandle * ) NULL );
		xTaskCreate( vLEDFlashTask, ( signed char * ) "LEDx", ledSTACK_SIZE, NULL, uxPriority, ( xTaskHandle * ) NULL );
	//}
}
/*-----------------------------------------------------------*/
//#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )
static portTASK_FUNCTION( vLEDFlashTask, pvParameters )
{
portTickType xFlashRate, xLastFlashTime;
unsigned portBASE_TYPE uxLED;

	/* The parameters are not used. */
	( void ) pvParameters;

	/* Calculate the LED and flash rate. */
	portENTER_CRITICAL();
	{
		/* See which of the eight LED's we should use. */
		uxLED = uxFlashTaskNumber;

		/* Update so the next task uses the next LED. */
		uxFlashTaskNumber++;
	}
	portEXIT_CRITICAL();
    //         333 + (333*0) = 333
	xFlashRate = ledFLASH_RATE_BASE + ( ledFLASH_RATE_BASE * ( portTickType ) uxLED );
	// 333 / 1 = 333
	xFlashRate /= portTICK_RATE_MS;

	/* We will turn the LED on and off again in the delay period, so each
	delay is only half the total period. */
	/// 333/2 = 166
	xFlashRate /= ( portTickType ) 2;

	/* We need to initialise xLastFlashTime prior to the first call to
	vTaskDelayUntil(). */
	xLastFlashTime = xTaskGetTickCount();

	for(;;)
	{
		/* Delay for half the flash period then turn the LED on. */
		vTaskDelayUntil( &xLastFlashTime, 3000 );
		//usart_write_line((&AVR32_USART0), "\\Flash..");
	}
} /*lint !e715 !e818 !e830 Function definition must be standard for task creation. */

