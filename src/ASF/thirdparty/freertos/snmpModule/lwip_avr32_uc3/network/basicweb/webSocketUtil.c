/*
 * webSocketUtil.c
 *
 * Created: 2017-11-24 10:47:07
 *  Author: STELLA
 */ 
/*
 Copyright (C) Information Equipment co.,LTD.
 All rights reserved.
 Code by JaeHyuk Cho <mailto:minzkn@infoeq.com>
 CVSTAG="$Header: /usr/local/mutihost/joinc/modules/moniwiki/data/text/RCS/Code_2fC_2fbase64,v 1.2 2007/01/24 02:16:30 root Exp root $"

 - Simple is best !
*/

#if !defined(__def_mzapi_source_base64_c__)
#define __def_mzapi_source_base64_c__ "base64.c"

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include <malloc.h>
#include "string.h"
#include "ctype.h"
#if !defined(__mzapi_peek_vector__)
# define __mzapi_peek_vector__(m_cast,m_base,m_sign,m_offset)         ((m_cast)(((unsigned char *)(m_base)) m_sign (size_t)(m_offset)))
#endif
#if !defined(mzapi_peek_byte)
# define mzapi_peek_byte(m_base,m_offset)                             (*__mzapi_peek_vector__(unsigned char *,m_base,+,m_offset))
#endif
#if !defined(mzapi_poke_byte)
# define mzapi_poke_byte(m_base,m_offset,m_value)                     (*__mzapi_peek_vector__(unsigned char *,m_base,+,m_offset)) = (unsigned char)(m_value)
#endif


static unsigned long (__mzapi_decode_base64__)(int s_character);
char * (mzapi_encode_base64)(const uint8_t * s_string);
char * (mzapi_decode_base64)(const uint8_t * s_string);
              
static unsigned long (__mzapi_decode_base64__)(int s_character)
{
	if((s_character) >= ((int)'a'))return((((unsigned long)(s_character)) - ((unsigned long)'a')) + 26lu);
	else if((s_character) >= ((int)'A'))return(((unsigned long)(s_character)) - ((unsigned long)'A'));
	else if((s_character) >= ((int)'0'))return((((unsigned long)(s_character)) - ((unsigned long)'0')) + 52lu);
	else if((s_character) == ((int)'+'))return(62lu);
	else if((s_character) == ((int)'/'))return(63lu);
	return(0lu);
}

char * (mzapi_encode_base64)(const uint8_t * s_string)
{
	static const unsigned char c_alpha_table[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="};
	char * s_result;
	size_t s_length = strlen(s_string), s_source_offset = (size_t)0, s_target_offset = (size_t)0;
	//if(s_length > 20)s_length=20;
	unsigned long s_value;
	int s_quad, s_trip;
	s_result = (char *)malloc((((s_length + ((size_t)2)) / ((size_t)3)) << 2) + ((size_t)1));
	if(s_result == ((char *)0))return((char *)0);
	while(s_source_offset < s_length)
	{
		s_value = ((unsigned long)(mzapi_peek_byte((void *)s_string, s_source_offset) & ((int)0xff))) << 8;
		if((s_source_offset + ((size_t)1)) < s_length)
		{
			s_value |= (unsigned long)(mzapi_peek_byte((void *)s_string, s_source_offset + ((size_t)1)) & ((int)0xff));
			s_trip = (int)1;
		}
		else s_trip = (int)0;
		s_value <<= 8;
		if((s_source_offset + ((size_t)2)) < s_length)
		{
			s_value |= (unsigned long)(mzapi_peek_byte((void *)s_string, s_source_offset + ((size_t)2)) & ((int)0xff));
			s_quad = (int)1;
		}
		else s_quad = (int)0;
		mzapi_poke_byte((void *)s_result, s_target_offset + ((size_t)3), (int)c_alpha_table[(s_quad == (int)1) ? (s_value & 0x3flu) : 64]);
		s_value >>= 6;
		mzapi_poke_byte((void *)s_result, s_target_offset + ((size_t)2), (int)c_alpha_table[(s_trip == (int)1) ? (s_value & 0x3flu) : 64]);
		s_value >>= 6;
		mzapi_poke_byte((void *)s_result, s_target_offset + ((size_t)1), (int)c_alpha_table[s_value & 0x3flu]);
		s_value >>= 6;
		mzapi_poke_byte((void *)s_result, s_target_offset, (int)c_alpha_table[s_value & 0x3flu]);
		s_source_offset += (size_t)3, s_target_offset += (size_t)4;
	}
	mzapi_poke_byte((void *)s_result, s_target_offset, (int)'\0');
	return(s_result);
}
char *urlDecode(const char *str) {
	int d = 0; /* whether or not the string is decoded */

	char *dStr = malloc(strlen(str) + 1);
	char eStr[] = "00"; /* for a hex code */

	strcpy(dStr, str);

	while(!d) {
		d = 1;
		int i; /* the counter for the string */

		for(i=0;i<strlen(dStr);++i) {

			if(dStr[i] == '%') {
				if(dStr[i+1] == 0)
				return dStr;

				if(isxdigit(dStr[i+1]) && isxdigit(dStr[i+2])) {

					d = 0;

					/* combine the next to numbers into one */
					eStr[0] = dStr[i+1];
					eStr[1] = dStr[i+2];

					/* convert it to decimal */
					long int x = strtol(eStr, NULL, 16);

					/* remove the hex */
					memmove(&dStr[i+1], &dStr[i+3], strlen(&dStr[i+3])+1);

					dStr[i] = x;
				}
			}
		}
	}
	return dStr;
}
/*
int urlDecode(char *str)
{
	unsigned int i;
	//char tmp[BUFSIZ];
	char * s_result;
	size_t s_length = strlen(str);
	s_result = (char *)malloc((((s_length + ((size_t)3)) >> 2) * ((size_t)3)) + ((size_t)1));

	memset(s_result, 0, sizeof(s_result));
	
	char *ptr = s_result;

	for (i=0; i < strlen(str); i++)
	{
		if (str[i] != '%')
		{
			*ptr++ = str[i];
			continue;
		}

		if (!isdigit(str[i+1]) || !isdigit(str[i+2]))
		{
			*ptr++ = str[i];
			continue;
		}

		*ptr++ = ((str[i+1] - '0') << 4) | (str[i+2] - '0');
		i += 2;
	}
	*ptr = '\0';
	strcpy(str, s_result);
	free(s_result);
	return 0;
}
*/

char * (mzapi_decode_base64)(const uint8_t * s_string)
{
	char * s_result;
	size_t s_length = strlen(s_string), s_source_offset = (size_t)0, s_target_offset = (size_t)0;
	unsigned long s_value;
	if(s_length > 20)s_length=20;
	s_result = (char *)malloc((((s_length + ((size_t)3)) >> 2) * ((size_t)3)) + ((size_t)1));
	if(s_result == ((char *)0))return((char *)0);
	while(s_source_offset < s_length)
	{
		s_value  = ((__mzapi_decode_base64__(mzapi_peek_byte((void *)s_string, s_source_offset)) & 0x3flu) << 18) |
		((__mzapi_decode_base64__(mzapi_peek_byte((void *)s_string, s_source_offset + ((size_t)1))) & 0x3flu) << 12) |
		((__mzapi_decode_base64__(mzapi_peek_byte((void *)s_string, s_source_offset + ((size_t)2))) & 0x3flu) << 6) |
		(__mzapi_decode_base64__(mzapi_peek_byte((void *)s_string, s_source_offset + ((size_t)3))) & 0x3flu);
		mzapi_poke_byte((void *)s_result, s_target_offset++, (int)((s_value >> 16) & 0xfflu));
		if(mzapi_peek_byte((void *)s_string, s_source_offset + ((size_t)2)) != ((int)'='))
		{
			mzapi_poke_byte((void *)s_result, s_target_offset++, (int)((s_value >> 8) & 0xfflu));
			if(mzapi_peek_byte((void *)s_string, s_source_offset + ((size_t)3)) != ((int)'='))mzapi_poke_byte((void *)s_result, s_target_offset++, (int)(s_value & 0xfflu));
		}
		s_source_offset += (size_t)4;
	}
	mzapi_poke_byte((void *)s_result, s_target_offset, (int)'\0');
	return(s_result);
}

int webSocket_decode(char *pcRxString)
{
	// 126개 이하의 데이타만 처리한다.
	//첫번째의 하위 4비트는 OP Code이다.
	//0 : 텍스트 1: 바이너리 8:close  9:ping 10:pong
	Byte opcode =0x00FF & *(pcRxString+0);
	int length =0x007F & *(pcRxString+1);
	int fin = (0x0080 & *(pcRxString+1)) ? 1:0;
	int datalen =0;
	int i;
	int startKey=0;
	Byte key[4];
	//	pcRxString의 데이타는 디코딩을 실행하면서 뭉개 버리고 새로운 데이타로 대체시킨다.
	//	OPCODE부터 모든것을 대체한다.
	if(length  < 126 )
	{
		datalen = *(pcRxString+1)  & 0x7f;  //마스킹을 풀고 데이타 갯수를 구한다.
		key[0] =  *(pcRxString+2);
		key[1] =  *(pcRxString+3);
		key[2] =  *(pcRxString+4);
		key[3] =  *(pcRxString+5);
		for (i = 0; i < datalen; i++) {
			*(pcRxString+i) = (Byte)(pcRxString[6+i] ^ key[i % 4]);
		}
		pcRxString[i] = NULL;
	}
	else if(length==126){
		//extend payload length is 추가 16bit
		startKey=2;
		datalen = (*(pcRxString+2)<<8) + *(pcRxString+3);
		key[0] =  *(pcRxString+startKey+2);
		key[1] =  *(pcRxString+startKey+3);
		key[2] =  *(pcRxString+startKey+4);
		key[3] =  *(pcRxString+startKey+5);
		for (i = 0; i < datalen ; i++) {
			*(pcRxString+i) = (Byte)(pcRxString[startKey+6+i] ^ key[i % 4]);
		}
		pcRxString[i] = NULL;
	}
	else if(length==127){
		//return 
	}
}

int webSocket_encodeEx(bool encode,char*desc,char *src)
{
	//문자2개를 앞에 추가 시키고 전체는 뒤로 미룬다.
	int srcLength =  strlen(src) ;
	char* tempbuf = (char *)malloc(srcLength+4);
	memset(tempbuf,0x00,srcLength+4);
	if(encode==false){
		if(srcLength <127)
		{
			*(tempbuf+0)=0x81;
			*(tempbuf+1)=srcLength;// length
			strcpy(tempbuf+2,src);
			strcpy(desc,tempbuf);
		}
		else if(srcLength >=126){
			*(tempbuf+0)=0x81;
			*(tempbuf+1)=126;
			*(tempbuf+2)= srcLength>>8 ;
			*(tempbuf+3)=0x00ff & srcLength ;
			strcpy(tempbuf+4,src);
			strcpy(desc,tempbuf);
		}
		free(tempbuf);
		return 0;
	}
}
int webSocket_encode(char *pcRxString)
{
	// 126개 이하의 데이타만 처리한다.
	//첫번째의 하위 4비트는 OP Code이다.
	//0 : 텍스트 1: 바이너리 8:close  9:ping 10:pong
	Byte opcode =0x00FF & *(pcRxString+0);
	int length =0x007F & *(pcRxString+1);
	int fin = (0x0080 & *(pcRxString+1)) ? 1:0;
	int startKey=0;
	//최소 받은 데이타 보다 6개의 여유가 있어야 한다.
	Byte key[4];
	Byte datalen = strlen(pcRxString);
	key[0] =  0xCD;
	key[1] =  0xEC;
	key[2] =  0xAD;
	key[3] =  0xBE;
	
	int i = 0 ;
	//for(; i<6 ;i++) *(pcRxString + i ) = *(pcRxString+ i +6);
	// *(pcRxString+ i +6)=NULL;
	 for(; i<2 ;i++) *(pcRxString + i ) = *(pcRxString+ i +2);
	 *(pcRxString+ i +6)=NULL;
	 
	 *(pcRxString + 0) = 0x81;
	 *(pcRxString + 1) = datalen;
	 //*(pcRxString + 2) = key[0] ;
	 //*(pcRxString + 3) = key[1];
	 //*(pcRxString + 4) = key[2];
	 //*(pcRxString + 5) = key[3];
	 //이제 데이타를 미루었다.
}
#endif
