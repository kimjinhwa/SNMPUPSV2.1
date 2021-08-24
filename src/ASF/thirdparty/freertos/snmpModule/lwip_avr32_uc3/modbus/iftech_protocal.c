/*
 * iftech_protocal.c
 *
 * Created: 2020-03-28 15:18:36
 *  Author: STELLA
 */ 

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <portmacro.h>
#include "err.h"
#include "iftech_protocal.h"
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

//extern struct ups_modbus_data upsModeBusData;
extern int udp_send_msg(char *msg,int len);
extern ups_modbus_data_t upsModeBusData;
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
extern bool is_Inverter_Operation_Fault_send_to_snmp;
extern bool is_maintanence_NFB_send_snmp; 

int16_t  before_Converter_State=0xff;
int16_t  before_Inverter_State=0xff;
int16_t  before_Converter_Operation=0xff;
int16_t  before_Inverter_Operation_Fault=0xff;

uint16_t calcCRC(uint8_t *au8Buffer, uint8_t u8length)
{
	unsigned int temp, temp2, flag;
	temp = 0xFFFF;
	for (unsigned char i = 0; i < u8length; i++)
	{
		temp = temp ^ au8Buffer[i];
		for (unsigned char j = 1; j <= 8; j++)
		{
			flag = temp & 0x0001;
			temp >>= 1;
			if (flag)
			temp ^= 0xA001;
		}
	}
	// Reverse byte order.
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;
	// the returned value is already swapped
	// crcLo byte is first & crcHi byte is last
	return temp;
}


uint8_t iCommErrorCount=0;

bool requestUpsData_22_32_33()
{
	// 3 + 56*2 + 2 + 1 : 기본 3 데이타 112  체크섬 2
	// 3 + 59*2 + 2 + 1 : 기본 3 데이타 118  체크섬 2  = 123
	//Byte buffer[RECEIVE_BUFFER_SIZE];
	//Byte *buffer;
	//isReceivingCommData = true;
	char buffer[RECEIVE_BUFFER_SIZE];
	memset(buffer,0x00,RECEIVE_BUFFER_SIZE);
	
	int revCount;
	int16_t *pData;
	buffer[0] = 0x01;  // slave ID
	//
	buffer[1] = 0x04;  // function code
	buffer[2] = 0x00;  // start addres Hihg
	buffer[3] = 0x00;  // start address low
	buffer[4] = 0x00;  // number of data highrequestUpsData_22_32_33
	//buffer[5] = 0x01;  // number of data low
	buffer[5] = 58;  // 전체데이타
	uint16_t crc = calcCRC(buffer, 6);
	Byte crc_h, crc_l;
	crc_h = crc >> 8;
	crc_l = crc & 0x00ff;
	buffer[6] = crc_h;// 0xC5;  // chcek sum high
	buffer[7] = crc_l;// 0xCD;  // check sum low
	buffer[8] = 0x00;
	
	usart_reset_status(&AVR32_USART0);  // 우선 수신 버퍼를 리셋한다.
	
	while( usart_read_char(&AVR32_USART0, &revCount) != USART_RX_EMPTY );	//버퍼에 데이타가 있다면 비운다.
	
	usart_write_byteArray(&AVR32_USART0,(const char *)buffer,8);			// 명령어를 송신한다.
	
	int receivedCount=3;
	//읽은 갯수는 receivedCount에 기록한다.
	//USART_SUCCESS USART_FAILURE USART_READ_TIMEOUT의 리턴값을 갖는다.
	int ret;
	for(int i = 0;i<3;i++){
		ret =usart_get_count_data(&AVR32_USART0,buffer,&receivedCount,500) ;
		iCommErrorCount++;
		if ( ret == USART_SUCCESS	) {
			iCommErrorCount=0;
			break;
		}
	}
	if(ret ==USART_READ_TIMEOUT ) 	{
		LWIP_DEBUGF_UDP(MODBUS_DEBUG, ("Time Out") ) ;
		return false;
	}
	else if(ret ==USART_FAILURE )	{
		LWIP_DEBUGF_UDP(MODBUS_DEBUG, ("ETC ERROR") );
		return false;
	}

	receivedCount = *(buffer+2);
	if(receivedCount == 0)
	{
		//free(buffer);
		//isReceivingCommData = false;
		if( iCommErrorCount > MAX_COMM_ERROR_COUNT  )
			memset(&upsModeBusData,NULL,sizeof(upsModeBusData));
		LWIP_DEBUGF_UDP(MODBUS_DEBUG, ("Error:Modbus Receive Data is 0 , Return ") ) ;
		iCommErrorCount++;
		return false;
	}
	int realDatacount = receivedCount ;  // check sum을 제외하고 본다.
	
	receivedCount = receivedCount+2; // 2개는 check sum이다.
	ret=usart_get_n_data(&AVR32_USART0,(buffer+3),&receivedCount,500) ;
	
	if( (calcCRC(buffer, receivedCount+3-2) >>     8 ) == buffer[receivedCount+3-2]  &&
	(calcCRC(buffer, receivedCount+3-2) & 0x00ff) == buffer[receivedCount+3-1] ){ }
	else  
	{	
		//에러 처리 부분이 빠져 있어서 계속 같은 데이타가 날라 갔다..
		// -> 다시 변경한다. 
		// 이 부분이 0 값을 날리게 한다.
		// 따라서 최대 에러값 이상일 경우만 리셋한다.
		if( iCommErrorCount > MAX_COMM_ERROR_COUNT  )
			memset(&upsModeBusData,NULL,sizeof(upsModeBusData));
		//free(buffer);
		iCommErrorCount++;
		return false;
	}
	//check sum 		//;
	pData =(int16_t *)&upsModeBusData ;//
	//pData = buffer; pData+=3; 		//LWIP_DEBUGF_UDP(MODBUS_DEBUG, ("\nreceivedCount=%d\n",receivedCount) );
	//taskENTER_CRITICAL() ;
	memset(&upsModeBusData,NULL,sizeof(upsModeBusData));
	for(int i=3; i< receivedCount ; i+=2  )
	{
		*pData =  (int16_t)( buffer[i]<<8 | buffer[i+1]) ;
		pData++;
	}
	//for(int i =0;i<receivedCount ;i++)LWIP_DEBUGF_UDP(MODBUS_DEBUG, ("%d ",buffer[i] )) ;
	//LWIP_DEBUGF_UDP(MODBUS_DEBUG, ("\r\nreceive count : %d ",receivedCount )) ; 확인 완료 항상 118개를 받는다.
	if((upsModeBusData.Input_Phase == 1) )
	{
		//단상의 Converter State(12) 는 삼상의 Input_r_volt_rms에 맵핑되어 있다.
		upsModeBusData.BMS_1_2_STATE =0;
		upsModeBusData.Inverter_State =0;
		upsModeBusData.Inverter_Operation_Fault=0;
		upsModeBusData.Year_made = ups_info.install_year;
		upsModeBusData.Month_made= ups_info.install_month;
		upsModeBusData.Date_made= ups_info.install_day;
		upsModeBusData.Input_s_volt_rms=0;
		upsModeBusData.Input_t_volt_rms=0;
		upsModeBusData.Input_s_current_rms=0;
		upsModeBusData.Input_t_current_rms=0;
		upsModeBusData.Bypass_s_volt_rms=0;
		upsModeBusData.Bypass_t_volt_rms=0;
		upsModeBusData.Bypass_s_current_rms=0;
		upsModeBusData.Bypass_t_current_rms=0;
	}	
	if((upsModeBusData.Output_Phase == 1) )
	{
		upsModeBusData.Inverter_v_volt_rms=0;
		upsModeBusData.Inverter_w_volt_rms=0;
		upsModeBusData.Inverter_V_curr_rms=0;
		upsModeBusData.Inverter_W_curr_rms=0;
		upsModeBusData.Output_s_volt_rms=0;
		upsModeBusData.Output_t_volt_rms=0;
		upsModeBusData.Output_v_current_rms=0;
		upsModeBusData.Output_w_current_rms=0;
		upsModeBusData.Output_S_Load=0;
		upsModeBusData.Output_T_Load=0;
	}
	//taskEXIT_CRITICAL();	// 여기 데이터 처리까지는 방해 받아서는 안된다.
	//free(buffer);
	//taskEXIT_CRITICAL();
	//isReceivingCommData = false;
	if(receivedCount == 0){
		iCommErrorCount++;
		 return false;
	};
	return true;
}

void checkTrap_2phase_iftech_22()
{

	if(  !(upsModeBusData.Converter_State  & BIT(11)) ) // D11: Utility line failure
	{											
		//계속 전송을 방지 한다.
		//if(!is_Converter_Operation_Fault_send_to_snmp)				//  정전이 됐고 SNMP가 전송 됐으면	
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,200);	
			is_Converter_Operation_Fault_send_to_snmp = true;

		}
	}
	else  // 정상적이다...그런데....정전후 복귀가 된것이라면...
	{
		if(is_Converter_Operation_Fault_send_to_snmp){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,200);	
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,201);	
			is_Converter_Operation_Fault_send_to_snmp = false;
		}
	}
	
	//정전 또는 입력 전원이상
	if(  (upsModeBusData.Converter_Operation_Fault  & BIT(3))  || 
		 (upsModeBusData.Converter_Operation_Fault  & BIT(4))  ||
		 (upsModeBusData.Converter_Operation_Fault  & BIT(5))  ||
		 (upsModeBusData.Converter_Operation_Fault  & BIT(7))  
	  ) 
	{
		//계속 전송을 방지 한다.
		//if(!is_Inverter_Operation_Fault_send_to_snmp)				
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,202);
			is_Inverter_Operation_Fault_send_to_snmp = true;
		}
	}
	else  
	{
		if(is_Inverter_Operation_Fault_send_to_snmp){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,202);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,203);
			is_Inverter_Operation_Fault_send_to_snmp=false;
		}
	}
	//차단기 켜짐 
	/*
	if(  (upsModeBusData.Inverter_Operation_Fault  & BIT(10))  )
	{
		if(!is_maintanence_NFB_send_snmp)
		{
			snmp_send_trap_ups_kepcos(SNMP_GENTRAP_ENTERPRISESPC,224);
			is_maintanence_NFB_send_snmp = true;
		}
	}
	else
	{
		if(is_maintanence_NFB_send_snmp){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,224);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,225);
			is_maintanence_NFB_send_snmp=false;
		}
	}
	*/
	/*
	Pawor Fail 또는 	Bypass
	Enterprises.Devices.12082.200 	200 - 장애 	201 - 정상
	정전 또는 	입력 전원이상 	Enterprises.Devices.12082.202 	202 - 장애 	203 - 정상
	Module #1 장애  	Enterprises.Devices.12082.212	210 - 장애	211 - 정상
	Module #2 장애	Enterprises.Devices.12082.214	212 - 장애	213 - 정상
	Module #3 장애	Enterprises.Devices.12082.216	214 - 장애	215 - 정상
	Module #4 장애	Enterprises.Devices.12082.218	216 - 장애	217 - 정상
	차단기 켜짐	Enterprises.Devices.12082.224	224 - 장애	225 - 정상
	*/	
}
void checkTrap_3phase_iftech_32_33()
{
	//정전 또는 입력 전원이상
	if(  (upsModeBusData.Converter_Operation_Fault  & 0x2EE0)  ) 
	{
		//if(!is_Inverter_Operation_Fault_send_to_snmp)				
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,202);
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,200);
			is_Inverter_Operation_Fault_send_to_snmp = true;
		}
	}
	else  
	{
		if(is_Inverter_Operation_Fault_send_to_snmp){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,202);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,203);
			is_Inverter_Operation_Fault_send_to_snmp=false;
		}
	}
	//차단기 켜짐 
	if(  (upsModeBusData.Inverter_Operation_Fault  & BIT(10))  )
	{
		//if(!is_maintanence_NFB_send_snmp)
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{                      
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,224);
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,202);
			is_maintanence_NFB_send_snmp = true;
		}
	}
	else
	{
		if(is_maintanence_NFB_send_snmp){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,224);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,225);
			is_maintanence_NFB_send_snmp=false;
		}
	}
	//  정전상태이면 파워모듈 fail혹은 bypass모드는 검사하지 않는다.
	if( is_Inverter_Operation_Fault_send_to_snmp ) return ;
	
	if(  !(upsModeBusData.Converter_State  & BIT(12)) ||
	!(upsModeBusData.Converter_State  & BIT(13)) ||
	!(upsModeBusData.Converter_State  & BIT(14))
	) // D11: Utility line failure
	{
		//if(!is_Converter_Operation_Fault_send_to_snmp)				//  정전이 됐고 SNMP가 전송 됐으면
		if(snmp_get_everyMinute()>60*100)  // 10ms마다 증가 하므로 100이면 1초
		{
			snmp_set_everyMinute(0);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,200);
			snmp_send_trap_ups(SNMP_GENTRAP_ENTERPRISESPC,200);
			is_Converter_Operation_Fault_send_to_snmp = true;
		}
	}
	else  // 정상적이다...그런데....정전후 복귀가 된것이라면...
	{
		if(is_Converter_Operation_Fault_send_to_snmp){
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,200);
			snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,201);
			is_Converter_Operation_Fault_send_to_snmp = false;
		}
	}


	/*
	Pawor Fail 또는 	Bypass
	Enterprises.Devices.12082.200 	200 - 장애 	201 - 정상
	정전 또는 	입력 전원이상 	Enterprises.Devices.12082.202 	202 - 장애 	203 - 정상
	Module #1 장애  	Enterprises.Devices.12082.212	210 - 장애	211 - 정상
	Module #2 장애	Enterprises.Devices.12082.214	212 - 장애	213 - 정상
	Module #3 장애	Enterprises.Devices.12082.216	214 - 장애	215 - 정상
	Module #4 장애	Enterprises.Devices.12082.218	216 - 장애	217 - 정상
	차단기 켜짐	Enterprises.Devices.12082.224	224 - 장애	225 - 정상
	*/	
}
void  write_log_event_iftech_22_32_33()
{
	//로그코드는 한번 기록을 했으면 바뀌었을 경우만 기록하자.
	//상태 코드가 바뀌면 트렙을 전송한다.
	// 이부분은 없애는 게 좋겠다.
	portTickType systemTime=getTimeLong();
	if( (before_Converter_State			 !=  upsModeBusData.Converter_State ) )
	{
		writeLog(systemTime,12, upsModeBusData.Converter_State);		// 12 Converter_State
		before_Converter_State = upsModeBusData.Converter_State;
		//snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,400);
	}
	if( (before_Inverter_State			 !=  upsModeBusData.Inverter_State ) )
	{
		writeLog(systemTime,13, upsModeBusData.Inverter_State);		// 12 Converter_State
		before_Inverter_State = upsModeBusData.Inverter_State;
		//snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,402);
	}
	if( (before_Converter_Operation			 !=  upsModeBusData.Converter_Operation_Fault ) )
	{
		writeLog(systemTime,14, upsModeBusData.Converter_Operation_Fault);		// 12 Converter_State
		before_Converter_Operation = upsModeBusData.Converter_Operation_Fault;
		//snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,404);
	}
	if( (before_Inverter_Operation_Fault			 !=  upsModeBusData.Inverter_Operation_Fault ) )
	{
		writeLog(systemTime,15, upsModeBusData.Inverter_Operation_Fault);		// 12 Converter_State
		before_Inverter_Operation_Fault = upsModeBusData.Inverter_Operation_Fault;
		//snmp_send_trap_ups_kepco(SNMP_GENTRAP_ENTERPRISESPC,406);
	}

	//Pawor Module Fail 또는 	Bypass Mode
	if( (upsModeBusData.Input_Phase == 1) && (upsModeBusData.Output_Phase == 1) )
		checkTrap_2phase_iftech_22();
	else
		checkTrap_3phase_iftech_32_33();

}
