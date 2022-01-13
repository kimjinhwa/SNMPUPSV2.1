/**************************************************************************
 *
 * \file
 *
 * \brief Ethernet module configuration file.
 *
 * This file contains the possible external configuration of the Ethernet module.
 *
 * Copyright (c) 2009-2016 Atmel Corporation. All rights reserved.
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
 ***************************************************************************/


#ifndef _CONF_ETH_H_
#define _CONF_ETH_H_

/*! Number of receive buffers. Max ethernet frame size is 1526. A Rx buffer is
128 Bytes long. So 12 Rx buffers are necessary to store one max sized frame.
Multiply that by 2 for performance. */
#define ETHERNET_CONF_NB_RX_BUFFERS        24

/*! USE_RMII_INTERFACE must be defined as 1 to use an RMII interface, or 0
to use an MII interface. */
#define ETHERNET_CONF_USE_RMII_INTERFACE   1

/*! Number of Transmit buffers */
#define ETHERNET_CONF_NB_TX_BUFFERS        10

/*! Size of each Transmit buffer. */
#define ETHERNET_CONF_TX_BUFFER_SIZE       512

/*! Clock definition */
#define ETHERNET_CONF_SYSTEM_CLOCK         60000000

/*! Use Auto negotiation to get speed and duplex */
#define ETHERNET_CONF_AN_ENABLE                      1

/*! Use auto cross capability. Used by default. */
#define ETHERNET_CONF_AUTO_CROSS_ENABLE              1
/*! Use direct cable. Ignored when ETHERNET_CONF_AN_ENABLE and
    ETHERNET_CONF_AUTO_CROSS_ENABLE are enabled. */
#define ETHERNET_CONF_CROSSED_LINK                   0

#define ETHERNET_CONF_ETHADDR0                        0x50
#define ETHERNET_CONF_ETHADDR1                        0x56
#define ETHERNET_CONF_ETHADDR2                        0x64
#define ETHERNET_CONF_ETHADDR3                        0x55
#define ETHERNET_CONF_ETHADDR4                        0x39
#define ETHERNET_CONF_ETHADDR5                        0x21 
// 한전 0x20 신내, 중계, 광장, 은평 

// 0x 14까지 영신에 출고
// 0x 3B까지 영신에 출고`
// 0x 47까지 영신에 출고
/*! The IP address being used. */
//#define IFTECH_PROTOCAL
//한국도로공사용을 정의 한다.
#define IFTECH_PROTOCAL_EXP
//#define MEGAGECH_PROTOCAL
//#define KEP_SEOUL_DIVSION

#ifdef IFTECH_PROTOCAL  // 한국도로공사의 IFTECH 버전을 사용할 경우에 정의 한다.31, 32, 11,
#define UPS_TYPE   31   
#endif
#ifdef IFTECH_PROTOCAL_EXP  // 한국도로공사의 IFTECH 버전을 사용할 경우에 정의 한다.31, 32, 11,
#define UPS_TYPE   11   
#endif

#ifdef  MEGAGECH_PROTOCAL //   50  Megatech 3p3p  // 51  Megatech 3p1p // 52  Megatech 1p1p 
#define UPS_TYPE   51   
#endif

#ifdef KEP_SEOUL_DIVSION	// 한국전력공사의 한강기전은 메가텍 프로토콜을 사용한다.50  Megatech 3p3p  
#define UPS_TYPE   51   
#endif

/*! The ipaddress being used. */
#define ETHERNET_CONF_IPADDR0                        192 
#define ETHERNET_CONF_IPADDR1                        168 
#define ETHERNET_CONF_IPADDR2                        0  
#define ETHERNET_CONF_IPADDR3                        57  

/*! The gateway address being used. */
#define ETHERNET_CONF_GATEWAY_ADDR0                  192 
#define ETHERNET_CONF_GATEWAY_ADDR1                  168 
#define ETHERNET_CONF_GATEWAY_ADDR2                  0 
#define ETHERNET_CONF_GATEWAY_ADDR3                  1 

/*! The network mask being used. */
#define ETHERNET_CONF_NET_MASK0                       255
#define ETHERNET_CONF_NET_MASK1                       255
#define ETHERNET_CONF_NET_MASK2                       255
#define ETHERNET_CONF_NET_MASK3                       0

#define webHTTP_PORT		( 80 )


#if (LWIP_DNS==1)
// Default this must be server address
//#error "Please, sets the DNS domaine address using define DNS_SERVER_NAME, and remove this #error"
// This is used if DNS is enbale
//#define DNS_SERVER_NAME "yourdnsdomain.com"
#define DNS_SERVER_NAME "mail.iftech.co.kr"
#endif

#endif
