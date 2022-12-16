
#if (HTTP_USED == 1)
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conf_eth.h"
#include "compiler.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "partest.h"
#include "portmacro.h"
//#include "wdt.h"
/* lwIP includes. */
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/memp.h"
#include "lwip/stats.h"
#include "lwip/init.h"
#if ( (LWIP_VERSION) == ((1U << 24) | (3U << 16) | (2U << 8) | (LWIP_VERSION_RC)) )
#include "netif/loopif.h"
#endif
#include "sockets.h"

/* ethernet includes */
#include "ethernet.h"
#include "networkTime.h"
#include "webSocketUtil.h"
#include "sha1.h"

#include "ups_modbus.h"
#include "debug.h"
#include "FlashMemory.h"

#include "rtc.h"
#include "pm.h"

#include "emailTask.h"
#include "eventSMTP.h"
#include "BasicWEB.h"
#include "IndexHtml.h"
#include "netdb.h"
#include "dns.h"
/*! The size of the buffer in which the dynamic WEB page is created. */

#define webMAX_PAGE_SIZE	512

/*! Standard GET response. */

/*! The port on which we listen. */


/*! Delay on close error. */
#define webSHORT_DELAY		( 100 )

typedef struct keyAndValue_{
	char id[10];
	char value[40];
} KeyAndValue;

char email_server[24] = "ift.iptime.org";
char dns_server[14] = "time.bora.net";
uint8_t cmd_sub_idEx(portCHAR* cmd,  KeyAndValue *array) ;
uint8_t cmd_parseEx(portCHAR *pcRxString, KeyAndValue *array,int argCount);

uint8_t cmd_parse(portCHAR *pcRxString, KeyAndValue *array,int argCount);
uint8_t cmd_sub_id(portCHAR* cmd,  KeyAndValue *array,int pos);
uint8_t parseDate(uint8_t *year,uint8_t *month, uint8_t *day , char *datetime);
uint8_t parseTime(uint8_t *hour,uint8_t *minute , char *strTime);
void inline setUpsInfoToweb(struct netconn *pxNetCon, KeyAndValue *keyvalue,int  keycount,portCHAR *pcRxString,char *commandString );
u32_t dns_lookup(void *callback_arg);
Bool getTimeFromServerExt(int tServer,ip_addr_t server_ipaddr);
int socket_netconn_write(struct netconn *pxNetCon,int dataLen);
portCHAR cDynamicPage[ webMAX_PAGE_SIZE ];

void snmp_coldstart_trap(void);

static void prvweb_ParseHTMLRequest( struct netconn *pxNetCon );


Bool getTimeFromServer(void);

//extern wdt_opt_t opt;
int isNowWebServiceRunningCount =0;
Bool stopModebusGet =false;
extern ups_info_t ups_info;
extern ups_modbus_data_t upsModeBusData;
extern uint16_t isModebusRunning;

int webFunction(int port);
portTASK_FUNCTION( vBasicWEBServer, pvParameters );
xQueueHandle xUdpMessageQueue ;
portTASK_FUNCTION( vBasicWEBServer, pvParameters )
{
	xUdpMessageQueue = xQueueCreate( 1, sizeof(bool *) );
	rtc_init(&AVR32_RTC, RTC_OSC_RC, RTC_PSEL_RC_1_76HZ);
	rtc_enable(&AVR32_RTC);
	data_ethernet_t ethernet_t;
	flash_read__ethernetInfo(&ethernet_t);
	//WEB PORT가 23이면 웹을 Disable한다.
	// :IPFinder로 살리면 다시 살아난다..80포트로
	if(ethernet_t.port == 23){
		while(1){
		isNowWebServiceRunningCount++;
		vTaskDelay( webSHORT_DELAY );
		}
	}
	while(1)webFunction(ethernet_t.port);
	/* Create a new tcp connection handle */
}
int webFunction(int port)
{
	int errorCheck;
	struct netconn *pxHTTPListener, *pxNewConnection;
	pxNewConnection = NULL;
	pxHTTPListener = netconn_new( NETCONN_TCP );
	if( pxHTTPListener == NULL )return -1;

	errorCheck=netconn_bind(pxHTTPListener, NULL, port );
	if(errorCheck != ERR_OK) return errorCheck;
	errorCheck=netconn_listen( pxHTTPListener );
	if(errorCheck != ERR_OK) return errorCheck;

	netconn_set_recvtimeout(pxHTTPListener ,1000);
	int count = 0;
	int accept_err ;
	//for(count=0 ;count<600;count++ )
	for(;;)
	{
		vTaskDelay( webSHORT_DELAY );
		stopModebusGet=false;
		//accept_err
		errorCheck=netconn_accept(pxHTTPListener, &pxNewConnection);// != ERR_OK)

		//while(netconn_accept(pxHTTPListener, &pxNewConnection) != ERR_OK)
		{
			count++;
			isNowWebServiceRunningCount++;
			if( pxHTTPListener->last_err != ERR_TIMEOUT  && pxHTTPListener->last_err != ERR_OK)
			{
				vTaskDelay( webSHORT_DELAY );
				LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n netconn_accept error %d",pxHTTPListener->last_err ) );
				break;
			}
			//else 
			//{    //ERR_CONN -13  not connected
			
		//		vTaskDelay( webSHORT_DELAY );
		//		vTaskDelay( webSHORT_DELAY );
		//	    break;	
		//	}
			//vTaskDelay( webSHORT_DELAY );
		}
		if(errorCheck == ERR_OK)
		{
			vParTestSetLED(0, pdTRUE);
			if(pxNewConnection != NULL)
			{
				stopModebusGet=true;
				//portENTER_CRITICAL();
				//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n netconn_accepted! ") );
				if(processRequestCheckAndWaitTimeout(1000)==0)
				{
				
					flash_read_ups_info(&ups_info);
					prvweb_ParseHTMLRequest(pxNewConnection);
				}
				//portEXIT_CRITICAL();
			}
		}
	}
	LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n netconn Closed..! ") );
	netconn_close(pxNewConnection );
	netconn_close(pxHTTPListener);
	netconn_delete(pxHTTPListener);
	netconn_delete(pxNewConnection );
	mem_free(pxNewConnection );
	mem_free(pxHTTPListener);
	pxNewConnection = NULL;
	vParTestSetLED(0, pdFALSE);
	
	vTaskDelay( webSHORT_DELAY );
	vTaskDelay(100);
}
	



void	set_batt_cells(struct netconn *pxNetCon ,portCHAR *pcRxString,uint8_t *Message_Digest)
{
	// 이미 있는 버퍼를 활용한다.
	memset(Message_Digest,0x00,21);

	data_ethernet_t ethernet_t;

	portCHAR *LocalpcRxString = pcRxString;
	for(;;){
		if( *(LocalpcRxString) == '='){
			LocalpcRxString = LocalpcRxString+1;
			break;
		}
		else LocalpcRxString = LocalpcRxString+1;
		if( *(LocalpcRxString) == NULL) return;
	};
	int batCells =0;
	while(*LocalpcRxString != NULL){
		*(Message_Digest + batCells++) = *LocalpcRxString;
		LocalpcRxString++;
	}
	batCells= atoi(Message_Digest);

	ups_info.installed_battery =batCells;
	flash_write_ups_info(&ups_info);
	sprintf(cDynamicPage,"SET_BATT=%d",ups_info.installed_battery );
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));
	flash_read_ups_info(&ups_info);
	
	
}
void	set_web_port(struct netconn *pxNetCon ,portCHAR *pcRxString,uint8_t *Message_Digest)
{
	// 이미 있는 버퍼를 활용한다.
	memset(Message_Digest,0x00,21);

	data_ethernet_t ethernet_t;

	portCHAR *LocalpcRxString = pcRxString;
	for(;;){
		if( *(LocalpcRxString) == '='){
			LocalpcRxString = LocalpcRxString+1;
			break;
		}
		else LocalpcRxString = LocalpcRxString+1;
		if( *(LocalpcRxString) == NULL) return;
	};
	int port =0;
	while(*LocalpcRxString != NULL){
		*(Message_Digest + port++) = *LocalpcRxString;
		LocalpcRxString++;
	}
	port = atoi(Message_Digest);
	flash_read__ethernetInfo(&ethernet_t);
	ethernet_t.port =port;
	flash_write__ethernetInfo(&ethernet_t);
	
	sprintf(cDynamicPage,"WEB_PORT=%d",ethernet_t.port );
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));
	while(1);
}
 void	set_mac_sAddress(struct netconn *pxNetCon ,portCHAR *pcRxString,uint8_t *Message_Digest)
 {
	 char *temp;
	 char macAddress[6] ;
	 temp = Message_Digest;  // 이미 있는 버퍼를 활용한다.
	 //char *request_string ="SET_MACA";
	 data_ethernet_t ethernet_t;

	 int pos = 0;  //SET_MACA
	 if(strstr(pcRxString,(const char*)"SET_MACA=") !=NULL && strlen(pcRxString) >19 )
	 {
		 strcpy(pcRxString,pcRxString+9); // SET_IPAD= 이후의 데이타를 카피한다.

		 for(pos=0; pos< strlen(pcRxString); pos++ )if( *(pcRxString + pos) == '.' )	break;
		 if( pos == NULL) return;
		 strncpy(temp,pcRxString,pos);  // 이제 192를 얻었다.
		 temp[pos]= NULL;
		 macAddress [0] = atoi(temp);	   // 이제 192.을 날리자.

		 strcpy(pcRxString,pcRxString+pos+1); // 두번째 데이타 이다.
		 for(pos=0; pos< strlen(pcRxString); pos++ )if( *(pcRxString + pos) == '.' )	break;
		 if( pos == NULL) return;
		 strncpy(temp,pcRxString,pos);
		 temp[pos]= NULL;
		 macAddress [1] = atoi(temp);

		 strcpy(pcRxString,pcRxString+pos+1); // 세번째 데이타 이다.
		 for(pos=0; pos< strlen(pcRxString); pos++ )if( *(pcRxString + pos) == '.' )	break;
		 if( pos == NULL) return;
		 strncpy(temp,pcRxString,pos);
		 temp[pos]= NULL;
		 macAddress [2] = atoi(temp);

		 strcpy(pcRxString,pcRxString+pos+1); // 네번째 데이타 이다.
		 for(pos=0; pos< strlen(pcRxString); pos++ )if( *(pcRxString + pos) == '.' )	break;
		 if( pos == NULL) return;
		 strncpy(temp,pcRxString,pos);
		 temp[pos]= NULL;
		 macAddress [3] = atoi(temp);

		 strcpy(pcRxString,pcRxString+pos+1); // 다섯번째 데이타 이다.
		 for(pos=0; pos< strlen(pcRxString); pos++ )if( *(pcRxString + pos) == '.' )	break;
		 if( pos == NULL) return;
		 strncpy(temp,pcRxString,pos);
		 temp[pos]= NULL;
		 macAddress [4] = atoi(temp);

		 strcpy(pcRxString,pcRxString+pos+1); // 여섯섯번째 데이타 이다.
		 for(pos=0; pos< strlen(pcRxString); pos++ )if( *(pcRxString + pos) == '.' )	break;
		 if( pos == NULL) return;
		 strncpy(temp,pcRxString,pos);
		 temp[pos]= NULL;
		 macAddress [5] = atoi(temp);

		 flash_read__ethernetInfo(&ethernet_t);
		 ethernet_t.Ethernet_Conf_EthAddr0 =macAddress[0]  ;
		 ethernet_t.Ethernet_Conf_EthAddr1 =macAddress[1]  ;
		 ethernet_t.Ethernet_Conf_EthAddr2 =macAddress[2]  ;
		 ethernet_t.Ethernet_Conf_EthAddr3 =macAddress[3]  ;
		 ethernet_t.Ethernet_Conf_EthAddr4 =macAddress[4]  ;
		 ethernet_t.Ethernet_Conf_EthAddr5 =macAddress[5]  ;
		 flash_write__ethernetInfo(&ethernet_t);

	 }
	 flash_read__ethernetInfo(&ethernet_t);
	 sprintf(cDynamicPage,"SET_MACA=%x.%x.%x.%x.%x.%x",
	 ethernet_t.Ethernet_Conf_EthAddr0,
	 ethernet_t.Ethernet_Conf_EthAddr1,
	 ethernet_t.Ethernet_Conf_EthAddr2,
	 ethernet_t.Ethernet_Conf_EthAddr3,
	 ethernet_t.Ethernet_Conf_EthAddr4,
	 ethernet_t.Ethernet_Conf_EthAddr5
	 );
	 
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));
 }
 void	set_time_serverIpAddress(struct netconn *pxNetCon ,portCHAR *pcRxString,uint8_t *Message_Digest)
 {
	 char *temp;
	 char ipAddress_code[4] ;
	 temp = Message_Digest;  // 이미 있는 버퍼를 활용한다.
	 int pos = 0;  //SET_TSV1이거나 2이다.
	 int timeServerPos;
	 if(strstr(pcRxString,(const char*)"SET_TSV1") != NULL ) timeServerPos = 0;
	 else 	timeServerPos=1;

	portCHAR *LocalpcRxString = pcRxString;
	for(;;){
		if( *(LocalpcRxString) == '='){
			LocalpcRxString = LocalpcRxString+1;
			break;
		}
		else LocalpcRxString = LocalpcRxString+1;
		if( *(LocalpcRxString) == NULL) return;
	};

	 //strcpy(pcRxString,pcRxString+9); // SET_IPAD= 이후의 데이타를 카피한다.
	 for(pos=0; pos< strlen(LocalpcRxString); pos++ )if( *(LocalpcRxString + pos) == '.' )	break;

	 if( pos == NULL) return;
	 //LWIP_DEBUGF_UDP(WEB_DEBUG, ("pos = %d ",pos ) );
	 strncpy(temp,LocalpcRxString,pos);  // 이제 192를 얻었다.
	 temp[pos]= NULL;
	 ipAddress_code [0] = atoi(temp);	   // 이제 192.을 날리자.
	 
	 strcpy(LocalpcRxString,LocalpcRxString+pos+1); // SET_IPAD= 이후의 데이타를 카피한다.
	 for(pos=0; pos< strlen(LocalpcRxString); pos++ )if( *(LocalpcRxString + pos) == '.' )	break;
	 if( pos == NULL) return;
	 //LWIP_DEBUGF_UDP(WEB_DEBUG, ("pos = %d ",pos ) );
	 strncpy(temp,LocalpcRxString,pos);  // 이제 168를 얻었다.
	 temp[pos]= NULL;
	 ipAddress_code [1] = atoi(temp);	   // 이제 168.을 날리자.
	 
	 strcpy(LocalpcRxString,LocalpcRxString+pos+1); // SET_IPAD= 이후의 데이타를 카피한다.
	 for(pos=0; pos< strlen(LocalpcRxString); pos++ )
	 {
		 if( *(LocalpcRxString + pos) == '.' )	break;
	 }
	 if( pos == NULL) return;
	 //LWIP_DEBUGF_UDP(WEB_DEBUG, ("pos = %d ",pos ) );
	 strncpy(temp,LocalpcRxString,pos);  // 이제 0를 얻었다.
	 temp[pos]= NULL;
	 ipAddress_code [2] = atoi(temp);	   // 이제 0.을 날리자.
	 
	 strcpy(LocalpcRxString,LocalpcRxString+pos+1); // SET_IPAD= 이후의 데이타를 카피한다.
	 
	 strcpy(temp,LocalpcRxString);  // 이제 55를 얻었다.
	 ipAddress_code [3] = atoi(temp);

	 timerserver_address_t time_server_a;
	 TimeServerIpAddressRead(timeServerPos,&time_server_a);   // 읽고
	 
	 time_server_a.Ethernet_Conf_IpAddr0 = ipAddress_code [0] ;
	 time_server_a.Ethernet_Conf_IpAddr1 = ipAddress_code [1] ;
	 time_server_a.Ethernet_Conf_IpAddr2 = ipAddress_code [2] ;
	 time_server_a.Ethernet_Conf_IpAddr3 = ipAddress_code [3] ;
	 
	 TimeServerIpAddressWrite(timeServerPos,&time_server_a);  //쓰고

	 TimeServerIpAddressRead(timeServerPos,&time_server_a);   // 다시 읽고
	 sprintf(cDynamicPage,"SET_TSVx=%d.%d.%d.%d",
	 time_server_a.Ethernet_Conf_IpAddr0,
	 time_server_a.Ethernet_Conf_IpAddr1,
	 time_server_a.Ethernet_Conf_IpAddr2,
	 time_server_a.Ethernet_Conf_IpAddr3);

	 socket_netconn_write( pxNetCon, strlen(cDynamicPage));
 }
 void	send_log_to_web(struct netconn *pxNetCon ,portCHAR *pcRxString,uint8_t *Message_Digest)
{
	log_t *tlog;
	
	int log_pos=0;
	int log_count=0;
	int get_last_log_count  =0 ;
	
	log_pos = getFirstLog(tlog);
	get_last_log_count = getLogCount(tlog);
	if(get_last_log_count >= MAX_LOG) get_last_log_count = MAX_LOG -1;
	
	do
	{
		
		sprintf(cDynamicPage,"log_pos%d:%u:%d:%d:",log_pos,tlog->systemTime,tlog->kind,tlog->event);
		socket_netconn_write( pxNetCon, strlen(cDynamicPage));

		log_count++;
		log_pos++;
		log_pos = log_pos % MAX_LOG  ;
		getNextLog(log_pos,tlog) ;
		
		if(log_count >= 100 )
		{
			log_count = log_count;
			log_count = log_count;
		}
	}while( log_count < get_last_log_count);
}
		
int sendKeyValue(struct netconn *pxNetCon ,KeyAndValue *keyvalue,int keyCount,portCHAR *send_buf)
{
	uint16_t sendCount=0;
	memset(send_buf,0x00,127);
	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	cDynamicPage[0]= 0x81;
	cDynamicPage[1]= 0x01;
	cDynamicPage[2]= NULL;
	for(int i=0;i<keyCount;i++){
		sprintf(send_buf,"%s=%s&",keyvalue[i].id,keyvalue[i].value);
		sendCount =  sendCount + strlen(send_buf);
		strncat(cDynamicPage,send_buf,strlen(send_buf));
	}
	cDynamicPage[1]= sendCount;
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
}

u32_t dns_lookup(void *callback_arg){
	LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\n%s",(char *)callback_arg));
	ip_addr_t addr;
	addr.addr =my_dns_lookup((const char *)callback_arg);
	if(addr.addr != IPADDR_NONE){
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\ndns_lookup: \"%s\": found = ", (const char *)callback_arg));
		ip_addr_debug_print(WEB_DEBUG, &(addr.addr));
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n"));
		return addr.addr;
	}
	return 0;
}
int sendSocketRelay(struct netconn *pxNetCon,portCHAR *send_buf)
{
	uint16_t sendCount=0;
	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	cDynamicPage[0]= 0x81;
	cDynamicPage[1]= 0x01;
	cDynamicPage[2]= NULL;
	sendCount =  sendCount + strlen(send_buf);
	strncat(cDynamicPage,send_buf,strlen(send_buf));
	cDynamicPage[1]= sendCount;
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
}

int sendMailToAllUser()
{
			
}
int sendEventmailUsingSocket(uint16_t event_1,uint16_t event_2,uint16_t event_3,uint16_t event_4){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = PP_HTONS(1024);
	addr.sin_addr.s_addr = dns_lookup("ift.iptime.org");
	if( addr.sin_addr.s_addr == 0){
		ip_addr_t ad;
		dns_gethostbyname(email_server, &ad, dns_lookup,	&email_server);	 // mail ddns를 못 찾으면 다시 요청한다.
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nift.iptime.org DNS Not Fount "));
		return 0;
	}
	struct netconn *xNetConn = NULL;
	xNetConn = netconn_new ( NETCONN_TCP );
	if(xNetConn==NULL){
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\nConnect To Mail server Fail xNetConn null.."));
		return 0;
	};
	int ret = netconn_connect ( xNetConn, &addr.sin_addr, addr.sin_port );
	if( ret != ERR_OK)	{
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\nConnect To Mail server Fail 1.."));
		ip_addr_debug_print(WEB_DEBUG, &(addr.sin_addr.s_addr));
		return 0;
	}
	struct netbuf *pxRxBuffer;
	char *send_buf ;
	send_buf = mem_malloc(255);
	if(send_buf==NULL){return 0;};
	memset(send_buf,0x00,255);
	
	nvram_send_event_mail_t emailAddress;
	for(int i = 0; i< 5; i++){
		flash_read_email_list(i,&emailAddress);
		sprintf(send_buf+strlen(send_buf),"sendto=%s&",emailAddress.email);//메일 ID
		sprintf(send_buf+strlen(send_buf),"cm=%02X&",emailAddress.cmd);//
	}
	
	sprintf(send_buf+strlen(send_buf),"e1=%04X&",event_1);
	sprintf(send_buf+strlen(send_buf),"e2=%04X&",event_2);
	sprintf(send_buf+strlen(send_buf),"e3=%04X&",event_3);
	sprintf(send_buf+strlen(send_buf),"e4=%04X&",event_4);
	netconn_write(xNetConn, send_buf, strlen(send_buf), NETCONN_DONTBLOCK);vTaskDelay(50);
	netconn_set_recvtimeout ( xNetConn, 1000 );
	if(netconn_recv( xNetConn, &pxRxBuffer)  == ERR_OK){
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nReceived Data :  "));
	}
	else{
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nNo Data Received"));
	};
	if( pxRxBuffer != NULL )
	{
		int usLength;
		portCHAR *pcRxString;
		netbuf_data( pxRxBuffer, ( void * ) &pcRxString, &usLength );
	}

	netbuf_delete( pxRxBuffer );
	netconn_disconnect(xNetConn);
	netconn_delete(xNetConn);
	mem_free(send_buf);
	return 1;
}
int webSocket_Function_S_MAILTEST(struct netconn *pxNetCon ,portCHAR *pcRxString){
	char *send_buf ;
	send_buf = mem_malloc(127);
	if(send_buf==NULL){return 0;};
	memset(send_buf,0x00,127);
	
	sprintf(send_buf+strlen(send_buf),"sendto=mvcjhkim@gmail.com&");//메일 ID
	sprintf(send_buf+strlen(send_buf),"cm=%02X&",0x01);//
	sprintf(send_buf+strlen(send_buf),"e1=%04X&",0x0002);//
	sprintf(send_buf+strlen(send_buf),"e2=%04X&",0x0003);//
	sprintf(send_buf+strlen(send_buf),"e3=%04X&",0x0004);//
	sprintf(send_buf+strlen(send_buf),"e4=%04X&",0x0005);//
	sendEventmailUsingSocket(1,2,3,4);
	sendSocketRelay(pxNetCon,send_buf);
	mem_free(send_buf);
	return 1;
}
int webSocket_Function_S_MAILCONF(struct netconn *pxNetCon ,portCHAR *pcRxString){
	//총 5개의 환경 변수를 받는다.
	// 그러므로 ID까지 6개를 받는다.
	int pamCount=6;
	KeyAndValue *keyvalue;
	keyvalue = (KeyAndValue  *)mem_malloc(sizeof(KeyAndValue)*pamCount);
	if(keyvalue == NULL){ 		return 0; 	}
	memset(keyvalue,0x00,sizeof(KeyAndValue)*pamCount);
	char *send_buf ;
	send_buf = mem_malloc(127);
	memset(send_buf,0x00,127);
	if(send_buf== NULL) return -1;
	strcpy(send_buf,pcRxString);
	int keycount = cmd_parse(send_buf,keyvalue,6);
	if(keycount > pamCount)  keycount = pamCount;
	
	nvram_send_event_mail_t emailAddress;
	for(int i = 1; i< keycount; i++){
		sprintf(emailAddress.email,"%s",keyvalue[i].value);
		emailAddress.cmd=0x01;
		flash_write_email_list(i -1,&emailAddress);
	}
	for(int i = 1; i< 5; i++){
		flash_read_email_list(i-1,&emailAddress);
		sprintf(keyvalue[i].id,"UM%d",i);
		sprintf(keyvalue[i].value,"%s",emailAddress.email);
	}
	sendKeyValue(pxNetCon ,keyvalue,5,send_buf);
	
	if(keycount>2)
	for(int i = 0; i< keycount; i++){
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nid:%d : %s  ",i,keyvalue[i].id));
		LWIP_DEBUGF_UDP(WEB_DEBUG,("  value : %s  "   , keyvalue[i].value));
	};
	mem_free(send_buf);
	mem_free(keyvalue);
	
	ip_addr_t addr;
	if( addr.addr != IPADDR_NONE){
	}

}
int webSocket_Function_SET_MAILID(struct netconn *pxNetCon ,portCHAR *pcRxString)
{
	//총 5개의 메일을 설정한다.
	int pamCount=5;
	KeyAndValue *keyvalue;
	keyvalue = (KeyAndValue  *)mem_malloc(sizeof(KeyAndValue)*pamCount);
	if(keyvalue == NULL){
		return 0;
	}
	memset(keyvalue,0x00,sizeof(KeyAndValue)*pamCount);
	char *send_buf ;
	send_buf = mem_malloc(127);
	memset(send_buf,0x00,127);
	if(send_buf== NULL) return -1;
	strcpy(send_buf,pcRxString);
	
	int keycount = cmd_parse(send_buf,keyvalue,5);
	
	nvram_send_event_mail_t emailAddress;
	for(int i = 1; i< keycount; i++){
		sprintf(emailAddress.email,"%s",keyvalue[i].value);
		emailAddress.cmd=0x01;
		flash_write_email_list(i -1,&emailAddress);
	}
	for(int i = 1; i< 5; i++){
		flash_read_email_list(i-1,&emailAddress);
		sprintf(keyvalue[i].id,"UM%d",i);
		sprintf(keyvalue[i].value,"%s",emailAddress.email);
	}
	sendKeyValue(pxNetCon ,keyvalue,5,send_buf);
	for(int i = 0; i< 5; i++){
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nid:%d : %s  ",i,keyvalue[i].id));
		LWIP_DEBUGF_UDP(WEB_DEBUG,("  value : %s  "   , keyvalue[i].value));
	};

	mem_free(send_buf);
	mem_free(keyvalue);
}

int webSocket_Function_S_PASSWD(struct netconn *pxNetCon ,portCHAR *pcRxString){

	KeyAndValue keyvalue[3];
	memset(keyvalue,0x00,sizeof(KeyAndValue)*3);
	//int keycount = 

	cmd_parseEx(pcRxString,keyvalue,2);

	//write_set_time(keyvalue[1].value );
	strcpy(ups_info.passwd,keyvalue[1].value);
	strcpy(ups_info.user_id,keyvalue[2].value);
	flash_write_ups_info(&ups_info);
	flash_read_ups_info(&ups_info);
	sprintf(cDynamicPage,"change_ok");
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));

	return 1;
}
int webSocket_Function_SET_BASIC(struct netconn *pxNetCon ,portCHAR *pcRxString)
{

	KeyAndValue *keyvalue;
	keyvalue = (KeyAndValue  *)mem_malloc(sizeof(KeyAndValue)*16);// 15개르 사용하고 있다.
	if(keyvalue == NULL){
		mem_free(keyvalue);
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\n Memory Alloc Error!!!!!  "));
		while(1);
	}
	memset(keyvalue,0x00,sizeof(KeyAndValue)*16);
	
	//UPS제조년월일 때문에 13개에서 1개을 더늘려 14개로 해 준다.
	//UPS업체명 때문에 14개에서 1개을 더늘려 15개로 해 준다.
	//int keycount = cmd_parse(pcrx,keyvalue,15);
	int keycount = cmd_parse(pcRxString,keyvalue,15);
	//mem_free(pcrx);
	
	//인자 값을 갖고 왔으면 기록을 한후 반영을 하고 인자값이 없으면 현재의 정보를 리턴한다.
	char *send_buf ;
	send_buf = mem_malloc(80);
	if(send_buf== NULL){
		mem_free(keyvalue);
		//mem_free(pcrx);
		//mem_free(send_buf );
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\n Memory Alloc Error!!!!!  "));
		while(1);
	}

	if( keycount < 2){
		//memset(send_buf,0x00,80);
		ups_info.ups_type = ups_info.ups_type; 
		ups_info.out_voltage=ups_info.out_voltage;
		ups_info.ups_version=SWVERSION;   // 121 -> 1.21
		ups_info.charging_voltage=ups_info.charging_voltage;
		uint8_t ayear,amonth,aday,aHour,aMinute ;
		ups_info.bat_install_year=ups_info.bat_install_year;
		ups_info.bat_install_month=ups_info.bat_install_month;
		ups_info.bat_install_day = ups_info.bat_install_day ;
		ups_info.log_write_period= ups_info.log_write_period;
		ups_info.ups_test_period= ups_info.ups_test_period;
		ups_info.ups_test_day= ups_info.ups_test_day;
		ups_info.ups_test_hour = ups_info.ups_test_hour ;
		ups_info.ups_test_minute = ups_info.ups_test_minute ;
		ups_info.limit_load= ups_info.limit_load;
		ups_info.limit_temp= ups_info.limit_temp;
		ups_info.limit_capacity = ups_info.limit_capacity ;
		ups_info.install_year= ups_info.install_year;
		ups_info.install_month= ups_info.install_month;
		ups_info.install_day= ups_info.install_day;
		/* UPS의 제조회사를 등록하기 위하여 사용한다.  keyvalue[13]  : SBC UPS 제조년월일 */
		setUpsInfoToweb(pxNetCon,keyvalue,keycount,send_buf,"SET_BASIC_A");
	}
	else
	{
		ups_info.ups_type =  atoi(keyvalue[1].value); // 31;   // 33, 31, 11, 50, 51, 52 , 80  SB0
		memset(ups_info.sysname, 0x00, sizeof(ups_info.sysname)) ;
		memcpy(ups_info.sysname, keyvalue[2].value, strlen(keyvalue[2].value)) ;// keyvalue[2].value  : SB1 UPS 모델 IFU550
		ups_info.out_voltage=atoi(keyvalue[3].value);//ups_info.company_code=00; 					//ups_info.capacity=30;					//ups_info.input_voltage=380;					// keyvalue[3].value  : SB2 UPS 정격전압   380
		ups_info.ups_version=SWVERSION;   // 121 -> 1.21
		ups_info.charging_voltage=atoi(keyvalue[4].value);// keyvalue[4]  : SB3 UPS 충전전압   220
		uint8_t ayear,amonth,aday,aHour,aMinute ;
		char dtemp[30];					// keyvalue[5]  : SB4 최종배터리교체일(년/월/일) 2018/10/10
		strcpy(dtemp,keyvalue[5].value );
		parseDate(&ayear,&amonth,&aday, dtemp);
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nDATE : %d/%d/%d  "   , ayear,amonth,aday));
		ups_info.bat_install_year=ayear;  // 2018 -> 18
		ups_info.bat_install_month=amonth;
		ups_info.bat_install_day = aday;
		ups_info.log_write_period=atoi(keyvalue[6].value);// keyvalue[6]  : SB5 로그기록주기 1 10 30 60 0 분
		ups_info.ups_test_period=atoi(keyvalue[7].value);// keyvalue[7]  : SB6 UPS테스트주기 0 1 2 주
		ups_info.ups_test_day=atoi(keyvalue[8].value);// keyvalue[8]  : SB7 테스트요일 0 1 2 3 4 5 6 월.화,수,...
		strcpy(dtemp,keyvalue[9].value );//ups_info.protocal = 0x00;// keyvalue[9]  : SB8 테스트시간(시:분)   00:30
		parseTime(&aHour,&aMinute, dtemp);
		ups_info.ups_test_hour = aHour;
		ups_info.ups_test_minute = aMinute;
		ups_info.limit_load=atoi(keyvalue[10].value);// keyvalue[10]  : SB9 한계부하(%)  100
		ups_info.limit_temp=atoi(keyvalue[11].value);					// keyvalue[11]  : SBA 한계온도(℃) 70.0
		ups_info.limit_capacity =atoi(keyvalue[12].value);					// keyvalue[12]  : SBB 한계용량(%)  100
		
		/* UPS의 제조년월일을 기록하기 위한 루틴을 추가한다 */
		//ups_info.charging_voltage=atoi(keyvalue[13].value);// keyvalue[13]  : SBC UPS 제조년월일
		strcpy(dtemp,keyvalue[13].value );
		parseDate(&ayear,&amonth,&aday, dtemp);
		//LWIP_DEBUGF_UDP(WEB_DEBUG,("\nUPS Install DATE : %d/%d/%d  "   , ayear,amonth,aday));
		ups_info.install_year=ayear;  // 2018 -> 18
		ups_info.install_month=amonth;
		ups_info.install_day= aday;
		/* UPS의 제조회사를 등록하기 위하여 사용한다.  keyvalue[13]  : SBC UPS 제조년월일 */
		memset(ups_info.upsIdentManufacturer, 0x00, sizeof(ups_info.upsIdentManufacturer)) ;
		memcpy(ups_info.upsIdentManufacturer, keyvalue[14].value, sizeof(keyvalue[14].value));
		//LWIP_DEBUGF_UDP(WEB_DEBUG,("\nCompanyName : %s  "   , ups_info.upsIdentManufacturer));
		flash_write_ups_info(&ups_info);
		setUpsInfoToweb(pxNetCon,keyvalue,keycount,send_buf,"SET_BASIC_R");
		flash_read_ups_info(&ups_info);
		// System Rebooting
		while(1) ;

	}
//	mem_free(pcrx);
	mem_free(send_buf);
	mem_free(keyvalue);
}
int webSocket_Function_SET_TIME(struct netconn *pxNetCon ,portCHAR *pcRxString)
{
	KeyAndValue keyvalue[3];
	memset(keyvalue,0x00,sizeof(KeyAndValue)*3);
	//int keycount = 
	cmd_parse(pcRxString,keyvalue,2);
	/*
	for(int i = 0; i< keycount; i++){
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\nid:%d : %s  ",i,keyvalue[i].id));
		LWIP_DEBUGF_UDP(WEB_DEBUG,("  value : %s  "   , keyvalue[i].value));
	}
	*/
	write_set_time(keyvalue[1].value );

	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	//cDynamicPage[0]= 0x81;
	//cDynamicPage[1]= strlen(pcRxString);
	//cDynamicPage[2]= NULL;
	strncpy(cDynamicPage,pcRxString,strlen(pcRxString));
	//netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));
}
int webSocket_Function_SET_TRAPADDR(struct netconn *pxNetCon ,portCHAR *pcRxString)
{
	char temp[4];
	//int pos = 0;
	if( strstr(pcRxString,(const char*)"SET_TR") == NULL) {netbuf_delete( pcRxString ); return  -99;};

	temp[0]= pcRxString[6];temp[1]= pcRxString[7];temp[2]= NULL;
	uint8_t trap_pos = 0 ;

	portCHAR *LocalpcRxString = pcRxString;
	for(;;){
		if( *(LocalpcRxString) == '='){
			LocalpcRxString = LocalpcRxString+1;
			break;
		}
		else LocalpcRxString = LocalpcRxString+1;
		if( *(LocalpcRxString) == NULL) return;
	};

	//strcpy(pcRxString,pcRxString+sizeof(9)); // SET_IPAD= 이후의 데이타를 카피한다.
	trap_pos = *(LocalpcRxString-2);//두칸 앞에가 인덱스이며 0~9까지이다;
	trap_pos -= 0x30;		
	if(trap_pos>9)trap_pos = 0;
	ip_addr_t val;
	val.addr = ipaddr_addr(LocalpcRxString) ;
	trap_address_t trap_address;
	trapIpAddressRead(trap_pos,&trap_address) ;

	trap_address.Ethernet_Conf_IpAddr0 =(val.addr & 0xFF000000) >> 24 ;// hash_code [0] ;
	trap_address.Ethernet_Conf_IpAddr1 =(val.addr & 0x00FF0000) >> 16 ;// hash_code [1] ;
	trap_address.Ethernet_Conf_IpAddr2 =(val.addr & 0x0000FF00) >> 8 ;// hash_code [2] ;
	trap_address.Ethernet_Conf_IpAddr3 =(val.addr & 0x000000FF) >> 0 ;// hash_code [3] ;
	trapIpAddressWrite(trap_pos,&trap_address) ;
	trapIpAddressRead(trap_pos,&trap_address) ;

	sprintf(pcRxString,"SET_TR%02d=%d.%d.%d.%d",trap_pos,
	trap_address.Ethernet_Conf_IpAddr0,
	trap_address.Ethernet_Conf_IpAddr1,
	trap_address.Ethernet_Conf_IpAddr2,
	trap_address.Ethernet_Conf_IpAddr3);
	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	//cDynamicPage[0]= 0x81;
	//cDynamicPage[1]= strlen(pcRxString);
	//cDynamicPage[2]= NULL;
	strncpy(cDynamicPage,pcRxString,strlen(pcRxString));
	//netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));
}


//err_t snmp_send_trap(s8_t generic_trap, struct snmp_obj_id *eoid, s32_t specific_trap,const char* strMessage);

void snmp_trap_userOid(char *uOid,s8_t generic_trap, s32_t specific_trap);
err_t snmp_send_trapDirect(s8_t generic_trap, struct snmp_obj_id *eoid, s32_t specific_trap,const char* strMessage);
int webSocket_Function_SET_TRAPTEST(struct netconn *pxNetCon ,portCHAR *pcRxString)
{
	uint8_t *testIp;
	uint8_t *trapCode;
	if( strstr(pcRxString,(const char*)"TRAPTEST") == NULL) {netbuf_delete( pcRxString ); return  -99;};
	portCHAR *LocalpcRxString = pcRxString;

	for(;;){  //&의 위치를 찾고 testIp를 위치시킨다,.
		if( *(LocalpcRxString) == '&'){
			//*LocalpcRxString = 0x00; //현재 위치는 널을 넣고, 포인터는 한칸 움직인다.
			LocalpcRxString++;
			break;
		}
		else LocalpcRxString = LocalpcRxString+1;
		if( *(LocalpcRxString) == NULL) return;
	};

	testIp = LocalpcRxString;
	for(;;){  //&의 위치를 찾고 testIp를 위치시킨다,.
		if( *(LocalpcRxString) == '='){
			*LocalpcRxString = 0x00; //현재 위치는 널을 넣고, 포인터는 한칸 움직인다.
			LocalpcRxString++;
			break;
		}
		else LocalpcRxString = LocalpcRxString+1;
		if( *(LocalpcRxString) == NULL) return;
	};
	trapCode = LocalpcRxString;
	//strcpy(pcRxString,pcRxString+sizeof(9)); // SET_IPAD= 이후의 데이타를 카피한다.
	ip_addr_t val;
	val.addr = ipaddr_addr(testIp) ;
	trap_address_t trap_address;

	trap_address.Ethernet_Conf_IpAddr0 =(val.addr & 0xFF000000) >> 24 ;// hash_code [0] ;
	trap_address.Ethernet_Conf_IpAddr1 =(val.addr & 0x00FF0000) >> 16 ;// hash_code [1] ;
	trap_address.Ethernet_Conf_IpAddr2 =(val.addr & 0x0000FF00) >> 8 ;// hash_code [2] ;
	trap_address.Ethernet_Conf_IpAddr3 =(val.addr & 0x000000FF) >> 0 ;// hash_code [3] ;


	//struct snmp_obj_id eoid = {10,{1,3,6,1,4,1,12236,1,1,11}};
	//snmp_send_trap(SNMP_GENTRAP_ENTERPRISESPC, struct snmp_obj_id *eoid, 200 ,"HELLOW TRAP");

	sprintf(cDynamicPage,"SEND_TRAP=%s, %s ",testIp, trapCode);
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));

	
	snmp_send_trapDirect(6, NULL, 63,trapCode);
	//snmp_trap_userOid(trapCode,SNMP_GENTRAP_ENTERPRISESPC,200);
}

int webSocket_Function_GET_ALLIP(struct netconn *pxNetCon ,portCHAR *pcRxString)
{
	ip_addr_t val;
	data_ethernet_t ethernet_t;
	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	flash_read__ethernetInfo(&ethernet_t);   
	sprintf(cDynamicPage,"%d.%d.%d.%d", ethernet_t.Ethernet_Conf_IpAddr0, ethernet_t.Ethernet_Conf_IpAddr1, ethernet_t.Ethernet_Conf_IpAddr2, ethernet_t.Ethernet_Conf_IpAddr3);

	sprintf(pcRxString,"&%d.%d.%d.%d", ethernet_t.Ethernet_Conf_Gateway_Addr0, ethernet_t.Ethernet_Conf_Gateway_Addr1, ethernet_t.Ethernet_Conf_Gateway_Addr2, ethernet_t.Ethernet_Conf_Gateway_Addr3);
	strcat(cDynamicPage,pcRxString );

	sprintf(pcRxString,"&%d.%d.%d.%d", ethernet_t.Ethernet_Conf_Net_Mask0, ethernet_t.Ethernet_Conf_Net_Mask1, ethernet_t.Ethernet_Conf_Net_Mask2, ethernet_t.Ethernet_Conf_Net_Mask3);
	strcat(cDynamicPage,pcRxString );
	
	sprintf(pcRxString,"&%x.%x.%x.%x.%x.%x", ethernet_t.Ethernet_Conf_EthAddr0, ethernet_t.Ethernet_Conf_EthAddr1, ethernet_t.Ethernet_Conf_EthAddr2, ethernet_t.Ethernet_Conf_EthAddr3, ethernet_t.Ethernet_Conf_EthAddr4, ethernet_t.Ethernet_Conf_EthAddr5 );
	strcat(cDynamicPage,pcRxString );

	sprintf(pcRxString ,"&%d",ethernet_t.port );
	strcat(cDynamicPage,pcRxString );


	uint8_t trap_pos = 0 ;
	trap_address_t trap_address;
	for(trap_pos=0;trap_pos<10;trap_pos++){
		trapIpAddressRead(trap_pos,&trap_address) ;
		sprintf(pcRxString,"&%d.%d.%d.%d",trap_address.Ethernet_Conf_IpAddr0, trap_address.Ethernet_Conf_IpAddr1, trap_address.Ethernet_Conf_IpAddr2, trap_address.Ethernet_Conf_IpAddr3);
		strcat(cDynamicPage,pcRxString );
	}

	timerserver_address_t time_server_a;
	TimeServerIpAddressRead(0,&time_server_a);   // 다시 읽고
	sprintf(pcRxString,"&%d.%d.%d.%d", time_server_a.Ethernet_Conf_IpAddr0, time_server_a.Ethernet_Conf_IpAddr1, time_server_a.Ethernet_Conf_IpAddr2, time_server_a.Ethernet_Conf_IpAddr3);
	strcat(cDynamicPage,pcRxString );
	
	TimeServerIpAddressRead(1,&time_server_a);   // 다시 읽고
	sprintf(pcRxString,"&%d.%d.%d.%d", time_server_a.Ethernet_Conf_IpAddr0, time_server_a.Ethernet_Conf_IpAddr1, time_server_a.Ethernet_Conf_IpAddr2, time_server_a.Ethernet_Conf_IpAddr3);
	strcat(cDynamicPage,pcRxString );

	sprintf(pcRxString,"&%d", ups_info.installed_battery);
	strcat(cDynamicPage,pcRxString );

	socket_netconn_write( pxNetCon, strlen(cDynamicPage));

	//
	//socket_netconn_write( pxNetCon, strlen(cDynamicPage));

}
int webSocket_Function_SET_IPADDR(struct netconn *pxNetCon ,portCHAR *pcRxString,char *req)
{
	strcpy(pcRxString,pcRxString+sizeof(req)); // SET_IPAD= 이후의 데이타를 카피한다.
	portCHAR *LocalpcRxString = pcRxString;
	for(;;){
		if( *(LocalpcRxString) == '='){
			LocalpcRxString = LocalpcRxString+1;
			break;
		}
		else LocalpcRxString = LocalpcRxString+1;
		if( *(LocalpcRxString) == NULL) return;
	};
	ip_addr_t val;
	val.addr = ipaddr_addr(LocalpcRxString) ;
	data_ethernet_t ethernet_t;
	flash_read__ethernetInfo(&ethernet_t);   // 읽고
	if( strcmp("SET_IPAD",req)==0){
		ethernet_t.Ethernet_Conf_IpAddr0 = (val.addr & 0xFF000000) >> 24 ; //hash_code [0] ;
		ethernet_t.Ethernet_Conf_IpAddr1 = (val.addr & 0x00FF0000) >> 16 ;//hash_code [1] ;
		ethernet_t.Ethernet_Conf_IpAddr2 = (val.addr & 0x0000FF00) >> 8 ;//hash_code [2] ;
		ethernet_t.Ethernet_Conf_IpAddr3 = (val.addr & 0x000000FF) >> 0 ;//hash_code [3] ;
		sprintf(cDynamicPage,"SET_IPAD=%d.%d.%d.%d",
		ethernet_t.Ethernet_Conf_IpAddr0,
		ethernet_t.Ethernet_Conf_IpAddr1,
		ethernet_t.Ethernet_Conf_IpAddr2,
		ethernet_t.Ethernet_Conf_IpAddr3);
	}//
	else if( strcmp("SET_GATE",req)==0){
		ethernet_t.Ethernet_Conf_Gateway_Addr0 = (val.addr & 0xFF000000) >> 24 ; //hash_code [0] ;
		ethernet_t.Ethernet_Conf_Gateway_Addr1 = (val.addr & 0x00FF0000) >> 16 ;//hash_code [1] ;
		ethernet_t.Ethernet_Conf_Gateway_Addr2 = (val.addr & 0x0000FF00) >> 8 ;//hash_code [2] ;
		ethernet_t.Ethernet_Conf_Gateway_Addr3 = (val.addr & 0x000000FF) >> 0 ;//hash_code [3] ;
		sprintf(cDynamicPage,"SET_GATE=%d.%d.%d.%d",
		ethernet_t.Ethernet_Conf_Gateway_Addr0,
		ethernet_t.Ethernet_Conf_Gateway_Addr1,
		ethernet_t.Ethernet_Conf_Gateway_Addr2,
		ethernet_t.Ethernet_Conf_Gateway_Addr3);
	}//
	else if( strcmp("SET_SUBM",req)==0){
		ethernet_t.Ethernet_Conf_Net_Mask0 = (val.addr & 0xFF000000) >> 24 ; //hash_code [0] ;
		ethernet_t.Ethernet_Conf_Net_Mask1 = (val.addr & 0x00FF0000) >> 16 ;//hash_code [1] ;
		ethernet_t.Ethernet_Conf_Net_Mask2 = (val.addr & 0x0000FF00) >> 8 ;//hash_code [2] ;
		ethernet_t.Ethernet_Conf_Net_Mask3 = (val.addr & 0x000000FF) >> 0 ;//hash_code [3] ;
		sprintf(cDynamicPage,"SET_SUBM=%d.%d.%d.%d",
		ethernet_t.Ethernet_Conf_Net_Mask0,
		ethernet_t.Ethernet_Conf_Net_Mask1,
		ethernet_t.Ethernet_Conf_Net_Mask2,
		ethernet_t.Ethernet_Conf_Net_Mask3);
	}//
	
	flash_write__ethernetInfo(&ethernet_t);  //쓰고
	flash_read__ethernetInfo(&ethernet_t);   // 다시 읽고
	socket_netconn_write( pxNetCon, strlen(cDynamicPage));
}
void sendErrorWebSocket(struct netconn *pxNetCon ,portCHAR *pcRxString,uint8_t errorCode ){
	cDynamicPage[0]= 0x81;
	cDynamicPage[1]=1;
	cDynamicPage[2]=errorCode;
	cDynamicPage[3]=0x00;
	netconn_write( pxNetCon, cDynamicPage,3, NETCONN_COPY );
}


//extern	Bool (*requestUpsData)();

extern Bool getDataFromSerial();
int webSocket_Function_UPS_EX_DATA(struct netconn *pxNetCon ,portCHAR *pcRxString,char* commandData)
{
	uint16_t *pData ;//=(int *)&upsModeBusData ;//
	uint16_t dataLen=0;


	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	pData =(int16_t *)&upsModeBusData ;
	//만일 시리얼 포트를 사용중이면 대기한다. 
	//if(getDataFromSerial()==false) return;

	//if(upsModeBusData.Bat_volt_rms != 245){
	//		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nupsModeBusData.Bat_volt_rms=%d",upsModeBusData.Bat_volt_rms) );
	//	}

	portTickType snmp_systemTime=getTimeLong();
	pData[10] =  (uint16_t)(0xFFFF & snmp_systemTime);
	pData[11] =  (uint16_t)(snmp_systemTime>>16);

	memcpy(cDynamicPage,commandData,strlen(commandData));
	dataLen+=strlen(commandData);
	for(int i=0;i<59;i++){
		sprintf( pcRxString, "%d:",  *(pData+i) );
		memcpy(cDynamicPage+dataLen,pcRxString,strlen(pcRxString));
		dataLen+=strlen(pcRxString);
		pcRxString = NULL;
	}
	socket_netconn_write( pxNetCon, dataLen);
}

int webSocket_proc(struct netconn *pxNetCon ,portCHAR *pcRxString)
{
	int err;
	portCHAR *findpos;
	SHA1Context sha;
	uint8_t Message_Digest[21];
	char hash_code[40+38+2];
	memset(hash_code,0x00,sizeof(hash_code));
	findpos = strstr(pcRxString,(const char*)"Key");

	//소켓에 key값이 없다면 소켓을 종료 하고 리턴한다.
	if(strstr(pcRxString,(const char*)"Key") == NULL)
	{
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nKey value not found error!\n") );
		netconn_delete( pxNetCon );
		pxNetCon=NULL;
		return -1;
	}

	strncpy(hash_code,(findpos+5),24);
	strcat((char *)hash_code,"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	//이제 shacode를 얻자.
	err = SHA1Reset(&sha);
	err = SHA1Input(&sha,  (const unsigned char *) hash_code, strlen(hash_code));
	err = SHA1Result(&sha, Message_Digest);
	Message_Digest[20]=NULL;
	char *s_encode;
	s_encode = mzapi_encode_base64(Message_Digest);
	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	strcpy( cDynamicPage, "HTTP/1.1 101 Switching Protocols\r\n" );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	strcpy( cDynamicPage, "Upgrade: websocket\r\n" );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	strcpy( cDynamicPage, "Connection: Upgrade\r\n" );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	strcpy( cDynamicPage, "Sec-WebSocket-Accept: " );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	strcpy( cDynamicPage, s_encode );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	strcpy( cDynamicPage, "\r\n" );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	strcpy( cDynamicPage, "\r\n" );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	free((void *)s_encode);

	struct netbuf *pxRxBuffer;
	unsigned portSHORT usLength;
	int errCount=0;
	err=0;
	pxNetCon->recv_timeout = 2000;
	do{
		err = netconn_recv( pxNetCon, &pxRxBuffer) ;
		if(err != ERR_OK){
			vTaskDelay( webSHORT_DELAY );
			//if( pxNetCon->last_err< 0){
			netbuf_delete( pxRxBuffer );
			LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\nnetconn last_err errrCount err:  %d %d %d",pxNetCon->last_err,errCount,err) );
			return pxNetCon->last_err;
			//}
		}
		else break;
	}while( err != ERR_OK );
	

	if( pxRxBuffer != NULL )
	{
		netbuf_data( pxRxBuffer, ( void * ) &pcRxString, &usLength );

		if( NULL != pcRxString   )
		{
			*(pcRxString+usLength) = NULL;
			webSocket_decode(pcRxString);

			int request_command=0;
			
			if( strncmp(  "UPS_DATA",pcRxString,8)  == 0)			request_command =1  ;
			else if( strncmp("UPS_EX_DATA",pcRxString,11)== 0){
					webSocket_Function_UPS_EX_DATA(pxNetCon ,pcRxString,"UPS_EX_DATA?");
			}
			else if( strncmp("UPS_LOG",pcRxString,7)  == 0)		request_command =2  ;
			else if( strncmp("SET_TIME",pcRxString,8)  == 0)	request_command =3  ;
			else if( strncmp("SET_IPAD",pcRxString,8)  == 0)	request_command =4  ;
			else if( strncmp("SET_GATE",pcRxString,8)  == 0)	request_command =5  ;
			else if( strncmp("SET_SUBM",pcRxString,8)  == 0)	request_command =6  ;
			else if( strncmp("SET_MACA",pcRxString,8)  == 0)	request_command =7  ;
			else if( strncmp("SET_TR",pcRxString,6)  == 0)		request_command =8  ;	 // 트랩어드레스
			else if( strncmp("SET_TSV1",pcRxString,8)  == 0)	request_command =9  ;	 // 네트웍 1차 타임서버.
			else if( strncmp("SET_TSV2",pcRxString,8)  == 0)	request_command =9  ;	 // 네트웍 2차 타임서버.
			else if( strncmp("REBOOT",pcRxString,6)  == 0)		request_command =11  ;
			else if( strncmp("WEB_PORT",pcRxString,8)  == 0)	request_command =12  ;
			else if( strncmp("SET_BASIC",pcRxString,9)  == 0)	request_command =13  ; //UPS의 기본 정보를 셋팅한다.
			else if( strncmp("SET_MAILID",pcRxString,9)  == 0)	request_command =14  ;
			else if( strncmp("S_MAILCONF",pcRxString,10)  == 0)	request_command =15  ;
			else if( strncmp("S_MAILTEST",pcRxString,10)  == 0)	request_command =16  ;
			else if( strncmp("changePasswd",pcRxString,12)  == 0)	request_command =17  ;
			else if( strncmp("TRAPTEST",pcRxString,8)  == 0)	request_command =18  ;
			else if( strncmp("R_ALL_IP",pcRxString,8)  == 0)	request_command =19  ;  // 모든 IP정보를 웹에 전달해 준다.
			else if( strncmp("SET_BATT",pcRxString,8)  == 0)	request_command =20  ;  // 모든 IP정보를 웹에 전달해 준다.

			//if(request_command == 1)  // UPS_DATA webSocket_Function_UPS_DATA(pxNetCon ,pcRxString);
			if(request_command == 2)
			send_log_to_web(pxNetCon ,pcRxString,Message_Digest);
			if(request_command == 3)  // SET_TIME
			webSocket_Function_SET_TIME(pxNetCon ,pcRxString);
			if(request_command == 4)
			webSocket_Function_SET_IPADDR(pxNetCon ,pcRxString,"SET_IPAD");
			if(request_command == 5)
			webSocket_Function_SET_IPADDR(pxNetCon ,pcRxString,"SET_GATE");
			if(request_command == 6)
			webSocket_Function_SET_IPADDR(pxNetCon ,pcRxString,"SET_SUBM");
			if(request_command == 7) // SET_MACA MAC_ADDRESS
			set_mac_sAddress(pxNetCon ,pcRxString,Message_Digest);
			if(request_command == 8)  // SET_TR
			webSocket_Function_SET_TRAPADDR(pxNetCon ,pcRxString)	;
			if(request_command == 9) // 타임서버
			set_time_serverIpAddress(pxNetCon ,pcRxString,Message_Digest);
			if(request_command == 11) // REBOOT
				while(1) ;
			if(request_command == 12) // WEB_PORT
			set_web_port(pxNetCon ,pcRxString,Message_Digest);
			if(request_command == 13) // SET_BASIC
			webSocket_Function_SET_BASIC(pxNetCon ,pcRxString);
			if(request_command == 14) 
			webSocket_Function_SET_MAILID(pxNetCon ,pcRxString);
			if(request_command == 15) //S_MAILCONF
			webSocket_Function_S_MAILCONF(pxNetCon ,pcRxString);
			if(request_command == 16) //S_MAILCONF
			webSocket_Function_S_MAILTEST(pxNetCon ,pcRxString);
			if(request_command == 17) 
			webSocket_Function_S_PASSWD(pxNetCon ,pcRxString);
			if(request_command == 18) 
			webSocket_Function_SET_TRAPTEST(pxNetCon ,pcRxString)	;
			if(request_command == 19) 
			webSocket_Function_GET_ALLIP(pxNetCon ,pcRxString)	;
			if(request_command == 20) 
			set_batt_cells(pxNetCon ,pcRxString,Message_Digest);
		}
		netbuf_delete( pxRxBuffer );
	}
	return 0;
}
					// keyvalue[0].value  : command
					// keyvalue[1].value  : SB0 UPS 종류(통신방식) 33 31 11 50 51 52 80 
					// keyvalue[2].value  : SB1 UPS 모델 IFU550
					// keyvalue[3].value  : SB2 UPS 정격전압   380
					// keyvalue[4]  : SB3 UPS 충전전압   220
					// keyvalue[5]  : SB4 최종배터리교체일(년/월/일) 2018/10/10
					// keyvalue[6]  : SB5 로그기록주기 1 10 30 60 0 분
					// keyvalue[7]  : SB6 UPS테스트주기 0 1 2 주
					// keyvalue[8]  : SB7 테스트요일 0 1 2 3 4 5 6 월.화,수,...
					// keyvalue[9]  : SB8 테스트시간(시:분)   00:30
					// keyvalue[10]  : SB9 한계부하(%)  100
					// keyvalue[11]  : SBA 한계온도(℃) 70.0
					// keyvalue[12]  : SBB 한계용량(%)  100
					// ---> keyvalue[13]  : SBC 제조년월일
					// keyvalue[14]  : SBD UPS 제조회사
//보내고 싶은 데이타를pcRxString 에 넣어준후 데이타의 길이를 알려 준다
//	socket_netconn_write( pxNetCon, dataLen);
int socket_netconn_write(struct netconn *pxNetCon,int dataLen)
{
	portCHAR* pcString = mem_malloc(dataLen+4);
	if(pcString == NULL) return -1;
	memset(pcString,0x00,dataLen+4);
	pcString[0]= 0x81;
	if(dataLen<126)
	{
		pcString[1]=dataLen;
		memcpy(pcString+2,cDynamicPage,dataLen);
		netconn_write( pxNetCon, pcString,dataLen+2, NETCONN_COPY );
	}
	else
	{
		pcString[1]=126;
		pcString[2]=dataLen >> 8;
		pcString[3]=0x00FF & dataLen;
		pcString[4]=NULL;
		memcpy(pcString+4,cDynamicPage,dataLen);
		netconn_write( pxNetCon, pcString,dataLen+4, NETCONN_COPY );
	}
	mem_free(pcString);
}
					
void inline setUpsInfoToweb(struct netconn *pxNetCon, KeyAndValue *keyvalue,int  keycount,portCHAR *send_buf,char *commandString)
{
	//uint16_t sendCount=0;
	memcpy(keyvalue[0].id, commandString, strlen( commandString) );
	memset(keyvalue[0].value, 0x00, sizeof(keyvalue[0].value) );
	
	sprintf(send_buf,"%d",ups_info.ups_type); //
	//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n%s",	tempChar ));
	memcpy(keyvalue[1].value,send_buf,strlen(send_buf));
	
	//sprintf(send_buf,"%s",ups_info.sysname); //
	//memcpy(keyvalue[2].value,send_buf,strlen(send_buf));
	memcpy(&keyvalue[2].value, ups_info.sysname,strlen(ups_info.sysname));					// keyvalue[2].value  : SB1 UPS 모델 IFU550
	//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n%s",	keyvalue[2].value ));

	sprintf(send_buf,"%d",ups_info.out_voltage);
	memcpy(keyvalue[3].value,send_buf, strlen(send_buf) );
	//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\n%s",	keyvalue[3].value ));

	sprintf(send_buf,"%d",ups_info.charging_voltage);
	memcpy(keyvalue[4].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d/%d/%d",ups_info.bat_install_year+2000,ups_info.bat_install_month,ups_info.bat_install_day);
	memcpy(keyvalue[5].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d",ups_info.log_write_period);
	memcpy(keyvalue[6].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d",ups_info.ups_test_period);
	memcpy(keyvalue[7].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d",ups_info.ups_test_day);
	memcpy(keyvalue[8].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d/%d",ups_info.ups_test_hour,ups_info.ups_test_minute);
	memcpy(keyvalue[9].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d",ups_info.limit_load);
	memcpy(keyvalue[10].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d",ups_info.limit_temp);
	memcpy(keyvalue[11].value,send_buf, strlen(send_buf) );

	sprintf(send_buf,"%d",ups_info.limit_capacity);
	memcpy(keyvalue[12].value,send_buf, strlen(send_buf) );
	/* UPS설치날짜 정보를 전송 한다 */
	sprintf(send_buf,"%d/%d/%d",ups_info.install_year+2000,ups_info.install_month,ups_info.install_day);
	memcpy(keyvalue[13].value,send_buf, strlen(send_buf) );   // 원래는 SBB가 되어야 하나 0번을 ID로 사용하기 때문에 하나가 밀려서 SBC가된다.
	//  SBD keyvalue[14].value  : SB1 UPS 모델 IFU550
	//strncpy(keyvalue[14].value, ups_info.upsIdentManufacturer,strlen(ups_info.upsIdentManufacturer));		 
	//sprintf(send_buf,"%s",ups_info.upsIdentManufacturer);
	memcpy(keyvalue[14].value,send_buf, strlen(send_buf) );   
	memcpy(keyvalue[14].value, ups_info.upsIdentManufacturer,strlen(ups_info.upsIdentManufacturer));		 

	memcpy(keyvalue[15].value,"0", strlen("0") );              // SBE
	memset(send_buf,0x00,sizeof(send_buf));

	/*
	for(int i = 0; i< 15; i++){
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nid:%d : %s  ",i,keyvalue[i].id));
		LWIP_DEBUGF_UDP(WEB_DEBUG,("  value : %s  "   , keyvalue[i].value));
	}
	*/
	
	int dataLen=0;
	memset(cDynamicPage,0x00,sizeof(cDynamicPage));
	sprintf(send_buf,"%s?",commandString);
	memcpy(cDynamicPage+dataLen,send_buf,strlen(send_buf));
	dataLen+=strlen(commandString)+1;  // ? 때문에 1개를 추가한다.
	for(int i=0;i<15;i++){
		sprintf(send_buf,"SB%X:%s&",i,keyvalue[i+1].value);
		memcpy(cDynamicPage+dataLen,send_buf,strlen(send_buf));
		dataLen+=strlen(send_buf);
		
	}
	socket_netconn_write(pxNetCon,dataLen);
}
void webHTML_netconn_write(struct netconn *pxNetCon,const char *webHTML_script)
{
	int pos =0;
	int size = strlen( webHTML_script);
	if( size <= webMAX_PAGE_SIZE)
	{
		strncpy( cDynamicPage, webHTML_script,size );
		netconn_write( pxNetCon, cDynamicPage,size, NETCONN_COPY );
		return;
	}
	
	for(;;){
		if( strlen( webHTML_script +pos ) > webMAX_PAGE_SIZE )
		{
			strncpy( cDynamicPage, webHTML_script + pos,webMAX_PAGE_SIZE );
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
			pos += webMAX_PAGE_SIZE ;
		}
		else{
			strcpy( cDynamicPage, webHTML_script + pos );
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
			break;
		}
	}
}

int getHexInString(portCHAR *str,int16_t *retarray ,int16_t length,char delimiter)
{
	int pos;
	int i;
	portCHAR temp[5];
	
	for(i =0; i< length ; i++)
	{
		memset(temp,0x00,sizeof(temp));
		for(pos=0; pos< sizeof(temp); pos++ )
		{
			if( *(str + pos) == delimiter )	break;
		}
		if(pos>sizeof(temp)) return -1; //이 경우는 나오면 안된다.   retarray의 최대갑은 6이다.
		strncpy(temp,str,pos);  // 이제 192를 얻었다.
		//retarray [i] = (int16_t)atoi(temp);	   // 이제 192.을 날리자.
		retarray [i] = (int16_t)strtol(temp,NULL,16);	   // 이제 192.을 날리자.
		str=strchr (str,delimiter) ; //이제 .000.000.00--등이 남았다.
		if(str == NULL) break;;
		strcpy(str,str+1);  // .을 제거한다.
	}
	if(i==length-1) return 1;
}

int getIntegerInString(portCHAR *str,int16_t *retarray ,int16_t length,char delimiter)
{
	int pos;
	int i;
	portCHAR temp[5];
	
	for(i =0; i< length ; i++)
	{
		memset(temp,0x00,sizeof(temp));
		for(pos=0; pos< sizeof(temp); pos++ )
		{
			if( *(str + pos) == delimiter )	break;
		}
		if(pos>sizeof(temp)) return -1; //이 경우는 나오면 안된다.   retarray의 최대갑은 6이다.
		strncpy(temp,str,pos);  // 이제 192를 얻었다.
		retarray [i] = (int16_t)atoi(temp);	   // 이제 192.을 날리자.
		str=strchr (str,delimiter) ; //이제 .000.000.00--등이 남았다.
		str = str+1;
		if(str == NULL) break;;
		//strcpy(str,str+1);  // .을 제거한다.
	}
	if(i==length-1) return 1;
}
void write_trap_ipaddress(int trap_pos,int16_t *ipaddress)
{
	trap_address_t trap_address;
	trapIpAddressRead(trap_pos,&trap_address) ;
	trap_address.Ethernet_Conf_IpAddr0 = ipaddress [0] ;
	trap_address.Ethernet_Conf_IpAddr1 = ipaddress [1] ;
	trap_address.Ethernet_Conf_IpAddr2 = ipaddress [2] ;
	trap_address.Ethernet_Conf_IpAddr3 = ipaddress [3] ;
	trapIpAddressWrite(trap_pos,&trap_address) ;
	trapIpAddressRead(trap_pos,&trap_address) ;
}

uint8_t write_set_reLoadtime(portCHAR *parameter){
	int reloadTime = 0;
	portCHAR temp[5] ;temp[0]=0x00;temp[1]=0x00;temp[2]=0x00;temp[3]=0x00;temp[4]=0x00;
	
	for(uint8_t i=0 ; i<5; i++){
		temp[i] = *(parameter+i) ;
		if(*(parameter+i)==NULL){
			temp[i] = NULL;
			break;
		}
	}
	reloadTime =   atoi(temp) ;
	return (uint8_t) reloadTime;

}
void write_set_time(portCHAR *parameter)
{
	
	time_t lTime;

	lTime = atoll(parameter)+2208965020 ;// 2206340820 + 2561640 + 118800;  // -2206340820000    70년  밀리세컨드

	setSystemTimeLong(lTime  );

	breakTime(lTime);
	rtc_set_value(&AVR32_RTC, lTime);
	rtc_enable(&AVR32_RTC);
	//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSet Time :  %d:%d:%d %d:%d:%d systemTime=%u ", year(),month(),day()+1,hour(),minute(),tmsecond(),getTimeLong()  ));
}
void write_timeserver_ipaddress(int timeServer_pos,int16_t *ipaddress)
{
	timerserver_address_t time_server_a;

	TimeServerIpAddressRead(timeServer_pos,&time_server_a) ;
	time_server_a.Ethernet_Conf_IpAddr0 = ipaddress [0] ;
	time_server_a.Ethernet_Conf_IpAddr1 = ipaddress [1] ;
	time_server_a.Ethernet_Conf_IpAddr2 = ipaddress [2] ;
	time_server_a.Ethernet_Conf_IpAddr3 = ipaddress [3] ;
	TimeServerIpAddressWrite(timeServer_pos,&time_server_a) ;
	TimeServerIpAddressRead(timeServer_pos,&time_server_a) ;
	getTimeFromServer();
}

/*
void html_SETUP_EMAIL(struct netconn *pxNetCon,portCHAR *pcRxString)
{
	//portCHAR e_mail_server[15];
	//portCHAR sender_mail_id[30];
	//Bool is_need_authentic;
	//portCHAR email_user_id[10];
	//portCHAR email_user_passwd[10];
	//Bool     is_send_mail_Event;
	
	//portCHAR send_event_mail[30];
	
	nvram_send_event_mail_t emailAddress;

	portCHAR commandType[30] ;
	portCHAR param[30] 	;

	pcRxString = strstr(pcRxString,(const char*)"?");
	
	smtp_info_t smtp_info;
	flash_read_smtp_info(&smtp_info);
	bool is_smtp_info_changed=false;
	
	while(pcRxString != NULL)
	{
		
		memset(commandType,0x00,30);
		memset(param,0x00,30);
		if( pcRxString == NULL || pcRxString == 0x00 ) break; ;
		for(int i=0;i<30;i++) // 여기서 name 을 얻고
		{
			if(pcRxString[i+1] == '=') break;
			if( pcRxString[i+1] == NULL ) break; ;
			commandType[i] = pcRxString[i+1];
		}
		pcRxString = strstr(pcRxString,(const char*)"=");
		for(int i=0;i<30;i++)  // 여기서 값을 얻는다.
		{
			if(pcRxString[i+1] == '&' || pcRxString[i+1] == NULL || pcRxString[i+1] == 0x20 || pcRxString[i+1] == '\r' || pcRxString[i+1] == '\n') break;
			if( pcRxString[i+1] == NULL ) break; ;
			param[i] = pcRxString[i+1];
		}
		
		if( strcmp(commandType,"mailServer") ==0 )
		{
			sprintf(smtp_info.e_mail_server,"%s",param);
			is_smtp_info_changed=true;
		};
		if( strcmp(commandType,"userSendMail") ==0 ){
			sprintf(smtp_info.sender_mail_id,"%s",param);
			
			is_smtp_info_changed=true;
		};
		if( strcmp(commandType,"userMailid") ==0 ) {
			sprintf(smtp_info.email_user_id,"%s",param);
			
			is_smtp_info_changed=true;
		};
		if( strcmp(commandType,"userMailPasswd") ==0 ){
			sprintf(smtp_info.email_user_passwd,"%s",param);
			
			is_smtp_info_changed=true;
		};
		if( strcmp(commandType,"checkSeverAuthentic") ==0 )
		{
			if( strcmp(param,"YES") ==0 ) {smtp_info.is_need_authentic = 1;;is_smtp_info_changed=true;}
			else {smtp_info.is_need_authentic = 0;;is_smtp_info_changed=true;}
		}
		if( strcmp(commandType,"checkEventMail") ==0 )
		{
			if( strcmp(param,"YES") ==0 ){ smtp_info.is_send_mail_Event = 1;is_smtp_info_changed=true;}
			else {smtp_info.is_send_mail_Event = 0;is_smtp_info_changed=true;};
		}
		pcRxString = strstr(pcRxString,(const char*)"&");
		
		if( strcmp(commandType,"userMail2") ==0 ){sprintf(emailAddress.email,"%s",param);flash_write_email_list(1,&emailAddress);};
		if( strcmp(commandType,"userMail3") ==0 ){sprintf(emailAddress.email,"%s",param);flash_write_email_list(2,&emailAddress);};
		if( strcmp(commandType,"userMail4") ==0 ){sprintf(emailAddress.email,"%s",param);flash_write_email_list(3,&emailAddress);};
		if( strcmp(commandType,"userMail5") ==0 ){sprintf(emailAddress.email,"%s",param);flash_write_email_list(4,&emailAddress);};
	}
	if(is_smtp_info_changed)flash_write_smtp_info(&smtp_info);
	// 여기까지해서 넘어온 데이타는 다 처리했다.
	
	webHTML_netconn_write(pxNetCon,webHTTP_OK);

	webHTML_netconn_write(pxNetCon,webHTML_HEAD_START);

	webHTML_netconn_write(pxNetCon,webHTML_HEAD_END);
	sprintf( cDynamicPage,"<script>\
	var mailServer='%s';\
	var userSendMail='%s';\
	var userMailid='%s';\
	var userMailPasswd='%s';\
	var checkSeverAuthentic=%d;\
	var checkEventMail=%d;\
	",
	smtp_info.e_mail_server	,
	smtp_info.sender_mail_id	,
	smtp_info.email_user_id	,
	smtp_info.email_user_passwd	,
	smtp_info.is_need_authentic	,
	smtp_info.is_send_mail_Event
	);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	for(int i = 0 ; i< 10 ; i ++)
	{
		flash_read_email_list(i,&emailAddress);
		sprintf( cDynamicPage,"var userMail%d='%s';",i+1,emailAddress.email);
		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	}
	webHTML_netconn_write(pxNetCon,"</script>");
	//webHTML_netconn_write(pxNetCon,webHTML_setupEmail_script);
	webHTML_netconn_write(pxNetCon,webHTML_CSS);
	webHTML_netconn_write(pxNetCon,webHTML_menu);
	webHTML_netconn_write(pxNetCon,webHTML_EMAILSETUP_content);
	webHTML_netconn_write(pxNetCon,webHTML_default_START_footer);
}
*/
void setupUpsReload(struct netconn *pxNetCon,char *commandType){
	//sprintf( cDynamicPage, "<script>alert('%s'+' 설정완료!');window.location.assign(window.location.pathname)</script>",commandType);		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
}
void html_SETUP_UPS(struct netconn *pxNetCon, portCHAR *commandType,portCHAR *parameter)
{
	//ups_info_t ups_info;
	webHTML_netconn_write(pxNetCon,indexHtml_1);
	sprintf( cDynamicPage,"<script>var userId='%s';var passwd='%s';",ups_info.user_id, ups_info.passwd);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	sprintf( cDynamicPage,"</script>");
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	webHTML_netconn_write(pxNetCon,setupUpsHtml_2);
	webHTML_netconn_write(pxNetCon,setupUpsHtml_3);
	webHTML_netconn_write(pxNetCon,setupUpsHtml_4);
	webHTML_netconn_write(pxNetCon,footerHtml);
	return;
	/*
	data_ethernet_t ethernet_t;
	int16_t ipaddress[6] ;
	portTickType systemTime=getTimeLong();
	memset(ipaddress,0x00,sizeof(ipaddress));
	flash_read__ethernetInfo(&ethernet_t);   // 기존의 값을 읽는다.
	
	
	webHTML_netconn_write(pxNetCon,webHTTP_OK);
	
	webHTML_netconn_write(pxNetCon,webHTML_HEAD_START);
	
	//Pass routine
	sprintf( cDynamicPage,"<script>\
	var userId='%s';var passwd='%s';",ups_info.user_id, ups_info.passwd);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	sprintf( cDynamicPage,"\
	if(idAns==null){\
		do{\
			var idAns = prompt('사용자 아이디를 입력하여 주십시요');\
		}while(idAns != userId);\
		do{\
			var passAns = prompt('패스워드를 입력하여 주십시요');\
		}while(passAns != passwd );\
	};\
	</script>");

	webHTML_netconn_write(pxNetCon,cDynamicPage);
	webHTML_netconn_write(pxNetCon,webHTML_HEAD_END);
	if( strcmp( commandType,"macaddress_input_text") == 0)
	{
		getHexInString(parameter,ipaddress,6,':');
		ethernet_t.Ethernet_Conf_EthAddr0 = ipaddress[0];
		ethernet_t.Ethernet_Conf_EthAddr1 = ipaddress[1];
		ethernet_t.Ethernet_Conf_EthAddr2= ipaddress[2];
		ethernet_t.Ethernet_Conf_EthAddr3 = ipaddress[3];
		ethernet_t.Ethernet_Conf_EthAddr4 = ipaddress[4];

		if(ipaddress[5]!=0)
		ethernet_t.Ethernet_Conf_EthAddr5 = ipaddress[5];

		flash_write__ethernetInfo(&ethernet_t);  //쓰고
		flash_read__ethernetInfo(&ethernet_t);   // 다시 읽고
		setupUpsReload(pxNetCon,commandType);
	}
	else if( strcmp( commandType,"ipaddress_input_text") == 0)
	{
		getIntegerInString(parameter,ipaddress,4,'.');
		ethernet_t.Ethernet_Conf_IpAddr0 = ipaddress[0];
		ethernet_t.Ethernet_Conf_IpAddr1 = ipaddress[1];
		ethernet_t.Ethernet_Conf_IpAddr2 = ipaddress[2];
		ethernet_t.Ethernet_Conf_IpAddr3 = ipaddress[3];
		flash_write__ethernetInfo(&ethernet_t);  //쓰고
		flash_read__ethernetInfo(&ethernet_t);   // 다시 읽고
		setupUpsReload(pxNetCon,commandType);
	}
	else if( strcmp( commandType,"gateway_input_text") == 0)
	{
		getIntegerInString(parameter,ipaddress,4,'.');
		ethernet_t.Ethernet_Conf_Gateway_Addr0 = ipaddress[0];
		ethernet_t.Ethernet_Conf_Gateway_Addr1 = ipaddress[1];
		ethernet_t.Ethernet_Conf_Gateway_Addr2 = ipaddress[2];
		ethernet_t.Ethernet_Conf_Gateway_Addr3 = ipaddress[3];
		flash_write__ethernetInfo(&ethernet_t);  //쓰고
		flash_read__ethernetInfo(&ethernet_t);   // 다시 읽고
		setupUpsReload(pxNetCon,commandType);
	}
	else if( strcmp( commandType,"subnetmask_input_text") == 0)
	{
		getIntegerInString(parameter,ipaddress,4,'.');
		ethernet_t.Ethernet_Conf_Net_Mask0 = ipaddress[0];
		ethernet_t.Ethernet_Conf_Net_Mask1 = ipaddress[1];
		ethernet_t.Ethernet_Conf_Net_Mask2 = ipaddress[2];
		ethernet_t.Ethernet_Conf_Net_Mask3 = ipaddress[3];
		flash_write__ethernetInfo(&ethernet_t);  //쓰고
		flash_read__ethernetInfo(&ethernet_t);   // 다시 읽고
		setupUpsReload(pxNetCon,commandType);
	}
	else if( strcmp( commandType,"web_port_input_text") == 0)
	{
		ethernet_t.port = atoi(parameter);
		flash_write__ethernetInfo(&ethernet_t);  //쓰고
		flash_read__ethernetInfo(&ethernet_t);   // 다시 읽고
		setupUpsReload(pxNetCon,commandType);
	}
	else if( strcmp( commandType,"trap_address_0") == 0 ){		
		getIntegerInString(parameter,ipaddress,4,'.');		
		write_trap_ipaddress(0,ipaddress);
		setupUpsReload(pxNetCon,commandType);	
	}
	else if( strcmp( commandType,"trap_address_1") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(1,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_2") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(2,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_3") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(3,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_4") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(4,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_5") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(5,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_6") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(6,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_7") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(7,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_8") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(8,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"trap_address_9") == 0 ){		getIntegerInString(parameter,ipaddress,4,'.');		write_trap_ipaddress(9,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"time_server_0") == 0  ){		getIntegerInString(parameter,ipaddress,4,'.');		write_timeserver_ipaddress(0,ipaddress);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"time_server_1") == 0  ){		getIntegerInString(parameter,ipaddress,4,'.');		write_timeserver_ipaddress(1,ipaddress);	}
	else if( strcmp( commandType,"set_systemtime") == 0 ){		write_set_time(parameter);setupUpsReload(pxNetCon,commandType);	}
	else if( strcmp( commandType,"reload_time") == 0 )	 {
		uint8_t reLoadTime  = write_set_reLoadtime(parameter);
		reLoadTime = flash_write_reLoadTime(&reLoadTime);
		setupUpsReload(pxNetCon,commandType);
	}
	sprintf( cDynamicPage, "<script>var autoLoadTime=%d</script>",flash_read_reLoadTime() );		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY ); //
	sprintf( cDynamicPage,"var agentIpAddress='%d.%d.%d.%d';",ethernet_t.Ethernet_Conf_IpAddr0,ethernet_t.Ethernet_Conf_IpAddr1,ethernet_t.Ethernet_Conf_IpAddr2,ethernet_t.Ethernet_Conf_IpAddr3);
	webHTML_netconn_write(pxNetCon,webHTML_websocket_script);
	flash_read__ethernetInfo(&ethernet_t);   // 기존의 값을 읽는다.
	sprintf( cDynamicPage,"<script>\
	var Eventlog = null;\
	var local_ipSet_station_name = { \
		ipaddress:'%d.%d.%d.%d',\
		gateway:'%d.%d.%d.%d',\
		subnetmask:'%d.%d.%d.%d',\
		port:%d,\
		macAddress:'%d.%d.%d.%d.%d.%d' \
	} ; </script>",ethernet_t.Ethernet_Conf_IpAddr0,ethernet_t.Ethernet_Conf_IpAddr1,ethernet_t.Ethernet_Conf_IpAddr2,ethernet_t.Ethernet_Conf_IpAddr3,\
	ethernet_t.Ethernet_Conf_Gateway_Addr0,ethernet_t.Ethernet_Conf_Gateway_Addr1,ethernet_t.Ethernet_Conf_Gateway_Addr2,ethernet_t.Ethernet_Conf_Gateway_Addr3,\
	ethernet_t.Ethernet_Conf_Net_Mask0,ethernet_t.Ethernet_Conf_Net_Mask1,ethernet_t.Ethernet_Conf_Net_Mask2,ethernet_t.Ethernet_Conf_Net_Mask3,\
	ethernet_t.port,\
	ethernet_t.Ethernet_Conf_EthAddr0,ethernet_t.Ethernet_Conf_EthAddr1,ethernet_t.Ethernet_Conf_EthAddr2,ethernet_t.Ethernet_Conf_EthAddr3,ethernet_t.Ethernet_Conf_EthAddr4,ethernet_t.Ethernet_Conf_EthAddr5\
	);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	
	sprintf( cDynamicPage,"<script>var jSystime=0x%04"X16_F"%04"X16_F";jSystime = jSystime*1000; </script>",(u16_t) (systemTime>>16),(u16_t) (systemTime & 0x0000ffff) );netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	
	timerserver_address_t time_server_a,time_server_b;TimeServerIpAddressRead(0,&time_server_a);   TimeServerIpAddressRead(1,&time_server_b);   // 읽고
	sprintf( cDynamicPage,"<script>var timeserver_ipaddress = {ip_address_0:'%d.%d.%d.%d',ip_address_1:'%d.%d.%d.%d' }; </script> ",
	time_server_a.Ethernet_Conf_IpAddr0,time_server_a.Ethernet_Conf_IpAddr1,time_server_a.Ethernet_Conf_IpAddr2,time_server_a.Ethernet_Conf_IpAddr3,
	time_server_b.Ethernet_Conf_IpAddr0,time_server_b.Ethernet_Conf_IpAddr1,time_server_b.Ethernet_Conf_IpAddr2,time_server_b.Ethernet_Conf_IpAddr3);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	sprintf( cDynamicPage,"<script> var trap_ipaddress = { ");netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
		for(int i=0;i<10;i++)
		{
			trap_address_t trap_address;
			trapIpAddressRead(i,&trap_address) ;
			sprintf( cDynamicPage,"ip_address_%d:'%d.%d.%d.%d',",	i,trap_address.Ethernet_Conf_IpAddr0,
			trap_address.Ethernet_Conf_IpAddr1,
			trap_address.Ethernet_Conf_IpAddr2,
			trap_address.Ethernet_Conf_IpAddr3);
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
		}

	webHTML_netconn_write(pxNetCon,"}; </script>  ");
	
	webHTML_netconn_write(pxNetCon,webHTML_setupups_script);
	
	webHTML_netconn_write(pxNetCon,webHTML_CSS);
	
	webHTML_netconn_write(pxNetCon,webHTML_menu);
	webHTML_netconn_write(pxNetCon,webHTML_UPSSETUP_content);
	webHTML_netconn_write(pxNetCon,"<script> (document.getElementById('reload_time')).value=autoLoadTime;</script>");
	
	webHTML_netconn_write(pxNetCon,webHTML_default_START_footer);
	*/
}
void html_default(struct netconn *pxNetCon,Bool bLogview)
{
	//ups_info_t ups_info;
	webHTML_netconn_write(pxNetCon,indexHtml_1);
	sprintf( cDynamicPage,"<script>var userId='%s';var passwd='%s';",ups_info.user_id, ups_info.passwd);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	sprintf( cDynamicPage,"var UpsSystemName='%s';",ups_info.upsIdentManufacturer);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	sprintf( cDynamicPage,"</script>");
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	webHTML_netconn_write(pxNetCon,indexHtml_2);
	webHTML_netconn_write(pxNetCon,indexHtml_3);
	webHTML_netconn_write(pxNetCon,indexHtml_4);
	webHTML_netconn_write(pxNetCon,footerHtml);

	return;

}

void html_SETUP_UPSBASIC(struct netconn *pxNetCon, portCHAR *commandType,portCHAR *parameter)
{
	webHTML_netconn_write(pxNetCon,indexHtml_1);
	sprintf( cDynamicPage,"<script>var userId='%s';var passwd='%s';",ups_info.user_id, ups_info.passwd);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	sprintf( cDynamicPage,"</script>");
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	webHTML_netconn_write(pxNetCon,setupUpsBasic_2);
	webHTML_netconn_write(pxNetCon,setupUpsBasic_3);
	webHTML_netconn_write(pxNetCon,setupUpsBasic_4);
	webHTML_netconn_write(pxNetCon,footerHtml);
	return;
	/*
	data_ethernet_t ethernet_t;
	flash_read__ethernetInfo(&ethernet_t);   // 기존의 값을 읽는다.

	webHTML_netconn_write(pxNetCon,webHTTP_OK);
	
	webHTML_netconn_write(pxNetCon,webHTML_HEAD_START);
	sprintf( cDynamicPage,"<script>var agentIpAddress= document.location.host;</script>",ethernet_t.Ethernet_Conf_IpAddr0,ethernet_t.Ethernet_Conf_IpAddr1,ethernet_t.Ethernet_Conf_IpAddr2,ethernet_t.Ethernet_Conf_IpAddr3);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	//Pass routine
	sprintf( cDynamicPage,"<script>\
	var userId='%s';var passwd='%s';",ups_info.user_id, ups_info.passwd);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	sprintf( cDynamicPage,"\
	if(idAns==null){\
		do{\
			var idAns = prompt('사용자 아이디를 입력하여 주십시요');\
		}while(idAns != userId);\
		do{\
			var passAns = prompt('패스워드를 입력하여 주십시요');\
		}while(passAns != passwd );\
	};\
	</script>");
	webHTML_netconn_write(pxNetCon,cDynamicPage);

	webHTML_netconn_write(pxNetCon,webHTML_websocket_script);
	webHTML_netconn_write(pxNetCon,webHTML_UPSSETUPBASIC_script);
	
	webHTML_netconn_write(pxNetCon,webHTML_HEAD_END);
	webHTML_netconn_write(pxNetCon,webHTML_CSS);
	
	webHTML_netconn_write(pxNetCon,webHTML_menu);
	webHTML_netconn_write(pxNetCon,webHTML_UPSSETUPBASIC_content);
	webHTML_netconn_write(pxNetCon,webHTML_EMAILSETUP_content);
	
	webHTML_netconn_write(pxNetCon,webHTML_default_START_footer);
	*/
}
static void prvweb_ParseHTMLRequest( struct netconn *pxNetCon )
{
	struct netbuf *pxRxBuffer;
	portCHAR *pcRxString;
	unsigned portSHORT usLength;

	pxNetCon->recv_timeout = 2000;
	while(netconn_recv( pxNetCon, &pxRxBuffer)  != ERR_OK)  // 여기서 타임아웃이 없네요... ㅜㅜ
	{
		vTaskDelay( webSHORT_DELAY );
		if(pxNetCon->last_err<0)
		{
			vParTestSetLED(2, pdTRUE);
			LWIP_DEBUGF_UDP(WEB_DEBUG, ("Web delay error %d and return...",pxNetCon->last_err) );
			netconn_close( pxNetCon );
			netconn_delete( pxNetCon );
			netbuf_delete( pxRxBuffer );
			pxNetCon=NULL;
			pxNetCon=NULL;
			vTaskDelay(300);
			vParTestSetLED(2, pdFALSE);
			return;
		}
	}
	
	if( pxRxBuffer != NULL ) netbuf_data( pxRxBuffer, ( void * ) &pcRxString, &usLength );
	LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nWeb Length %d ",usLength) );

	if( pxRxBuffer != NULL )
	{
		*(pcRxString+pxRxBuffer->ptr->len) = '\0';
		
		
		
		if(( NULL != pcRxString               ) && ( NULL != strstr( pcRxString,(const char*) "favicon" ) )   )  // 단어를 포함하면.
		{
			//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\n favicon.ico !! \n"));
			webHTML_netconn_write(pxNetCon,"HTTP/1.1 404 Not Found");
			webHTML_netconn_write(pxNetCon,"Content-Type: 'image/ico'");
			webHTML_netconn_write(pxNetCon,"\r\n");
			webHTML_netconn_write(pxNetCon,"<!DOCTYPE HTML><head>");
			webHTML_netconn_write(pxNetCon,"<html>\r\n");
			webHTML_netconn_write(pxNetCon,"<body>\r\n");
			webHTML_netconn_write(pxNetCon,"<head/>\r\n");
			webHTML_netconn_write(pxNetCon,"</head/>\r\n");
			webHTML_netconn_write(pxNetCon,"</body>\r\n");
			webHTML_netconn_write(pxNetCon,"</html>\r\n");
		}
		else if(( NULL != pcRxString ) && ( NULL != strstr( pcRxString,(const char*) "websocket" ) )  && ( !strncmp( pcRxString, "GET", 3 ) )  )  // 단어를 포함하면.
		{
			vParTestSetLED(2, pdTRUE);
			if( webSocket_proc(pxNetCon, pcRxString) != 0){  // -100
				LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nForce return for error!\n") );
			}
			vTaskDelay(1000);
			vParTestSetLED(2, pdFALSE);
		}
		else if(	( NULL != pcRxString               )   && (NULL != strstr( pcRxString,(const char*) "GET /SETUP_EMAIL.html") )    ) //이제 GET으로 데이타를 확인한다.
		{
			if(strstr(pcRxString,(const char*)"reboot") != NULL) while(1) ;

			char *decode_pcRxString =  urlDecode(pcRxString);
			//html_SETUP_EMAIL(pxNetCon,decode_pcRxString);
			free(decode_pcRxString);
		}
		else if(	( NULL != pcRxString               )   && (NULL != strstr( pcRxString,(const char*) "GET /CHANGE_PASSWD.html") )    )
		{
			webHTML_netconn_write(pxNetCon,indexHtml_1);
			sprintf( cDynamicPage,"<script>var userId='%s';var passwd='%s';",ups_info.user_id, ups_info.passwd);
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
			sprintf( cDynamicPage,"</script>");
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

			webHTML_netconn_write(pxNetCon,change_passwd_2);
			webHTML_netconn_write(pxNetCon,change_passwd_3);
			webHTML_netconn_write(pxNetCon,change_passwd_4);
			webHTML_netconn_write(pxNetCon,footerHtml);
			return;
			/*
			data_ethernet_t ethernet_t;

			flash_read__ethernetInfo(&ethernet_t);   // 기존의 값을 읽는다.

			webHTML_netconn_write(pxNetCon,webHTTP_OK);
			webHTML_netconn_write(pxNetCon,webHTML_passwd_script);
			webHTML_netconn_write(pxNetCon,webHTML_HEAD_START);
			webHTML_netconn_write(pxNetCon,webHTML_HEAD_END);
			webHTML_netconn_write(pxNetCon,webHTML_CSS);
			webHTML_netconn_write(pxNetCon,webHTML_menu);


			sprintf( cDynamicPage,"<script>\
			var userId='%s';var passwd='%s';",ups_info.user_id, ups_info.passwd);
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
			
			sprintf( cDynamicPage,"var agentIpAddress='%d.%d.%d.%d';",ethernet_t.Ethernet_Conf_IpAddr0,ethernet_t.Ethernet_Conf_IpAddr1,ethernet_t.Ethernet_Conf_IpAddr2,ethernet_t.Ethernet_Conf_IpAddr3);
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

			sprintf( cDynamicPage,"\
			if(idAns==null){\
				do{\
					var idAns = prompt('사용자 아이디를 입력하여 주십시요');\
				}while(idAns != userId);\
				do{\
					var passAns = prompt('패스워드를 입력하여 주십시요');\
				}while(passAns != passwd );\
			};\
			</script>");
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
			webHTML_netconn_write(pxNetCon,webHTML_PASSWD_content );

			webHTML_netconn_write(pxNetCon,webHTML_default_START_footer);
			*/
		}
		else if(	( NULL != pcRxString               )   && (NULL != strstr( pcRxString,(const char*) "GET /SETUP_UPS.html") )    ) //이제 GET으로 데이타를 확인한다.
		{
			html_SETUP_UPS(pxNetCon,"","");
		}
		else if(	( NULL != pcRxString               )   && ( NULL != strstr( pcRxString,(const char*) "GET /SETUP_UPSBASIC.html" ) )    )
		{
			html_SETUP_UPSBASIC(pxNetCon,"","");
		}
		else if(	( NULL != pcRxString               )   && ( NULL != strstr( pcRxString, (const char*)"GET /UPS_LOGVIEW.html" ) )    ) //이제 GET으로 데이타를 확인한다.
		{
			html_default(pxNetCon,true);
		}
		else if(	( NULL != pcRxString               )   && ( NULL != strstr( pcRxString,(const char*) "GET /index.html" ) )    ) //이제 GET으로 데이타를 확인한다.
		{
			//LWIP_DEBUGF_UDP(WEB_DEBUG, ("html_default()\n") );
			html_default(pxNetCon,false);
		}
		else if(	( NULL != pcRxString               )   && ( NULL != strstr( pcRxString,(const char*) "GET" ) )    )
		html_default(pxNetCon,false);
	}
	netconn_close( pxNetCon );
	netconn_delete( pxNetCon );
	netbuf_delete( pxRxBuffer );
	pxNetCon=NULL;
	vTaskDelay(300);
}
#define TIME_SERVER_PORT	37
Bool getTimeFromServerExt(int tServer,ip_addr_t server_ipaddr)
{
	//int iReturn =0;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = PP_HTONS(TIME_SERVER_PORT);
	
	timerserver_address_t time_server_a;
	if(server_ipaddr.addr == 0 ){
		TimeServerIpAddressRead(tServer,&time_server_a);   // 읽고
		if(time_server_a.Ethernet_Conf_IpAddr0 == 0 ) return false; // IPADDRESS가 0이면 종료한다.
		addr.sin_addr.s_addr = time_server_a.Ethernet_Conf_IpAddr0 << 24 | time_server_a.Ethernet_Conf_IpAddr1 << 16 | time_server_a.Ethernet_Conf_IpAddr2 << 8 | time_server_a.Ethernet_Conf_IpAddr3  ;
	}
	else
	addr.sin_addr.s_addr = server_ipaddr.addr;
	
	struct netconn *xNetConn = NULL;
	xNetConn = netconn_new ( NETCONN_TCP );
	if(xNetConn==NULL){
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("Connect To Time server Fail xNetConn null.."));
		return 0;
	};
	int ret = netconn_connect ( xNetConn, &addr.sin_addr, addr.sin_port );
	if( ret != ERR_OK)	{
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("Connect To Time server Fail 1.."));
		ip_addr_debug_print(WEB_DEBUG, &(addr.sin_addr.s_addr));
		netconn_disconnect(xNetConn);
		netconn_delete(xNetConn);
		return 0;
	}
	struct netbuf *pxRxBuffer;
	char *send_buf ;
	//netconn_write(xNetConn, send_buf, strlen(send_buf), NETCONN_DONTBLOCK);vTaskDelay(100);
	netconn_set_recvtimeout ( xNetConn, 1000 );
	if(netconn_recv( xNetConn, &pxRxBuffer)  == ERR_OK){
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nReceive Time Data :  "));
	}
	else{
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nNo Time Data Receive"));
	};
	if( pxRxBuffer == NULL )
	{
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\nData Null"));
	};
	int usLength=0;
	typedef union ServerTimeUnion
	{
		char Buffer[4];
		uint32_t  time;
	} ServerTimeUnion_t;
	ServerTimeUnion_t TimeUnion;
	memset(TimeUnion.Buffer,0x00,4);
	if(pxRxBuffer != NULL){
		char *p;
		netbuf_data( pxRxBuffer, ( void * )&p, &usLength );

		if( pxRxBuffer->ptr->tot_len ==4 ){
			for(int i=0;i<4;i++)
			TimeUnion.Buffer[i] =  p[i];

			TimeUnion.time -= 6*3600 ;// for local time
			TimeUnion.time -= 35*60 ;// for local time
			TimeUnion.time -= 54 ;// for local time
			setSystemTimeLong(TimeUnion.time);
			breakTime(TimeUnion.time);
			rtc_set_value(&AVR32_RTC, TimeUnion.time);
			rtc_enable(&AVR32_RTC);
			LWIP_DEBUGF_UDP(WEB_DEBUG, ("\r\nWeb Server Started %d:%d:%d %d:%d:%d systemTime=%u ",
			year(),month(),day()+1,hour(),minute(),tmsecond(),getTimeLong()  ));
		}
	}
	netbuf_delete( pxRxBuffer );
	netconn_disconnect(xNetConn);
	netconn_delete(xNetConn);
	mem_free(send_buf);
	return 1;

}

Bool getTimeFromServer(void)
{
	ip_addr_t addr;
	if( getTimeFromServerExt(0,addr)==false )
	{
		if( getTimeFromServerExt(1,addr)==true ) return true;
	}
	else return false;
}
#define MAX_ARG_COUNT	20
uint8_t cmd_sub_id(portCHAR* cmd,  KeyAndValue *array,int pos) {
	portCHAR *argv[3];
	int i = 0;
	argv[i] = strtok(cmd, "=");
	if (argv[i] != NULL)
	strncpy(array[pos].id, argv[i] , min(sizeof(argv[i] ),MAX_ARG_COUNT));
	do {
		argv[++i] = strtok(NULL, "=");
		if(argv[i]!=NULL) strncpy(array[pos].value, argv[i], min(strlen(argv[i] ),MAX_ARG_COUNT));
	} while (i < 2 && (argv[i] != NULL));
	return 0;
}


uint8_t cmd_parseEx(portCHAR *pcRxString, KeyAndValue *array,int argCount) 
{
	int i=0;
	int j=0;
	portCHAR *argv[1];
	portCHAR temp[20];
	*argv = strtok(pcRxString, "?");
	if(*argv != NULL) strcpy(array[i].id,argv[0]);

	*argv = strtok(NULL, "?");
	if(argv[0]!=NULL)
			strcpy(pcRxString, argv[0]);

	//*argv = strtok(pcRxString, "&");

	i++;
	for(j=0;j<20;j++)temp[j]=0;
	j=0;
	while( *(pcRxString+j) !=  '=')	
	{
		temp[j++] =*(pcRxString+j) ; 	
	}
	strcpy( array[i].id, temp);
	pcRxString = (pcRxString+j+1) ;

	for(j=0;j<20;j++)temp[j]=0;
	j=0;
	while( *(pcRxString+j) !=  '&')	
	{
		temp[j++] =*(pcRxString+j) ; 	
	}
	strcpy( array[i].value, temp);
	pcRxString = (pcRxString+j+1) ;


	i++;
	for(j=0;j<20;j++)temp[j]=0;
	j=0;
	while( *(pcRxString+j) !=  '=')	
	{
		temp[j++] =*(pcRxString+j) ; 	
	}
	strcpy(array[i].id, temp);
	pcRxString = (pcRxString+j+1) ;

	for(j=0;j<20;j++)temp[j]=0;
	j=0;
	while( *(pcRxString+j) !=  '&')	
	{
		temp[j++] =*(pcRxString+j) ; 	
	}
	strcpy( array[i].value, temp);
	pcRxString = (pcRxString+j+1) ;

}
uint8_t cmd_sub_idEx(portCHAR* cmd,  KeyAndValue *array) {
	portCHAR *argv[1];
	int i = 0;
	*argv = strtok(cmd, "=");
	if (*argv != NULL)
		strncpy(array->id, argv[0] , strlen(argv[0]));
	*argv = strtok(NULL, "=");
	if (*argv != NULL)
		strncpy(array->value, argv[0] , strlen(argv[0]));

	return 0;
}
uint8_t cmd_parse(portCHAR *pcRxString, KeyAndValue *array,int argCount) 
{
	uint8_t i = 0;
	portCHAR *argv[20]; 
	portCHAR *temp[20];

	char *mem; 
	mem = (char *)malloc(MAX_ARG_COUNT* 20);
	if(argCount > 20)
	{
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\n Exceed argCount Error!!!!!  "));
		return 0; 
	}
	if( mem == NULL){ 
		LWIP_DEBUGF_UDP(WEB_DEBUG,("\r\n cmd_parse Memory Alloc Error!!!!!  "));
		return 0; 
	}
	memset(mem, 0x00, MAX_ARG_COUNT*20);
	for (int k = 0; k< 20; k++) { temp[k] = mem + k*MAX_ARG_COUNT; };
	
	argv[i] = strtok(pcRxString, "?");
	if(argv[i]!=NULL){
		sprintf(temp[i],"%s" ,argv[i]);
		argv[i] = strtok(NULL, "?");
		if(argv[i]!=NULL)
		{
			strcpy(pcRxString, argv[i]);// 이렇게 사용해도 된다.pcRxString=&argv[i];
		}
		else pcRxString = 0x00;
	}
	// 이제 다시 시작한다.
	++i;
	if(pcRxString!=NULL)
	{
		argv[i] = strtok(pcRxString, "&");
		if (argv[i] != NULL) {
			strncpy(temp[i], argv[i],min(strlen(argv[i]),MAX_ARG_COUNT));
		}
		do {
			argv[++i] = strtok(NULL, "&");
			if (argv[i] != NULL) {
				strncpy(temp[i], argv[i],min(strlen(argv[i]),MAX_ARG_COUNT));
			}
		} while (i< argCount && (argv[i] != NULL));
	}
	for (uint8_t j = 0; j < i; j++) {
		cmd_sub_id(temp[j], array, j);
	}
	free(mem);
	return i;
}
uint8_t parseDate(uint8_t *year,uint8_t *month, uint8_t *day , char *datetime){
	char *argv[3];
	uint8_t i = 0;
	argv[i] = strtok(datetime,"/"); //year
	*year =0; 	*month =0; 	*day = 0;
	do{
		argv[++i] = strtok(NULL,"/");
	} while( i< 2 && (argv[i] != NULL));
	if( i<1 ) return -1;
	*year = atoi(argv[0])-2000;
	*month = atoi(argv[1]);
	*day = atoi(argv[2]);
}
uint8_t parseTime(uint8_t *hour,uint8_t *minute , char *strTime){
	char *argv[2];
	uint8_t i = 0;
	argv[i] = strtok(strTime,"/"); //year
	*hour =0;
	*minute =0;
	do{
		argv[++i] = strtok(NULL,"/");
	} while( i< 2 && (argv[i] != NULL));
	if( i==0 ) return -1;
	*hour = atoi(argv[0]);
	*minute = atoi(argv[1]);
	if( *hour >= 24)*hour= 23;
	if( *minute >= 60)*minute= 59;
}
#endif

	/*
	netconn_write( pxNetCon, webHTTP_OK, (u16_t) strlen( webHTTP_OK ), NETCONN_COPY );
	webHTML_netconn_write(pxNetCon,webHTML_HEAD_START);
	webHTML_netconn_write(pxNetCon,webHTML_HEAD_END);
	uint16_t *pData ;
	
	pData =&upsModeBusData ;

	sprintf( cDynamicPage,"<script> var autoLoadTime = %d;  setInterval( function(){ WebSocketToSnmp('UPS_EX_DATA','');}, autoLoadTime);</script>", flash_read_reLoadTime()*1000*360) ;
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	portTickType snmp_systemTime=getTimeLong();
	pData[10] =  (uint16_t)(0xFFFF & snmp_systemTime);
	pData[11] =  (uint16_t)(snmp_systemTime>>16);

	sprintf( cDynamicPage,"<script>");
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	sprintf( cDynamicPage,"\
	var upsModeBusData = { \
		Year_made:%d,\
		Month_made:%d,\
		Date_made:%d,\
		Ups_Capacitor:%d,\
		Input_Phase:%d,\
		Input_Voltage:%d,\
		Output_Phase:%d,\
		Output_Voltage:%d,\
		Company_code_And_upstype:%d,\
		Installed_Battery_Cells:%d,",
		pData[0] ,pData[1]  ,pData[2] ,pData[3] ,pData[4] ,pData[5] ,pData[6] ,pData[7] ,pData[8] ,pData[9]\
		);
		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
		
		sprintf( cDynamicPage,"\
		reserved_2:%d,\
		BMS_1_2_STATE:%d,\
		Converter_State:%d,\
		Inverter_State:%d,\
		Converter_Operation_Fault:%d,\
		Inverter_Operation_Fault:%d,\
		Input_r_volt_rms:%d,\
		Input_s_volt_rms:%d,\
		Input_t_volt_rms:%d,\
		Input_r_current_rms:%d,",\
		pData[10],pData[11],pData[12],pData[13],pData[14],pData[15],pData[16],pData[17],pData[18],pData[19]\
		);
		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
		
		sprintf( cDynamicPage,"\
		Input_s_current_rms:%d,\
		Input_t_current_rms:%d,\
		Input_frequency:%d,\
		Bypass_r_volt_rms:%d,\
		Bypass_s_volt_rms:%d,\
		Bypass_t_volt_rms:%d,\
		Bypass_r_current_rms:%d,\
		Bypass_s_current_rms:%d,\
		Bypass_t_current_rms:%d,\
		Bypass_Frequency:%d,",\
		pData[20],pData[21],pData[22],pData[23],pData[24],pData[25],pData[26],pData[27],pData[28],pData[29]\
		);
		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
		
		
		sprintf( cDynamicPage,"\
		Inverter_u_volt_rms:%d,\
		Inverter_v_volt_rms:%d,\
		Inverter_w_volt_rms:%d,\
		Inverter_u_curr_rms:%d,\
		Inverter_V_curr_rms:%d,\
		Inverter_W_curr_rms:%d,\
		Inverter_Frequency:%d,\
		Bat_volt_rms:%d,\
		Bat_current_rms:%d,\
		Input_kva_address_KVA:%d,\
		Input_kw_KW:%d,",\
		pData[30],pData[31],pData[32],pData[33],pData[34],pData[35],pData[36],pData[37],pData[38],pData[39],pData[40]\
		);
		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
		
		sprintf( cDynamicPage,"\
		Input_kvar_KVAR:%d,\
		Input_power_factor_Pf:%d,\
		Output_r_volt_rms:%d,\
		Output_s_volt_rms:%d,\
		Output_t_volt_rms:%d,\
		Output_u_current_rms:%d,\
		Output_v_current_rms:%d,\
		Output_w_current_rms:%d,\
		Output_frequency:%d,\
		Output_kva_KVA:%d,\
		",\
		pData[41],pData[42],pData[43],pData[44],pData[45],pData[46],pData[47],pData[48],pData[49],pData[50]\
		);
		netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
		
		sprintf( cDynamicPage,"\
		Output_kw_KW:%d,\
		Output_kvar_KVAR:%d,\
		Output_Power_factor_Pf:%d,\
		Output_R_Load:%d,\
		Output_S_Load:%d,\
		Output_T_Load:%d,\
		BMS_Bat_Voltage:%d,\
		Battery_Room_Temper:%d,\
	};",\
	pData[51],pData[52],pData[53],pData[54],pData[55],pData[56],pData[57],pData[58]\
	);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	
	unsigned long sssTime = getTimeLong() ;
	webHTML_netconn_write(pxNetCon,"var snmpDateTime = new Date() ;");
	
	sprintf( cDynamicPage,"	var snmp_time_value_text = '%d/%02d/%02d %02d:%02d:%02d';", year(),month(),day()+1,hour(),minute(),tmsecond() );
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	
	webHTML_netconn_write(pxNetCon,"var dataCount=0;\
	function doDate(){\
		var str;var now = new Date();\
		str = now.getFullYear()+'/' +  (now.getMonth()+1)+'/'  +  now.getDate() + ' ' + now.getHours() +':' + now.getMinutes() + ':' + now.getSeconds();\
		document.getElementById('system_time_value').innerHTML = str;\
		dataCount++;\
		if( dataCount*1000 >= autoLoadTime )dataCount =0;\
		document.getElementById('status_bar').innerHTML =  dataCount;}\
		setInterval(doDate, 1000);");
	

	sprintf(cDynamicPage,"function imageDivHide(){	if(  document.getElementById('imageDiv').style.display == 'block' ||    document.getElementById('imageDiv').style.display == '' ) {		document.getElementById('imageDiv').style.display = 'none';		document.getElementById('statusInfoDiv').style.display = 'block'	}}");
	       	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );

	sprintf(cDynamicPage,"function imageDivShow(){	if(  document.getElementById('imageDiv').style.display == 'none'  ) {		document.getElementById('imageDiv').style.display = 'block';		document.getElementById('statusInfoDiv').style.display = 'none'	}};");
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );


	data_ethernet_t ethernet_t;
	int16_t ipaddress[6] ;

	memset(ipaddress,0x00,sizeof(ipaddress));
	flash_read__ethernetInfo(&ethernet_t);   // 기존의 값을 읽는다.
	sprintf( cDynamicPage,"var agentIpAddress='%d.%d.%d.%d';",ethernet_t.Ethernet_Conf_IpAddr0,ethernet_t.Ethernet_Conf_IpAddr1,ethernet_t.Ethernet_Conf_IpAddr2,ethernet_t.Ethernet_Conf_IpAddr3);
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	
	sprintf( cDynamicPage,"var systemTime='%u'*1000- 2208965020*1000 ;var systemTime_1='%u';",snmp_systemTime,sssTime );
	netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
	
	
	webHTML_netconn_write(pxNetCon,"</script>");
	
	{
		log_t tlog;
		int log_start_pos = getFirstLog(&tlog);
		webHTML_netconn_write(pxNetCon,"<script> var Eventlog = [");
		while(tlog.kind != 0xff)
		{
			sprintf( cDynamicPage,"{time:0x%04"X16_F"%04"X16_F",kind:%d,event:%d},",(u16_t) (tlog.systemTime>>16),(u16_t) (tlog.systemTime & 0x0000ffff),tlog.kind, tlog.event);
			netconn_write( pxNetCon, cDynamicPage, (u16_t) strlen( cDynamicPage ), NETCONN_COPY );
			log_start_pos++;
			if(log_start_pos >= MAX_LOG) break;
			getNextLog(log_start_pos,&tlog);
		}
		webHTML_netconn_write(pxNetCon,"]; </script> ");
	}
	
	webHTML_netconn_write(pxNetCon,webCanvasScript);
	webHTML_netconn_write(pxNetCon,webHTML_websocket_script);
	webHTML_netconn_write(pxNetCon,webHTML_default_script);
	webHTML_netconn_write(pxNetCon,webHTML_CSS);
	webHTML_netconn_write(pxNetCon,webHTML_menu);
	webHTML_netconn_write(pxNetCon,webHTML_default_START_content);
	webHTML_netconn_write(pxNetCon,webHTML_default_START_log);
	webHTML_netconn_write(pxNetCon,webHTML_default_START_footer);
*/

