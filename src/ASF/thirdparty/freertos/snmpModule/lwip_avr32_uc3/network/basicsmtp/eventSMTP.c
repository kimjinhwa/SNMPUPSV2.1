#include <string.h>

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"
#include "eventSMTP.h"

// Demo includes.
#include "portmacro.h"
#include "partest.h"
#include "intc.h"
#include "gpio.h"

// lwIP includes.
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/memp.h"
#include "lwip/stats.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/arch.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/init.h"
#if ( (LWIP_VERSION) == ((1U << 24) | (3U << 16) | (2U << 8) | (LWIP_VERSION_RC)) )
#include "netif/loopif.h"
#endif

//! SMTP default port
#define SMTP_PORT     25
//#define SMTP_PORT     587
//! SMTP EHLO code answer
#define SMTP_EHLO_STRING                  "220"
//! SMTP end of transmission code answer
#define SMTP_END_OF_TRANSMISSION_STRING   "221"
//! SMTP OK code answer
#define SMTP_OK_STRING                    "250"
//! SMTP start of transmission code answer
#define SMTP_START_OF_TRANSMISSION_STRING "354"
//! SMTP DATA<CRLF>
#define SMTP_DATA_STRING                  "DATA\r\n"
//! SMTP <CRLF>.<CRLF>
#define SMTP_MAIL_END_STRING              "\r\n.\r\n"
//! SMTP QUIT<CRLFCRLF>
#define SMTP_QUIT_STRING                  "QUIT\r\n"


//! Server address
//#error configure SMTP server address
//portCHAR cServer[] = "192.168.0.1";
portCHAR cServer[] = "192.168.0.252";
//portCHAR cServer[] = "1.241.236.190";

//! Fill here the mailfrom with your mail address
//#error configure SMTP mail sender
char cMailfrom[] = "MAIL FROM: <mvcjhkim@mail.iftech.co.kr>\r\n";

//! Fill here the mailto with your contact mail address
//#error configure SMTP mail recipient
char cMailto[] = "RCPT TO: <mvcjhkim@mail.iftech.co.kr>\r\n";

//! Fill here the mailcontent with the mail you want to send
//#error configure SMTP mail content
char cMailcontent[] ="Subject: *** my snmp mail test ***\r\nFROM: \"UPS\" <mvcjhkim@gmail.com>\r\nTO: \"Dear manager\" <mvcjhkim@gmail.com>\r\n\r\nNot emergency Just Test here.";

//! buffer for SMTP response
portCHAR cTempBuffer[200];


//_____ D E C L A R A T I O N S ____________________________________________
//! interrupt handler.
#if __GNUC__
__attribute__((__naked__))
#elif __ICCAVR32__
#pragma shadow_registers = full   // Naked.
#endif
void vpushb_ISR( void );

//! soft interrupt handler. where treatment should be done
#if __GNUC__
__attribute__((__noinline__))
#endif
static portBASE_TYPE prvpushb_ISR_NonNakedBehaviour( void );



//! Basic SMTP client task definition
#include "myDNS.h"

void sendEventMail(char *mess)
{
	//int lSocket = -1;
	struct netconn *xNetConn = NULL;
	struct ip_addr local_ip; 
	struct ip_addr remote_ip; 
	int loopcount=0;
	struct netbuf *netbuffer;
	portCHAR *pcRxString;
	int rc1, rc2;
	
	LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nemail socket open"));
	//xNetConn = netconn_new ( NETCONN_TCP );
	if ( xNetConn == NULL ) { 
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nNetconn new error...No memory for new connection?"));
		return;
	}
	local_ip.addr = inet_addr("192.168.0.55");
	remote_ip.addr = inet_addr("192.168.0.252");
	
	//taskENTER_CRITICAL() ;
	//rc1 = netconn_bind ( xNetConn, &local_ip, 0 ); 
	//taskEXIT_CRITICAL();
	
	if( netconn_close(xNetConn) != ERR_OK ) LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nNetconn close error.."));;
	if( netconn_delete(xNetConn) != ERR_OK ) LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nNetconn delete error.."));;
	xNetConn = NULL;
	LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nemail socket close"));
	vTaskDelay(200);
	return;
	
	//xNetConn = netconn_new ( NETCONN_TCP ); 
	
	netconn_set_nonblocking(xNetConn, false);
	LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nemail socket open"));
	rc1 = netconn_bind ( xNetConn, &local_ip, 0 ); 
	vTaskDelay( 200 );
	rc2 = netconn_connect ( xNetConn, &remote_ip, SMTP_PORT );
	vTaskDelay( 200 );
	if ( rc1 != ERR_OK ||  !( rc2 == ERR_OK || rc2 == -5 )  )
	{
	  netconn_close(xNetConn);
	  netconn_delete ( xNetConn );
	  LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nNetconn connect error..%d %d.",rc1,rc2));
	  return;
	}
	//
	//while(netconn_recv( xNetConn, &netbuffer)  != ERR_OK)
	//{
		//vTaskDelay( 100 );
		//if(loopcount > 10) break;
		//loopcount++;
	//}
	//
	//
	//if( pxRxBuffer != NULL )
    //{
      //unsigned portSHORT usLength;
	  //netbuf_data( netbuffer, ( void * ) &pcRxString, &usLength );
	  //*(pcRxString+usLength) = 0x00;
	  //LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSMTP Ready..%s %d",netbuffer,usLength));
	//}
	//
	//lSocket = socket(AF_INET, SOCK_STREAM, 0) ;
	
	netbuf_delete( netbuffer );
	vTaskDelay( 100 );
	netbuf_free(netbuffer);
	vTaskDelay( 100 );
	vTaskDelay( 100 );
	netconn_close(xNetConn);
	vTaskDelay( 100 );
	netconn_delete(xNetConn);
	vTaskDelay( 100 );
	//xNetConn = NULL;
	LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nemail socket close"));
	return;
	//close(lSocket);
}

/*
void sendEventMail(char *mess)
{
  struct sockaddr_in stServeurSockAddr;
  int lRetval;
  int lSocket = -1;
  int iReturn =0;
  int wait_count =0;
  struct ip_addr resoved_addr;
  const char mail_server_name ="mail.iftech.co.kr";
  //err_t errt = gethost_byname ( mail_server_name, &resoved_addr );
  //dns_gethostbyname
  
    memset(&stServeurSockAddr, 0, sizeof(stServeurSockAddr));
    stServeurSockAddr.sin_len = sizeof(stServeurSockAddr);
	//stServeurSockAddr.sin_addr.s_addr = resoved_addr.addr;
	stServeurSockAddr.sin_addr.s_addr = inet_addr("192.168.0.252");
	//stServeurSockAddr.sin_addr.s_addr = inet_addr("1.241.236.190");
    stServeurSockAddr.sin_port = PP_HTONS(SMTP_PORT);
    stServeurSockAddr.sin_family = AF_INET;
    //stServeurSockAddr.sin_addr.s_addr = inet_addr(cServer);

    // socket as a stream
    if ( (lSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	//if ( (lSocket = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      // socket failed, blink a LED and stay here
      for (int i=0;i<3;i++) {
        vParTestToggleLED( 4 );
        vTaskDelay( 200 );
      }
      close(lSocket);
	  return;
    }
	
	u32_t opt ;
	opt = lwip_fcntl(lSocket, F_GETFL, 0);
	opt |= O_NONBLOCK;
	iReturn = lwip_fcntl(lSocket, F_SETFL, opt);
    // connect to the server
    //if(connect(lSocket,(struct sockaddr *)&stServeurSockAddr, sizeof(stServeurSockAddr)) < 0)

	iReturn = lwip_connect(lSocket,(struct sockaddr *)&stServeurSockAddr, sizeof(stServeurSockAddr));
	LWIP_DEBUGF_UDP(WEB_DEBUG, ("\lwip_connect return =%d\n",iReturn) );	
	vTaskDelay( 10 );
	iReturn =0;
	if( iReturn < 0)
    {
      // connect failed, blink a LED and stay here
      for (int i=0;i<3;i++) {
        vParTestToggleLED( 4 );
        vTaskDelay( 200 );
      }
      close(lSocket);
	  return;
    }
    else
    {
//Server: 220 SMTP Ready
      // wait for SMTP Server answer
	  wait_count=0;
      do
      {
        lRetval = recv(lSocket, cTempBuffer, sizeof(cTempBuffer), 0);
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nwait for SMTP Server answer=%s\n",lRetval) );
		vTaskDelay( 200 );
		wait_count++;
		if(wait_count > 5) 
		{
			 close(lSocket);
			 return;
		}
      }while (lRetval <= 0);
      if (strncmp(cTempBuffer, SMTP_EHLO_STRING, sizeof(cTempBuffer)) >= 0)
      {
//Client: EHLO smtp.domain.com
        // send ehlo
		LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSMTP_OK_STRING_0=%s\n",cTempBuffer) );	
        send(lSocket, "HELO ", 5, 0);
        send(lSocket, cServer, strlen(cServer), 0);
        send(lSocket, "\r\n", 2, 0);
//Server: 250
        // wait for SMTP Server answer
		wait_count=0;
        do
        {
          lRetval = recv(lSocket, cTempBuffer, sizeof(cTempBuffer), 0);
				vTaskDelay( 200 );
		  		wait_count++;
		  		if(wait_count > 5)
		  		{
			  		close(lSocket);
			  		return;
		  		}

        }while (lRetval <= 0);
        if (strncmp(cTempBuffer, SMTP_OK_STRING, sizeof(cTempBuffer)) >= 0)
        {
//Client: MAIL FROM:<sender@domain.com>
          // send MAIL FROM
		  LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSMTP_OK_STRING_1=%s\n",cTempBuffer) );	
          send(lSocket, cMailfrom, strlen(cMailfrom), 0);
//Server: 250 OK
          // wait for SMTP Server answer
		  wait_count=0;
          do
          {
            lRetval = recv(lSocket, cTempBuffer, sizeof(cTempBuffer), 0);
			vTaskDelay( 200 );
		  		wait_count++;
		  		if(wait_count > 5)
		  		{
			  		close(lSocket);
			  		return;
		  		}

          }while (lRetval <= 0);
          if (strncmp(cTempBuffer, SMTP_OK_STRING, sizeof(cTempBuffer)) >= 0)
          {
//Client: RCPT TO:<receiver@domain.com>
            // send RCPT TO
			LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSMTP_OK_STRING_2=%s\n",cTempBuffer) );	
            send(lSocket, cMailto, strlen(cMailto), 0);
//Server: 250 OK
            // wait for SMTP Server answer
			wait_count=0;
            do
            {
              lRetval = recv(lSocket, cTempBuffer, sizeof(cTempBuffer), 0);
			  vTaskDelay( 200 );
		  		wait_count++;
		  		if(wait_count > 5)
		  		{
			  		close(lSocket);
			  		return;
		  		}
            }while (lRetval <= 0);
            if (strncmp(cTempBuffer, SMTP_OK_STRING, sizeof(cTempBuffer)) >= 0)
            {
//Client: DATA<CRLF>
              // send DATA
			  LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSMTP_OK_STRING_3=%s\n",cTempBuffer) );	
              send(lSocket, SMTP_DATA_STRING, 6, 0);
//Server: 354 Start mail input; end with <CRLF>.<CRLF>
              // wait for SMTP Server answer
			  wait_count=0;
              do
              {
                lRetval = recv(lSocket, cTempBuffer, sizeof(cTempBuffer), 0);
		  		if(wait_count > 5)
		  		{
			  		close(lSocket);
			  		return;
		  		}
				
              }while (lRetval <= 0);
              if (strncmp(cTempBuffer, SMTP_START_OF_TRANSMISSION_STRING, sizeof(cTempBuffer)) >= 0)
              {
                // send content
				LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSMTP_OK_STRING_4=%s\n",cTempBuffer) );	
				//mess
                //send(lSocket, cMailcontent, strlen(cMailcontent), 0);
				//"Subject: *** SPAM ***\r\nFROM: \"Your Name here\" <sender@domain.com>\r\nTO: \"Your Contact here\" <recipient@domain.com>\r\n\r\nSay what you want here.";
				//sprintf(cTempBuffer,"Subject: SNMP EVENT MESSAGE\r\nFROM: \"IFTECH SNMP Manager\" <mvcjhkim@mail.iftech.co.kr>\r\nTO: \"SNMP MANAGER\" <manager@kepco.com>\r\n\r\n%s.",mess);
				//LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nmessage=%s\n",cTempBuffer) );	
				send(lSocket, cMailcontent, strlen(cMailcontent), 0);
//Client: <CRLF>.<CRLF>
                // send "<CRLF>.<CRLF>"
                send(lSocket, SMTP_MAIL_END_STRING, 5, 0);
//Server: 250 OK
                // wait for SMTP Server answer
				wait_count=0;
                do
                {
                  lRetval = recv(lSocket, cTempBuffer, sizeof(cTempBuffer), 0);
		  			if(wait_count > 5)
		  			{
			  			close(lSocket);
			  			return;
		  			}
                }while (lRetval <= 0);
                if (strncmp(cTempBuffer, SMTP_OK_STRING, sizeof(cTempBuffer)) >= 0)
                {
//Client: QUIT<CRLFCRLF>
                  // send QUIT
				  LWIP_DEBUGF_UDP(WEB_DEBUG, ("\nSMTP_OK_STRING_5=%s\n",cTempBuffer) );	
                  send(lSocket, SMTP_QUIT_STRING, 8, 0);
//Server: 221 smtp.domain.com closing transmission
					wait_count =0;
                  do
                  {
                    lRetval = recv(lSocket, cTempBuffer, sizeof(cTempBuffer), 0);
					wait_count++;
		  			if(wait_count > 5)
		  			{
			  			close(lSocket);
			  			return;
		  			}
					
                  }while (lRetval <= 0);
                  if (strncmp(cTempBuffer, SMTP_END_OF_TRANSMISSION_STRING, sizeof(cTempBuffer)) >= 0)
                  {
                    vParTestSetLED( 3 , pdTRUE );
                  }
                }
              }
            }
          }
        }
        // close socket
        close(lSocket);
      }
    }
  
}
*/