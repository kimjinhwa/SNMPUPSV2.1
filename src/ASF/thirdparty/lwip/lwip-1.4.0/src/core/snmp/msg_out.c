/**
 * @file
 * SNMP output message processing (RFC1157).
 *
 * Output responses and traps are build in two passes:
 *
 * Pass 0: iterate over the output message backwards to determine encoding lengths
 * Pass 1: the actual forward encoding of internal form into ASN1
 *
 * The single-pass encoding method described by Comer & Stevens
 * requires extra buffer space and copying for reversal of the packet.
 * The buffer requirement can be prohibitively large for big payloads
 * (>= 484) therefore we use the two encoding passes.
 */

/*
 * Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Christiaan Simons <christiaan.simons@axon.tv>
 */

#include "lwip/opt.h"
#include "string.h"

#if LWIP_SNMP /* don't build if not configured for use in lwipopts.h */

#include "lwip/udp.h"
#include "lwip/netif.h"
#include "lwip/snmp.h"
#include "lwip/snmp_asn1.h"
#include "lwip/snmp_msg.h"
#include "FlashMemory.h"
#include "lwip/memp.h"

#include "sys.h"
#include "networkTime.h"

extern struct udp_pcb *snmp1_pcb;
err_t snmp_send_trap_ups_exp(s8_t generic_trap, s32_t specific_trap);

struct snmp_trap_dst
{
  /* destination IP address in network order */
  ip_addr_t dip;
  /* set to 0 when disabled, >0 when enabled */
  u8_t enable;
};
struct snmp_trap_dst trap_dst[SNMP_TRAP_DESTINATIONS];


/** TRAP message structure */
struct snmp_msg_trap trap_msg;

void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue );
static u16_t snmp_resp_header_sum(struct snmp_msg_pstat *m_stat, u16_t vb_len);
static u16_t snmp_trap_header_sum(struct snmp_msg_trap *m_trap, u16_t vb_len);
static u16_t snmp_varbind_list_sum(struct snmp_varbind_root *root);

static u16_t snmp_resp_header_enc(struct snmp_msg_pstat *m_stat, struct pbuf *p);
static u16_t snmp_trap_header_enc(struct snmp_msg_trap *m_trap, struct pbuf *p);
static u16_t snmp_varbind_list_enc(struct snmp_varbind_root *root, struct pbuf *p, u16_t ofs);
err_t snmp_send_trapDirect(s8_t generic_trap, struct snmp_obj_id *eoid, s32_t specific_trap,const char* strMessage);

/**
 * Sets enable switch for this trap destination.
 * @param dst_idx index in 0 .. SNMP_TRAP_DESTINATIONS-1
 * @param enable switch if 0 destination is disabled >0 enabled.
 */
void
snmp_trap_dst_enable(u8_t dst_idx, u8_t enable)
{
  if (dst_idx < SNMP_TRAP_DESTINATIONS)
  {
    trap_dst[dst_idx].enable = enable;
  }
}

/**
 * Sets IPv4 address for this trap destination.
 * @param dst_idx index in 0 .. SNMP_TRAP_DESTINATIONS-1
 * @param dst IPv4 address in host order.
 */
void
snmp_trap_dst_ip_set(u8_t dst_idx, ip_addr_t *dst)
{
  if (dst_idx < SNMP_TRAP_DESTINATIONS)
  {
    ip_addr_set(&trap_dst[dst_idx].dip, dst);
  }
}

/**
 * Sends a 'getresponse' message to the request originator.
 *
 * @param m_stat points to the current message request state source
 * @return ERR_OK when success, ERR_MEM if we're out of memory
 *
 * @note the caller is responsible for filling in outvb in the m_stat
 * and provide error-status and index (except for tooBig errors) ...
 */
err_t
snmp_send_response(struct snmp_msg_pstat *m_stat)
{
	/*출력된 데이타
	0x30 0x4d 0x2 0x1 0x0 0x4 0x6 0x70 0x75 0x62 0x6c 0x69 0x63 0xa2 0x40 0x2
	0x1 0x2f 0x2 0x1 0x0 0x2 0x1 0x0 0x30 0x35 0x30 0x33 0x6 0x8 0x2b 0x6
	0x1 0x2 0x1 0x1 0x1 0x0 0x4 0x27 0x33 0x32 0x42 0x69 0x74 0x2c 0x52 0x49
	0x53 0x43 0x20 0x43 0x50 0x55 0x20 0x36 0x30 0x4d 0x68 0x7a 0x2c 0x53 0x4e 0x4d
	0x50 0x20 0x26 0x20 0x57 0x45 0x42 0x20 0x53 0x65 0x72 0x76 0x65 0x72 0x0
	*/
  struct snmp_varbind_root emptyvb = {NULL, NULL, 0, 0, 0};
  struct pbuf *p;
  u16_t tot_len;
  err_t err;

  /* pass 0, calculate length fields */
  tot_len = snmp_varbind_list_sum(&m_stat->outvb);
  tot_len = snmp_resp_header_sum(m_stat, tot_len);

  /* try allocating pbuf(s) for complete response */
  p = pbuf_alloc(PBUF_TRANSPORT, tot_len, PBUF_POOL);
  if (p == NULL)
  {
    LWIP_DEBUGF_UDP(SNMP_MSG_DEBUG, ("snmp_snd_response() tooBig\n"));

    /* can't construct reply, return error-status tooBig */
    m_stat->error_status = SNMP_ES_TOOBIG;
    m_stat->error_index = 0;
    /* pass 0, recalculate lengths, for empty varbind-list */
    tot_len = snmp_varbind_list_sum(&emptyvb);
    tot_len = snmp_resp_header_sum(m_stat, tot_len);
    /* retry allocation once for header and empty varbind-list */
    p = pbuf_alloc(PBUF_TRANSPORT, tot_len, PBUF_POOL);
  }
  if (p != NULL)
  {
    /* first pbuf alloc try or retry alloc success */
    u16_t ofs;

    LWIP_DEBUGF_UDP(SNMP_MSG_DEBUG, ("snmp_snd_response() p != NULL\n"));

    /* pass 1, size error, encode packet ino the pbuf(s) */
    ofs = snmp_resp_header_enc(m_stat, p);  // 헤더를 완성한다.
    if (m_stat->error_status == SNMP_ES_TOOBIG)
    {
      snmp_varbind_list_enc(&emptyvb, p, ofs);
    }
    else
    {
		// 이곳으로 우선은 들어 왔다.
      snmp_varbind_list_enc(&m_stat->outvb, p, ofs);
    }

    switch (m_stat->error_status)
    {
      case SNMP_ES_TOOBIG:
        snmp_inc_snmpouttoobigs();
        break;
      case SNMP_ES_NOSUCHNAME:
        snmp_inc_snmpoutnosuchnames();
        break;
      case SNMP_ES_BADVALUE:
        snmp_inc_snmpoutbadvalues();
        break;
      case SNMP_ES_GENERROR:
        snmp_inc_snmpoutgenerrs();
        break;
    }
    snmp_inc_snmpoutgetresponses();   // 단순 증가 snmpoutgetresponses++;
    snmp_inc_snmpoutpkts();  // snmpoutpkts++;

    /** @todo do we need separate rx and tx pcbs for threaded case? */
    /** connect to the originating source */
    udp_connect(m_stat->pcb, &m_stat->sip, m_stat->sp);


  /*
  // for my debug	mvcjhkim
  u8_t	*octepptr;
  octepptr = (u8_t *)p->payload;
  for(int i = 1 ; i<= p->tot_len; i++ ) {
	  LWIP_DEBUGF_UDP(UDP_DEBUG, ("0x%"X16_F" ",*octepptr));
	  octepptr++;
	  if( !(i%16) )LWIP_DEBUGF_UDP(UDP_DEBUG, ("\n"));
  }
    출력된 데이타 
	0x30 0x4d 0x2 0x1 0x0 0x4 0x6 0x70 0x75 0x62 0x6c 0x69 0x63 0xa2 0x40 0x2
	0x1 0x2f 0x2 0x1 0x0 0x2 0x1 0x0 0x30 0x35 0x30 0x33 0x6 0x8 0x2b 0x6
	0x1 0x2 0x1 0x1 0x1 0x0 0x4 0x27 0x33 0x32 0x42 0x69 0x74 0x2c 0x52 0x49
	0x53 0x43 0x20 0x43 0x50 0x55 0x20 0x36 0x30 0x4d 0x68 0x7a 0x2c 0x53 0x4e 0x4d
	0x50 0x20 0x26 0x20 0x57 0x45 0x42 0x20 0x53 0x65 0x72 0x76 0x65 0x72 0x0	
	*/


    err = udp_send(m_stat->pcb, p);
	
	
	
	
    if (err == ERR_MEM)
    {
      /** @todo release some memory, retry and return tooBig? tooMuchHassle? */
      err = ERR_MEM;
    }
    else
    {
      err = ERR_OK;
    }
    /** disassociate remote address and port with this pcb */
    udp_disconnect(m_stat->pcb);
    pbuf_free(p);
    LWIP_DEBUGF_UDP(SNMP_MSG_DEBUG, ("snmp_snd_response() done\n"));
    return err;
  }
  else
  {
    /* first pbuf alloc try or retry alloc failed
       very low on memory, couldn't return tooBig */
    return ERR_MEM;
  }
}


void snmp_coldstart_trap(void)
{
	trap_msg.outvb.head = NULL;
	trap_msg.outvb.tail = NULL;
	trap_msg.outvb.count = 0;
	static struct snmp_obj_id eoid = {10,{1,3,6,1,6,3,1,1,5,1}};
	snmp_send_trap(SNMP_GENTRAP_COLDSTART,&eoid , 0,NULL);
}

void snmp_trap_userOid(char *uOid,s8_t generic_trap, s32_t specific_trap)
{
	int count=0;
	char *oidValue;
	static struct snmp_obj_id eoid = {0,{1,3,6,1,6,3,1,1,5,1}};
	// .을 찾아서 시작점을 찾는다.
	oidValue++; uOid++;

	while(*uOid != NULL){
		oidValue=uOid;
		for(;;){   //이제 oidValue를 얻었다
			
			if( *uOid == '.' || *uOid == NULL )
			{
				if( *uOid == '.'){	*uOid=0x00; uOid++;}
				else if( *uOid == NULL){}// 널이면 증가 시키지 않는다.
				eoid.id[count++] = atoi(oidValue);
				eoid.len = count;
				break;
			}
			
			else uOid++;
		}
	}
	
	trap_msg.outvb.head = NULL;
	trap_msg.outvb.tail = NULL;
	trap_msg.outvb.count = 0;
	snmp_send_trap(generic_trap, &eoid, specific_trap,NULL);
}
err_t snmp_send_trap_ups(s8_t generic_trap, s32_t specific_trap)
{
	snmp_send_trap_ups_exp(generic_trap,specific_trap);
	trap_msg.outvb.head = NULL;
	trap_msg.outvb.tail = NULL;
	trap_msg.outvb.count = 0;

	static struct snmp_obj_id eoid = {8,{1,3,6,1,2,1,33,2}};
	//snmp_send_trap( generic_trap,&eoid,specific_trap,NULL);
	return snmp_send_trap( generic_trap,&eoid,specific_trap,NULL);
};
err_t snmp_send_trap_ups_kepco(s8_t generic_trap, s32_t specific_trap)
{

	trap_msg.outvb.head = NULL;
	trap_msg.outvb.tail = NULL;
	trap_msg.outvb.count = 0;

	//static struct snmp_obj_id eoid = {7,{1,3,6,1,2,1,33}};
	static struct snmp_obj_id eoid = {9,{1,3,6,1,4,1,12236,2,12082}};
	//한전의 경우  gerneric과 specific의 2가지 데이타를 전송 한다.
	//SNMP_GENTRAP_ENTERPRISESPC
	return snmp_send_trap( generic_trap,&eoid,specific_trap,NULL);
}
/*
SNMP Trap: SENA-UPSLINK-MIB::senaUpsPowerRestored	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.26\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsAwaitingPower	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.21\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsUserSpecificAlarm	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.25\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsTestInProg	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.24\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsShutdownPending	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.22\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsCommLost	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.20\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsFanFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.16\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsDiagTestFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.19\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpslinkUPSTraps#	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsGenFault	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.18\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsFuseFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.17\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsSysOff	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.15\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsShutdownImminent	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.23\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutOff	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.14\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsUpsoffAr	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.12\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutputBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.7\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutoffAr	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.11\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsBypassBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.10\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutOverload	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.8\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsInputBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.6\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsOnBattery	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.2\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsTempBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.5\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsChargerFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.13\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryDeprecated	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.4\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryLow	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.3\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.1\s"]
SNMP Trap: SENA-UPSLINK-MIB::senaUpsOnBypass	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.9\s"]

*/
err_t snmp_send_trap_ups_exp(s8_t generic_trap, s32_t specific_trap)
{

	uint8_t trapCode[36];

	vParTestSetLED(0, pdTRUE);
	vParTestSetLED(1, pdTRUE);
	vParTestSetLED(2, pdTRUE);

	switch( specific_trap){
		case 1: strcpy(trapCode,"UPS communication has been lost");	//31
			break;	
		case 2:
			strcpy(trapCode,"UPS has switched to battery power");	//33
			break;	
		case 7:
			strcpy(trapCode,"Batteries are low");	
			break;	
		case 8: strcpy(trapCode,"UPS communication has established");	//31
			break;	
		case 9:
			strcpy(trapCode,"UPS utility power has been restored"); //35	
			break;	
		case 11:
			strcpy(trapCode,"UPS returned from a low battery");	
			break;	
		case 59:
			strcpy(trapCode,"Static Switch in Inverter Mode");	
			break;	
		case 60:
			strcpy(trapCode,"entering Bypass Mode");	
			break;	
		default:
			strcpy(trapCode,"Undefined Message");	
			break;
	}
	snmp_send_trapDirect(generic_trap, NULL, 63,trapCode);
	vParTestSetLED(0, pdFALSE);
	vParTestSetLED(1, pdFALSE);
	vParTestSetLED(2, pdFALSE);
	//trap_msg.outvb.head = NULL;
	//trap_msg.outvb.tail = NULL;
	//trap_msg.outvb.count = 0;
//
	//static struct snmp_obj_id eoid = {10,{1,3,6,1,4,1,12236,1,1,11}};
	//
	//return snmp_send_trap( generic_trap,&eoid,specific_trap,NULL);
}


static void ocstrncpy_1(u8_t *dst, u8_t *src, u16_t n)
{
	u16_t i = n;
	while (i > 0) {
		i--;
		*dst++ = *src++;
	}
}
/**
 * Sends an generic or enterprise specific trap message.
 *
 * @param generic_trap is the trap code
 * @param eoid points to enterprise object identifier
 * @param specific_trap used for enterprise traps when generic_trap == 6
 * @return ERR_OK when success, ERR_MEM if we're out of memory
 *
 * @note the caller is responsible for filling in outvb in the trap_msg
 * @note the use of the enterpise identifier field
 * is per RFC1215.
 * Use .iso.org.dod.internet.mgmt.mib-2.snmp for generic traps
 * and .iso.org.dod.internet.private.enterprises.yourenterprise
 * (sysObjectID) for specific traps.
 */
 //전체 길이는 고정한다. 0x59
 //데이타 부분과 specific부분만 수정을 한다.

err_t snmp_send_trapDirect(s8_t generic_trap, struct snmp_obj_id *eoid, s32_t specific_trap,const char* strMessage)
{
	char  trapMessageStruct[] = {
	0x30,0x59,0x02,0x01,0x00,	0x04,0x06,0x70,0x75,0x62,	0x6c,0x69,0x63,0xa4,0x4c,	0x06,0x07,0x2b,0x06,0x01,
	0x04,0x01,0x87,0x27,0x40,	0x04,0xc0,0xa8,0x00,0xc8,	0x02,0x01,0x06,0x02,0x01,	0x03,0x43,0x01,0x64,0x30,
	0x32,0x30,0x30,0x06,0x07,	0x2b,0x06,0x01,0x04,0x01,	0x87,0x27,0x04,0x25,0x49,	0x6e,0x74,0x65,0x72,0x6e,
	0x61,0x6c,0x20,0x64,0x69,	0x61,0x67,0x6e,0x6f,0x73,	0x74,0x69,0x63,0x20,0x73,	0x65,0x6c,0x66,0x2d,0x74,
	0x65,0x73,0x74,0x20,0x66,	0x61,0x69,0x6c,0x65,0x64,	0x2e
	}; 
	int generic_pos = 32; 
	int specific_trap_pos=35;
	int msg_pos=  54;
	int maxStr_len = 0x25;
	int strMessageLen=0;

	for(int i=0;i<0x25;i++)trapMessageStruct[msg_pos + i] = 0x20;// fill space
	strMessageLen=strlen(strMessage);
	strMessageLen = min(0x25,strMessageLen);// 최대 0x25를 넘지 않게 한다. 
	for(int i=0;i< strMessageLen;i++){
		trapMessageStruct[msg_pos +i] = strMessage[i];
	}
	//memcpy((char *)trapMessageStruct[msg_pos],strMessage,strMessageLen);
	trapMessageStruct[generic_pos] = min(127,generic_trap);
	trapMessageStruct[specific_trap_pos] = min(127,specific_trap);

	struct snmp_trap_dst *td;
	struct netif *dst_if;
	struct snmp_varbind *vb;
	
	ip_addr_t dst_ip;
	u16_t i,tot_len=0x5B; // 59 + 2개

	ip_addr_t dst;
	trap_address_t trap_address;
	
	for (i=0;i<SNMP_TRAP_DESTINATIONS;i++)  // 10로 되어 있다.
	{
		
		trapIpAddressRead(i,&trap_address) ;
		if( trap_address.Ethernet_Conf_IpAddr0 != 0xFF)
		{
			//trap_dst 를 enable시킨다
			snmp_trap_dst_enable(i, 1);
			IP4_ADDR(&dst,trap_address.Ethernet_Conf_IpAddr0,
			trap_address.Ethernet_Conf_IpAddr1,
			trap_address.Ethernet_Conf_IpAddr2,
			trap_address.Ethernet_Conf_IpAddr3);
			snmp_trap_dst_ip_set(i,&dst);
		}
	}
	
	for (i=0, td = &trap_dst[i]; i<SNMP_TRAP_DESTINATIONS; i++, td++)
	{
		if ((td->enable != 0) && !ip_addr_isany(&td->dip))
		{
			/* network order trap destination */
			ip_addr_copy(trap_msg.dip, td->dip);
			/* lookup current source address for this dst */
			dst_if = ip_route(&td->dip);
			ip_addr_copy(dst_ip, dst_if->ip_addr);
			udp_send_msg(trapMessageStruct,tot_len, ipaddr_ntoa(&td->dip),162 );
		}
	}
	return ERR_OK;
}
err_t snmp_send_trap(s8_t generic_trap, struct snmp_obj_id *eoid, s32_t specific_trap,const char* strMessage)
{
	struct snmp_trap_dst *td;
	struct netif *dst_if;
	struct snmp_varbind *vb;
	u8_t strMessageLen=0;
	if(strMessage != NULL) {
		strMessageLen=strlen(strMessage);
		//vb = (struct snmp_varbind *)memp_malloc(MEMP_SNMP_VARBIND);
	}
	
	ip_addr_t dst_ip;
	struct pbuf *p;
	u16_t i,tot_len;

	
	ip_addr_t dst;
	trap_address_t trap_address;
	
	for (i=0;i<SNMP_TRAP_DESTINATIONS;i++)  // 10로 되어 있다.
	{
		
		trapIpAddressRead(i,&trap_address) ;
		if( trap_address.Ethernet_Conf_IpAddr0 != 0xFF)
		{
			//trap_dst 를 enable시킨다
			snmp_trap_dst_enable(i, 1);
			IP4_ADDR(&dst,trap_address.Ethernet_Conf_IpAddr0,
			trap_address.Ethernet_Conf_IpAddr1,
			trap_address.Ethernet_Conf_IpAddr2,
			trap_address.Ethernet_Conf_IpAddr3);
			snmp_trap_dst_ip_set(i,&dst);
		}
	}
	
	for (i=0, td = &trap_dst[i]; i<SNMP_TRAP_DESTINATIONS; i++, td++)
	{
		if ((td->enable != 0) && !ip_addr_isany(&td->dip))
		{
			/* network order trap destination */
			ip_addr_copy(trap_msg.dip, td->dip);
			/* lookup current source address for this dst */
			dst_if = ip_route(&td->dip);
			ip_addr_copy(dst_ip, dst_if->ip_addr);
			
			/* @todo: what about IPv6? */
			trap_msg.sip_raw[0] = ip4_addr1(&dst_ip);
			trap_msg.sip_raw[1] = ip4_addr2(&dst_ip);
			trap_msg.sip_raw[2] = ip4_addr3(&dst_ip);
			trap_msg.sip_raw[3] = ip4_addr4(&dst_ip);
			trap_msg.gen_trap = generic_trap;
			trap_msg.spc_trap = specific_trap;
			
			trap_msg.enterprise = eoid;
			trap_msg.ts = getTimeLong();
			/* pass 0, calculate length fields */
			tot_len = snmp_varbind_list_sum(&trap_msg.outvb);
			tot_len = snmp_trap_header_sum(&trap_msg, tot_len);

			/* allocate pbuf(s) */
			p = pbuf_alloc(PBUF_TRANSPORT, tot_len, PBUF_POOL);

			if (p != NULL)
			{
				u16_t ofs;

				/* pass 1, encode packet ino the pbuf(s) */
				ofs = snmp_trap_header_enc(&trap_msg, p);
				//start
				if(strMessage != NULL){
					/*
					if (vb != NULL)
					{
						vb->next = NULL;
						vb->prev = NULL;
						vb->value_type = SNMP_ASN1_OC_STR;
						vb->value = memp_malloc(MEMP_SNMP_VALUE);
						if (vb->value != NULL)
						{
							vb->value_len =  strlen(strMessage);
							
							memcpy(vb->value,strMessage,strMessageLen);
							trap_msg.outvb.head = vb->value;
						}
					}
					else{
						memp_free(MEMP_SNMP_VARBIND, vb);
					}
					*/
				}
				
				//end
				snmp_varbind_list_enc(&trap_msg.outvb, p, ofs);

				snmp_inc_snmpouttraps();
				snmp_inc_snmpoutpkts();
				
				//IP_ADDR_BROADCAST를 사용할 수 도 있을까?
				/** send to the TRAP destination */
				udp_sendto(trap_msg.pcb, p, &trap_msg.dip, SNMP_TRAP_PORT);
				pbuf_free(p);
				if(vb->value != NULL) memp_free(MEMP_SNMP_VARBIND, vb->value);
				if(vb != NULL) memp_free(MEMP_SNMP_VARBIND, vb);

			}
			else
			{
				return ERR_MEM;
			}
		}
	}
	return ERR_OK;
}

void snmp_authfail_trap(void)
{
  //u8_t enable;
  //snmp_get_snmpenableauthentraps(&enable);
  //if (enable == 1)
  {
    trap_msg.outvb.head = NULL;
    trap_msg.outvb.tail = NULL;
    trap_msg.outvb.count = 0;
	static struct snmp_obj_id eoid = {10,{1,3,6,1,6,3,1,1,5,1}};
    snmp_send_trap(SNMP_GENTRAP_AUTHFAIL, &eoid, 0,NULL);
  }
}

/**
 * Sums response header field lengths from tail to head and
 * returns resp_header_lengths for second encoding pass.
 *
 * @param vb_len varbind-list length
 * @param rhl points to returned header lengths
 * @return the required lenght for encoding the response header
 */
static u16_t
snmp_resp_header_sum(struct snmp_msg_pstat *m_stat, u16_t vb_len)
{
  u16_t tot_len;
  struct snmp_resp_header_lengths *rhl;

  rhl = &m_stat->rhl;
  tot_len = vb_len;
  snmp_asn1_enc_s32t_cnt(m_stat->error_index, &rhl->erridxlen);
  snmp_asn1_enc_length_cnt(rhl->erridxlen, &rhl->erridxlenlen);
  tot_len += 1 + rhl->erridxlenlen + rhl->erridxlen;

  snmp_asn1_enc_s32t_cnt(m_stat->error_status, &rhl->errstatlen);
  snmp_asn1_enc_length_cnt(rhl->errstatlen, &rhl->errstatlenlen);
  tot_len += 1 + rhl->errstatlenlen + rhl->errstatlen;

  snmp_asn1_enc_s32t_cnt(m_stat->rid, &rhl->ridlen);
  snmp_asn1_enc_length_cnt(rhl->ridlen, &rhl->ridlenlen);
  tot_len += 1 + rhl->ridlenlen + rhl->ridlen;

  rhl->pdulen = tot_len;
  snmp_asn1_enc_length_cnt(rhl->pdulen, &rhl->pdulenlen);
  tot_len += 1 + rhl->pdulenlen;

  rhl->comlen = m_stat->com_strlen;
  snmp_asn1_enc_length_cnt(rhl->comlen, &rhl->comlenlen);
  tot_len += 1 + rhl->comlenlen + rhl->comlen;

  snmp_asn1_enc_s32t_cnt(snmp_version, &rhl->verlen);
  snmp_asn1_enc_length_cnt(rhl->verlen, &rhl->verlenlen);
  tot_len += 1 + rhl->verlen + rhl->verlenlen;

  rhl->seqlen = tot_len;
  snmp_asn1_enc_length_cnt(rhl->seqlen, &rhl->seqlenlen);
  tot_len += 1 + rhl->seqlenlen;

  return tot_len;
}

/**
 * Sums trap header field lengths from tail to head and
 * returns trap_header_lengths for second encoding pass.
 *
 * @param vb_len varbind-list length
 * @param thl points to returned header lengths
 * @return the required lenght for encoding the trap header
 */
static u16_t
snmp_trap_header_sum(struct snmp_msg_trap *m_trap, u16_t vb_len)
{
  u16_t tot_len;
  struct snmp_trap_header_lengths *thl;

  thl = &m_trap->thl;
  tot_len = vb_len;

  snmp_asn1_enc_u32t_cnt(m_trap->ts, &thl->tslen);
  snmp_asn1_enc_length_cnt(thl->tslen, &thl->tslenlen);
  tot_len += 1 + thl->tslen + thl->tslenlen;

  snmp_asn1_enc_s32t_cnt(m_trap->spc_trap, &thl->strplen);
  snmp_asn1_enc_length_cnt(thl->strplen, &thl->strplenlen);
  tot_len += 1 + thl->strplen + thl->strplenlen;

  snmp_asn1_enc_s32t_cnt(m_trap->gen_trap, &thl->gtrplen);
  snmp_asn1_enc_length_cnt(thl->gtrplen, &thl->gtrplenlen);
  tot_len += 1 + thl->gtrplen + thl->gtrplenlen;

  thl->aaddrlen = 4;
  snmp_asn1_enc_length_cnt(thl->aaddrlen, &thl->aaddrlenlen);
  tot_len += 1 + thl->aaddrlen + thl->aaddrlenlen;

  snmp_asn1_enc_oid_cnt(m_trap->enterprise->len, &m_trap->enterprise->id[0], &thl->eidlen);
  snmp_asn1_enc_length_cnt(thl->eidlen, &thl->eidlenlen);
  tot_len += 1 + thl->eidlen + thl->eidlenlen;

  thl->pdulen = tot_len;
  snmp_asn1_enc_length_cnt(thl->pdulen, &thl->pdulenlen);
  tot_len += 1 + thl->pdulenlen;

  thl->comlen = sizeof(snmp_publiccommunity) - 1;
  snmp_asn1_enc_length_cnt(thl->comlen, &thl->comlenlen);
  tot_len += 1 + thl->comlenlen + thl->comlen;

  snmp_asn1_enc_s32t_cnt(snmp_version, &thl->verlen);
  snmp_asn1_enc_length_cnt(thl->verlen, &thl->verlenlen);
  tot_len += 1 + thl->verlen + thl->verlenlen;

  thl->seqlen = tot_len;
  snmp_asn1_enc_length_cnt(thl->seqlen, &thl->seqlenlen);
  tot_len += 1 + thl->seqlenlen;

  return tot_len;
}

/**
 * Sums varbind lengths from tail to head and
 * annotates lengths in varbind for second encoding pass.
 *
 * @param root points to the root of the variable binding list
 * @return the required lenght for encoding the variable bindings
 */
static u16_t snmp_varbind_list_sum(struct snmp_varbind_root *root)
{
  struct snmp_varbind *vb;
  u32_t *uint_ptr;
  s32_t *sint_ptr;
  u16_t tot_len;

  tot_len = 0;
  vb = root->tail;
  while ( vb != NULL )
  {
    /* encoded value lenght depends on type */
    switch (vb->value_type)
    {
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG):
        sint_ptr = (s32_t*)vb->value;
        snmp_asn1_enc_s32t_cnt(*sint_ptr, &vb->vlen);
        break;
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_GAUGE):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_TIMETICKS):
        uint_ptr = (u32_t*)vb->value;
        snmp_asn1_enc_u32t_cnt(*uint_ptr, &vb->vlen);
        break;
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR):
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_NUL):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_IPADDR):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_OPAQUE):
        vb->vlen = vb->value_len;
        break;
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OBJ_ID):
        sint_ptr = (s32_t*)vb->value;
        snmp_asn1_enc_oid_cnt(vb->value_len / sizeof(s32_t), sint_ptr, &vb->vlen);
        break;
      default:
        /* unsupported type */
        vb->vlen = 0;
        break;
    };
    /* encoding length of value length field */
    snmp_asn1_enc_length_cnt(vb->vlen, &vb->vlenlen);
    snmp_asn1_enc_oid_cnt(vb->ident_len, vb->ident, &vb->olen);
    snmp_asn1_enc_length_cnt(vb->olen, &vb->olenlen);

    vb->seqlen = 1 + vb->vlenlen + vb->vlen;
    vb->seqlen += 1 + vb->olenlen + vb->olen;
    snmp_asn1_enc_length_cnt(vb->seqlen, &vb->seqlenlen);

    /* varbind seq */
    tot_len += 1 + vb->seqlenlen + vb->seqlen;

    vb = vb->prev;
  }

  /* varbind-list seq */
  root->seqlen = tot_len;
  snmp_asn1_enc_length_cnt(root->seqlen, &root->seqlenlen);
  tot_len += 1 + root->seqlenlen;

  return tot_len;
}

/**
 * Encodes response header from head to tail.
 */
	/*출력된 데이타
	0x30 0x4d 0x2 0x1 0x0 0x4 0x6 0x70 0x75 0x62 0x6c 0x69 0x63 0xa2 0x40 0x2
	0x1 0x2f 0x2 0x1 0x0 0x2 0x1 0x0 0x30 0x35 0x30 0x33 0x6 0x8 0x2b 0x6
	0x1 0x2 0x1 0x1 0x1 0x0 0x4 0x27 0x33 0x32 0x42 0x69 0x74 0x2c 0x52 0x49
	0x53 0x43 0x20 0x43 0x50 0x55 0x20 0x36 0x30 0x4d 0x68 0x7a 0x2c 0x53 0x4e 0x4d
	0x50 0x20 0x26 0x20 0x57 0x45 0x42 0x20 0x53 0x65 0x72 0x76 0x65 0x72 0x0
	*/

static u16_t
snmp_resp_header_enc(struct snmp_msg_pstat *m_stat, struct pbuf *p)
{
  u16_t ofs;

  ofs = 0;					  /*     0			(0x20)               16 		  = 0x30  */
  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_CONSTR | SNMP_ASN1_SEQ));
  /* 0x30	*/
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_stat->rhl.seqlen);   /* length */
  /* 0x30	 0x4d */
  ofs += m_stat->rhl.seqlenlen;
  /*								0			0		               2 		  = 0x02  */
  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG));
  /* 0x30	 0x4d 0x2 */
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_stat->rhl.verlen);
  /* 0x30	 0x4d 0x2 0x1 */
  ofs += m_stat->rhl.verlenlen;
  snmp_asn1_enc_s32t(p, ofs, m_stat->rhl.verlen, snmp_version);
  /* 0x30	 0x4d 0x2 0x1 0x0 */
  ofs += m_stat->rhl.verlen;
  /*								0			0		               4 		  = 0x04  */	
  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR));
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 */
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_stat->rhl.comlen);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 <- public*/
  ofs += m_stat->rhl.comlenlen;
  snmp_asn1_enc_raw(p, ofs, m_stat->rhl.comlen, m_stat->community);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63  */
  ofs += m_stat->rhl.comlen;
  /*								(0x80)			0x20		             2 		  = 0xA2  */	 
  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_CONTXT | SNMP_ASN1_CONSTR | SNMP_ASN1_PDU_GET_RESP));
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 */
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_stat->rhl.pdulen);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 <- lenth*/  
  ofs += m_stat->rhl.pdulenlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG));
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 <- integer*/  
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_stat->rhl.ridlen);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x01 <- length*/  
  ofs += m_stat->rhl.ridlenlen;
  snmp_asn1_enc_s32t(p, ofs, m_stat->rhl.ridlen, m_stat->rid);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x2f <- id*/  
  
  ofs += m_stat->rhl.ridlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG));
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x2f 0x2 <- integer*/  
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_stat->rhl.errstatlen);
  ofs += m_stat->rhl.errstatlenlen;
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x2f 0x2 0x01<- length*/  
  snmp_asn1_enc_s32t(p, ofs, m_stat->rhl.errstatlen, m_stat->error_status);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x2f 0x2 0x01 0x00 <- error*/  
  ofs += m_stat->rhl.errstatlen;
  
  
  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG));
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x2f 0x2 0x01 0x00 */  
  /* 0x02 <- integer */  
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_stat->rhl.erridxlen);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x2f 0x2 0x01 0x00 */
  /* 0x02 0x01 <- length */

  ofs += m_stat->rhl.erridxlenlen;
  snmp_asn1_enc_s32t(p, ofs, m_stat->rhl.erridxlen, m_stat->error_index);
  /* 0x30	 0x4d 0x2 0x1 0x0 0x04 0x06 0x70 0x75 0x62 0x6c 0x69 0x63 0xA2 0x40 0x2 0x2f 0x2 0x01 0x00 */
  /* 0x02 0x01 0x00 <- index */

  ofs += m_stat->rhl.erridxlen;
/* 이렇게 헤더가 완성 되었다 */
  return ofs;
}

/**
 * Encodes trap header from head to tail.
 */
static u16_t
snmp_trap_header_enc(struct snmp_msg_trap *m_trap, struct pbuf *p)
{
  u16_t ofs;

  ofs = 0;
  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_CONSTR | SNMP_ASN1_SEQ));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.seqlen);
  ofs += m_trap->thl.seqlenlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.verlen);
  ofs += m_trap->thl.verlenlen;
  snmp_asn1_enc_s32t(p, ofs, m_trap->thl.verlen, snmp_version);
  ofs += m_trap->thl.verlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.comlen);
  ofs += m_trap->thl.comlenlen;
  snmp_asn1_enc_raw(p, ofs, m_trap->thl.comlen, (u8_t *)&snmp_publiccommunity[0]);
  ofs += m_trap->thl.comlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_CONTXT | SNMP_ASN1_CONSTR | SNMP_ASN1_PDU_TRAP));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.pdulen);
  ofs += m_trap->thl.pdulenlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OBJ_ID));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.eidlen);
  ofs += m_trap->thl.eidlenlen;
  snmp_asn1_enc_oid(p, ofs, m_trap->enterprise->len, &m_trap->enterprise->id[0]);
  ofs += m_trap->thl.eidlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_IPADDR));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.aaddrlen);
  ofs += m_trap->thl.aaddrlenlen;
  snmp_asn1_enc_raw(p, ofs, m_trap->thl.aaddrlen, &m_trap->sip_raw[0]);
  ofs += m_trap->thl.aaddrlen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.gtrplen);
  ofs += m_trap->thl.gtrplenlen;
  snmp_asn1_enc_u32t(p, ofs, m_trap->thl.gtrplen, m_trap->gen_trap);
  ofs += m_trap->thl.gtrplen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.strplen);
  ofs += m_trap->thl.strplenlen;
  snmp_asn1_enc_u32t(p, ofs, m_trap->thl.strplen, m_trap->spc_trap);
  ofs += m_trap->thl.strplen;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_TIMETICKS));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, m_trap->thl.tslen);
  ofs += m_trap->thl.tslenlen;
  snmp_asn1_enc_u32t(p, ofs, m_trap->thl.tslen, m_trap->ts);
  ofs += m_trap->thl.tslen;

  return ofs;
}

/**
 * Encodes varbind list from head to tail.
 */
static u16_t
snmp_varbind_list_enc(struct snmp_varbind_root *root, struct pbuf *p, u16_t ofs)
{
  struct snmp_varbind *vb;
  s32_t *sint_ptr;
  u32_t *uint_ptr;
  u8_t *raw_ptr;

  snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_CONSTR | SNMP_ASN1_SEQ));
  ofs += 1;
  snmp_asn1_enc_length(p, ofs, root->seqlen);
  ofs += root->seqlenlen;

  vb = root->head;
  while ( vb != NULL )
  {
    snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_CONSTR | SNMP_ASN1_SEQ));
    ofs += 1;
    snmp_asn1_enc_length(p, ofs, vb->seqlen);
    ofs += vb->seqlenlen;

    snmp_asn1_enc_type(p, ofs, (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OBJ_ID));
    ofs += 1;
    snmp_asn1_enc_length(p, ofs, vb->olen);
    ofs += vb->olenlen;
    snmp_asn1_enc_oid(p, ofs, vb->ident_len, &vb->ident[0]);
    ofs += vb->olen;

    snmp_asn1_enc_type(p, ofs, vb->value_type);
    ofs += 1;
    snmp_asn1_enc_length(p, ofs, vb->vlen);
    ofs += vb->vlenlen;

    switch (vb->value_type)
    {
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG):
        sint_ptr = (s32_t*)vb->value;
        snmp_asn1_enc_s32t(p, ofs, vb->vlen, *sint_ptr);
        break;
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_GAUGE):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_TIMETICKS):
        uint_ptr = (u32_t*)vb->value;
        snmp_asn1_enc_u32t(p, ofs, vb->vlen, *uint_ptr);
        break;
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_IPADDR):
      case (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_OPAQUE):
        raw_ptr = (u8_t*)vb->value;
        snmp_asn1_enc_raw(p, ofs, vb->vlen, raw_ptr);
        break;
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_NUL):
        break;
      case (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OBJ_ID):
        sint_ptr = (s32_t*)vb->value;
        snmp_asn1_enc_oid(p, ofs, vb->value_len / sizeof(s32_t), sint_ptr);
        break;
      default:
        /* unsupported type */
        break;
    };
    ofs += vb->vlen;
    vb = vb->next;
  }
  return ofs;
}

int udp_send_msg(char *msg,int len,char *ipaddress,int port)
{
	err_t err;
	struct udp_pcb *pcb;
	pcb = snmp1_pcb;
	if (pcb== NULL){ return ERR_MEM; }
	struct pbuf *p;
	p = pbuf_alloc(PBUF_TRANSPORT,len , PBUF_POOL);
	memcpy(p->payload,msg,len);
	if (p == NULL){ return ERR_MEM; }
    
	ip_addr_t sip;
	sip.addr = ipaddr_addr(ipaddress) ;
	udp_connect(pcb, &sip, port);

	err = udp_sendto(pcb, p, &sip, port);
	if (err == ERR_MEM) err = ERR_MEM;
	else err = ERR_OK;
	udp_disconnect(pcb);
	pbuf_free(p);
	if (err == ERR_OK)return 0; 
	return -1;
}
#endif /* LWIP_SNMP */
