/*
 * iftech_protocal.h
 *
 * Created: 2020-03-28 15:19:07
 *  Author: STELLA
 */ 


#ifndef IFTECH_PROTOCAL_H_
#define IFTECH_PROTOCAL_H_


uint16_t calcCRC(uint8_t *au8Buffer, uint8_t u8length);
bool requestUpsData_22_32_33();
void checkTrap_2phase_iftech_22();
void checkTrap_3phase_iftech_32_33();
void  write_log_event_iftech_22_32_33();


#endif /* IFTECH_PROTOCAL_H_ */