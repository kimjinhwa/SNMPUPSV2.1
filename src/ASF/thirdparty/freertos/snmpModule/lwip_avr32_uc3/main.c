/*****************************************************************************
 *
 * \file
 *
 * \brief FreeRTOS and lwIP example for AVR32 UC3.
 *
 * Copyright (c) 2009-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 *****************************************************************************/


 /*! \mainpage
 * \section intro Introduction
 * This documents data structures, functions, variables, defines, enums, and
 * typedefs in the software for the lwIP basic two-in-one web server and TFTP server demo (with DHCP) example.
 *
 * The given example is an example using freeRTOS, the current lwIP stack and MACB driver.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC for AVR32 and IAR Systems compiler
 * for AVR32. Other compilers may or may not work.
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/uc3/">Atmel AVR UC3</A>.\n
 */

/* Environment include files. */
#include <stdlib.h>
#include <string.h>

//#include <avr/pgmspace.h>
//pgm_read_byte()

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Demo file headers. */
#include "partest.h"
#include "ethernet.h"
#include "netif/etharp.h"
#include "flash.h"
#include "emailTask.h"
#include "sysclk.h"
#include "power_clocks_lib.h"
#include "gpio.h"
#include "usart.h"

//#include "wolfssl/ssl.h"

#include "portmacro.h"

#include "ups_modbus.h"
#include "networkTime.h"
#include "FlashMemory.h"

#ifndef pdMS_TO_TICKS
#define pdMS_TO_TICKS( xTimeInMs ) ( ( portTickType ) ( ( ( portTickType ) ( xTimeInMs ) * ( portTickType ) configTICK_RATE_HZ ) / ( portTickType ) 1000 ) )
#endif


#  define EXAMPLE_TARGET_PBACLK_FREQ_HZ FOSC0  // PBA clock target frequency, in Hz
//#  define EXAMPLE_TARGET_PBACLK_FREQ_HZ 24000000    // PBA clock target frequency, in Hz
#  define EXAMPLE_USART                 (&AVR32_USART0)
#  define EXAMPLE_USART_RX_PIN          AVR32_USART0_RXD_0_0_PIN
#  define EXAMPLE_USART_RX_FUNCTION     AVR32_USART0_RXD_0_0_FUNCTION
#  define EXAMPLE_USART_TX_PIN          AVR32_USART0_TXD_0_0_PIN
#  define EXAMPLE_USART_TX_FUNCTION     AVR32_USART0_TXD_0_0_FUNCTION
#  define EXAMPLE_USART_CLOCK_MASK      AVR32_USART0_CLK_PBA
#  define EXAMPLE_PDCA_CLOCK_HSB        AVR32_PDCA_CLK_HSB
#  define EXAMPLE_PDCA_CLOCK_PB         AVR32_PDCA_CLK_PBA

/* Priority definitions for most of the tasks in the demo application. */
//#define mainLED_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )
//우선순위를 변경한다.
#define mainETH_TASK_PRIORITY       ( tskIDLE_PRIORITY + 2 )
//#define display_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )
#define modbusups_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3 )

#define watchdog_TASK_PRIORITY     ( tskIDLE_PRIORITY + 4 )

/* Baud rate used by the serial port tasks. */
#define mainCOM_BAUD_RATE      ( ( unsigned portLONG ) 9600 )
#define mainCOM_BAUD_RATE_2400      ( ( unsigned portLONG ) 2400 )
#define mainCOM_BAUD_RATE_9600      ( ( unsigned portLONG ) 9600 )

#define mainCOM_BUFFER_LEN     ( ( unsigned portLONG ) 70 )

/* An address in the internal Flash used to count resets.  This is used to check that
the demo application is not unexpectedly resetting. */
#define mainRESET_COUNT_ADDRESS     ( ( void * ) 0xC0000000 )

/**
 * \fn     main
 * \brief  start the software here
 *         1) Configure system clocks.
 *         2) Setup the GPIO in output for the board's LED.
 *         3) Start Display task.
 *         4) Start the LED flash tasks just to provide visual feedback that the
 *         demo is executing.
 *         5) Start the ethernet tasks.
 *         6) Start FreeRTOS.
 * \return 0, which should never occur.
 *
 */

void serial_init();
void serial_init_2400();
void serial_init_9600();
void serial_init_2400()
{
	sysclk_enable_pba_module(USART_SYSCLK);
	static const usart_options_t USART_OPTIONS =
	{
		.baudrate     = mainCOM_BAUD_RATE_2400,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};
	static const gpio_map_t USART_GPIO_MAP =
	{
		{EXAMPLE_USART_RX_PIN, EXAMPLE_USART_RX_FUNCTION},
		{EXAMPLE_USART_TX_PIN, EXAMPLE_USART_TX_FUNCTION}
	};
	gpio_enable_module(USART_GPIO_MAP,
	sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));
	usart_init_rs232(USART, &USART_OPTIONS, sysclk_get_pba_hz());
};
void serial_init_9600()
{
	sysclk_enable_pba_module(USART_SYSCLK);
	static const usart_options_t USART_OPTIONS =
	{
		.baudrate     = mainCOM_BAUD_RATE_9600,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};
	static const gpio_map_t USART_GPIO_MAP =
	{
		{EXAMPLE_USART_RX_PIN, EXAMPLE_USART_RX_FUNCTION},
		{EXAMPLE_USART_TX_PIN, EXAMPLE_USART_TX_FUNCTION}
	};
	gpio_enable_module(USART_GPIO_MAP,
	sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));
	usart_init_rs232(USART, &USART_OPTIONS, sysclk_get_pba_hz());
};


void serial_init()
{
	//pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
	sysclk_enable_pba_module(USART_SYSCLK);
		
	static const usart_options_t USART_OPTIONS =
	{
		.baudrate     = mainCOM_BAUD_RATE,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};
     
	static const gpio_map_t USART_GPIO_MAP =
	{
		{EXAMPLE_USART_RX_PIN, EXAMPLE_USART_RX_FUNCTION},
		{EXAMPLE_USART_TX_PIN, EXAMPLE_USART_TX_FUNCTION}
	};

	
	// USART options.

	// Assign GPIO to USART.
	gpio_enable_module(USART_GPIO_MAP,
	sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

	// Initialize USART in RS232 mode.
	//usart_init_rs232(EXAMPLE_USART, &USART_OPTIONS, 29142857);
	//usart_init_rs232(EXAMPLE_USART, &USART_OPTIONS, FOSC0);
	usart_init_rs232(USART, &USART_OPTIONS, sysclk_get_pba_hz());
	// Hello world!
	//usart_write_line(EXAMPLE_USART, "Hello, this is the AVR UC3 MCU saying hello! (press enter)\r\n");

};
#include "lwip/opt.h"

#include "wdt.h"
void wdt_scheduler(void);
// Min value of 1s
#define WDT_MIN_VALUE_US   1000000*6
// Min value of 4s
#define WDT_MAX_VALUE_US   1000000*6*10
// Step of 1s
#define WDT_CTRL_STEP_US   1000000*6
// To specify which current Watchdog value
wdt_opt_t opt = {
          .us_timeout_period = WDT_MIN_VALUE_US  // Timeout Value
};
void wdt_scheduler(void);

void wdt_scheduler(void)
{
	volatile avr32_pm_t* pm = &AVR32_PM;
	// If Reset Cause is due to a Watchdog reset just relaunch Watchdog 
	if (pm->RCAUSE.wdt) {
		wdt_reenable();
		//usart_write_line(&AVR32_USART0,"Reset Cause is due to a Watchdog reset just relaunch Watchdog\r\n");
	}
	// If Reset Cause is due to a Power On reset,
	else if (pm->RCAUSE.por) {
		opt.us_timeout_period = WDT_MIN_VALUE_US ;
		// Save current value in GPLP register
		pm_write_gplp(pm, 0, opt.us_timeout_period);
		wdt_enable(&opt);
		//usart_write_line(&AVR32_USART0,"Reset Cause is due to a Power On reset\r\n");
	}
	// If Reset Cause is due to an External reset, 
	else if (pm->RCAUSE.ext) {
		// Reload current value stored in GPLP register
		opt.us_timeout_period = pm_read_gplp(pm, 0);
		opt.us_timeout_period += WDT_CTRL_STEP_US;

		if (opt.us_timeout_period >= WDT_MAX_VALUE_US)
		opt.us_timeout_period = WDT_MIN_VALUE_US;

		wdt_enable(&opt);

		// Save new value in GPLP register
		pm_write_gplp(pm,0,opt.us_timeout_period);
		//usart_write_line(&AVR32_USART0,"Reset Cause is due to an External reset\r\n");
	}	
	// Else relaunch Watchdog 
	else{
		opt.us_timeout_period = WDT_MIN_VALUE_US;

		// Save start value of watchdog in GPLP register
		pm_write_gplp(pm, 0, opt.us_timeout_period);
		wdt_enable(&opt);
		//usart_write_line(&AVR32_USART0,"Else relaunch Watchdog \r\n");
	}
}

//#include "print_funcs_tcpip.h"


void FlashInit(){
	data_ethernet_t ethernet_t;
	flash_read__ethernetInfo(&ethernet_t);   // 읽고
}
void printSystemInfo(){
	char tempbuf[100];
	data_ethernet_t ethernet_t;
	flash_read__ethernetInfo(&ethernet_t);   // 읽고
	#ifdef IFTECH_MODBUS_PROTOCAL
	usart_write_line(EXAMPLE_USART, "\r\n System booting...");
	usart_write_line(EXAMPLE_USART, "\r\n VERSION 1.0.0");
	sprintf(tempbuf,"\r\n ipaddress = %d.%d.%d.%d",ethernet_t.Ethernet_Conf_IpAddr0,ethernet_t.Ethernet_Conf_IpAddr1,ethernet_t.Ethernet_Conf_IpAddr2,ethernet_t.Ethernet_Conf_IpAddr3);
	usart_write_line(EXAMPLE_USART,tempbuf);
	sprintf(tempbuf,"\r\n subnetmask = %d.%d.%d.%d",ethernet_t.Ethernet_Conf_Net_Mask0,ethernet_t.Ethernet_Conf_Net_Mask1,ethernet_t.Ethernet_Conf_Net_Mask2,ethernet_t.Ethernet_Conf_Net_Mask3);
	usart_write_line(EXAMPLE_USART,tempbuf);
	sprintf(tempbuf,"\r\n gateway = %d.%d.%d.%d",ethernet_t.Ethernet_Conf_Gateway_Addr0,ethernet_t.Ethernet_Conf_Gateway_Addr1,ethernet_t.Ethernet_Conf_Gateway_Addr2,ethernet_t.Ethernet_Conf_Gateway_Addr3);
	usart_write_line(EXAMPLE_USART,tempbuf);
	sprintf(tempbuf,"\r\n macaddress = %X:%X:%X:%X:%X:%X",ethernet_t.Ethernet_Conf_EthAddr0,ethernet_t.Ethernet_Conf_EthAddr1,ethernet_t.Ethernet_Conf_EthAddr2,ethernet_t.Ethernet_Conf_EthAddr3,ethernet_t.Ethernet_Conf_EthAddr4,ethernet_t.Ethernet_Conf_EthAddr5);
	usart_write_line(EXAMPLE_USART,tempbuf);

	usart_write_line(EXAMPLE_USART, "\r\n SERIAL 9600,8,NONE");
	#else if MEGATECH_PROTOCAL
	//usart_write_line(EXAMPLE_USART, "Q\r\r");
	//usart_write_line(EXAMPLE_USART, "Q1\r\r");
	#endif
}
	// 1) Configure system clocks.
	//SYSCLK_SRC_RCSYS
	//taskENTER_CRITICAL();
	//test_main();
	//while(1);
	//taskEXIT_CRITICAL();

	//ethernet_t.Ethernet_Conf_IpAddr0 = hash_code [0] ;
	//ethernet_t.Ethernet_Conf_IpAddr1 = hash_code [1] ;
	//ethernet_t.Ethernet_Conf_IpAddr2 = hash_code [2] ;
	//ethernet_t.Ethernet_Conf_IpAddr3 = hash_code [3] ;
	//flash_read__ethernetInfo(&ethernet_t);
	//usart_write_line(&AVR32_USART0,"Good To See You first line\r\n");
	// LED 테스크도 사용하지 않는다.
	//vStartLEDFlashTasks( mainLED_TASK_PRIORITY );
	
	// 5) Start the ethernet tasks.
	//vStartEthernetTaskLauncher( configMAX_PRIORITIES );
	

	//e_mail task..임시적으로 사용하지 않는다.
	//vEmail_Start(display_TASK_PRIORITY);
	//시리얼...read Test
    //modbusups_TASK_PRIORITY
    
    // SNMP의 sysUpTime을 위해 사용한다.

void FlashInit();
extern ups_info_t ups_info;

xQueueHandle modbusQueue = 0;

int main( void )
{
	sysclk_init();
	serial_init() ;
	FlashInit();
	printSystemInfo() ;
	flash_read_ups_info(&ups_info);

	modbusQueue = xQueueCreate( 5, sizeof(Byte) );

	if(ups_info.ups_type== 31 || ups_info.ups_type== 32 || ups_info.ups_type== 33  ) //아이에프텍
		serial_init_9600();
	else if(ups_info.ups_type== 50 || ups_info.ups_type== 51 || ups_info.ups_type== 52  )  // 메가텍
		serial_init_2400();
	
	wdt_scheduler();
	wdt_disable();

	vParTestInitialise();

	usart_write_line(EXAMPLE_USART, "\r\n net service started...");
	vStartEthernetTaskLauncher( mainETH_TASK_PRIORITY );
	
	usart_write_line(EXAMPLE_USART, "\r\n serial modebus service started...");
	vStartUPSModeBus(modbusups_TASK_PRIORITY);

	usart_write_line(EXAMPLE_USART, "\r\n timer service started...");
	vStartSoftTimer(watchdog_TASK_PRIORITY );
	// 6) Start FreeRTOS.




	vTaskStartScheduler();
	/* Will only reach here if there was insufficient memory to create the idle task. */
	return 0;
}
/*-----------------------------------------------------------*/
	
    //sysclk_init();
	//usart_putchar(&AVR32_USART0,0xAA);
	//#define AVR32_USART0                       (*((volatile avr32_usart_t*)AVR32_USART0_ADDRESS))
	/*
	
	
	usart_write_line(&AVR32_USART0,"Good To See You second line\r\n");
	
	
	*/
	
	/*
	const unsigned long xDelay = 1000 / 1000;
	vTaskDelay( xDelay ) ;
	usart_write_line(&AVR32_USART0,"Good To See You\r\n");
	vTaskDelay( xDelay ) ;
	usart_write_line(&AVR32_USART0,"Good To See You\r\n");
	vTaskDelay( xDelay ) ;
	usart_write_line(&AVR32_USART0,"Good To See You\r\n");
	vTaskDelay( xDelay ) ;
	usart_write_line(&AVR32_USART0,"Good To See You\r\n");
	*/
	
	
	// 2) Setup the GPIO in output for the board's LED.
	
		// 3) Start Display task.
		//vDisplay_Start(display_TASK_PRIORITY);

		// 4) Start the LED flash tasks just to provide visual feedback that the
		// demo is executing.