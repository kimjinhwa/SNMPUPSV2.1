	/*
 * ups_modbus.cpp
 *
 * Created: 2017-11-16 17:20:58
 *  Author: STELLA
 * 2/17 : 통신에러 부분을 수정한다. 
 * 통신에러는 MAX_COMM_ERROR_COUNT로 설정한다. 
 * 통신에러카운트는 UPS_MODULE내의 전역 변수로 설정되며
 * 통신이 성공하면 0으로 셋팅된다.
 * MAX_COMM_ERROR_COUNT에 도달하면 전체 데이타는 0xFF로 설정한다.
 */ 
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "partest.h"
#include "udp.h"
#include "led.h"
#include "usart.h"
/* lwIP includes. */
//#include "lwip/api.h"
//#include "lwip/tcpip.h"
//#include "lwip/memp.h"
//#include "lwip/stats.h"
#include "lwip/init.h"
#include "megatech_protocal.h"
#include "iftech_protocal.h"

/* Demo program include files. */
#include "partest.h"
#include "flash.h"
#include "usart.h"
#include "ups_modbus.h"
#include "string.h"
#include "debug.h"
#include "mem.h"

#include "sockets.h"
/* ethernet includes */
#include "ethernet.h"
#include "wdt.h"
#include "networkTime.h"

#define BIT(n)  1<<n
	
#define modbusSTACK_SIZE		configMINIMAL_STACK_SIZE
//#define modbusSTACK_SIZE		384
//#define modbusSTACK_SIZE		512+64

//#define USART_SERIAL_ISR_HANDLER     USART0_Handler

//#define BIT(n)	(0x00000001 << n)

#define ledFLASH_RATE_BASE	( ( portTickType ) 333 )

#define RECEIVE_BUFFER_SIZE	150
#define MAX_COMM_ERROR_COUNT 5
#ifndef SNMP_GENTRAP_ENTERPRISESPC
#define SNMP_GENTRAP_ENTERPRISESPC 6
#endif



static volatile unsigned portBASE_TYPE uxFlashTaskNumber = 0;
static portTASK_FUNCTION( vModbusUpsTask, pvParameters );

//Bool parse_I_megatec(char *str);
//bool upsInformationCommand_I_megatec();
//Bool parse_F_megatec(char *str);
//bool upsInformationCommand_F_megatec();
//void strtoreplace(char* str);
//unsigned char strToInt(char *str );

#include "print_funcs_tcpip.h"
#include "FlashMemory.h"

//bool modbus_data_request = false;
bool isSerialLineUsed=false;
ups_info_t ups_info;
//struct ups_modbus_data upsModeBusData;
ups_modbus_data_t upsModeBusData;

uint16_t modebusPrcessCount=0;
uint16_t isModebusRunning=0;

//Installed_Battery_Cells	10	NEP 33
//				11	NEP 32
//58*2 = 116개의 버퍼가 필요하다.


//extern wdt_opt_t opt;
extern Bool isNowWebServiceRunning;
uint32_t  snmp_get_everyMinute();
void  snmp_set_everyMinute(uint16_t value);
int wait_command() ;

bool is_Converter_Operation_Fault_send_to_snmp=false;
bool is_Converter_Operation_Fault_send_to_snmp_b7=false;
bool is_Converter_Operation_Fault_send_to_snmp_b6=false;
bool is_Converter_Operation_Fault_send_to_snmp_b5=false;
bool is_Converter_Operation_Fault_send_to_snmp_b4=false;
bool is_Converter_Operation_Fault_send_to_snmp_b3=false;
bool is_Converter_Operation_Fault_send_to_snmp_b2=false;
bool is_Converter_Operation_Fault_send_to_snmp_b1=false;
bool is_Converter_Operation_Fault_send_to_snmp_b0=false;

bool is_Inverter_Operation_Fault_send_to_snmp=false;
bool is_maintanence_NFB_send_snmp=false; 

void vStartUPSModeBus( unsigned portBASE_TYPE uxPriority )
{
	signed portBASE_TYPE xLEDTask;
	xTaskCreate( vModbusUpsTask, ( signed char * ) "LEDx", modbusSTACK_SIZE, NULL, uxPriority, ( xTaskHandle * ) NULL );
}
//char command_buffer[20] ;
//int command_buffer_pos=0;
//
int check_uart_receive_char()
{
	int rx_char;
	int retval = USART_SUCCESS;
	  for(int i=0;i<100;i++)
	  {
		  rx_char = usart_getchar(&AVR32_USART0);
		  if (rx_char == USART_FAILURE)
		  {
			  usart_write_line(&AVR32_USART0, "Error!!!\r\n");
			  retval = USART_FAILURE;
			  break;
		  }
		  if (rx_char == '\x03')
		  {
			  retval = USART_FAILURE;
			  break;
		  }
		  usart_putchar(&AVR32_USART0, rx_char);
		  if(rx_char == '>') wait_command() ;
		  if (rx_char == '\r')
		  { // Add a LF and consider this as the end of the line.
			  usart_putchar(&AVR32_USART0, '\n');
			  break;
		  }
		  vTaskDelay(10);
	  }
	  return retval;	
}

int getIntegerSerial()
{
	int rx_char;
	int buffer_count=0;
	Byte command_buffer[20];
	do{
		rx_char = usart_getchar(&AVR32_USART0);
		command_buffer[buffer_count++] = rx_char ;
		if (rx_char == '\r')
		{
		
		}
	}while(rx_char != '\r');
	command_buffer[buffer_count] = NULL;
	return atoi((const char*)command_buffer);
}
int parse_serial_comand(Byte *command)
{
	int revInt;
	//strupr()
	if(  strstr(command ,(const char*)"macaddress") != NULL  )
	{
		data_ethernet_t ethernet_t;
		flash_read__ethernetInfo(&ethernet_t);   // 읽고
		
		usart_write_line(&AVR32_USART0,"macaddress[5]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_EthAddr5 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");

		flash_write__ethernetInfo(&ethernet_t);  //쓰고
	}

	if(  strstr(command , (const char*)"ipaddress") != NULL  ) 
	{
		data_ethernet_t ethernet_t;
		flash_read__ethernetInfo(&ethernet_t);   // 읽고
		
		usart_write_line(&AVR32_USART0,"ipAddress[0]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_IpAddr0 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");


		usart_write_line(&AVR32_USART0,"ipAddress[1]:");
		revInt = getIntegerSerial();
		print_dbg_char_hex(revInt);
		ethernet_t.Ethernet_Conf_IpAddr1 =revInt ;
		usart_write_line(&AVR32_USART0,"\r\n");

		usart_write_line(&AVR32_USART0,"ipAddress[2]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_IpAddr2 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");

		usart_write_line(&AVR32_USART0,"ipAddress[4]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_IpAddr3 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");
		
	    flash_write__ethernetInfo(&ethernet_t);  //쓰고
	}
	if(  strstr(command , (const char*)"gateway") != NULL  ) 
	{
		data_ethernet_t ethernet_t;
		flash_read__ethernetInfo(&ethernet_t);   // 읽고
		
		usart_write_line(&AVR32_USART0,"gateway[0]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_Gateway_Addr0 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");


		usart_write_line(&AVR32_USART0,"gateway[1]:");
		revInt = getIntegerSerial();
		print_dbg_char_hex(revInt);
		ethernet_t.Ethernet_Conf_Gateway_Addr1 =revInt ;
		usart_write_line(&AVR32_USART0,"\r\n");

		usart_write_line(&AVR32_USART0,"gateway[2]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_Gateway_Addr2 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");

		usart_write_line(&AVR32_USART0,"gateway[4]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_Gateway_Addr3 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");
	    flash_write__ethernetInfo(&ethernet_t);  //쓰고
	}
	if(  strstr(command , (const char*)"subnetmask") != NULL  ) 
	{
		data_ethernet_t ethernet_t;
		flash_read__ethernetInfo(&ethernet_t);   // 읽고
		
		usart_write_line(&AVR32_USART0,"netmask[0]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_Net_Mask0 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");


		usart_write_line(&AVR32_USART0,"netmask[1]:");
		revInt = getIntegerSerial();
		print_dbg_char_hex(revInt);
		ethernet_t.Ethernet_Conf_Net_Mask1 =revInt ;
		usart_write_line(&AVR32_USART0,"\r\n");

		usart_write_line(&AVR32_USART0,"netmask[2]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_Net_Mask2 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");

		usart_write_line(&AVR32_USART0,"netmask[4]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_Net_Mask3 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");
	    flash_write__ethernetInfo(&ethernet_t);  //쓰고
	}

	if(  strstr(command , (const char*)"reboot") != NULL  ) 
	{
		while(1) ;
	}
}
int wait_command()
{
	int rx_char;
	int retval = USART_SUCCESS;
	int command_time = 10000;
	Byte command_buffer[20];
	int buffer_count=0;
	
	while( usart_read_char(&AVR32_USART0, &rx_char) != USART_RX_EMPTY );	//버퍼에 데이타가 있다면 비운다.	
	usart_write_line(&AVR32_USART0,"\r\ncommand>");
	
	for(int i=0;i<command_time;i++)
	{
	  rx_char = usart_getchar(&AVR32_USART0);
	  if (rx_char == USART_FAILURE)
	  {
		  usart_write_line(&AVR32_USART0, "Error!!!\r\n");
		  usart_reset_status(&AVR32_USART0);
		  retval = USART_FAILURE;
		  break;
	  }
	  if (rx_char == '\x03')
	  {
		  retval = USART_FAILURE;
		  break;
	  }
	  //usart_putchar(&AVR32_USART0, rx_char);
	  command_buffer[buffer_count++] = rx_char ;
	  if( buffer_count >= 19 ) buffer_count = 18;
	  command_buffer[buffer_count]=NULL;
	  if (rx_char == '\r')
	  { // Add a LF and consider this as the end of the line.
		  
		  usart_write_line(&AVR32_USART0,command_buffer);
		  usart_write_line(&AVR32_USART0,"\r\n");
		  parse_serial_comand(command_buffer);
		  usart_write_line(&AVR32_USART0,"\r\ncommand>");
		  //for(int i = 0 ; i<buffer_count ; i++ ) 	usart_putchar(&AVR32_USART0, '\n');
  		  buffer_count= 0;
		  break;
	  }
	  vTaskDelay(1);
	}
	usart_write_line(&AVR32_USART0,"\r\n end command!");
	return retval;	
}

//ISR(USARTC0_DRE_vect)
//{
//	
//}
//	0			"아이에프텍 모드버스 프로토골 Ver1.0 3상3상",
//	1			"아이에프텍 모드버스 프로토골 Ver1.0 3상1상",
//	2			"아이에프텍 모드버스 프로토골 Ver1.0 1상1상",
//	3			"Megatec프로토콜  3상3상",
//	4			"Megatec프로토콜  3상1상",
//	5			"Megatec프로토콜  1상1상"

err_t snmp_send_trap_ups_kepco(s8_t generic_trap, s32_t specific_trap) ;
err_t snmp_send_trap_ups(s8_t generic_trap, s32_t specific_trap) ;
void snmp_coldstart_trap(void);
Bool isMegatecSupport_3P ;

void serial_init_2400();
void serial_init_9600();

static portTASK_FUNCTION( vModbusUpsTask, pvParameters )
{
	//portTickType xFlashRate, xLastFlashTime;
	portTickType xTimeBefore, xTotalTimeSuspended;
	unsigned portBASE_TYPE uxLED;
	//Bool (*requestUpsData)();
	void (*write_log_event)();
	
	// 처음 프로세서를 가동할 때 프로토콜을 결정 한다.
	flash_read_ups_info(&ups_info);
	if(ups_info.ups_type== 31 || ups_info.ups_type== 32 || ups_info.ups_type== 33  ) //아이에프텍
	{
		requestUpsData = requestUpsData_22_32_33;
		write_log_event = write_log_event_iftech_22_32_33;
		//serial_init_9600();
	}
	else if(ups_info.ups_type== 50 || ups_info.ups_type== 51 || ups_info.ups_type== 52  )  // 메가텍
	{
		//serial_init_2400();
		requestUpsData = requestUpsStatus_megatec;
		write_log_event = write_log_event_megatec;
	}
	else if(ups_info.ups_type== 80 || ups_info.ups_type== 81 || ups_info.ups_type== 82  )  // sena
	{  
		requestUpsData = requestUpsData_22_32_33;
		write_log_event = write_log_event_iftech_22_32_33;
	}
	else
	{  
		requestUpsData = requestUpsData_22_32_33;
		write_log_event = write_log_event_iftech_22_32_33;
	}
	
	( void ) pvParameters;

	/*
	portENTER_CRITICAL();
	{
		uxLED = uxFlashTaskNumber;
		uxFlashTaskNumber++;
	}
	portEXIT_CRITICAL();
	*/
	// UDP 네트웍이 UP 되는 시간을 기다려 준다.
	vTaskDelay( 4000);
	snmp_coldstart_trap();
	/*
	xFlashRate = ledFLASH_RATE_BASE + ( ledFLASH_RATE_BASE * ( portTickType ) uxLED );
	xFlashRate /= portTICK_RATE_MS;
	
	xFlashRate /= ( portTickType ) 2;
	xLastFlashTime = xTaskGetTickCount();
	*/
	int rx_char;
	int wait_count=0;
	wdt_clear();
	if(ups_info.ups_type== 50 || ups_info.ups_type== 51 || ups_info.ups_type== 52)
	{  // if megatec then get ups information from ups
		while(1){  //rtx_char에 데이타가 있으면 리턴한다 그렇지 않다면 나올필요가 없다..
				   //이 경우에도 Web은 작동 하므로 웹	에서 프로토콜을 변경하여 준다...
			modebusPrcessCount++;
			vParTestSetLED(1, pdTRUE);
			// 3p3p  3p1p  1p1p all applied
			rx_char= upsInformationCommand_I_megatec() ;
			rx_char= upsInformationCommand_F_megatec() ;
			if(ups_info.ups_type== 50 || ups_info.ups_type== 51)	
			{
				upsInformationCommand_GF_megatec();
			}
            if(rx_char)break;
			vParTestSetLED(1, pdFALSE);
			vTaskDelay( 1000);
		};
		if(ups_info.ups_type== 50 ) { upsModeBusData.Input_Phase = 3; upsModeBusData.Output_Phase= 3; }
		else if(ups_info.ups_type== 51 ) { upsModeBusData.Input_Phase = 3; upsModeBusData.Output_Phase= 1; }
		else if(ups_info.ups_type== 52 ) { upsModeBusData.Input_Phase = 1; upsModeBusData.Output_Phase= 1; }
		else { upsModeBusData.Input_Phase = 3; upsModeBusData.Output_Phase= 1; }


		//xTimeBefore= xTaskGetTickCount();
		isMegatecSupport_3P = requestUps_G1_megatec();

		//simulate
        //isMegatecSupport_3P = false;

		//xTotalTimeSuspended = xTaskGetTickCount()-xTimeBefore ;
		upsModeBusData.Year_made =2000+ ups_info.install_year;
		upsModeBusData.Month_made= ups_info.install_month;
		upsModeBusData.Date_made= ups_info.install_day;

		upsModeBusData.Input_Voltage= ups_info.input_voltage;
		upsModeBusData.Input_frequency = ups_info.input_frequency;
		
		upsModeBusData.Output_Voltage = ups_info.out_voltage;
		upsModeBusData.Output_frequency= ups_info.output_frequency;
		upsModeBusData.Installed_Battery_Cells=ups_info.installed_battery;
		if( ups_info.charging_voltage <= 1) 
			ups_info.charging_voltage = ups_info.installed_battery*13.2;

		upsModeBusData.Company_code_And_upstype = ups_info.ups_type;
	}
	for(;;)
	{
		vParTestSetLED(3, pdTRUE);
		modebusPrcessCount++;
		//잠시 중단
		Bool bRet=false;
		//if( isNowWebServiceRunning==false && isNowSNMPServiceRunning == false) 
		if( isNowWebServiceRunning==false )
		{
			//통신에 문제가 있으면 한번더 수행하라.
			//한전에서 축전지 전압에 문제가 가끔씩 생긴다고 하여 루틴을 추가 한다.
			for( int i = 0 ; i < 2; i++ )
			{
				vParTestSetLED(1, pdFALSE);
				//xTimeBefore = xTaskGetTickCount();
				taskENTER_CRITICAL() ;
				isModebusRunning=true;
				bRet = requestUpsData();   // 173 ms taken
				//에러가 생기면 한번만 더 해 본다.
				if(!bRet)requestUpsData();   // 173 ms taken
				isModebusRunning=false;
				taskEXIT_CRITICAL();
				//xTotalTimeSuspended = xTaskGetTickCount()-xTimeBefore ;
				if(bRet == true) break;       // 한번 더 수행하라..
				else{
					vTaskDelay( 1);
				}
				//if(i>=3){ break; }
			}
			vParTestSetLED(1, !bRet);
		}
		vParTestSetLED(3, pdFALSE);
		//portEXIT_CRITICAL();
		if(bRet ) // 데이타를 받아 오면 그때 데이타 검사를 수행한다.
		{
			write_log_event();
		}

		vTaskDelay( 1000);
	}
}

Bool getDataFromSerial(){
	
	int reTraycount=10;
	while(isModebusRunning == true)
	{
			vTaskDelay(100);
			if(reTraycount--)return false;
	}
	reTraycount=10;
	while( requestUpsData() == false)	
	{
			vTaskDelay(100);
			if(reTraycount--)return false;
	}
	return true;
}
