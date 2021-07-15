/*
 * megatech_protocal.h
 *
 * Created: 2018-07-17 14:20:43
 *  Author: STELLA
 */ 


#ifndef MEGATECH_PROTOCAL_H_
#define MEGATECH_PROTOCAL_H_


unsigned char strToInt(char *str );
char *strtokExt(char* str, char* desti);
void strtoreplace(char* str);
void strtoreplace(char* str);

bool requestUpsStatus_Q1_megatec();
int megatec_command_CR(uint8_t *send_command,uint8_t sLength,uint8_t startByte,int watingTime,uint8_t *receive_data);
Bool parseQ1_megatec(char *str);

bool requestUps_G1_megatec();
bool parseG1_megatec(char *str);

bool requestUps_G2_megatec();
Bool parse_G2_megatec(char *str);

bool requestUps_G3_megatec();
Bool parseG3_megatec(char *str);

bool upsInformationCommand_I_megatec();
Bool parse_I_megatec(char *str);

bool upsInformationCommand_F_megatec();
Bool parse_F_megatec(char *str);
bool upsInformationCommand_GF_megatec();
Bool parse_GF_megatec(char *str);
Bool requestUpsStatus_megatec();

void checkTrap_2phase_megatec();
void checkTrap_3phase_megatec();

void  write_log_event_megatec();
void TurnOnOff_Q_megatec();
void Test10Second_T_megatec();
void CancelShutDown_C_megatec();
void TestUntilBatteryLow_TL_megatec();
void CancelTest_CT_megatec();
#endif /* MEGATECH_PROTOCAL_H_ */
