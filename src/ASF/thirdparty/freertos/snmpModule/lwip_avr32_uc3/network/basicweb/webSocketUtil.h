/*
 * webSocketUtil.h
 *
 * Created: 2017-11-24 10:47:59
 *  Author: STELLA
 */ 


#ifndef WEBSOCKETUTIL_H_
#define WEBSOCKETUTIL_H_


static unsigned long (__mzapi_decode_base64__)(int s_character);
char * (mzapi_encode_base64)(const uint8_t * s_string);
char * (mzapi_decode_base64)(const uint8_t * s_string);
//int urlDecode(char *str);
char *urlDecode(const char *str);
int webSocket_decode(char *pcRxString);
int webSocket_encode(char *pcRxString);
int webSocket_encodeEx(bool encode,char*desc,char *src);
#endif /* WEBSOCKETUTIL_H_ */