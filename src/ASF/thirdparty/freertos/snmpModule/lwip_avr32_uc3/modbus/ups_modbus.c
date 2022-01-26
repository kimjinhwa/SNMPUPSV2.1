	/*
 * ups_modbus.cpp
 *
 * Created: 2017-11-16 17:20:58
 *  Author: STELLA
 * 2/17 : ��ſ��� �κ��� �����Ѵ�. 
 * ��ſ����� MAX_COMM_ERROR_COUNT�� �����Ѵ�. 
 * ��ſ���ī��Ʈ�� UPS_MODULE���� ���� ������ �����Ǹ�
 * ����� �����ϸ� 0���� ���õȴ�.
 * MAX_COMM_ERROR_COUNT�� �����ϸ� ��ü ����Ÿ�� 0xFF�� �����Ѵ�.
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
ups_info_t ups_info;
//struct ups_modbus_data upsModeBusData;
ups_modbus_data_t upsModeBusData;
xppc_data_t	xppc_data;

uint16_t modebusPrcessCount=0;

//Installed_Battery_Cells	10	NEP 33
//				11	NEP 32
//58*2 = 116���� ���۰� �ʿ��ϴ�.


//extern wdt_opt_t opt;
uint32_t  snmp_get_everyMinute();
void  snmp_set_everyMinute(uint16_t value);
err_t snmp_send_trap_ups_exp(s8_t generic_trap, s32_t specific_trap);
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


//-----------------------------------
/* moved to Header file
typedef enum
{
	eSNMP,
	eWEB
}DataSource_t;
*/



xQueueHandle xQueue;

static const Data_t xStructsToSend[3] =
{
	{100,eMODBUS},
	{200,eSNMP},
	{300,eWEB}
};


signed portBASE_TYPE  addTo_QueueTask(DataSource_t eTask)
{
	signed portBASE_TYPE sStatus;
	if(eTask == eMODBUS) {
		    sStatus	=  xQueueSendToBack(xQueue,&xStructsToSend[0],1000);
			vTaskDelay(10);		
			return sStatus; 
		}
		
	else if(eTask == eSNMP){
		    sStatus	=  xQueueSendToBack(xQueue,&xStructsToSend[1],1000);
			vTaskDelay(10);		
			return sStatus; 
	}
	else if(eTask == eWEB){
		    sStatus	=  xQueueSendToBack(xQueue,&xStructsToSend[2],1000);
			vTaskDelay(10);		
			return sStatus; 
	}
}

signed portBASE_TYPE  receiveFrom_QueueTask(DataSource_t eTask){
	Data_t xStructsReceive;	
	xQueueReceive(xQueue,&xStructsReceive,0);
	//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n Queue is %d",xStructsReceive.eDatasource) );
	return  xStructsReceive.eDatasource;
}
////////////////////////////

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
		flash_read__ethernetInfo(&ethernet_t);   // �а�
		
		usart_write_line(&AVR32_USART0,"macaddress[5]:");
		revInt = getIntegerSerial();
		ethernet_t.Ethernet_Conf_EthAddr5 =revInt ;
		print_dbg_char_hex(revInt);
		usart_write_line(&AVR32_USART0,"\r\n");

		flash_write__ethernetInfo(&ethernet_t);  //����
	}

	if(  strstr(command , (const char*)"ipaddress") != NULL  ) 
	{
		data_ethernet_t ethernet_t;
		flash_read__ethernetInfo(&ethernet_t);   // �а�
		
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
		
	    flash_write__ethernetInfo(&ethernet_t);  //����
	}
	if(  strstr(command , (const char*)"gateway") != NULL  ) 
	{
		data_ethernet_t ethernet_t;
		flash_read__ethernetInfo(&ethernet_t);   // �а�
		
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
	    flash_write__ethernetInfo(&ethernet_t);  //����
	}
	if(  strstr(command , (const char*)"subnetmask") != NULL  ) 
	{
		data_ethernet_t ethernet_t;
		flash_read__ethernetInfo(&ethernet_t);   // �а�
		
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
	    flash_write__ethernetInfo(&ethernet_t);  //����
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
	
	while( usart_read_char(&AVR32_USART0, &rx_char) != USART_RX_EMPTY );	//���ۿ� ����Ÿ�� �ִٸ� ����.	
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
//	0			"���̿����� ������ ������� Ver1.0 3��3��",
//	1			"���̿����� ������ ������� Ver1.0 3��1��",
//	2			"���̿����� ������ ������� Ver1.0 1��1��",
//	3			"Megatec��������  3��3��",
//	4			"Megatec��������  3��1��",
//	5			"Megatec��������  1��1��"

err_t snmp_send_trap_ups_kepco(s8_t generic_trap, s32_t specific_trap) ;
err_t snmp_send_trap_ups(s8_t generic_trap, s32_t specific_trap) ;
void snmp_coldstart_trap(void);

void serial_init_2400();
void serial_init_9600();
Bool bModebusSuccess=true;
static portTASK_FUNCTION( vModbusUpsTask, pvParameters )
{
	//portTickType xFlashRate, xLastFlashTime;

	xQueue = xQueueCreate(3,sizeof(Data_t));

	portTickType xTimeBefore, xTotalTimeSuspended;
	unsigned portBASE_TYPE uxLED;
	//Bool (*requestUpsData)();
	void (*write_log_event)();
	
	// ó�� ���μ����� ������ �� ���������� ���� �Ѵ�.
	flash_read_ups_info(&ups_info);
	if(ups_info.ups_type== 11 || ups_info.ups_type== 32 || ups_info.ups_type== 33  ) //���̿�����
	{
		requestUpsData = requestUpsData_22_32_33;
		write_log_event = write_log_event_iftech_22_32_33;
		//serial_init_9600();
	}
	else if(ups_info.ups_type== 50 || ups_info.ups_type== 51 || ups_info.ups_type== 52  )  // �ް���
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

	// UDP ��Ʈ���� UP �Ǵ� �ð��� ��ٷ� �ش�.
	vTaskDelay( 4000);
	snmp_coldstart_trap();
	int rx_char;
	int wait_count=3;
	wdt_clear();
	if(ups_info.ups_type== 50 || ups_info.ups_type== 51 || ups_info.ups_type== 52)
	{  // if megatec then get ups information from ups
		while(wait_count--){  //rtx_char�� ����Ÿ�� ������ �����Ѵ� �׷��� �ʴٸ� �����ʿ䰡 ����..
				   //�� ��쿡�� Web�� �۵� �ϹǷ� ��	���� ���������� �����Ͽ� �ش�...
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
			//���⼭ ��� ������ �ȵǰ� ������ Ŀ�ؼ��� ��ٷ� �ش�.
			wdt_clear();
			vTaskDelay( 300);
		};
		if(ups_info.ups_type== 50 ) { upsModeBusData.Input_Phase = 3; upsModeBusData.Output_Phase= 3; }
		else if(ups_info.ups_type== 51 ) { upsModeBusData.Input_Phase = 3; upsModeBusData.Output_Phase= 1; }
		else if(ups_info.ups_type== 52 ) { upsModeBusData.Input_Phase = 1; upsModeBusData.Output_Phase= 1; }
		else { upsModeBusData.Input_Phase = 3; upsModeBusData.Output_Phase= 1; }


		//xTimeBefore= xTaskGetTickCount();

		//simulate

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
	//��â�� ���� ����� ��� �Ʒ��� �ڵ�� �ǹ̰� ��� �����Ѵ�.
	//else{
		//int revCount;	
		//int ret;
		//char buffer[RECEIVE_BUFFER_SIZE];
		//int receivedCount;
	//
		//while(requestUpsData() != USART_SUCCESS)
		//{
			//wdt_clear();
			//vTaskDelay(2000)	;
		//}
	//}

	wdt_clear();

	int communication_errorCount=0;
	for(;;)
	{
		//processRequestCheckAndWaitTimeout(1000); //���μ����� ���� �ִٸ� ��ٸ���.
		vParTestSetLED(3, pdTRUE);
		modebusPrcessCount++;
		addTo_QueueTask(eMODBUS);//�������� �����Ѵ�.
		portENTER_CRITICAL();
		//ó�� ���۽ÿ��� true�� �ִ�.
		if( bModebusSuccess) {
			bModebusSuccess= requestUpsData();   // 173 ms taken
			if(bModebusSuccess == false )  //��ſ� ������ ����� Ʈ���� �߻���Ų��.
			{
				communication_errorCount++;
				if(communication_errorCount >= COMMUNICATION_ERRCOUNT_TRAP)
					snmp_send_trap_ups_exp(6,1);//UPS communication has been lost
			}
			//������ ���ٸ� Ʈ���� ����
		}
		else{  
			//���� ��ſ� ������ ���� �����̸� Ʈ���� �ѹ� �� �߻���Ű��

			communication_errorCount==0;
			snmp_send_trap_ups_exp(6,1);//UPS communication has been lost
			bModebusSuccess= requestUpsData();   // 173 ms taken
			if(bModebusSuccess == true)  //��ſ� ������ ������
			{
				snmp_send_trap_ups_exp(6,8);//UPS communication has established 
			}
			// ��� ������ �ִٸ�  Ʈ���� ��� �۽ŵȴ�.
		}
		

		portEXIT_CRITICAL();
		receiveFrom_QueueTask(eMODBUS);
		vParTestSetLED(3, pdFALSE);
		portEXIT_CRITICAL();
		if(bModebusSuccess ) // ����Ÿ�� �޾� ���� �׶� ����Ÿ �˻縦 �����Ѵ�.
		{
			write_log_event();
		}
		vTaskDelay( 2000);
	}
}


unsigned portBASE_TYPE getQueueRemainCount( xQueueHandle xQueue );
int16_t processRequestCheckAndWaitTimeout(int processTimeOut)
{
	int8_t number;
	number = getQueueRemainCount(xQueue);
	if( number != 0    )
	{  // ����Ÿ�� ��û�ϰ� �ִ� ���̴� �㰡�� �ش�.
		while(1){ // SNMP�� WEB���� ����Ÿ�� ��û�ϰ� ���� ���� �ϴµ� ���� �ɸ��� �ð��� ��ٷ� ��� �Ѵ�.
			// �ʹ� �ð��� ���� �ɸ��� WatchDog�� �߻��� �� ������, Waiting�ϴ� ���� ��ŭ�� ���⼭ ��ġ���� Ŭ���� ���� �ش�.
			vParTestSetLED(1, pdTRUE);
			vTaskDelay(50);
			//if(( processTimeOut % 1000)  == 0 )  	wdt_clear();
			 number =  getQueueRemainCount(xQueue);
			if(number == 0 ) {
					vParTestSetLED(1, pdFALSE);
					wdt_clear();	
					return 0;
			}
			vParTestSetLED(1, pdFALSE);
			vTaskDelay(50);
		}
		vParTestSetLED(1, pdFALSE);
		return -1;
	}
	return 0;
}





