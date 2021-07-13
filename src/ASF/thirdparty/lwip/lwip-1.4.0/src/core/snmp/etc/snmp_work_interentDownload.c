#ifndef SNMP_H_
#define SNMP_H_
 
#include "lwip/snmp_structs.h"
#include "lwip/tcpip.h"
#include <stdint.h>
#include "pmbus.h"
 
#define SNMP_SEND_TRAP                 1
 
typedef struct snmp_message
{
   uint32_t                 COMMAND;
   struct ip_addr                        ADDRESS;
   unsigned char                        TRAP;
   char                                         *message;
} SNMP_MESSAGE, *SNMP_MESSAGE_PTR;
 
extern void snmp_thread_entry(void *arg);
 
extern void send_trap(struct ip_addr addr, unsigned char trap, char *message);
 
#endif /* SNMP_H_ */
 
 
#include "private_mib.h"
#include "lwip/snmp.h"
#include "lwip/snmp_msg.h"
#include "lwip/snmp_asn1.h"
#include "lwip/snmp_structs.h"
#include "lwip/mem.h"
#include "snmp.h"
 
extern struct snmp_msg_trap trap_msg;
 
// Private MIB code from python script here
 
void
lwip_privmib_init(void)
{
 
}
 
void send_trap_callback( void * parameters )
{
                struct snmp_obj_id eoid;
                // There should be a way to do this directly.
                eoid.id[0] = enterprises_ids[1];
                eoid.len = 1;
 
                snmp_send_trap(SNMP_GENTRAP_ENTERPRISESPC, &eoid, 1);
                snmp_varbind_list_free(&trap_msg.outvb);
}
 
void snmp_thread_entry(void *arg)
{
        SNMP_MESSAGE_PTR snmp_message;
        struct snmp_obj_id objid;
 
        // There should be a better way to initialize this.
        objid.len = 1;
        objid.id[0] = private_ids[0];
 
        snmp_init();
 
 
        cyg_mbox_create(&snmp_mailbox_handle, &snmp_mailbox);
 
        while (1)
        {
                snmp_message = (SNMP_MESSAGE_PTR) cyg_mbox_get(snmp_mailbox_handle);
                if (snmp_message->COMMAND == SNMP_SEND_TRAP)
                {
                        struct ip_addr addr;
                        // Reverse the bytes because snmp_trap_dst_ip_set reverses them again.
                        addr.addr = htonl(snmp_message->ADDRESS.addr);
                        snmp_trap_dst_ip_set(0,&addr);
                        snmp_trap_dst_enable(0,snmp_message->TRAP);
 
                        struct snmp_varbind *vb = snmp_varbind_alloc(&objid, SNMP_ASN1_OPAQUE, strlen(snmp_message->message)+1);
                        if (vb == NULL)
                        {
                                diag_printf("SNMP failure to allocate memory\n");
                                continue;
                        }
                        strcpy (vb->value, snmp_message->message);
                        snmp_varbind_tail_add(&trap_msg.outvb,vb);
                        tcpip_callback(send_trap_callback, NULL);
                }
                free(snmp_message->message);
                free(snmp_message);
        }
}
 
void send_trap(struct ip_addr addr, unsigned char trap, char *message)
{
        SNMP_MESSAGE_PTR snmp_message = (SNMP_MESSAGE_PTR) malloc(sizeof(SNMP_MESSAGE));
        snmp_message->COMMAND = SNMP_SEND_TRAP;
        snmp_message->TRAP = trap;
        snmp_message->ADDRESS = addr;
        snmp_message->message = (char*) malloc(strlen(message)+1);
        strcpy(snmp_message->message, message);
        cyg_mbox_put(snmp_mailbox_handle, (void *) snmp_message);
}
 
 
Code to start the snmp thread:
 
void start_network(cyg_addrword_t p)
{
        struct ip_addr ipaddr, netmask, gw;
        struct netif *netif;
 
        /* Set network address variables */
        IP_GATEWAY(&gw);
        IP_ADDRESS(&ipaddr);
        IP_MASK(&netmask);
 
        //tcpip_init(NULL, NULL);
 
        // Check return for null
        //netif_result = netif_add(&netif, &ipaddr, &netmask, &gw, NULL, init_ip, tcpip_input);
        //netif_set_up(&netif);
 
    cyg_lwip_sequential_init();
 
    netif = netif_find("et0");
    netif_set_ipaddr(netif, &ipaddr);
    netif_set_netmask(netif, &netmask);
    netif_set_gw(netif, &gw);
 
    netif_set_up(netif);
 
    cyg_lwip_thread_new("snmp", snmp_thread_entry, NULL,
                        snmp_stack, STACK_SIZE, 7);
 
    cyg_thread_exit();
}