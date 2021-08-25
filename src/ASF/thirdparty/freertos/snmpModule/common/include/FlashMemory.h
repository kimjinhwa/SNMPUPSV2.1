/*
 * flash.h
 *
 * Created: 2017-11-27 20:34:26
 *  Author: STELLA
 */ 


#ifndef FLASHMEMORY_H_
#define FLASHMEMORY_H_
#include "stdio.h"
//#define portLONG        long
//typedef unsigned portLONG portTickType;
//! Structure type containing variables to store in NVRAM using a specific
//! memory map.
#define  MAX_LOG	200
/*
typedef const struct {
	uint8_t  var8;
	uint16_t var16;
	uint8_t  var8_3[3];
	uint32_t var32;
} nvram_data_t;
*/
 
typedef const struct {
	uint8_t  Ethernet_Conf_EthAddr0;    // 0
	uint8_t  Ethernet_Conf_EthAddr1;		
	uint8_t  Ethernet_Conf_EthAddr2;
	uint8_t  Ethernet_Conf_EthAddr3;
	uint8_t  Ethernet_Conf_EthAddr4;
	uint8_t  Ethernet_Conf_EthAddr5;	//5
	
	uint8_t  Ethernet_Conf_IpAddr0;		//6
	uint8_t  Ethernet_Conf_IpAddr1;
	uint8_t  Ethernet_Conf_IpAddr2;
	uint8_t  Ethernet_Conf_IpAddr3;		//9
	
	uint8_t  Ethernet_Conf_Gateway_Addr0; 
	uint8_t  Ethernet_Conf_Gateway_Addr1; 
	uint8_t  Ethernet_Conf_Gateway_Addr2; 
	uint8_t  Ethernet_Conf_Gateway_Addr3; //14
	
	uint8_t  Ethernet_Conf_Net_Mask0; 
	uint8_t  Ethernet_Conf_Net_Mask1; 
	uint8_t  Ethernet_Conf_Net_Mask2; 
	uint8_t  Ethernet_Conf_Net_Mask3;  // 18
	uint8_t	port;
} nvram_data_ethernet_t;

typedef struct {
	uint8_t  Ethernet_Conf_EthAddr0;    // 0
	uint8_t  Ethernet_Conf_EthAddr1;
	uint8_t  Ethernet_Conf_EthAddr2;
	uint8_t  Ethernet_Conf_EthAddr3;
	uint8_t  Ethernet_Conf_EthAddr4;
	uint8_t  Ethernet_Conf_EthAddr5;	//5
	
	uint8_t  Ethernet_Conf_IpAddr0;		//6
	uint8_t  Ethernet_Conf_IpAddr1;
	uint8_t  Ethernet_Conf_IpAddr2;
	uint8_t  Ethernet_Conf_IpAddr3;		//9
	
	uint8_t  Ethernet_Conf_Gateway_Addr0;
	uint8_t  Ethernet_Conf_Gateway_Addr1;
	uint8_t  Ethernet_Conf_Gateway_Addr2;
	uint8_t  Ethernet_Conf_Gateway_Addr3; //14
	
	uint8_t  Ethernet_Conf_Net_Mask0;
	uint8_t  Ethernet_Conf_Net_Mask1;
	uint8_t  Ethernet_Conf_Net_Mask2;
	uint8_t  Ethernet_Conf_Net_Mask3;  // 18
	
	uint8_t	port;
} data_ethernet_t;


typedef const struct {
	uint8_t  Ethernet_Conf_IpAddr0;		//6
	uint8_t  Ethernet_Conf_IpAddr1;
	uint8_t  Ethernet_Conf_IpAddr2;
	uint8_t  Ethernet_Conf_IpAddr3;		//9
} nvram_trap_address_t;    // 10개를 사용할 계획이므로  총 40바이트......18 + 40 = 58개 사용

typedef const struct {
	uint8_t  Ethernet_Conf_IpAddr0;		//6
	uint8_t  Ethernet_Conf_IpAddr1;
	uint8_t  Ethernet_Conf_IpAddr2;
	uint8_t  Ethernet_Conf_IpAddr3;		//9
} nvram_trap_address_tt;    // 10개를 사용할 계획이므로  총 40바이트......18 + 40 = 58개 사용

typedef struct {
	uint8_t  Ethernet_Conf_IpAddr0;		//6
	uint8_t  Ethernet_Conf_IpAddr1;
	uint8_t  Ethernet_Conf_IpAddr2;
	uint8_t  Ethernet_Conf_IpAddr3;		//9
} trap_address_t;    // 10개를 사용할 계획이므로  총 40바이트......18 + 40 = 58개 사용


typedef const struct {
	uint8_t  Ethernet_Conf_IpAddr0;		//6
	uint8_t  Ethernet_Conf_IpAddr1;
	uint8_t  Ethernet_Conf_IpAddr2;
	uint8_t  Ethernet_Conf_IpAddr3;		//9
} nvram_timeserver_address_t;    // 10개를 사용할 계획이므로  총 40바이트......18 + 40 = 58개 사용

typedef struct {
	uint8_t  Ethernet_Conf_IpAddr0;		//6
	uint8_t  Ethernet_Conf_IpAddr1;
	uint8_t  Ethernet_Conf_IpAddr2;
	uint8_t  Ethernet_Conf_IpAddr3;		//9
} timerserver_address_t;    // 10개를 사용할 계획이므로  총 40바이트......18 + 40 = 58개 사용


typedef const struct {
	uint8_t head;
	uint8_t last_log_position;
}	nvram_log_pos_t;

typedef struct {
	uint8_t head;
	uint8_t last_log_position;
}	log_pos_t;

typedef const struct {        // 6바이트로그 플래시 메모리에 사용한다. 총 1000개를 사용하여 6000개의 데이타를 사용한다.
	uint32_t systemTime;
	uint8_t	kind;	
	uint16_t	event;	
} nvram_log_t;
typedef const struct {        // 6바이트로그 플래시 메모리에 사용한다. 총 1000개를 사용하여 6000개의 데이타를 사용한다.
	uint32_t systemTime;
	uint8_t	kind;
	uint16_t	event;
} nvram_log_tg;

typedef struct {        // 6바이트로그 플래시 메모리에 사용한다. 총 1000개를 사용하여 6000개의 데이타를 사용한다.
	uint32_t systemTime;
	uint8_t	kind;
	uint16_t	event;
} log_t;

typedef struct {
	uint8_t email[30];
	uint8_t	cmd;
	}nvram_send_event_mail_t ;

typedef struct {
	uint8_t e_mail_server[30];
	uint8_t sender_mail_id[30];
	uint8_t email_user_id[30];
	uint8_t email_user_passwd[20];
	Bool	is_need_authentic;
	Bool    is_send_mail_Event;
	} smtp_info_t;
	
typedef struct {
	uint8_t install_year;  // 2018 -> 18
	uint8_t install_month;
	uint8_t install_day;
	uint8_t	ups_type;   // 33, 31, 11,
	uint8_t company_code;
	uint8_t upsIdentManufacturer[31];  // 0..31
	uint8_t upsIdentModel[63];  // 0..63
	uint8_t upsIdentUPSSoftwareVersion[11];  // 
	uint8_t upsIdentAgentSoftwareVersion[11];  
	uint8_t upsIdentName[63];  // 0..63
	uint8_t upsIdentAttachedDevices[30];  // 0..63  to big --> down size to 30 
	uint8_t syscontact[17]; 
	uint8_t sysname[40];
	uint8_t syslocation[18];
	uint16_t capacity;
	uint16_t input_voltage;
	uint16_t input_frequency;
	uint16_t inputMin_voltage;
	uint16_t inputMax_voltage;
	uint16_t charging_voltage;
	uint16_t out_voltage;
	uint16_t output_frequency;
	uint16_t installed_battery;
	uint16_t ups_version;   // 121 -> 1.21
	uint16_t protocal;   // 121 -> 1.21
	uint8_t bat_install_year;  // 2018 -> 18
	uint8_t bat_install_month;
	uint8_t bat_install_day;
	uint8_t log_write_period;   // 로그기록주기 분
	uint8_t ups_test_period;    // 로그기록주기  0 1 2주
	uint8_t ups_test_day;		//   테스트요일
	uint8_t ups_test_hour;		//   테스트시간
	uint8_t ups_test_minute;    //   테스트시간
	uint8_t limit_load;			// keyvalue[10]  : SB9 한계부하(%)  100
	uint8_t limit_temp;			// keyvalue[11]  : SBA 한계온도(℃) 70.0
	uint8_t limit_capacity;		// keyvalue[12]  : SBB 한계용량(%)  100
	uint8_t alarm_audible;		// keyvalue[12]  : SBB 한계용량(%)  100
	uint8_t user_id[15];		
	uint8_t passwd[15];		
	uint8_t batLastReplaceDate[8];  // 
}ups_info_t;   // total 138 byte
	
//void initial_flashMemory();
// 로그데이타를 기록한다.
void writeLog(uint32_t time, uint8_t kind, uint16_t evt);
// 첫 번째 로그를 갖고 온다.
uint8_t getFirstLog(log_t *tlog) ;

//portTickType  get_last_log_time();

// 다음번 로그를 갖고 온다.
// 없으면 -1을 리턴한다. 있다면 보낸 위치를 리턴한다.
int8_t getNextLog(int iPosition,log_t *tlog);
// 특정 위치의 로그를 리턴한다.
int get_nvram_getlogposition(uint16_t pos,log_t *tlog);

// write ehternet info
uint8_t flash_write_reLoadTime(uint8_t *reloadTime);
uint8_t flash_read_reLoadTime();
void flash_read__ethernetInfo(data_ethernet_t *ethernet_t);
void flash_write__ethernetInfo(data_ethernet_t *ethernet_t);
void trapIpAddressRead(uint8_t pos,trap_address_t *trap_address);
void trapIpAddressWrite(uint8_t pos,trap_address_t *trap_address);
void TimeServerIpAddressRead(uint8_t pos,timerserver_address_t *timeServer_address);
void TimeServerIpAddressWrite(uint8_t pos,timerserver_address_t *trap_address);
uint8_t getLogCount();
uint8_t getLogPoslast_log_position(nvram_log_pos_t *nvram_data);
uint8_t getLogPosHead(nvram_log_pos_t *nvram_data);
//void getLogPosPointer(log_t *log);

void getLogPosPointer(log_pos_t *log_pos);
//void flash_write_email_list(int pos, const uint8_t *emailAddress);
void flash_write_email_list(int pos,nvram_send_event_mail_t *emailAddress);
void flash_read_email_list(int pos, nvram_send_event_mail_t *emailAddress);
void flash_write_smtp_info(smtp_info_t *smtp_info);
void flash_read_smtp_info(smtp_info_t *smtp_info);
void flash_write_ups_info(ups_info_t *upsinfo);
void flash_read_ups_info(ups_info_t *upsinfo);
//__attribute__((__section__(".flash_nvram")))
//extern  nvram_log_pos_t	nvram_log_pos;


#endif /* FLASH_H_ */