#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sys.h"

#include "print_funcs.h"
#include "flashc.h"
#include "FlashMemory.h"
#include "conf_eth.h"
#include "lwip/ip_addr.h"

//__attribute__((__section__(".flash_nvram"))) 
//static nvram_log_tg nvrma_log_garbage[2] ;

__attribute__((__section__(".flash_nvram")))
static nvram_log_pos_t	nvram_log_pos;       /*2 byte */ 

__attribute__((__section__(".flash_nvram")))
static nvram_log_t nvrma_log_0[MAX_LOG] ;   /* 7byte * 200  = 1400 byte  총 3072 - 1400 -2  */

__attribute__((__section__(".flash_nvram")))
static smtp_info_t nvram_smtp_info;  /*  67 byte 총 3072 - 1400 -2 - 67 = 1603 free*/

__attribute__((__section__(".flash_nvram")))
static nvram_send_event_mail_t nvram_send_event_mail[10] ; /* 30*10 = 300byte..  1603 - 300 = 1303byte free */
	
__attribute__((__section__(".flash_nvram")))
static ups_info_t nvram_ups_info ; /*  1303 - 130  = 1003 byte free */
	
__attribute__((__section__(".flash_nvram")))  static const uint8_t nvram_reloadTime[1] ; /*  1303 - 131  = 1002 byte free */

//__attribute__((__section__(".flash_nvram")))
//static nvram_log_t nvrma_log_1[MAX_LOG] ;

//__attribute__((__section__(".flash_nvram")))
//static nvram_data_t flash_nvram_data;

__attribute__((__section__(".userpage")))
static nvram_data_ethernet_t nvram_data_ethernet;  /* 20 byte */

__attribute__((__section__(".userpage")))
static nvram_trap_address_t nvram_time_server_address[2] ;   /* 4byte * 2 = 8byte */

__attribute__((__section__(".userpage")))
static nvram_trap_address_t nvram_trap_address[10] ;   /*4byte * 10 = 40 byte*/  


/*  total userpage = 20 + 8 + 40 = 68 byte   200 -68 = 132byte free*/

uint8_t getLogPosHead(nvram_log_pos_t *nvram_data) ;
uint8_t getLogPoslast_log_position(nvram_log_pos_t *nvram_data) ;

/*! \brief This is an example demonstrating flash read / write data accesses
 *         using the FLASHC driver.
 *
 * \param caption     Caption to print before running the example.
 * \param nvram_data  Pointer to the NVRAM data structure to use in the example.
 */


extern ups_info_t ups_info;

uint8_t flash_write_reLoadTime(uint8_t *reloadTime){
	flashc_memset8((void *)&(nvram_reloadTime), *reloadTime, sizeof(uint8_t), true);
	uint8_t reloadTime_[1];
	flashc_memcpy( reloadTime_,(void *)&(nvram_reloadTime),  sizeof(nvram_reloadTime),false);
	return reloadTime_[0];
}

uint8_t flash_read_reLoadTime(){
	uint8_t reloadTime_[1];
	flashc_memcpy( reloadTime_,(void *)&(nvram_reloadTime),  sizeof(nvram_reloadTime),false);
	return reloadTime_[0];
}
#define UPS_COMP_N_VER	"IFTECH UPS V2.0.2"
void flash_read__ethernetInfo(data_ethernet_t *ethernet_t)
{
	nvram_data_ethernet_t *nvram_data ;
	nvram_data = &nvram_data_ethernet;
	
	if(  nvram_data->Ethernet_Conf_EthAddr0 == 0xff ||  nvram_data->Ethernet_Conf_EthAddr0 == 0x00 )
	{
		//{0x59, 0x56, 0x64, 0x55, 0x53, 0x15};
		ethernet_t->Ethernet_Conf_EthAddr0 = ETHERNET_CONF_ETHADDR0;
		ethernet_t->Ethernet_Conf_EthAddr1 = ETHERNET_CONF_ETHADDR1;
		ethernet_t->Ethernet_Conf_EthAddr2 = ETHERNET_CONF_ETHADDR2;
		ethernet_t->Ethernet_Conf_EthAddr3 = ETHERNET_CONF_ETHADDR3;
		ethernet_t->Ethernet_Conf_EthAddr4 = ETHERNET_CONF_ETHADDR4;
		ethernet_t->Ethernet_Conf_EthAddr5 = ETHERNET_CONF_ETHADDR5;
		
		ethernet_t->Ethernet_Conf_IpAddr0 = ETHERNET_CONF_IPADDR0;
		ethernet_t->Ethernet_Conf_IpAddr1 = ETHERNET_CONF_IPADDR1;
		ethernet_t->Ethernet_Conf_IpAddr2 = ETHERNET_CONF_IPADDR2;
		ethernet_t->Ethernet_Conf_IpAddr3 = ETHERNET_CONF_IPADDR3;
		
		ethernet_t->Ethernet_Conf_Gateway_Addr0 = ETHERNET_CONF_GATEWAY_ADDR0;
		ethernet_t->Ethernet_Conf_Gateway_Addr1 = ETHERNET_CONF_GATEWAY_ADDR1;
		ethernet_t->Ethernet_Conf_Gateway_Addr2 = ETHERNET_CONF_GATEWAY_ADDR2;
		ethernet_t->Ethernet_Conf_Gateway_Addr3 = ETHERNET_CONF_GATEWAY_ADDR3;
		
		ethernet_t->Ethernet_Conf_Net_Mask0 = ETHERNET_CONF_NET_MASK0;
		ethernet_t->Ethernet_Conf_Net_Mask1 = ETHERNET_CONF_NET_MASK1;
		ethernet_t->Ethernet_Conf_Net_Mask2 = ETHERNET_CONF_NET_MASK2;
		ethernet_t->Ethernet_Conf_Net_Mask3 = ETHERNET_CONF_NET_MASK3;
		
		ethernet_t->port = webHTTP_PORT;

		flash_write__ethernetInfo(ethernet_t);

		trap_address_t addr={192,168,0,252};
		trapIpAddressWrite(0,&addr);

		smtp_info_t smtp_info;
		sprintf(smtp_info.e_mail_server,"%s",(char *)"infinitetech.iptime.org");
		sprintf(smtp_info.email_user_id,"%s",(char *)"mvcjhkim@gmail.com");
		sprintf(smtp_info.email_user_passwd,"%s",(char *)"jhkim1260");
		sprintf(smtp_info.sender_mail_id,"%s",(char *)"mvcjhkim@gmail.com");
		smtp_info.is_need_authentic=1;
		smtp_info.is_send_mail_Event=1;
		flash_write_smtp_info(&smtp_info);
		
		nvram_send_event_mail_t emailAddress;
		sprintf(emailAddress.email,"%s","mvcjhkim@gmail.com");
		emailAddress.cmd = 0x01;
		for(int i=0;i<10;i++)
		flash_write_email_list(i,&emailAddress);
		
		//ups_info_t ups_info;
		memset((char *)&ups_info,0x00,sizeof(ups_info_t));
		memcpy(ups_info.upsIdentManufacturer,UPS_COMP_N_VER	,sizeof(UPS_COMP_N_VER	)) ;
		memcpy(ups_info.upsIdentModel,"MODEL IF-M00K ",sizeof("MODEL IF-M00K ")) ;
		memcpy(ups_info.upsIdentUPSSoftwareVersion,"V1.0.0",sizeof("V1.0.0")) ;//  max length is 11
		memcpy(ups_info.batLastReplaceDate,"01/01/21",sizeof("01/01/21")) ;//  max length is 8  

		//Software , H :support Hanggang , 
		//2.2.13 : - bug fix for battery voltage while communication .
		//         - .iso.org.dod.internet.mgmt.mib-2.upsMIB.upsObjects.upsIdent.upsIdentAgentSoftwareVersion.0 
		memcpy(ups_info.upsIdentAgentSoftwareVersion,"SH2.2.13",sizeof("SH2.2.13")) ;//  max length is 11
		memcpy(ups_info.upsIdentName,"0P0P NTC",sizeof("3P3P NTC")) ;
		memcpy(ups_info.upsIdentAttachedDevices,"Communication device",sizeof("Communication device")) ;

		memcpy( ups_info.sysname,"UPS IF3P3801P220AA",sizeof("UPS IF3P3801P220AA")) ;
		

		ups_info.ups_type = UPS_TYPE;   // 31, 32, 11,


		ups_info.company_code=00;
		ups_info.capacity=30;
		ups_info.input_voltage=380;
		ups_info.out_voltage=380;
		ups_info.installed_battery=32;
		ups_info.ups_version=121;   // 121 -> 1.21
		ups_info.install_year=20;  // 2018 -> 18
		ups_info.install_month=2;
		ups_info.install_day = 10;
		ups_info.protocal = 0x00;
		ups_info.bat_install_year=20;
		ups_info.bat_install_month=01;
		ups_info.bat_install_day=01;
		ups_info.charging_voltage=220;
		ups_info.limit_load=100;
		ups_info.limit_temp=85;
		ups_info.limit_capacity=100;
		ups_info.ups_test_day=0;
		ups_info.ups_test_hour=0;
		ups_info.ups_test_minute=0;
		ups_info.ups_test_period=0;
		ups_info.input_frequency=60;
		ups_info.output_frequency=60;
		memcpy( ups_info.user_id,"admin",5) ;
		memcpy( ups_info.passwd,"admin",5) ;

		flash_write_ups_info(&ups_info);
		flash_read_ups_info(&ups_info);
		uint8_t reLoadTime = 10;
		flash_write_reLoadTime( &reLoadTime);
	}
	//버전관리를 위해 이부분은 새로 써 넣는다.
	memcpy(ups_info.upsIdentManufacturer,UPS_COMP_N_VER	,sizeof(UPS_COMP_N_VER	)) ;
	memcpy(ups_info.upsIdentUPSSoftwareVersion,"V2.2.2",sizeof("V1.0.0")) ;//  max length is 11
	flash_write_ups_info(&ups_info);
		
	ethernet_t->Ethernet_Conf_EthAddr0 = nvram_data->Ethernet_Conf_EthAddr0;
	ethernet_t->Ethernet_Conf_EthAddr1 = nvram_data->Ethernet_Conf_EthAddr1;
	ethernet_t->Ethernet_Conf_EthAddr2 = nvram_data->Ethernet_Conf_EthAddr2;
	ethernet_t->Ethernet_Conf_EthAddr3 = nvram_data->Ethernet_Conf_EthAddr3;
	ethernet_t->Ethernet_Conf_EthAddr4 = nvram_data->Ethernet_Conf_EthAddr4;
	ethernet_t->Ethernet_Conf_EthAddr5 = nvram_data->Ethernet_Conf_EthAddr5;
	
	ethernet_t->Ethernet_Conf_IpAddr0 = nvram_data->Ethernet_Conf_IpAddr0;
	ethernet_t->Ethernet_Conf_IpAddr1 = nvram_data->Ethernet_Conf_IpAddr1;
	ethernet_t->Ethernet_Conf_IpAddr2 = nvram_data->Ethernet_Conf_IpAddr2;
	ethernet_t->Ethernet_Conf_IpAddr3 = nvram_data->Ethernet_Conf_IpAddr3;
	
	ethernet_t->Ethernet_Conf_Gateway_Addr0 = nvram_data->Ethernet_Conf_Gateway_Addr0;
	ethernet_t->Ethernet_Conf_Gateway_Addr1 = nvram_data->Ethernet_Conf_Gateway_Addr1;
	ethernet_t->Ethernet_Conf_Gateway_Addr2 = nvram_data->Ethernet_Conf_Gateway_Addr2;
	ethernet_t->Ethernet_Conf_Gateway_Addr3 = nvram_data->Ethernet_Conf_Gateway_Addr3;
	
	ethernet_t->Ethernet_Conf_Net_Mask0 = nvram_data->Ethernet_Conf_Net_Mask0;
	ethernet_t->Ethernet_Conf_Net_Mask1 = nvram_data->Ethernet_Conf_Net_Mask1;
	ethernet_t->Ethernet_Conf_Net_Mask2 = nvram_data->Ethernet_Conf_Net_Mask2;
	ethernet_t->Ethernet_Conf_Net_Mask3 = nvram_data->Ethernet_Conf_Net_Mask3;
	
	ethernet_t->port	 = nvram_data->port;
	
	//return 0;
}

void flash_write_ups_info(ups_info_t *upsinfo)
{
	flashc_memcpy((void *)&(nvram_ups_info) , upsinfo , sizeof(ups_info_t),   true);
}
void flash_read_ups_info(ups_info_t *upsinfo)
{
	//memcpy( ups_info,(void *)&(nvram_ups_info),  sizeof(ups_info_t));

	portENTER_CRITICAL();
	flashc_memcpy( upsinfo,(ups_info_t *)&(nvram_ups_info),  sizeof(ups_info_t),false);
	portEXIT_CRITICAL();
}

void flash_write_smtp_info(smtp_info_t *smtp_info)
{
	smtp_info_t *nv_smtp_info;
	nv_smtp_info= &nvram_smtp_info ;

	flashc_memcpy((void *)&(nvram_smtp_info) , smtp_info , sizeof(smtp_info_t),   true);
}
void flash_read_smtp_info(smtp_info_t *smtp_info)
{
	smtp_info_t *nv_smtp_info;
	nv_smtp_info = &nvram_smtp_info ;
	flashc_memcpy( smtp_info , &nvram_smtp_info,  sizeof(smtp_info_t),false);

}

//void flash_write_email_list(int pos, const uint8_t *emailAddress)
void flash_write_email_list(int pos,nvram_send_event_mail_t *emailAddress)
{
	nvram_send_event_mail_t *nvram_emailAddress ;
	nvram_emailAddress = &(nvram_send_event_mail[pos]);
	//flashc_memcpy((void *)&(nvram_emailAddress->email) , emailAddress , 30*sizeof(uint8_t),   true);
	flashc_memcpy((void *)&(nvram_emailAddress->email) , emailAddress , sizeof(nvram_send_event_mail_t),   true);
}
void flash_read_email_list(int pos, nvram_send_event_mail_t *emailAddress)
{
	 nvram_send_event_mail_t *nvram_emailAddress ;
	 nvram_emailAddress = &(nvram_send_event_mail[pos]);
	 flashc_memcpy( emailAddress , nvram_emailAddress,  sizeof(nvram_send_event_mail_t),false);
}

void flash_write__ethernetInfo(data_ethernet_t *ethernet_t)
{
	nvram_data_ethernet_t *nvram_data ;
	nvram_data = &nvram_data_ethernet;
	flashc_memset((void *)nvram_data, 0x00, 8, sizeof(nvram_data_ethernet_t), true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_EthAddr0,   &ethernet_t->Ethernet_Conf_EthAddr0, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_EthAddr1,   &ethernet_t->Ethernet_Conf_EthAddr1, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_EthAddr2,   &ethernet_t->Ethernet_Conf_EthAddr2, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_EthAddr3,   &ethernet_t->Ethernet_Conf_EthAddr3, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_EthAddr4,   &ethernet_t->Ethernet_Conf_EthAddr4, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_EthAddr5,   &ethernet_t->Ethernet_Conf_EthAddr5, sizeof(uint8_t),   true);
	
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr0,   &ethernet_t->Ethernet_Conf_IpAddr0, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr1,   &ethernet_t->Ethernet_Conf_IpAddr1, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr2,   &ethernet_t->Ethernet_Conf_IpAddr2, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr3,   &ethernet_t->Ethernet_Conf_IpAddr3, sizeof(uint8_t),   true);
	
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Gateway_Addr0,   &ethernet_t->Ethernet_Conf_Gateway_Addr0, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Gateway_Addr1,   &ethernet_t->Ethernet_Conf_Gateway_Addr1, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Gateway_Addr2,   &ethernet_t->Ethernet_Conf_Gateway_Addr2, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Gateway_Addr3,   &ethernet_t->Ethernet_Conf_Gateway_Addr3, sizeof(uint8_t),   true);

	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Net_Mask0,   &ethernet_t->Ethernet_Conf_Net_Mask0, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Net_Mask1,   &ethernet_t->Ethernet_Conf_Net_Mask1, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Net_Mask2,   &ethernet_t->Ethernet_Conf_Net_Mask2, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_Net_Mask3,   &ethernet_t->Ethernet_Conf_Net_Mask3, sizeof(uint8_t),   true);

	flashc_memcpy((void *)&nvram_data->port,   &ethernet_t->port, sizeof(uint8_t),   true);
}

void TimeServerIpAddressRead(uint8_t pos,timerserver_address_t *timeServer_address)
{
	timerserver_address_t *nvram_data ;
	nvram_data = &nvram_time_server_address[pos];
	if( nvram_data->Ethernet_Conf_IpAddr0 == 0xff  )
	{
		timeServer_address->Ethernet_Conf_IpAddr0 = (uint8_t)61;
		timeServer_address->Ethernet_Conf_IpAddr1 = (uint8_t)100;
		timeServer_address->Ethernet_Conf_IpAddr2 = (uint8_t)12;
		timeServer_address->Ethernet_Conf_IpAddr3 = (uint8_t)187;
		TimeServerIpAddressWrite(0,timeServer_address);
		TimeServerIpAddressWrite(1,timeServer_address);
	}

	timeServer_address->Ethernet_Conf_IpAddr0 = nvram_data->Ethernet_Conf_IpAddr0;
	timeServer_address->Ethernet_Conf_IpAddr1 = nvram_data->Ethernet_Conf_IpAddr1;
	timeServer_address->Ethernet_Conf_IpAddr2 = nvram_data->Ethernet_Conf_IpAddr2;
	timeServer_address->Ethernet_Conf_IpAddr3 = nvram_data->Ethernet_Conf_IpAddr3;
}

void TimeServerIpAddressWrite(uint8_t pos,timerserver_address_t *trap_address)
{
	flashc_memcpy( (void*)&nvram_time_server_address[pos].Ethernet_Conf_IpAddr0,&trap_address->Ethernet_Conf_IpAddr0, sizeof(uint8_t),   true);
	flashc_memcpy( (void*)&nvram_time_server_address[pos].Ethernet_Conf_IpAddr1,&trap_address->Ethernet_Conf_IpAddr1, sizeof(uint8_t),   true);
	flashc_memcpy( (void*)&nvram_time_server_address[pos].Ethernet_Conf_IpAddr2,&trap_address->Ethernet_Conf_IpAddr2, sizeof(uint8_t),   true);
	flashc_memcpy( (void*)&nvram_time_server_address[pos].Ethernet_Conf_IpAddr3,&trap_address->Ethernet_Conf_IpAddr3, sizeof(uint8_t),   true);
}

void trapIpAddressRead(uint8_t pos,trap_address_t *trap_address)
{
	nvram_trap_address_tt *nvram_data ;
	nvram_data = &nvram_trap_address[pos];
	
	if(nvram_data->Ethernet_Conf_IpAddr0 == 0xff)
	{
		//default trap address;
		trap_address->Ethernet_Conf_IpAddr0 = 192;
		trap_address->Ethernet_Conf_IpAddr1 = 168;
		trap_address->Ethernet_Conf_IpAddr2 = 0;
		for(int i=0;i<10;i++)
		{
			trap_address->Ethernet_Conf_IpAddr3 = 244+i;
			trapIpAddressWrite(i,trap_address);
		}
		//trapIpAddressWrite(pos,trap_address);
	}
	
	nvram_data = &nvram_trap_address[pos];
	
	trap_address->Ethernet_Conf_IpAddr0 = nvram_data->Ethernet_Conf_IpAddr0;
	trap_address->Ethernet_Conf_IpAddr1 = nvram_data->Ethernet_Conf_IpAddr1;
	trap_address->Ethernet_Conf_IpAddr2 = nvram_data->Ethernet_Conf_IpAddr2;
	trap_address->Ethernet_Conf_IpAddr3 = nvram_data->Ethernet_Conf_IpAddr3;
	
}


void trapIpAddressWrite(uint8_t pos,trap_address_t *trap_address)
{
	nvram_trap_address_t *nvram_data ;
	nvram_data = &nvram_trap_address[pos];
		
	flashc_memset((void *)&nvram_data,0x00 , 8, sizeof(nvram_trap_address_t), true);
	
	//trap_address->Ethernet_Conf_IpAddr0 = 0x01;
	//trap_address->Ethernet_Conf_IpAddr1 = 0x02;
	//trap_address->Ethernet_Conf_IpAddr2 = 0x03;
	//trap_address->Ethernet_Conf_IpAddr3 = 0x04;
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr0,  &trap_address->Ethernet_Conf_IpAddr0, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr1,  &trap_address->Ethernet_Conf_IpAddr1, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr2,  &trap_address->Ethernet_Conf_IpAddr2, sizeof(uint8_t),   true);
	flashc_memcpy((void *)&nvram_data->Ethernet_Conf_IpAddr3,  &trap_address->Ethernet_Conf_IpAddr3, sizeof(uint8_t),   true);
}

void getLogPosPointer(log_pos_t *log_pos)
{
	log_pos->head = getLogPosHead(&nvram_log_pos);
	log_pos->last_log_position = getLogPoslast_log_position(&nvram_log_pos);
}

void writeLog(uint32_t time, uint8_t kind, uint16_t evt)
{
	uint8_t last_log_position, iLogWritePos;
	log_pos_t log_pos_data;
	if(getLogPosHead(&nvram_log_pos) == 0xff || getLogPoslast_log_position(&nvram_log_pos) == 0xff )
	{
		flashc_memset((void *)&nvrma_log_0      ,0x00 , 8, sizeof(nvram_log_t)	, true);
		flashc_memset((void *)&nvram_log_pos      ,0x00 , 8, sizeof(log_pos_t), true);
	}
	
	last_log_position = getLogPoslast_log_position(&nvram_log_pos) ;//  getLogPoslast_log_position(&nvram_log_pos);
	last_log_position = getLogPoslast_log_position(&nvram_log_pos);
	if(last_log_position < MAX_LOG) 	iLogWritePos = last_log_position ;
	else								iLogWritePos = getLogPosHead(&nvram_log_pos)-1;
	if(iLogWritePos>=MAX_LOG)iLogWritePos=0;
	
	//nvram_log_t  *nvram;
	log_t log_data;
	log_data.systemTime=  time;
	log_data.kind=  kind;
	log_data.event=  evt;
	
	//nvram = &nvrma_log[iLogWritePos] ;  // head+last_log_position 이번지의 값을 가져온다.
	flashc_memset((void *)&nvrma_log_0[iLogWritePos],0x00 , 8, sizeof(nvram_log_t), true);
	flashc_memcpy((void *)&nvrma_log_0[iLogWritePos],  &log_data, sizeof(log_t),   true);
	last_log_position++;
	//log_pos_data.head = (void *)&nvram_log_pos.head;
	log_pos_data.head = getLogPosHead(&nvram_log_pos);
	log_pos_data.last_log_position = getLogPoslast_log_position(&nvram_log_pos);
	//flashc_memcpy((void *)&log_pos_data,  &nvram_log_pos, sizeof(log_pos_data),   true);
	if(last_log_position >= MAX_LOG )  //최대로그를 채웠을 경우만 iHead를 증가 시킨다.
	{
		log_pos_data.head++;
		last_log_position=MAX_LOG;
	}
	log_pos_data.last_log_position	 = last_log_position;
	flashc_memcpy((void *)&nvram_log_pos,  &log_pos_data, sizeof(log_pos_t),   true);
}

int get_nvram_getlogposition(uint16_t pos,log_t *tlog)
{
	tlog->event = nvrma_log_0[pos].event ;
	tlog->kind = nvrma_log_0[pos].kind ;
	tlog->systemTime = nvrma_log_0[pos].systemTime ;
	
	//tlog->event = nvrma_log_1[pos].event ;
	//tlog->kind = nvrma_log_1[pos].kind ;
	//tlog->systemTime = nvrma_log_1[pos].systemTime ;
	return pos;
	//if( tlog->kind != 0xFF) return pos;
	//else return -1;
}


uint8_t getLogCount(nvram_log_pos_t *nvram_data)
{
	//uint8_t ret= getLogPoslast_log_position(&nvram_log_pos);
	uint8_t ret= nvram_data->last_log_position;
	return ret;
}
uint8_t getLogPosHead(nvram_log_pos_t *nvram_data)
{
	uint8_t ret = 0;
	//nvram_data  = &nvram_log_pos;
	ret = nvram_data->head ;
	return ret;
}


uint8_t getLogPoslast_log_position(nvram_log_pos_t *nvram_data)
{
	uint8_t ret = 0;
	//nvram_data =
	//nvram_data  = &nvram_log_pos;
	ret = nvram_data->last_log_position ;
	return ret ;
}

/*
void initial_flashMemory()
{
	uint8_t t1,t2;
	t1 =0;t2=0;
	
	if(getLogPosHead(&nvram_log_pos) == 0xff || getLogPoslast_log_position(&nvram_log_pos) == 0xff )
	{
		//print_nvram_logpos(&nvram_log_pos);
		flashc_memset((void *)&nvram_log_pos,0x00 , 8, sizeof(nvram_log_pos), true);
		//print_nvram_logpos(&nvram_log_pos);
		flashc_memcpy((void *)&nvram_log_pos.head,  &t1, sizeof(uint8_t),   true);
		flashc_memcpy((void *)&nvram_log_pos.last_log_position,  &t2, sizeof(uint8_t),   true);
		//print_nvram_logpos(&nvram_log_pos);
		//print_nvram_logpos(&nvram_log_pos);
	}
	
}
*/
uint8_t getFirstLog(log_t *tlog)
{
	
	uint8_t iHead,ilast_log_position;

	//iHead =	getLogPosHead(&nvram_log_pos);
	//ilast_log_position = getLogPoslast_log_position(&nvram_log_pos);
	iHead = getLogPosHead(&nvram_log_pos);
	
	get_nvram_getlogposition(iHead ,tlog);
	
	tlog->event = nvrma_log_0[iHead].event ;
	tlog->kind = nvrma_log_0[iHead].kind ;
	tlog->systemTime = nvrma_log_0[iHead].systemTime ;

	//tlog->event = nvrma_log_1[iHead].event ;
	//tlog->kind = nvrma_log_1[iHead].kind ;
	//tlog->systemTime = nvrma_log_1[iHead].systemTime ;

	return iHead;
}

//값이 없다면 -1을 리턴한다.
int8_t getNextLog(int iPosition,log_t *tlog)
{
	//iPosition = iPosition % MAX_LOG ;

	tlog->event = nvrma_log_0[iPosition].event ;
	tlog->kind = nvrma_log_0[iPosition].kind ;
	tlog->systemTime = nvrma_log_0[iPosition].systemTime ;
	
	//tlog->event = nvrma_log_1[iPosition].event ;
	//tlog->kind = nvrma_log_1[iPosition].kind ;
	//tlog->systemTime = nvrma_log_1[iPosition].systemTime ;
	//return get_nvram_getlogposition(iPosition,&nvrma_log[iPosition] ,tlog);
}



/*
void getAllLog()
{
	log_t	 tlog;
	uint8_t ilast_log_position;
	ilast_log_position = getFirstLog(&tlog) ;
	for(int i = ilast_log_position ; i < MAX_LOG ; i++)
	{
		if(getNextLog(i,&tlog) == -1) break;
		print_dbg("logPos:\t ");
		print_dbg_char_hex(getLogPosHead(&nvram_log_pos));
		print_dbg(" ");
		print_dbg_char_hex(getLogPoslast_log_position(&nvram_log_pos));
		print_dbg(" ");
		print_dbg_char_hex(tlog.systemTime );print_dbg(" ");
		print_dbg_char_hex(tlog.kind );print_dbg(" ");
		print_dbg_char_hex(tlog.event );print_dbg(" ");
		print_dbg("\r\n");
	}
	
}

*/

/*
static void print_nvram_logpos(nvram_log_pos_t *nvram_data)
{
	print_dbg("logPos:\t0x");
	print_dbg_char_hex(nvram_data->head);print_dbg(" ");
	print_dbg_char_hex(nvram_data->last_log_position);print_dbg(" ");
}
*/


