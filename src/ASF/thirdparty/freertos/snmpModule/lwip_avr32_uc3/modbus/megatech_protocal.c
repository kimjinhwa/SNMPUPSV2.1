#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <portmacro.h>
#include <math.h>

#include "err.h"
#include "ups_modbus.h"
#include "megatech_protocal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "flashmemory.h"
#include "networkTime.h"

uint32_t  snmp_get_everyMinute();
void  snmp_set_everyMinute(uint16_t value);
err_t snmp_send_trap_ups_kepco(s8_t generic_trap, s32_t specific_trap) ;
err_t snmp_send_trap_ups(s8_t generic_trap, s32_t specific_trap) ;
err_t snmp_send_trap_ups_exp(s8_t generic_trap, s32_t specific_trap);

//extern struct ups_modbus_data upsModeBusData;
extern int udp_send_msg(char *msg,int len,char *ipaddress,int port);

extern ups_modbus_data_t upsModeBusData;
extern xppc_data_t	xppc_data;

extern ups_info_t ups_info;
extern bool is_Converter_Operation_Fault_send_to_snmp;
extern bool is_Converter_Operation_Fault_send_to_snmp_b7;
extern bool is_Converter_Operation_Fault_send_to_snmp_b6;
extern bool is_Converter_Operation_Fault_send_to_snmp_b5;
extern bool is_Converter_Operation_Fault_send_to_snmp_b4;
extern bool is_Converter_Operation_Fault_send_to_snmp_b3;
extern bool is_Converter_Operation_Fault_send_to_snmp_b2;
extern bool is_Converter_Operation_Fault_send_to_snmp_b1;
extern bool is_Converter_Operation_Fault_send_to_snmp_b0;
extern int16_t  before_Inverter_State;

float upsRankedCurrent=0.0;
int16_t charging_method;


unsigned char strToInt(char *str )
{
	unsigned char retByte=0;
	for(int i=0;i<8;i++)
	{
		if( *(str+i)  == 0x30 ) retByte |= ( 0 << 7-i ) ;
		else					retByte |= ( 1 << 7-i );
	}
	return retByte;
}

char *strtokExt(char* str, char* desti)
{
	int i=0;
	for(i=0;i<strlen(str);i++)
	{
		if(*(str+i)==0x20)
		{
			desti[i]=0x00;
			return str+i+1;
		}
		else if(*(str+i)==0x00)
		{
			desti[i]=0x00;
			return NULL;
		}
		desti[i] = str[i];
	}
	desti[i]=0x00;
}
void strtoreplace(char* str)
{
	for(int i=0;i<strlen(str);i++)
		if(*(str+i)==0x0D)
			*(str+i)=0x00;
}

bool requestUpsStatus_Q1_megatec()
{
	int16_t *pData;
	pData =(int16_t *)&upsModeBusData ;//
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	if(buffer == NULL) 	{		return false;	} ;// memory alloc error;
	memset(buffer,0x00,RECEIVE_BUFFER_SIZE);

	int receivedCount = megatec_command_CR("Q1\r",3,'(',500,buffer); // (는 빼고 넘긴다.

	if(receivedCount > 0) 
	{

		parseQ1_megatec(buffer+1);
		free(buffer);
		return true;
	}
	free(buffer);
    return false	;
}
// cr이 올때 까지 데이타를 읽는다.
// if return < 0  : error
//    return = 0 : just send data
//    return  > 0 : receiveData count 
int megatec_command_CR(uint8_t *send_command,uint8_t sLength,uint8_t startByte,int watingTime,uint8_t *receive_data)
{
	int revCount=0;
	int receive=0;
	int ret=0 ;


	usart_reset_status(&AVR32_USART0);  // 우선 수신 버퍼를 리셋한다.
	while( usart_read_char(&AVR32_USART0, &revCount) != USART_RX_EMPTY );
	usart_write_byteArray(&AVR32_USART0,(const char *)send_command,sLength);			// 명령어를 송신한다.
    if( watingTime == 0 ) return 0;
	do{
		if(checkUsartData(&AVR32_USART0,watingTime) == 0 )return -1;
		ret = usart_read_char(&AVR32_USART0, &receive);
		*(receive_data+revCount) = (uint8_t)receive;
		revCount++;
		if(revCount>=RECEIVE_BUFFER_SIZE)break;
	}while(receive!='\r');
	*(receive_data+revCount-1) = 0x00;
	return revCount;

	/*
	usart_reset_status(&AVR32_USART0);  // 우선 수신 버퍼를 리셋한다.
	while( usart_read_char(&AVR32_USART0, &revCount) != USART_RX_EMPTY )
	{
	//	vTaskDelay(5);
	};	//버퍼에 데이타가 있다면 비운다.
	usart_write_byteArray(&AVR32_USART0,(const char *)send_command,sLength);			// 명령어를 송신한다.
    if( watingTime == 0 ) return 0;
//   (000.0 323.0 001.0 001 00.0 00.0 27.5 01110001
	revCount=0;
	for(int iii=0;iii< RECEIVE_BUFFER_SIZE	;iii++)
	{
		if(checkUsartData(&AVR32_USART0,watingTime) == 0 )
		{
			vTaskDelay(10);
			return -1; 
		}
		ret = usart_read_char(&AVR32_USART0, &receive);
		if(ret == USART_SUCCESS) 
		{
			if(receive=='\r') // Finish to receive Data...so return;
			{ 
				*(receive_data+revCount) = 0x00; // we don't use \r so replace \r to 0x00..
				return revCount;
			}
			else *(receive_data+revCount) = (uint8_t)receive;
			if(*(receive_data+0) != startByte ) {
				revCount=0; //  set revCount to 0 from receive startByte
				return -1;
			}
			else revCount++;
			
		}
		else{
			return -1;
		}
	}
	return -1;
	*/
}
Bool parseQ1_megatec(char *str)
{
	int input_fault_voltage =0;
	int ups_status=0;
	char destini[10];
	char *ptr = destini;

	str = strtokExt(str, ptr);  //b I/P Voltage
	if (ptr == NULL)return	false;
	upsModeBusData.Input_r_volt_rms = (int) atof(ptr);  //소수점을 정수화 한다. // I/P voltage is 208.4V.
	//simulate
	//upsModeBusData.Input_r_volt_rms = 222;
	if(ups_info.ups_type == 50  ||  ups_info.ups_type == 51 ){// 메가텍 3P 장비이지만 
			upsModeBusData.Input_s_volt_rms = upsModeBusData.Input_r_volt_rms ;
			upsModeBusData.Input_t_volt_rms = upsModeBusData.Input_r_volt_rms ;
			upsModeBusData.Input_s_volt_rms = upsModeBusData.Input_r_volt_rms ;
			upsModeBusData.Input_t_volt_rms = upsModeBusData.Input_r_volt_rms ;
	}
	else if(ups_info.ups_type ==  52)  {// 단상 입력이면
		upsModeBusData.Input_s_volt_rms =0; 
		upsModeBusData.Input_t_volt_rms =0; 
	}
	upsModeBusData.Input_Voltage= (int) atof(ptr);  //소수점을 정수화 한다. // I/P voltage is 208.4V.

	str = strtokExt(str, ptr);  //c  I/P Fault Voltage
	if (ptr == NULL)return	false;
	input_fault_voltage = (int) atof(ptr);  //I/P fault voltage is 140.0V.

	str = strtokExt(str, ptr);
	if (ptr == NULL)return	false;// d  O/P voltage is 208.4V.
	upsModeBusData.Output_r_volt_rms = (int) atof(ptr);  
	//simulate
	//upsModeBusData.Output_r_volt_rms = 222;
	upsModeBusData.Output_Voltage = upsModeBusData.Output_r_volt_rms;  
	upsModeBusData.Inverter_u_volt_rms=upsModeBusData.Output_r_volt_rms ;
	upsModeBusData.Inverter_v_volt_rms=upsModeBusData.Output_s_volt_rms ;
	upsModeBusData.Inverter_w_volt_rms=upsModeBusData.Output_t_volt_rms ;
	if((upsModeBusData.Output_Phase == 1) )
	{
	upsModeBusData.Output_s_volt_rms =0;
	upsModeBusData.Output_t_volt_rms = 0;
	upsModeBusData.Inverter_v_volt_rms=upsModeBusData.Output_s_volt_rms ;
	upsModeBusData.Inverter_w_volt_rms=upsModeBusData.Output_t_volt_rms ;
	}
		
	str = strtokExt(str, ptr);  // e output current
	if (ptr == NULL)return	false;
	upsModeBusData.Output_R_Load=(int)atof(ptr);
	upsModeBusData.Output_u_current_rms = 
		upsModeBusData.Output_u_current_rms = upsRankedCurrent*(upsModeBusData.Output_R_Load/100.0);
	upsModeBusData.Inverter_U_curr_rms=upsModeBusData.Output_u_current_rms ; 

		//upsModeBusData.Ups_Capacitor
	//simulate
	//upsModeBusData.Output_u_current_rms = 111;
	
	str = strtokExt(str, ptr);  //f  input frequency
	if (ptr == NULL)return	false;
	upsModeBusData.Input_frequency = (int) (atof(ptr)*10);  //I/P frequency is 59.9 HZ.
	//simulate
	//upsModeBusData.Input_frequency = 600;
	upsModeBusData.Bypass_Frequency = upsModeBusData.Input_frequency ;
	upsModeBusData.Output_frequency= upsModeBusData.Output_Voltage>10 ? upsModeBusData.Input_frequency: 0;
	upsModeBusData.Inverter_Frequency= upsModeBusData.Output_Voltage>10 ? upsModeBusData.Input_frequency: 0;
	
	str = strtokExt(str, ptr);  // g battery voltage
	if (ptr == NULL)return	false;
	//단상이든 3상이든 셀수를 곱하여 전압을 얻는다. 
	//삼상은 다시 전압을 얻기 때문에 상관이 없다.
	upsModeBusData.Bat_volt_rms = (int) (atof(ptr)*ups_info.installed_battery) ;  //h  Battery voltage is 2.05V.
	
	str = strtokExt(str, ptr);  // i  Temperaqture
	if (ptr == NULL)return	false;
	upsModeBusData.Battery_Room_Temper = (int) atof(ptr);  //Temperature is 25.0 degrees of centigrade.

	str = strtokExt(str, ptr);  // g ups status
	if (ptr == NULL)return	false;
	
	if(strlen(ptr) < 8) return;
	ups_status = strToInt(ptr);
	upsModeBusData.Inverter_State = ups_status;
	return true;
}

//Battery voltage, Battery Capacity,Battery Remaining, Battery current, Temperature,I/P freq,Freq bypss,o/p freq,
bool requestUps_G1_megatec()
{
	int16_t *pData;
	pData =(int16_t *)&upsModeBusData ;//
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	if(buffer == NULL) 	{
				return false;	
				} ;// memory alloc error;
	memset(buffer,0x00,RECEIVE_BUFFER_SIZE);


	int receivedCount;
	receivedCount = megatec_command_CR("G1\r",3,'!',500,buffer); // (는 빼고 넘긴다.
	if(receivedCount > 0) 
	{
		//Q1 command 에서 이미 배터리의 데이타는 잘 받았다.
		//변경 Q1 데이타 보다틑 G1데이타가 가장 정확한 데이타로 본다.
		//따라서 만일 G1 command 에서 배터리데이타가 없어도 
		//G1에서 얻은 데이타를 사용한다.
		if(parseG1_megatec(buffer+1))
		{
			free(buffer);
			return true;
		}
		else{
			free(buffer);
			return false;
		}
			
	}
	free(buffer);
    return false	;
}

bool parseG1_megatec(char *str)
{
	int voltage =0;
	char *argv[1];
	char destini[20];
	char *ptr = destini;

	//if(!(ups_info.ups_type == 50 || ups_info.ups_type == 51 )) // must megatec and 3P3P or 3P1P

	strtoreplace(str);
	//1 b ---------Battery Voltage-
	*argv = strtok(str," ");
	if (argv[0] == NULL) return false;
	voltage= (int) atof(argv[0]);
	upsModeBusData.Bat_volt_rms= voltage;
	xppc_data.upsSmartBatteryVoltage = voltage;

	//2 c ----------Battery Capacity percentage PPP
	*argv = strtok(NULL," ");
	if (argv[0] == NULL) return false;
	voltage= (int) atof(argv[0]);
	xppc_data.upsSmartBatteryCapacity = voltage;
	//upsModeBusData.Input_r_volt_rms = voltage;

	//3 d -----------Battery Time remainning...NNNN
	*argv = strtok(NULL," ");
	if (argv[0] == NULL) return false;
	voltage= (int) atof(argv[0]);
	upsModeBusData.reserved_3_batteryTimeRemain= voltage;
	xppc_data.upsSmartBatteryRunTimeRemaining = voltage;
	// 4 e ----------Battery current in changing mode or discharging mode RRR.R
	*argv = strtok(NULL," ");
	if (argv[0] == NULL) return false;
	voltage= (int) atof(argv[0]);
	upsModeBusData.Bat_current_rms = voltage;
	xppc_data.upsChargeOrDischargeBatteryCurrent=voltage;
	//5 f ------------Temperature.
	*argv = strtok(NULL," ");
	if (argv[0] == NULL) return false;
	voltage= (int) atof(argv[0]);
	upsModeBusData.Battery_Room_Temper= voltage;
	xppc_data.upsSmartBatteryTemperature=voltage;

	//6 g -------------Input Frequency
	*argv = strtok(NULL," ");
	if (argv[0] == NULL) return false;
	voltage= (int) (atof(argv[0])*10);
	upsModeBusData.Input_frequency= voltage;

	//7 h ------------Bypass frequency-
	*argv = strtok(NULL," ");
	if (argv[0] == NULL) return false;
	voltage= (int) atof(argv[0])*10;
	upsModeBusData.Bypass_Frequency= voltage;

	//8 i ------------output frequency-
	*argv = strtok(NULL," ");
	if (argv[0] == NULL) return false;
	voltage= (int) (atof(argv[0])*10);
	upsModeBusData.Output_frequency = voltage;

	return true;
}
	
bool requestUps_G2_megatec()
{
	int16_t *pData;
	pData =(int16_t *)&upsModeBusData ;//
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	if(buffer == NULL) 	{		return false;	} ;// memory alloc error;
	memset(buffer,0x00,RECEIVE_BUFFER_SIZE);


	int receivedCount = megatec_command_CR("G2\r",3,'!',500,buffer); // (는 빼고 넘긴다.
	if(receivedCount > 0) 
	{
		if(parse_G2_megatec(buffer+1))
		{
			free(buffer);
			return true;
		}
		else{
			
			free(buffer);
			return false;
		}
	}
	free(buffer);
    return false	;
}
// !00000010 00000100 00000000<cr>
// Status of rectifier
// BIT(7)	BIT(6)						BIT(5)					BIT(4)		 BIT(3)					BIT(2)		BIT(1)			BIT(0) 
// No Use	Rectifier Rotation Error	1:Low Batery ShutDown	Low Battery	 1:Three in-One Out		Backup		1:Boost Charge	1:Rectifier Operating 
//																			 0:Three in three out   AC Normal   0:Float Charge									
// Status of inverter
// BIT(7)	BIT(6)	BIT(5)					BIT(4)		 BIT(3)				BIT(2)					BIT(1)								BIT(0) 
// No Use	No Use	BYPASS FREQUENCY FAIL	1:Manual Bypass Breaker ON		1:BYPASS AC Normal		1:Static switch in Inverter Mode	 Inverter Operating 
//										    0:Manual Bypass Breaker OFF		0:BYPASS AC AbNormal	0:Static switch in Bypass Mode						 0:Three in three out   AC Normal   0:Float Charge									
// Fault Condition of inverter
// BIT(7)	 No Use   
//BIT(6)	1:Emergency Stop(EPO)
//BIT(5)	1:High Dc Shotdown
//BIT(4)    1:Manual Bypass Breaker on Shutdown
//BIT(3)    1:Overload Shutdown	
//BIT(2)	1:Inverter O/P Fail Shutdown
//BIT(1)    1:Overtemperature Shutdown
//BIT(0)    1:Short Circuit Shutdown 
Bool parse_G2_megatec(char *str)
{
	int init_size = strlen(str);
	int ups_temperature=0;
	char *argv[1];
	unsigned char ups_status_1=0;
	unsigned char ups_status_2=0;
	unsigned char ups_status_3=0;

	strtoreplace(str);
// 1  status of rectifie    BIT(1) : 0 float charge 1 boot charge
	argv[0] = strtok(str," ");
    if( argv[0]== NULL) return false;
	ups_status_1= strToInt(argv[0]);  
	charging_method = (ups_status_1  & BIT(1)) >> 1; 

// 2 status of UPS
	argv[0] = strtok(NULL," ");
    if( argv[0]== NULL) return false;
	ups_status_2= strToInt(argv[0]); 
// 3 The fault condition of inverter
	argv[0] = strtok(NULL," ");
    if( argv[0]== NULL) return false;
	ups_status_3= strToInt(argv[0]);  

	//	Inverter_State 정전 BIT(10) 
	//	Inverter_State 부동 균등 BIT(9) 
	//Q1에서 Inverter_State는 얻었다.
	//Inverter_State 의 상위 8bit에 3P UPS STATUS를 넣는다.
	upsModeBusData.Inverter_State &= 0x00ff ;
	upsModeBusData.Inverter_State |= (ups_status_2 << 8);
	upsModeBusData.Converter_State = (ups_status_3 << 8) + ups_status_1;

	//새롭게 아래의 항목을 집어 넣었다. 
	upsModeBusData.Inverter_Operation_Fault=ups_status_3;

    if( ups_status_1  & BIT(3) ) 
	{
		upsModeBusData.Input_Phase=3;
		upsModeBusData.Output_Phase=1;
	}
	else
	{
		upsModeBusData.Input_Phase=3;
		upsModeBusData.Output_Phase=3;
	}
	return true;
}
bool requestUps_G3_megatec()
{
	int16_t *pData;
	pData =(int16_t *)&upsModeBusData ;//
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	if(buffer == NULL) 	{		return false;	} ;// memory alloc error;
	memset(buffer,0x00,RECEIVE_BUFFER_SIZE);


	int receivedCount = megatec_command_CR("G3\r",3,'!',500,buffer); // (는 빼고 넘긴다.
	if(receivedCount > 0) 
	{
		if(parseG3_megatec(buffer+1))
		{
			free(buffer);
			return true;
		}
		else{
			free(buffer);
			return false;
		}
	}
	free(buffer);
    return false	;
}
Bool parseG3_megatec(char *str)
{
	int voltage =0;
	char *argv[1];
	char destini[20];
	char *ptr = destini;  

	// 1 ------------------------------
	str = strtokExt(str, ptr);  // RST Input Voltage
	if (ptr == NULL) return false;

		*argv = strtok(ptr,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Input_r_volt_rms = voltage;

		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Input_s_volt_rms = voltage;

		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Input_t_volt_rms = voltage;

	// 2 ------------------------------
	str = strtokExt(str, ptr);  // RST Bypass Voltage
	if (ptr == NULL) return false;

		*argv = strtok(ptr,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Bypass_r_volt_rms= voltage;

		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Bypass_s_volt_rms= voltage;

		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Bypass_t_volt_rms= voltage;

	// 3 ------------------------------
	str = strtokExt(str, ptr);  // RST output Voltage
	if (ptr == NULL) return false;

		*argv = strtok(ptr,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Output_r_volt_rms= voltage;
		upsModeBusData.Inverter_u_volt_rms =upsModeBusData.Output_r_volt_rms;
		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Output_s_volt_rms= voltage;
		upsModeBusData.Inverter_v_volt_rms =upsModeBusData.Output_s_volt_rms;

		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false; voltage= (int) atof(argv[0]);  
		upsModeBusData.Output_t_volt_rms= voltage;
		upsModeBusData.Inverter_w_volt_rms =upsModeBusData.Output_t_volt_rms;
		
		
	// 4 ------------------------------
	str = strtokExt(str, ptr);  // RST Load Percentage
	if (ptr == NULL) return false;

		*argv = strtok(ptr,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Output_R_Load= voltage;
		upsModeBusData.Output_u_current_rms = upsRankedCurrent*(upsModeBusData.Output_R_Load/100.0);

		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Output_S_Load= voltage;
		upsModeBusData.Output_v_current_rms = upsRankedCurrent*(upsModeBusData.Output_S_Load/100.0);

		*argv = strtok(NULL,"/");
		if( argv[0] == NULL) return false;
		voltage= (int) atof(argv[0]);  
		upsModeBusData.Output_T_Load= voltage;
		upsModeBusData.Output_w_current_rms = upsRankedCurrent*(upsModeBusData.Output_T_Load/100.0);
	//------------------------------
	return true;
}

bool upsInformationCommand_I_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	int receivedCount = megatec_command_CR("I\r",2,'#',500,buffer); 
	
	if(receivedCount > 0) 
	{
		parse_I_megatec(buffer+1);
	}
	free(buffer);
	if(receivedCount > 0) return true;
	else return false;
}
Bool parse_I_megatec(char *str)
{
		char temp[63];
		memset(temp,0x00,63);
		memset(ups_info.upsIdentManufacturer,0x00,31) ;
		memset(ups_info.upsIdentModel,0x00,63) ;
		memset(ups_info.upsIdentUPSSoftwareVersion,0x00,11) ;

		if(strlen(str) < 35 ) return false;
        strncpy(temp, str, 15);
		temp[15]=0x00;
		memcpy(ups_info.upsIdentManufacturer,temp,15) ;

        strncpy(temp, str+15, 10);
		temp[10]=0x00;
		memcpy(ups_info.upsIdentModel,temp,10) ;

        strncpy(temp, str+25, 10);
		temp[10]=0x00;
		memcpy(ups_info.upsIdentUPSSoftwareVersion,temp,10) ;
		
		return true;
}

bool upsInformationCommand_GF_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	int receivedCount = megatec_command_CR("GF\r",3,'!',500,buffer); 
	
	if(receivedCount > 0) 
	{
		parse_GF_megatec(buffer+1);
	}
	free(buffer);
	if(receivedCount > 0) return true;
	else return false;
}
//!220V/380V 3P4W 060 220V/380V 3P4W 060 220V/380V 3P4W 060 360 150KVA    //a Rect_Volt 14byte 
//b Recifier Frequency CCC
//c Bypss Source Voltage 14byte
//d Bypass Source Frequency
//e O/P Voltage  14 char
//f 0/p Frequcncy QQQ
// g Battery Voltages :SSS
//h : PowerRating 10 character
Bool parse_GF_megatec(char *str)
{
	char argv[15];
	float output_voltage=0;
	float output_current=0;
	int i;
	strtoreplace(str);
	// a  Rectifier Voltage of Phase 
	strncpy(argv,str,14); argv[14]='\0';
	if (*argv == NULL)return	false;
	str = str+14+1;  // space를 건너 뛴다.
	
	//b Rectifier Frequency Rating
	strncpy(argv,str,3); argv[3]='\0';
	if (*argv == NULL)return	false;
	str = (str +3+1);  // space를 건너 뛴다.
	output_voltage=  atof(argv); 

	// c Bypass source voltage 
	strncpy(argv,str,14); argv[14]='\0';
	if (*argv == NULL)return	false;
	str = (str+14+1);  // space를 건너 뛴다.
	
	//d Bypass Frequency 
	strncpy(argv,str,3); argv[3]='\0';
	if (*argv == NULL)return	false;
	(str) = (str+3+1);  // space를 건너 뛴다.
	output_voltage=  atof(argv); 
	ups_info.input_frequency=output_voltage;

	// e Output voltage 
	strncpy(argv,str,14); argv[14]='\0';
	if (*argv == NULL)return	false;
	str= (str+14+1);  // space를 건너 뛴다.
	

	//f Output Frequency 
	strncpy(argv,str,3); argv[3]='\0';
	if (*argv == NULL)return	false;
	str= (str+3+1);  // space를 건너 뛴다.
	output_voltage=  atof(argv); 
	ups_info.output_frequency=output_voltage;
	ups_info.output_frequency=output_voltage;

	//g Battery Voltage 
	strncpy(argv,str,3); argv[3]='\0';
	if (*argv == NULL)return	false;

	//(str) = (str+3+1);  // space를 건너 뛴다.
	//(str) = (str+3+1);  // space를 건너 뛴다.
	(str) = (str+3+1);  // space를 건너 뛴다.
	//(str) = (str+3+1);  // space를 건너 뛴다.
	output_voltage=  atof(argv); 
	ups_info.charging_voltage=output_voltage;

	//h Power Rating 
	strncpy(argv,str,14); argv[14]='\0';
	if (*argv == NULL)return	false;
	for( i=0;argv[i] != NULL ;i++){
		if(argv[i] >= 0x40 || argv[i]<0x30){ argv[i]=0x00;break;};
	}	
	output_voltage=  atof(argv); 
	ups_info.capacity = output_voltage;
	upsModeBusData.Ups_Capacitor= ups_info.capacity;
	//str= (str+14+2);  // space를 건너 뛴다.

	return true;

}
bool upsInformationCommand_F_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	int receivedCount = megatec_command_CR("F\r",2,'#',500,buffer); 
	
	if(receivedCount > 0) 
	{
		parse_F_megatec(buffer+1);
	}
	free(buffer);
	if(receivedCount > 0) return true;
	else return false;
}

Bool parse_F_megatec(char *str)
{
	char *argv[1];
	float output_voltage=0;
	float output_current=0;
	strtoreplace(str);
	// 1  rating voltage
	*argv = strtok(str, " ");  
	if (*argv == NULL)return	false;
	ups_info.out_voltage = (uint16_t) atof(argv[0]); 
	//upsModeBusData.Output_Voltage= (int) atof(argv[0]); 
	output_voltage=  atof(argv[0]); 
	// 2   rating current
	*argv = strtok(NULL, " ");  
	if (*argv == NULL)return	false;
	output_current =   atof(argv[0]);  
	upsRankedCurrent =output_current*1.027;
	//3상이면 1.732를 곱하여준다.
	if(ups_info.ups_type == 50 ) ups_info.capacity =(uint16_t) (output_voltage*output_current*1.73205/1000.0) ;
	else ups_info.capacity =(uint16_t) (output_voltage*output_current/1000.0) ;
	//upsModeBusData.Ups_Capacitor= (int) atof(argv[0]); 
	//upsModeBusData.Ups_Capacitor= (int) atof(argv[0]); 
	upsModeBusData.Ups_Capacitor= ups_info.capacity;

	// 3   battery voltage
	*argv = strtok(NULL, " ");  
	if (*argv == NULL)return	false;
	ups_info.charging_voltage = (int) atof(argv[0]);  

	// 4 frequency
	*argv = strtok(NULL, " ");  
	if (*argv == NULL)return	false;
	ups_info.output_frequency = (int)( atof(argv[0])*10);  
	//upsModeBusData.Inverter_Frequency= (int) atof(argv[0]); 

	return true;

}
void TurnOnOff_Q_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	 megatec_command_CR("Q\r",2,'#',500,buffer); 
	free(buffer);
}

void Test10Second_T_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	 megatec_command_CR("T\r",2,'#',500,buffer); 
	free(buffer);
}
void TestUntilBatteryLow_TL_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	megatec_command_CR("TL\r",3,'#',500,buffer); 
	free(buffer);
}

void CancelShutDown_C_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	megatec_command_CR("C\r",2,'#',500,buffer); 
	free(buffer);
}
void CancelTest_CT_megatec()
{
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	megatec_command_CR("CT\r",3,'#',500,buffer); 
	free(buffer);
}

Bool requestUpsStatus_megatec()
{
	Bool ret;
	if(ups_info.ups_type== 50 || ups_info.ups_type== 51)
	{
			ret = requestUpsStatus_Q1_megatec();
			if(!ret)return ret;
			ret = requestUps_G1_megatec() ;
			if(!ret)return ret;
			ret = requestUps_G2_megatec();
			if(!ret)return ret;
			ret = requestUps_G3_megatec();
			if(!ret)return ret;
	}
	else{
		ret = requestUpsStatus_Q1_megatec();
			if(!ret)return ret;
	}
	return true;
}
void checkTrap_2phase_megatec()
{
	//정전// D7: Utility line failure
	if(  upsModeBusData.Inverter_State  & BIT(7))    	 
	{											
		//계속 전송을 방지 한다.
		//if(!is_Converter_Operation_Fault_send_to_snmp)				//  정전이 됐고 SNMP가 전송 됐으면	
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,200);	
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,300);
			snmp_send_trap_ups_exp(0,6);//senaUpsInputBad
			is_Converter_Operation_Fault_send_to_snmp_b7 = true;
		}
	}
	else  // 정상적이다...그런데....정전후 복귀가 된것이라면...
	{
		if(is_Converter_Operation_Fault_send_to_snmp_b7){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,200);	
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,201);	
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,300);
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,301);
			snmp_send_trap_ups_exp(0,26);//senaUpsPowerRestored
			is_Converter_Operation_Fault_send_to_snmp_b7 = false;
		}
	}
	
	//6  	1 : Battery Low
	if(  upsModeBusData.Inverter_State  & BIT(6) 	  ) 
	{
		//계속 전송을 방지 한다.
		//if(!is_Inverter_Operation_Fault_send_to_snmp)				
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,202);
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,302);
			snmp_send_trap_ups_exp(0,3);//senaUpsBatteryLow
			snmp_send_trap_ups_exp(0,1);//senaUpsBatteryLow
			is_Converter_Operation_Fault_send_to_snmp_b6 = true;
		}
	}
	else  
	{
		if(is_Converter_Operation_Fault_send_to_snmp_b6){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,202);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,203);
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,302);
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,303);
			snmp_send_trap_ups_exp(0,2);//senaUpsOnBattery
			is_Converter_Operation_Fault_send_to_snmp_b6=false;
		}
	}
	// 5  	1 : Bypass/Boost or Buck Active
	if(  upsModeBusData.Inverter_State  & BIT(5) 	  )
	{
		//계속 전송을 방지 한다.
		//if(!is_Inverter_Operation_Fault_send_to_snmp)
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,204);
			snmp_send_trap_ups_exp(0,9);//senaUpsOnBypass
			is_Converter_Operation_Fault_send_to_snmp_b5 = true;
		}
	}
	else
	{
		if(is_Converter_Operation_Fault_send_to_snmp_b5){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,204);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,205);
			snmp_send_trap_ups_exp(0,26);//senaUpsPowerRestored
			is_Converter_Operation_Fault_send_to_snmp_b5=false;
		}
	}
	//4  	1 : UPS Failed
	if(  upsModeBusData.Inverter_State  & BIT(4) 	  )
	{
		//계속 전송을 방지 한다.
		//if(!is_Inverter_Operation_Fault_send_to_snmp)
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,206);
			snmp_send_trap_ups_exp(0,18);//senaUpsGenFault
			is_Converter_Operation_Fault_send_to_snmp_b4 = true;
		}
	}
	else
	{
		if(is_Converter_Operation_Fault_send_to_snmp_b4){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,206);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,207);
			snmp_send_trap_ups_exp(0,26);//senaUpsPowerRestored
			is_Converter_Operation_Fault_send_to_snmp_b4=false;
		}
	}
}
void checkTrap_3phase_megatec()
{

}
void  write_log_event_megatec()
{
	//로그코드는 한번 기록을 했으면 바뀌었을 경우만 기록하자.
	portTickType systemTime=getTimeLong();
	if( (before_Inverter_State			 !=  upsModeBusData.Inverter_State ) )
	{
		writeLog(systemTime,13, upsModeBusData.Inverter_State);		// 12 Converter_State
		before_Inverter_State = upsModeBusData.Inverter_State;
		snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,402);
	}
	checkTrap_2phase_megatec();
}
/*
bool command_megatec(uint8_t *send_command,uint8_t sLength,uint8_t *receive_data,uint8_t rLength)
{
	int revCount;
	int ret ;
	
	usart_reset_status(&AVR32_USART0);  // 우선 수신 버퍼를 리셋한다.
	while( usart_read_char(&AVR32_USART0, &revCount) != USART_RX_EMPTY );	//버퍼에 데이타가 있다면 비운다.

	usart_write_byteArray(&AVR32_USART0,(const char *)send_command,sLength);			// 명령어를 송신한다.
	ret=usart_get_count_data(&AVR32_USART0,receive_data,&revCount,100) ;
	if(ret ==USART_READ_TIMEOUT )	{			}
	else if(ret ==USART_FAILURE )	{			}

	if(rLength == revCount) return true;
	else return false;
}
*/

