/*
 * print_funcs_tcpip.h
 *
 * Created: 2017-12-04 09:53:03
 *  Author: STELLA
 */ 


#ifndef PRINT_FUNCS_TCPIP_H_
#define PRINT_FUNCS_TCPIP_H_

int8_t print_dbg_tcpip(const char *str);
int8_t print_dbg_char_hex_tcpip(const char *str,int len);
//int print_dbg_tcpip_close();
int print_dbg_tcpip_connect(const char *debug_host,int debug_port);

//void broadCastMyMacAndIpAddress();



#endif /* PRINT_FUNCS_TCPIP_H_ */