/*
 * print_funcs_tcpip.c
 *
 * Created: 2017-12-04 09:52:50
 *  Author: STELLA
 */ 
/* lwIP includes. */

/* Standard includes. */

#include <stdio.h>
#include <string.h>

#include "conf_eth.h"



/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "partest.h"


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

#ifdef LWIP_DEBUGF_TCPIP
#include "print_funcs_tcpip.h"

//#define SOCK_DEBUG_PORT	8082
//#define SOCK_DEBUG_HOST	"192.168.0.252"

int debug_socket=-1;

int print_dbg_tcpip_connect(const char *debug_host,int debug_port)
{
/*
	int ret ;
	if ( debug_socket != -1 || debug_socket !=  ((u32_t)0xffffffffUL) ) return debug_socket;
	
	struct sockaddr_in addr;	
	memset(&addr, 0, sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = PP_HTONS(debug_port);
	
	addr.sin_addr.s_addr = inet_addr(debug_host);
	
	//pxNetCon->recv_timeout = 1500;
	debug_socket = lwip_socket(AF_INET, SOCK_STREAM, 0);
	/ nonblocking 
    u32_t opt ;
	lwip_fcntl(debug_socket, F_GETFL, 0);
	opt |= O_NONBLOCK;
	ret = lwip_fcntl(debug_socket, F_SETFL, opt);
	
	ret = lwip_connect(debug_socket, (struct sockaddr*)&addr, sizeof(addr));
*/	
	//lwip_write(debug_socket,"Connected",9);
	//if(ret == 0 )
	//	LWIP_DEBUG_TCPIP(WEB_DEBUG, ("Connected Socket = %d  ",debug_socket) ) ;

	//lwip_write(debug_socket,"Connected",9);
	//return debug_socket;
	// SO_RCVTIMEO
	// SO_CONTIMEO 
	// SO_SNDTIMEO
	return 0;
}

int print_dbg_tcpip_close()
{
	if( debug_socket >= 0) 
	if( lwip_close(debug_socket)	== -1)
		debug_socket = -1  ;
}


int8_t print_dbg_tcpip(const char *str)
{
	/*
	int len;
	for(len=0; len < 254 ;len++ )	
	{
		if( *(str+len) == NULL) break;
	}
	len++;	
	*/
	struct sockaddr_in name;	
	memset(&name, 0, sizeof(name));
	lwip_getpeername (debug_socket,&name, sizeof(name));
	if( name.sin_port == 0 )
	{
		if( debug_socket >= 0) 
		{
			lwip_close(debug_socket);
			debug_socket=-1;
		}
		debug_socket = print_dbg_tcpip_connect("192.168.0.252",8082);
	}
	if( debug_socket >= 0)
	{
		 lwip_write(debug_socket, str, strlen(str));
	}
	
}

int8_t print_dbg_char_hex_tcpip(const char *str, int len)
{
	char buf[7];
	if( debug_socket < 0) return -1;
	for(int i ; i< len; i++)
	{
		for(int k ; k< 7; k++)buf[k] = NULL;	
		sprintf(buf,"0x%0x ",buf[i]);
		if( lwip_write(debug_socket, buf, 6) == -1) debug_socket = -1 ;
	}
	
}
#endif