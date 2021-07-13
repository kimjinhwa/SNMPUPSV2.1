//
  //myDNS.c
 
//  Created: 2018-04-24 21:26:29
//   Author: STELLA
 

//err_t errt = netconn_gethostbyname ( mail_server_name, &resoved_addr );

#include "api.h"
#include "myDNS.h"
#include "tcpip.h"
#include "string.h"
#include "udp.h"
#include "mem.h"

/** DNS server port address */
#ifndef DNS_SERVER_PORT
#define DNS_SERVER_PORT           53
#endif


#define DNS_SERVER_ADDRESS(ipaddr)        (ip4_addr_set_u32(ipaddr, ipaddr_addr("208.67.222.222"))) /* resolver1.opendns.com */
#define DNS_SERVER_ADDRESS_1(ipaddr)        (ip4_addr_set_u32(ipaddr, ipaddr_addr("164.124.101.2"))) /* resolver1.opendns.com */
#define SIZEOF_DNS_HDR 12
#define SIZEOF_DNS_QUERY 4
#define SIZEOF_DNS_ANSWER 10

#define DNS_STATE_UNUSED          0
#define DNS_STATE_NEW             1
#define DNS_STATE_ASKING          2
#define DNS_STATE_DONE            3
#ifndef DNS_MAX_TTL
#define DNS_MAX_TTL               604800
#endif

static u8_t*                  dns_payload;
static struct udp_pcb        *dns_pcb;

struct dns_query {
  /* DNS query record starts with either a domain name or a pointer
     to a name already present somewhere in the packet. */
  u16_t type;
  u16_t cls;
};

struct dns_hdr {
	PACK_STRUCT_FIELD(u16_t id);
	PACK_STRUCT_FIELD(u8_t flags1);
	PACK_STRUCT_FIELD(u8_t flags2);
	PACK_STRUCT_FIELD(u16_t numquestions);
	PACK_STRUCT_FIELD(u16_t numanswers);
	PACK_STRUCT_FIELD(u16_t numauthrr);
	PACK_STRUCT_FIELD(u16_t numextrarr);
} PACK_STRUCT_STRUCT;
struct dns_answer {
  /* DNS answer record starts with either a domain name or a pointer
     to a name already present somewhere in the packet. */
  u16_t type;
  u16_t cls;
  u32_t ttl;
  u16_t len;
};

struct dns_table_entry {
	u8_t  state;
	u8_t  numdns;
	u8_t  tmr;
	u8_t  retries;
	u8_t  seqno;
	u8_t  err;
	u32_t ttl;
	char name[DNS_MAX_NAME_LENGTH];
	ip_addr_t ipaddr;
	/* pointer to callback on DNS query done */
	dns_found_callback found;
	void *arg;
};



static struct dns_table_entry dns_table[DNS_TABLE_SIZE];
static ip_addr_t              dns_servers[DNS_MAX_SERVERS];
static u8_t                   dns_payload_buffer[LWIP_MEM_ALIGN_BUFFER(DNS_MSG_SIZE)];

void dns_init()
{
	ip_addr_t dnsserver;
	dns_payload = (u8_t *)LWIP_MEM_ALIGN(dns_payload_buffer);
	
	if (dns_pcb == NULL) {
		dns_pcb = udp_new();
		if (dns_pcb != NULL) {
			udp_bind(dns_pcb, IP_ADDR_ANY, 0);
			udp_recv(dns_pcb, dns_recv, NULL);
			
			DNS_SERVER_ADDRESS(&dnsserver);
			dns_setserver(0, &dnsserver);  // DNS의 아이피를 맟추어 준다.
			DNS_SERVER_ADDRESS_1(&dnsserver);
			dns_setserver(1, &dnsserver);  // DNS의 아이피를 맟추어 준다.
		}
	}
}


err_t dns_gethostbyname(const char *hostname, ip_addr_t *addr, dns_found_callback found,
                  void *callback_arg)
{
  
  u32_t ipaddr;
  
  if ((dns_pcb == NULL) || (addr == NULL) ||
      (!hostname) || (!hostname[0]) ||
      (strlen(hostname) >= DNS_MAX_NAME_LENGTH)) {
    return ERR_ARG;
  }
  
  ipaddr = ipaddr_addr(hostname);
  if (ipaddr == IPADDR_NONE) {
    ipaddr = dns_lookup(hostname);
  }
  
  if (ipaddr != IPADDR_NONE) {
    ip4_addr_set_u32(addr, ipaddr);
    return ERR_OK;
  }
  //return dns_enqueue(hostname, found, callback_arg);
}
static void
do_dns_found(const char *name, ip_addr_t *ipaddr, void *arg)
{
	struct dns_api_msg *msg = (struct dns_api_msg*)arg;

	LWIP_ASSERT("DNS response for wrong host name", strcmp(msg->name, name) == 0);
	LWIP_UNUSED_ARG(name);

	if (ipaddr == NULL) {
		/* timeout or memory error */
		*msg->err = ERR_VAL;
		} else {
		/* address was resolved */
		*msg->err = ERR_OK;
		*msg->addr = *ipaddr;
	}
	/* wake up the application task waiting in netconn_gethostbyname */
	sys_sem_signal(msg->sem);
}



err_t gethost_byname(const char *name, ip_addr_t *addr)
{

  
  dns_init();
  
  
  err_t err;
  /*
  struct dns_api_msg msg;
  
  sys_sem_t sem;
  
  LWIP_ERROR("netconn_gethostbyname: invalid name", (name != NULL), return ERR_ARG;);
  LWIP_ERROR("netconn_gethostbyname: invalid addr", (addr != NULL), return ERR_ARG;);



  //err = sys_sem_new(&sem, 0);
  
  
  //if (err != ERR_OK) {
  //	  return err;
  //}

  msg.name = name;
  msg.addr = addr;
  msg.err = &err;
  msg.sem = &sem;
  
  //tcpip_callback(do_gethostbyname, &msg);
  //sys_sem_wait(&sem);
  //sys_sem_free(&sem);
  */
  return err;
}

void do_gethostbyname(void *arg)
{
  struct dns_api_msg *msg = (struct dns_api_msg*)arg;

  *msg->err = dns_gethostbyname(msg->name, msg->addr, do_dns_found, msg);
  if (*msg->err != ERR_INPROGRESS) {
    /* on error or immediate success, wake up the application
     * task waiting in netconn_gethostbyname */
    sys_sem_signal(msg->sem);
  }
}


static void
dns_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
	
  u16_t i;
  char *pHostname;
  struct dns_hdr *hdr;
  struct dns_answer ans;
  struct dns_table_entry *pEntry;
  u16_t nquestions, nanswers;
  
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(pcb);
  LWIP_UNUSED_ARG(addr);
  LWIP_UNUSED_ARG(port);

  // is the dns message too big ? 
  if (p->tot_len > DNS_MSG_SIZE) {
    LWIP_DEBUGF(DNS_DEBUG, ("dns_recv: pbuf too big\n"));
    // free pbuf and return 
    goto memerr;
  }
	
  // is the dns message big enough ? 
  if (p->tot_len < (SIZEOF_DNS_HDR + SIZEOF_DNS_QUERY + SIZEOF_DNS_ANSWER)) {
    LWIP_DEBUGF(DNS_DEBUG, ("dns_recv: pbuf too small\n"));
    // free pbuf and return 
    goto memerr;
  }

  // copy dns payload inside static buffer for processing  
  if (pbuf_copy_partial(p, dns_payload, p->tot_len, 0) == p->tot_len) {
    // The ID in the DNS header should be our entry into the name table. 
    hdr = (struct dns_hdr*)dns_payload;
    i = htons(hdr->id);
    if (i < DNS_TABLE_SIZE) {
      pEntry = &dns_table[i];
      if(pEntry->state == DNS_STATE_ASKING) {
        // This entry is now completed. 
        pEntry->state = DNS_STATE_DONE;
        pEntry->err   = hdr->flags2 & DNS_FLAG2_ERR_MASK;

        // We only care about the question(s) and the answers. The authrr
        //   and the extrarr are simply discarded. 
        nquestions = htons(hdr->numquestions);
        nanswers   = htons(hdr->numanswers);

        // Check for error. If so, call callback to inform. 
        if (((hdr->flags1 & DNS_FLAG1_RESPONSE) == 0) || (pEntry->err != 0) || (nquestions != 1)) {
          LWIP_DEBUGF(DNS_DEBUG, ("dns_recv: \"%s\": error in flags\n", pEntry->name));
          // call callback to indicate error, clean up memory and return 
          goto responseerr;
        }

#if DNS_DOES_NAME_CHECK
        // Check if the name in the "question" part match with the name in the entry. 
        if (dns_compare_name((unsigned char *)(pEntry->name), (unsigned char *)dns_payload + SIZEOF_DNS_HDR) != 0) {
          LWIP_DEBUGF(DNS_DEBUG, ("dns_recv: \"%s\": response not match to query\n", pEntry->name));
          // call callback to indicate error, clean up memory and return 
          goto responseerr;
        }
#endif // DNS_DOES_NAME_CHECK 

        // Skip the name in the "question" part 
        pHostname = (char *) dns_parse_name((unsigned char *)dns_payload + SIZEOF_DNS_HDR) + SIZEOF_DNS_QUERY;

        while (nanswers > 0) {
          // skip answer resource record's host name 
          pHostname = (char *) dns_parse_name((unsigned char *)pHostname);

          // Check for IP address type and Internet class. Others are discarded. 
          SMEMCPY(&ans, pHostname, SIZEOF_DNS_ANSWER);
          if((ans.type == PP_HTONS(DNS_RRTYPE_A)) && (ans.cls == PP_HTONS(DNS_RRCLASS_IN)) &&
             (ans.len == PP_HTONS(sizeof(ip_addr_t))) ) {
            // read the answer resource record's TTL, and maximize it if needed 
            pEntry->ttl = ntohl(ans.ttl);
            if (pEntry->ttl > DNS_MAX_TTL) {
              pEntry->ttl = DNS_MAX_TTL;
            }
            // read the IP address after answer resource record's header 
            SMEMCPY(&(pEntry->ipaddr), (pHostname+SIZEOF_DNS_ANSWER), sizeof(ip_addr_t));
            LWIP_DEBUGF(DNS_DEBUG, ("dns_recv: \"%s\": response = ", pEntry->name));
            ip_addr_debug_print(DNS_DEBUG, (&(pEntry->ipaddr)));
            LWIP_DEBUGF(DNS_DEBUG, ("\n"));
            // call specified callback function if provided 
            if (pEntry->found) {
              (*pEntry->found)(pEntry->name, &pEntry->ipaddr, pEntry->arg);
            }
            // deallocate memory and return 
            goto memerr;
          } else {
            pHostname = pHostname + SIZEOF_DNS_ANSWER + htons(ans.len);
          }
          --nanswers;
        }
        LWIP_DEBUGF(DNS_DEBUG, ("dns_recv: \"%s\": error in response\n", pEntry->name));
        // call callback to indicate error, clean up memory and return 
        goto responseerr;
      }
    }
  }

  // deallocate memory and return 
  goto memerr;

responseerr:
  // ERROR: call specified callback function with NULL as name to indicate an error 
  if (pEntry->found) {
    (*pEntry->found)(pEntry->name, NULL, pEntry->arg);
  }
  // flush this entry 
  pEntry->state = DNS_STATE_UNUSED;
  pEntry->found = NULL;

memerr:
  // free pbuf 
  pbuf_free(p);
  
  return;
}


static unsigned char *
dns_parse_name(unsigned char *query)
{
	unsigned char n;

	do {
		n = *query++;
		/** @see RFC 1035 - 4.1.4. Message compression */
		if ((n & 0xc0) == 0xc0) {
			/* Compressed name */
			break;
			} else {
			/* Not compressed name */
			while (n > 0) {
				++query;
				--n;
			};
		}
	} while (*query != 0);

	return query + 1;
}

static u8_t
dns_compare_name(unsigned char *query, unsigned char *response)
{
	unsigned char n;

	do {
		n = *response++;
		/** @see RFC 1035 - 4.1.4. Message compression */
		if ((n & 0xc0) == 0xc0) {
			/* Compressed name */
			break;
			} else {
			/* Not compressed name */
			while (n > 0) {
				if ((*query) != (*response)) {
					return 1;
				}
				++response;
				++query;
				--n;
			};
			++query;
		}
	} while (*response != 0);

	return 0;
}

static u32_t dns_lookup(const char *name)
{
	u8_t i;
	#if DNS_LOCAL_HOSTLIST || defined(DNS_LOOKUP_LOCAL_EXTERN)
	u32_t addr;
	#endif /* DNS_LOCAL_HOSTLIST || defined(DNS_LOOKUP_LOCAL_EXTERN) */
	#if DNS_LOCAL_HOSTLIST
	if ((addr = dns_lookup_local(name)) != IPADDR_NONE) {
		return addr;
	}
	#endif /* DNS_LOCAL_HOSTLIST */
	#ifdef DNS_LOOKUP_LOCAL_EXTERN
	if((addr = DNS_LOOKUP_LOCAL_EXTERN(name)) != IPADDR_NONE) {
		return addr;
	}
	#endif /* DNS_LOOKUP_LOCAL_EXTERN */

	/* Walk through name list, return entry if found. If not, return NULL. */
	for (i = 0; i < DNS_TABLE_SIZE; ++i) {
		if ((dns_table[i].state == DNS_STATE_DONE) &&
		(strcmp(name, dns_table[i].name) == 0)) {
			LWIP_DEBUGF(DNS_DEBUG, ("dns_lookup: \"%s\": found = ", name));
			ip_addr_debug_print(DNS_DEBUG, &(dns_table[i].ipaddr));
			LWIP_DEBUGF(DNS_DEBUG, ("\n"));
			return ip4_addr_get_u32(&dns_table[i].ipaddr);
		}
	}

	return IPADDR_NONE;
}


void dns_setserver(u8_t numdns, ip_addr_t *dnsserver)
{
	if ((numdns < DNS_MAX_SERVERS) && (dns_pcb != NULL) &&
	(dnsserver != NULL) && !ip_addr_isany(dnsserver)) {
		dns_servers[numdns] = (*dnsserver);
	}
}

static err_t dns_send(u8_t numdns, const char* name, u8_t id)
{
  err_t err;
  struct dns_hdr *hdr;
  struct dns_query qry;
  struct pbuf *p;
  char *query, *nptr;
  const char *pHostname;
  u8_t n;

  LWIP_DEBUGF(DNS_DEBUG, ("dns_send: dns_servers[%"U16_F"] \"%s\": request\n",
              (u16_t)(numdns), name));
  LWIP_ASSERT("dns server out of array", numdns < DNS_MAX_SERVERS);
  LWIP_ASSERT("dns server has no IP address set", !ip_addr_isany(&dns_servers[numdns]));

  /* if here, we have either a new query or a retry on a previous query to process */
  p = pbuf_alloc(PBUF_TRANSPORT, SIZEOF_DNS_HDR + DNS_MAX_NAME_LENGTH +
                 SIZEOF_DNS_QUERY, PBUF_RAM);
  if (p != NULL) {
    LWIP_ASSERT("pbuf must be in one piece", p->next == NULL);
    /* fill dns header */
    hdr = (struct dns_hdr*)p->payload;
    memset(hdr, 0, SIZEOF_DNS_HDR);
    hdr->id = htons(id);
    hdr->flags1 = DNS_FLAG1_RD;
    hdr->numquestions = PP_HTONS(1);
    query = (char*)hdr + SIZEOF_DNS_HDR;
    pHostname = name;
    --pHostname;

    /* convert hostname into suitable query format. */
    do {
      ++pHostname;
      nptr = query;
      ++query;
      for(n = 0; *pHostname != '.' && *pHostname != 0; ++pHostname) {
        *query = *pHostname;
        ++query;
        ++n;
      }
      *nptr = n;
    } while(*pHostname != 0);
    *query++='\0';

    /* fill dns query */
    qry.type = PP_HTONS(DNS_RRTYPE_A);
    qry.cls = PP_HTONS(DNS_RRCLASS_IN);
    SMEMCPY(query, &qry, SIZEOF_DNS_QUERY);

    /* resize pbuf to the exact dns query */
    pbuf_realloc(p, (u16_t)((query + SIZEOF_DNS_QUERY) - ((char*)(p->payload))));

    /* connect to the server for faster receiving */
    udp_connect(dns_pcb, &dns_servers[numdns], DNS_SERVER_PORT);
    /* send dns packet */
    err = udp_sendto(dns_pcb, p, &dns_servers[numdns], DNS_SERVER_PORT);

    /* free pbuf */
    pbuf_free(p);
  } else {
    err = ERR_MEM;
  }

  return err;
}