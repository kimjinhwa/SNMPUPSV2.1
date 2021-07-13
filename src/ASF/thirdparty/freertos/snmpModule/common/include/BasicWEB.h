/*****************************************************************************
 *
 * \file
 *
 * \brief Basic WEB Server for AVR32 UC3.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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
 *****************************************************************************/


#ifndef BASIC_WEB_SERVER_H
#define BASIC_WEB_SERVER_H

#include "portmacro.h"

#define webHTTP_OK	"HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n"

portTASK_FUNCTION_PROTO( vBasicWEBServer, pvParameters );

void write_set_time(portCHAR *parameter);
int webSocket_Function_S_MAILCONF(struct netconn *pxNetCon ,portCHAR *pcRxString);
int webSocket_Function_SET_MAILID(struct netconn *pxNetCon ,portCHAR *pcRxString);


const static char *webHTML_HEAD_START = "<!DOCTYPE HTML><html><head><meta charset='EUC-KR'/>\n ";
const static char *webHTML_HEAD_END = "<link href=""data:image/x-icon;base64,AAABAAEAEBAAAAAAAABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAJuWADKgmgAAm5UAFpqVAAablgAsnJYASpmUAAKblgAnm5UAKJuWAD6clgBKm5YAJZqUAASalQAJm5YAXZyXAACalQAXm5YAo1xmAACblgDrm5UAGpuWAHablgD5m5YA8ZuWAOCblgAAm5YAfpuWAIOblgDnm5UAPZuWAAeblgCam5UAAJuWAP+TkAACm5YA4ZuWADWblgDtm5YAiJuWAPGblgDgm5YAv5uWAPKblgB+mZUAEJqVAB+blgDgm5YAMQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAm5YAJZuWAP+clwD/nJcA/5uWAP6blgAiAAAAAAAAAACblgAbm5YA/pyXAP+clwD/m5YA/5uWAC4AAAAAm5UAGZyXAP+blgDVmpQAC5qVAA2blgDZnJcA/5uWAFGblgBCnJcA/5uWAOGalAAQmpUACpuWAMyclwD/m5YAJZuWAPublgDmmZUAAAAAAAAAAAAAAAAAAJuWAJKclwD/nJcA/5uWAKackwAAm5UANpqUABWZlQAAm5YA25uWAP6clwD/m5YAQZuXAACblwD5m5cA+ZqXAAaZlAAAm5YA/5uWAP+alQAAmpcABpuWAPublgD6nZgAAJuWACyclwD/nJcA/5uWAJ8AAAAAAAAAAAAAAAAAAAAAm5YAGpyXAP+clwD/mpYAIgAAAACblgBcm5UAJAAAAACblgCLnJcA/5uWAHmclwD/m5YALAAAAAAAAAAAm5YAL5yXAP+blgDXm5YAy5yXAP+blQA3AAAAAAAAAACblQAknJcA/5uWAIyZlQAAm5YAxpyXAP+clwD/nJcA/5yXAP+blgDHmpQAAJqXAACblgC7nJcA/5yXAP+clwD/nJcA/5uWANGakQAGAAAAAAAAAACYkgACm5YAVZuWAFGYlAABAAAAAAAAAAAAAAAAAAAAAJeUAACblgBOm5YAWJmSAAOZjwAAm5MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//8AAP//AAD//wAArGYAAKgdAAD//wAAw8MAAJmZAAA8PAAAZmYAAD58AAC8PAAAgYEAAP//AAD//wAA//8AAA=="" rel=""icon"" type=""image/x-icon"" /></head>\n ";
const static char *webHTML_setupEmail_script="\
<script>\
	function onBodyLoadEvent()\
	{\
		var text =  document.getElementById('mailServer');\
		text.value =mailServer;\
		text =  document.getElementById('userSendMail');\
		text.value =userSendMail;\
		text =  document.getElementById('userMailid');\
		text.value =userMailid;\
		text =  document.getElementById('userMailPasswd');\
		text.value =userMailPasswd;\
		text =  document.getElementById('checkSeverAuthentic');\
		text.checked =checkSeverAuthentic;\
		text =  document.getElementById('checkEventMail');\
		text.checked =checkEventMail;\
		text =  document.getElementById('UM1');\
		text.value =userMail1;\
		text =  document.getElementById('UM2');\
		text.value =userMail2;\
		text =  document.getElementById('UM3');\
		text.value =userMail3;\
		text =  document.getElementById('UM4');\
		text.value =userMail4;\
		text =  document.getElementById('UM5');\
		text.value =userMail5;\
	};\
</script>\
";

const static char *webHTML_passwd_script="\
<script>\
	function onBodyLoadEvent(){ \
	};\
function WebSocketToSnmp(sendParam,param) {              \
	var ws = new WebSocket('ws://'+agentIpAddress+':80/echo');\
	var paramm='?passwd_text='+document.getElementById('passwd_text').value+'&';\
	ws.onopen = function() { document.getElementById('status_bar').innerHTML = '데이타 요청중';                  \
		console.log(agentIpAddress + ' Connected OK!');\
		ws.send(sendParam+paramm);\
		console.log(' UPS Data Request....');\
	};\
	ws.onmessage = function (evt) \
	{   document.getElementById('status_bar').innerHTML = '데이타 수신 완료';              	  \
		if( evt.data == 'change_ok')               \
		{ alert('설정완료'); }\
		else { alert('다시 시도해 주세요'); }\
	};\
	ws.onclose = function() { document.getElementById('status_bar').innerHTML = '데이타 변경 완료';console.log('Connection is closed...');                };\
	ws.onerror= function() {  if(sendParam == 'changePasswd') alert('설정 혹은 데이타를 가져오는데 오류가 발생했습니다. 적용 혹은 UPS설정 버튼을 눌러 주세요'); \
		                      else document.getElementById('status_bar').innerHTML = '통신에러';\
							  console.log('Connection is error...');                };\
}\
</script>\
";
const static char *webHTML_setupups_script="\
<script>\
	function pad(d) {\
		return (d < 10) ? '0' + d.toString() : d.toString();\
	}\
	function onBodyLoadEvent(){ \
		if( window.location.href.indexOf('SETUP_UPS') > -1  ) 		(document.getElementById('menu')).style.height='950px';\
		var text = document.getElementById('network_Legend');\
		text = document.getElementById('ipaddress_input_text');\
		text.value = local_ipSet_station_name.ipaddress;\
		\
		text = document.getElementById('gateway_input_text');\
		text.value = local_ipSet_station_name.gateway;\
		\
		text = document.getElementById('subnetmask_input_text');\
		text.value = local_ipSet_station_name.subnetmask;\
		\
		text = document.getElementById('subnetmask_input_text');\
		text.value = local_ipSet_station_name.subnetmask;\
		\
		text = document.getElementById('macaddress_input_text');\
		var mac_address = (local_ipSet_station_name.macAddress).split('.');;\
		\
		var str =\
		text.value = parseInt(mac_address[0]).toString(16)+':'+\
		parseInt(mac_address[1]).toString(16)+':'+\
		parseInt(mac_address[2]).toString(16)+':'+\
		parseInt(mac_address[3]).toString(16)+':'+\
		parseInt(mac_address[4]).toString(16)+':'+\
		parseInt(mac_address[5]).toString(16) ;\
		\
		text = document.getElementById('trap_address_0');\
		text.value = trap_ipaddress.ip_address_0;\
		text = document.getElementById('trap_address_1');\
		text.value = trap_ipaddress.ip_address_1;\
		text = document.getElementById('trap_address_2');\
		text.value = trap_ipaddress.ip_address_2;\
		text = document.getElementById('trap_address_3');\
		text.value = trap_ipaddress.ip_address_3;\
		text = document.getElementById('trap_address_4');\
		text.value = trap_ipaddress.ip_address_4;\
		text = document.getElementById('trap_address_5');\
		text.value = trap_ipaddress.ip_address_5;\
		text = document.getElementById('trap_address_6');\
		text.value = trap_ipaddress.ip_address_6;\
		text = document.getElementById('trap_address_7');\
		text.value = trap_ipaddress.ip_address_7;\
		text = document.getElementById('trap_address_8');\
		text.value = trap_ipaddress.ip_address_8;\
		text = document.getElementById('trap_address_9');\
		text.value = trap_ipaddress.ip_address_9;\
		text = document.getElementById('time_server_0');\
		text.value = timeserver_ipaddress.ip_address_0;\
		text = document.getElementById('time_server_1');\
		text.value = timeserver_ipaddress.ip_address_1;\
		text = document.getElementById('set_systemtime');\
		var d = new Date();\
		text.value = (d.getFullYear()) ;\
		text.value +=  pad((d.getMonth()+1));\
		text.value +=  pad( d.getDate());\
		text.value +=  pad( d.getHours());\
		text.value +=  pad( d.getMinutes());\
		text.value +=  pad( d.getSeconds());\
		if(Eventlog != null) showLogTable();\
	};\
	function radioUpsProtocalkindSetup(value_text)\
	{\
		var rates = document.getElementById(value_text);\
		var rate_value;\
		for(var i = 0; i < rates.length; i++){\
			if(rates[i].checked){\
				rate_value = rates[i].value;\
				window.location.assign(window.location.pathname+'?'+value_text+'='+rate_value);\		
			};\
		};\
	};\
	function setNetwork_IpSetup(value_text)\
	{\
		if(value_text == 'ONOFF') window.location.assign(window.location.pathname+'?reboot=true');\
		else{ \
		var text = document.getElementById(value_text);\
		window.location.assign(window.location.pathname+'?'+value_text+'='+text.value);\
		};\
	};\
	function setSystemTime(value_text)\
	{\
		var date = new Date();  \
		window.location.assign(window.location.pathname+'?'+value_text+'='+  Date.parse(Date()) /1000);\
	}\
</script>\
	";

const static char *webCanvasScript ="\
<script>\
var ctx ;\
var canvas;\
var start_x = 15;\
var start_y = 5;\
var scale_x = 1.0 ;\
var scale_y = 1.0 ;\
var step =5;\
 var bMainPower_1=false;\
 var bMainPower_2=false;\
 var bMainPower_3=false;\
 var bMainPower_4=false;\
 var bMainPower_5=false;\
 var bBypassPower_1=false;\
 var bMaintanancePower_1=false;\
 function checkBIT(compare,n) {\
	 return  (compare & (0x00000001 << n)) > 0 ;\
 }\
 function isInput_NFB_StatusOn(){\
	 if(  checkBIT(upsModeBusData.Converter_State, 4)  )  return true;\
	 return false;\
 }\
 function isOutput_NFB_StatusOn(){\
	 if(  checkBIT(upsModeBusData.Converter_State, 3)  ) return true;\
	 return false;\
 }\
 function isPowerOn() {\
	 if( isInput_NFB_StatusOn() &&  upsModeBusData.Input_r_volt_rms > 50)  return true;\
	 return false;\
 }\
 function isBypass_NFB_StatusOn(){\
	 if(  checkBIT(upsModeBusData.Inverter_State, 3)  )  return true;\
	 return false;\
 }\
 function isConverterRun(){\
	 if(  checkBIT(upsModeBusData.Converter_State, 12)  )  return true;\
	 return false;\
 }\
 function isBattery_NFBOn(){\
	 if(  checkBIT(upsModeBusData.Converter_State, 05)  )  return true;\
	 return false;\
 }\
 function isBattery_PowerOn(){\
	 if(  isBattery_NFBOn() &&  upsModeBusData.Bat_volt_rms > 50  )  return true;\
	 return false;\
 }\
 function isInverterRun(){\
	 if(  checkBIT(upsModeBusData.Converter_State, 13)  )  return true;\
	 return false;\
 }\
 \
 function isMaintanenceMode(){\
	 if(  checkBIT(upsModeBusData.Inverter_Operation_Fault, 10)  ) return true;\
	 return false;\
 }\
 \
 function isInverterTransferMode(){\
	 if(  checkBIT(upsModeBusData.Converter_State, 14)  )  return true;\
	 return false;\
 }\
 function isBypassTransferMode(){\
	 if(  !checkBIT(upsModeBusData.Converter_State, 14)  )  return true;\
	 return false;\
 }\
function goHome(){ window.location.href=window.location.host;};\
function  drawUpsDiagram()\
 {	\
	 /*if(  upsModeBusData.Input_Phase == 1)  {imageDivHide();return ;};*/\
	 if( (upsModeBusData.Company_code_And_upstype ) == 50) {imageDivHide();return ;};\
	 if( (upsModeBusData.Company_code_And_upstype ) == 51) {imageDivHide();return ;};\
	 if( (upsModeBusData.Company_code_And_upstype ) == 52) {imageDivHide();return ;};\
	 bMainPower_1 = isPowerOn() ;\
	 bMainPower_2 = isPowerOn() && isInput_NFB_StatusOn();\
	 bMainPower_3 = isConverterRun() || isBattery_PowerOn();\
	 bMainPower_4 = isPowerOn() &&  isBypass_NFB_StatusOn() && isBypassTransferMode()  ;\
	 bMainPower_4 |=  isMaintanenceMode() && isOutput_NFB_StatusOn();\
	 bMainPower_4 |= isInverterTransferMode();\
	 bMainPower_5 =  bMainPower_4 &&  isOutput_NFB_StatusOn();\
	 bMainPower_5 |= isPowerOn() && isMaintanenceMode() ;\
	 bMainPower_5 |=  isPowerOn() && isBypass_NFB_StatusOn()  && isBypassTransferMode() && isOutput_NFB_StatusOn();\
	 bBypassPower_1 = isPowerOn() && isBypass_NFB_StatusOn() ;\
	 bMaintanancePower_1=  isPowerOn() && isMaintanenceMode() ;\
	 drawInit();\
	 if( bMainPower_1 ) mainPowerLine('red');\
	 else 		mainPowerLine('blue');\
	 if( isInput_NFB_StatusOn() ) sw_1('ON');\
	 else	sw_1('OFF');\
	 if( bMainPower_2 )sw_1_Line_1('red');\
	 else sw_1_Line_1('blue');\
	 if(  isConverterRun() ) converterONOFF('ON');\
	 else  converterONOFF('OFF');\
	 if(bMainPower_3) internalLine('red');\
	 else  internalLine('blue');\
	 if( isInverterRun() ) { inverterONOFF('ON');}\
	 else {inverterONOFF('OFF');}\
	 if( isInverterTransferMode() ){\
		 scr_1_ONOFF('ON');\
		 scr_2_ONOFF('OFF');\
	 }\
	 else{\
		 scr_1_ONOFF('OFF');\
		 scr_2_ONOFF('ON');\
	 }\
	 if(bMainPower_4)scr_1_line('red');\
	 else scr_1_line('blue');\
	 if(isBypass_NFB_StatusOn() )sw_2('ON');\
	 else sw_2('OFF');\
	 if(isMaintanenceMode())sw_3('ON');\
	 else sw_3('OFF');\
	 if(isOutput_NFB_StatusOn())sw_4('ON');\
	 else sw_4('OFF');\
	 if( isBattery_NFBOn() )  draw_batteryGroup('ON');\
	 else 	draw_batteryGroup('OFF');\
	 if(bBypassPower_1)BypassLine('red');\
	 else BypassLine('blue');\
	 if(bMainPower_5 )EmergencyLine('red');\
	 else EmergencyLine('blue');\
 }\
 function drawInit(){\
	canvas = document.getElementById('ups_canvas');\
	ctx = canvas.getContext('2d');\
	ctx.clearRect(0, 0, canvas.width, canvas.height);\
}\
function mainPowerLine(fill_color) {\
	var x=scale_x*25,y=scale_y*125;\
	(start_x + step*0)*scale_x;\
	(start_y - step*0)*scale_y;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y);\
	x =x+scale_x*50 ; y = y;\
	ctx.lineTo(x,y); 	\
	x =x ; y = y - scale_y*90;\
	ctx.lineTo(x,y); 	\
	x =x +scale_x*50; y = y;  \
	ctx.lineTo(x,y);\
	x =x ; y = y +scale_y*15;  \
	ctx.lineTo(x,y);\
	x =x -scale_x*35 ; y =  y;  \
	ctx.lineTo(x,y); \
	x =x ; y =  y+scale_y*35;  \
	ctx.lineTo(x,y); \
	x =x+scale_x*35 ; y =  y;  \
	ctx.lineTo(x,y); \
	x =x ; y =  y+scale_y*15;  \
	ctx.lineTo(x,y); \
	x =x -scale_x*35 ; y =  y;  \
	ctx.lineTo(x,y); \
	x =x ; y =  y+scale_y*25; \
	ctx.lineTo(x,y); \
	x =x +scale_x*35; y = y; \
	ctx.lineTo(x,y); \
	x =x ; y =  y+scale_y*15; \
	ctx.lineTo(x,y); \
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
}\
function clear() {	ctx.clearRect(0, 0, canvas.width, canvas.height);}\
function sw1_design(xx,yy,fill_color)\
{\
	var x=scale_x*xx,y=scale_y*yy;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*10;\
	ctx.lineTo(x,y); \
	x =x+scale_x*45 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*10; \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*13 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*15; \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*16 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y);\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
}\
function sw_bat(xx,yy,fill_color)\
{\
	var x=scale_x*xx,y=scale_y*yy;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x+scale_x*15 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y -scale_y*15;  \
	ctx.lineTo(x,y); 	\
	x =x+scale_x*3 ; y = y;\
	ctx.lineTo(x,y); \
	x =x ; y = y +scale_y*45;  \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*3 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y-scale_y*15; \
	ctx.lineTo(x,y); \
	x =x-scale_x*15 ; y = y; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	}\
function BypassLine(fill_color)\
{\
	var x=scale_x*145,y=scale_y*85;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*375; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*15; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
}\
function sw_1_Line_1(fill_color)\
{\
	var x=scale_x*145,y=scale_y*125;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*115 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*15; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	}\
function bat_line(xx,yy,fill_color)\
{\
	var x=scale_x*xx,y=scale_y*yy;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x+scale_x*15 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*20;  \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*15 ; y = y; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	}\
function battery(xx,yy,fill_color)\
{\
	var x=scale_x*xx,y=scale_y*yy;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x+scale_x*105 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*25;  \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*105 ; y = y; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	x=scale_x*(xx+10),y=scale_y*(yy +19);\
	ctx.fillStyle = 'black';\
 	ctx.font = '14pt sans-serif';\
 	ctx.fillText('BATTERY',x,y);\
	}\
function sw_1(onoff)\
{\
	if(onoff=='ON'){\
	sw1_design(111,110,'red');\
	sw_1_Line_1('red');\
	}\
	else \
	{\
	sw1_design(111,110-13,'green');\
	sw_1_Line_1('green');\
	}\
}\
function sw_2(onoff)\
{\
	if(onoff=='ON'){\
	sw1_design(111,70,'red');\
	}\
	else \
	{\
	sw1_design(111,70-13,'green');\
	}\
}\
function sw_3(onoff)\
{\
	if(onoff=='ON'){\
	sw1_design(111,18,'red');\
	}\
	else \
	{\
	sw1_design(111,20-13,'green');\
	}\
}\
function sw_4(onoff)\
{\
	if(onoff=='ON'){\
	sw1_design(616,110,'red');\
	}\
	else \
	{\
	sw1_design(616,110-13,'green');\
	}\
}\
function draw_batteryGroup(onoff)\
{\
	if(onoff=='ON'){\
	sw_bat(343,145,'red');\
	bat_line(343,160,'red')\
	}\
	else\
	{\
	sw_bat(343+18,145,'green');\
	bat_line(343,160,'green')\
	}\
	battery(300,180,'cyan')\
}\
function draw_10(fill_color)\
{\
	var x=scale_x*440,y=scale_y*125;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*80 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*15; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
}\
function EmergencyLine(fill_color)\
{\
	var x=scale_x*650,y=scale_y*(125);\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*50 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y -scale_y*75; \
	ctx.lineTo(x,y); \
	x =x-scale_x*555  ; y = y ; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*570  ; y = y ;  \
	ctx.lineTo(x,y); 	\
	x =x ; y = y+scale_y*90;\
	ctx.lineTo(x,y); \
	x =x+scale_x*30  ; y = y ;  \
	ctx.lineTo(x,y); 	\
	x =x ; y = y+scale_y*15;\
	ctx.lineTo(x,y); \
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
}\
function internalLine(fill_color)\
{\
	var x=scale_x*310,y=scale_y*125;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*80 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*15; \
	ctx.lineTo(x,y); 	\
	x =x+scale_x*(-32.5) ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*20; \
	ctx.lineTo(x,y); 	\
	x =x+scale_x*(-15) ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*(-20); \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
}\
function converterBox(fill_color)\
{\
	var x=scale_x*260,y=scale_y*(125 + 7.5);\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*30;\
	ctx.lineTo(x,y); \
	x =x+scale_x*50 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*30; \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*50  ; y = y ; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	x=scale_x*260,y=scale_y*(125 + 7.5);\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x+scale_x*50  ; y =  y - scale_y*30 ; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	x=scale_x*260,y=scale_y*(125 );\
	ctx.fillStyle = 'black';\
 	ctx.font = '24pt sans-serif';\
 	ctx.fillText('~',x,y);\
 	\
	x=scale_x*285,y=scale_y*(120 );\
	ctx.fillStyle = 'black';\
 	ctx.font = '24pt sans-serif';\
 	ctx.fillText('_',x,y);\
}\
function converterONOFF(onoff)\
{\
	if(onoff=='ON'){\
	converterBox('red');\
	}\
	else \
	{\
	converterBox('green');\
	}\
}\
function inverterBox(fill_color)\
{\
	var x=scale_x*390,y=scale_y*(125 + 7.5);\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*30;\
	ctx.lineTo(x,y); \
	x =x+scale_x*50 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*30; \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*50  ; y = y ; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	x=scale_x*(390),y=scale_y*(125 + 7.5 );\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x+scale_x*50  ; y =  y - scale_y*30 ; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	x=scale_x*390,y=scale_y*(125 );\
	ctx.fillStyle = 'black';\
 	ctx.font = '24pt sans-serif';\
 	ctx.fillText('~',x,y);\
 	\
	x=scale_x*418,y=scale_y*(120 );\
	ctx.fillStyle = 'black';\
 	ctx.font = '24pt sans-serif';\
 	ctx.fillText('_',x,y);\
}\
function inverterONOFF(onoff)\
{\
	if(onoff=='ON'){\
	inverterBox('red');\
	draw_10('red');\
	}\
	else \
	{\
	inverterBox('green');\
	draw_10('green');\
	}\
}\
function scr_1_designF(xx,yy,fill_color)\
{\
	var x=scale_x*xx,  y=scale_y*(yy);\
	var mov_x=16, mov_y=12;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	ctx.lineTo(x =x+scale_x*mov_x,y = y);\
	ctx.lineTo(x ,y = y-scale_y*mov_y);\
	ctx.lineTo(x =x+scale_x*mov_x ,y = y+scale_y*mov_y);\
	ctx.lineTo(x =x+scale_x*mov_x ,y );\
	ctx.moveTo(x =x-scale_x*mov_x ,y );\
	ctx.lineTo(x =x+scale_x*mov_x/1.5 ,y=y-scale_y*mov_y/1.5 );\
	ctx.moveTo(x =x-scale_x*mov_x/1.5 ,y=y+scale_y*mov_y/1.5 );\
	ctx.moveTo(x =x ,y = y-scale_y*mov_y );\
	ctx.lineTo(x =x ,y = y+scale_y*mov_y*2 );\
	ctx.moveTo(x =x ,y = y-scale_y*mov_y );\
	ctx.lineTo(x =x-scale_x*mov_x ,y = y+scale_y*mov_y);\
	ctx.lineTo(x =x ,y = y-scale_y*mov_y);\
	y=y+1;\
	ctx.moveTo(x =x-scale_x*mov_x, y= y+scale_y*mov_y*2);\
	ctx.lineTo(x =x+scale_x*mov_x,y = y);\
	ctx.moveTo(x, y=y-scale_y*mov_y);\
	ctx.lineTo(x ,y=y+scale_y*mov_y*2);\
	ctx.moveTo(x, y=y-scale_y*mov_y);\
	ctx.lineTo(x=x+scale_x*mov_x ,y=y-scale_y*mov_y);\
	ctx.lineTo(x            ,y=y+scale_y*mov_y*2);\
	ctx.lineTo(x=x-scale_x*mov_x ,y=y-scale_y*mov_y);\
	ctx.moveTo(x=x+scale_x*mov_x ,y=y );\
	ctx.lineTo(x=x+scale_x*mov_x ,y=y );\
	ctx.moveTo(x=x-scale_x*mov_x*2 ,y=y );\
	ctx.lineTo(x=x-scale_x*mov_x/1.5 ,y=y+scale_y*mov_y/1.5);\
	ctx.stroke();\
}\
function scr_1_designR(xx,yy,fill_color)\
{\
}\
function scr_1(fill_color)\
{\
	var x=scale_x*520,y=scale_y*(125 +25);\
	var mov_x=49, mov_y=52;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*mov_y;\
	ctx.lineTo(x,y); \
	x =x+scale_x*mov_x ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*mov_y; \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*mov_x  ; y = y ; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	scr_1_designF(521,112,'black');\
}\
function scr_2(fill_color)\
{\
	var x=scale_x*520,y=scale_y*(125 -30 );\
	var mov_x=49, mov_y=49;\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*mov_y;\
	ctx.lineTo(x,y); \
	x =x+scale_x*mov_x ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y +scale_y*mov_y; \
	ctx.lineTo(x,y); 	\
	x =x-scale_x*mov_x  ; y = y ; \
	ctx.lineTo(x,y); 	\
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
	scr_1_designF(520,115-57,'black');\
}\
function scr_1_line(fill_color)\
{\
	var x=scale_x*570,y=scale_y*(125);\
	ctx.strokeStyle = 'black';\
	ctx.lineWidth = '1';\
	ctx.fillStyle = fill_color;\
	ctx.beginPath();\
	ctx.moveTo(x, y);\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*20 ; y = y; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y -scale_y*25; \
	ctx.lineTo(x,y); \
	x =x-scale_x*20  ; y = y ; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y-scale_y*15;\
	ctx.lineTo(x,y); \
	x =x+scale_x*40  ; y = y ; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y+scale_y*40;\
	ctx.lineTo(x,y); \
	x =x+scale_x*20  ; y = y ; \
	ctx.lineTo(x,y); 	\
	x =x ; y = y+scale_y*15;\
	ctx.lineTo(x,y); \
	ctx.closePath();\
	ctx.stroke();\
	ctx.fill();\
}\
function scr_1_ONOFF(onoff)\
{\
	if(onoff=='ON'){\
	scr_1('red');\
	}\
	else \
	{\
	scr_1('green');\
	}\
}\
function scr_2_ONOFF(onoff)\
{\
	if(onoff=='ON'){\
	scr_2('red');\
	}\
	else \
	{\
	scr_2('green');\
	}\
}\
</script>\
";
const static char *webHTML_websocket_script="\
<script>\
var receive_count=0;\
var receive_data='';\
var sdate = new Date();\
function menuSelect(menuname,pa,pv){\
    if( document.URL.includes('SETUP_UPS.html',0) ) window.location.assign(window.location.origin+'/SETUP_UPSBASIC.html');\
	if(document.getElementById('content_UpsSetup') == null ) window.location.assign(window.location.origin+'/SETUP_UPSBASIC.html');\
   	document.getElementById('content_UpsSetup').style.display='none';\
   	document.getElementById('content_EmailSetup').style.display='none';\
   	document.getElementById(menuname).style.display='block';\
	   if( menuname == 'content_UpsSetup') WebSocketToSnmp('SET_BASIC','');\
};\
function getParam(sname) {\
	var params = location.search.substr(location.search.indexOf('?') + 1);\
	var sval = '';\
	params = params.split('&');\
	for (var i = 0; i < params.length; i++) {\
		temp = params[i].split('=');\
		if ([temp[0]] == sname) { sval = temp[1]; }\
	}\
	return sval;\
}\
function setDataFromSnmp()\
{\
	document.getElementById('SB8').value = document.getElementById('SB8').value.replace(':','/');\
	var sendCommand='SET_BASIC';\
	var param='?'+document.getElementById('SB0').id+'='+document.getElementById('SB0').value+'&';\
	param += document.getElementById('SB1').id+'='+document.getElementById('SB1').value+'&';\
	param += document.getElementById('SB2').id+'='+document.getElementById('SB2').value+'&';\
	param += document.getElementById('SB3').id+'='+document.getElementById('SB3').value+'&';\
	param += document.getElementById('SB4').id+'='+document.getElementById('SB4').value+'&';\
	param += document.getElementById('SB5').id+'='+document.getElementById('SB5').value+'&';\
	param += document.getElementById('SB6').id+'='+document.getElementById('SB6').value+'&';\
	param += document.getElementById('SB7').id+'='+document.getElementById('SB7').value+'&';\
	param += document.getElementById('SB8').id+'='+document.getElementById('SB8').value+'&';\
	param += document.getElementById('SB9').id+'='+document.getElementById('SB9').value+'&';\
	param += document.getElementById('SBA').id+'='+document.getElementById('SBA').value+'&';\
	param += document.getElementById('SBB').id+'='+document.getElementById('SBB').value+'&';\
	param += document.getElementById('SBC').id+'='+document.getElementById('SBC').value+'&';\
	param += document.getElementById('SBD').id+'='+document.getElementById('SBD').value+'&';\
	WebSocketToSnmp(sendCommand,param);\
	document.getElementById('status_bar').innerHTML = '단말기를 재 부팅합니다.';   \
}\
\
function WebSocketToSnmp(sendParam,param) {              \
	var ws = new WebSocket('ws://'+agentIpAddress+':80/echo');\
	ws.onopen = function() { document.getElementById('status_bar').innerHTML = '데이타 요청중';                  \
		console.log(agentIpAddress + ' Connected OK!');\
		ws.send(sendParam+param);\
		console.log(' UPS Data Request....');\
	};\
	ws.onmessage = function (evt) \
	{   document.getElementById('status_bar').innerHTML = '데이타 수신 완료';              	  \
		receive_count++;\
		if(sendParam == 'UPS_EX_DATA')               \
		{\
				console.log(evt.data);\
				receive_data = evt.data.split('?');\
				parse_ups_data(receive_data[1]);\
				WebSocketPaseDataSetToWeb();\
				receive_count=0;\
				receive_data='';\
				drawUpsDiagram();\
				ws.close();\
		}\
		else    if(sendParam == 'SET_BASIC')               \
		{	                  \
			parse_ups_set_basic(evt.data);\
		}\
	};\
	ws.onclose = function() { console.log('Connection is closed...');                };\
	ws.onerror= function() {  if(sendParam == 'SET_BASIC') alert('설정 혹은 데이타를 가져오는데 오류가 발생했습니다. 적용 혹은 UPS설정 버튼을 눌러 주세요'); \
		                      else document.getElementById('status_bar').innerHTML = '통신에러';\
							  console.log('Connection is error...');                };\
}\
function parse_ups_set_basic(evtdata){\
	console.log(evtdata);\
	var rev_str = evtdata.split('?');\
	console.log(rev_str[0]);\
	if( rev_str[0] != null ) if( rev_str[0] == 'SET_BASIC_R' ) 	alert('설정완료'); \
	console.log(rev_str[1]);\
	rev_str = rev_str[1].split('&');\
	console.log(rev_str.length);\
	var i = 0;\
	for(var i =  0 ; i< rev_str.length-1; i++)\
	{\
		var idvalue = rev_str[i].split(':');\
		if(idvalue[0] != null && idvalue[1] != null ) { \
		if(document.getElementById(idvalue[0]) != null ) document.getElementById(idvalue[0]).value = idvalue[1];\
		};\
	};\
	document.getElementById('SB8').value = document.getElementById('SB8').value.replace('/',':');\
}\
function parse_ups_data(evtdata)\
{	\
	if(evtdata != null) console.log('parse....'+evtdata);\
	else console.log('Null Data Received...');\
	var rev_str = evtdata.split(':');\
	if(rev_str == null) console.log('receive data is null.....'+evtdata);\
	if(rev_str == null) return;\
	var i = 0;\
	upsModeBusData.Year_made = rev_str[i++] ;\
	upsModeBusData.Month_made = rev_str[i++] ;\
	upsModeBusData.Date_made = rev_str[i++] ;\
	upsModeBusData.Ups_Capacitor = rev_str[i++] ;\
	upsModeBusData.Input_Phase = rev_str[i++] ;\
	upsModeBusData.Input_Voltage = rev_str[i++] ;\
	upsModeBusData.Output_Phase = rev_str[i++] ;\
	upsModeBusData.Output_Voltage = rev_str[i++] ;\
	upsModeBusData.Company_code_And_upstype = rev_str[i++] ;\
	upsModeBusData.Installed_Battery_Cells = rev_str[i++] ;\
	\
	upsModeBusData.reserved_2 = rev_str[i++] ;\
	upsModeBusData.BMS_1_2_STATE = rev_str[i++] ;\
	upsModeBusData.Converter_State = rev_str[i++] ;\
	upsModeBusData.Inverter_State = rev_str[i++] ;\
	upsModeBusData.Converter_Operation_Fault = rev_str[i++] ;\
	upsModeBusData.Inverter_Operation_Fault = rev_str[i++] ;\
	upsModeBusData.Input_r_volt_rms = rev_str[i++] ;\
	upsModeBusData.Input_s_volt_rms = rev_str[i++] ;\
	upsModeBusData.Input_t_volt_rms = rev_str[i++] ;\
	upsModeBusData.Input_r_current_rms = rev_str[i++] ;\
	\
	upsModeBusData.Input_s_current_rms = rev_str[i++] ;\
	upsModeBusData.Input_t_current_rms = rev_str[i++] ;\
	upsModeBusData.Input_frequency = rev_str[i++] ;\
	upsModeBusData.Bypass_r_volt_rms = rev_str[i++] ;\
	upsModeBusData.Bypass_s_volt_rms = rev_str[i++] ;\
	upsModeBusData.Bypass_t_volt_rms = rev_str[i++] ;\
	upsModeBusData.Bypass_r_current_rms = rev_str[i++] ;\
	upsModeBusData.Bypass_s_current_rms = rev_str[i++] ;\
	upsModeBusData.Bypass_t_current_rms = rev_str[i++] ;\
	upsModeBusData.Bypass_Frequency = rev_str[i++] ;\
	\
	upsModeBusData.Inverter_u_volt_rms = rev_str[i++] ;\
	upsModeBusData.Inverter_v_volt_rms = rev_str[i++] ;\
	upsModeBusData.Inverter_w_volt_rms = rev_str[i++] ;\
	upsModeBusData.Inverter_u_curr_rms = rev_str[i++] ;\
	upsModeBusData.Inverter_V_curr_rms = rev_str[i++] ;\
	upsModeBusData.Inverter_W_curr_rms = rev_str[i++] ;\
	upsModeBusData.Inverter_Frequency = rev_str[i++] ;\
	upsModeBusData.Bat_volt_rms = rev_str[i++] ;\
	upsModeBusData.Bat_current_rms = rev_str[i++] ;\
	upsModeBusData.Input_kva_address_KVA = rev_str[i++] ;\
	\
	upsModeBusData.Input_kw_KW = rev_str[i++] ;\
	upsModeBusData.Input_kvar_KVAR = rev_str[i++] ;\
	upsModeBusData.Input_power_factor_Pf = rev_str[i++] ;\
	upsModeBusData.Output_r_volt_rms = rev_str[i++] ;\
	upsModeBusData.Output_s_volt_rms = rev_str[i++] ;\
	upsModeBusData.Output_t_volt_rms = rev_str[i++] ;\
	upsModeBusData.Output_u_current_rms = rev_str[i++] ;\
	upsModeBusData.Output_v_current_rms = rev_str[i++] ;\
	upsModeBusData.Output_w_current_rms = rev_str[i++] ;\
	upsModeBusData.Output_frequency = rev_str[i++] ;\
	\
	upsModeBusData.Output_kva_KVA = rev_str[i++] ;\
	upsModeBusData.Output_kw_KW = rev_str[i++] ;\
	upsModeBusData.Output_kvar_KVAR = rev_str[i++] ;\
	upsModeBusData.Output_Power_factor_Pf = rev_str[i++] ;\
	upsModeBusData.Output_R_Load = rev_str[i++] ;\
	upsModeBusData.Output_S_Load = rev_str[i++] ;\
	upsModeBusData.Output_T_Load = rev_str[i++] ;\
	upsModeBusData.BMS_Bat_Voltage = rev_str[i++] ;\
	upsModeBusData.Battery_Room_Temper = rev_str[i++] ;\
	\
	systemTime = (upsModeBusData.BMS_1_2_STATE * 0x10000)*1000  +  upsModeBusData.reserved_2 *1000  - 2208965020*1000 ;\
}\
</script>\
";

const static char *webHTML_default_script="\
<script>\
function showLogTable()\
{\
	var table1 = document.getElementById('log_table');	\
	var tr_ = document.createElement('tr');\
	var td1_,td2_,td3_;\
	if( table1.rows.length > 1) return;\
	tr_.setAttribute('bgColor', '#FFFFCC'); \
	tr_.setAttribute('height', '30'); \
	td1_ = document.createElement('td');\
        td1_.setAttribute('width', '30%');\
        td1_.innerText = '시간';\
		td2_ = document.createElement('td');\
        td2_.setAttribute('width', '10%');\
        td2_.innerText = '종류';\
		td3_ = document.createElement('td');\
        td3_.setAttribute('width', '60%'); \
        td3_.innerText = '내용';\
        tr_.appendChild(td1_); tr_.appendChild(td2_); tr_.appendChild(td3_);\
        table1.appendChild(tr_);\
	for(var i=0;i<Eventlog.length;i++)\
	{\
		var tr = document.createElement('tr');\
		var td1,td2,td3;\
		tr.setAttribute('bgColor', '#FFFFCC'); \
		tr.setAttribute('height', '30'); \
		td1 = document.createElement('td');\
	        td1.setAttribute('width', '30%'); \
	        td1.innerText = ((new Date(Eventlog[i].time*1000)).getFullYear()-70)+'-'+((new Date(Eventlog[i].time*1000)).getMonth()+1)+'-'+((new Date(Eventlog[i].time*1000)).getDate())+' '+((new Date(Eventlog[i].time*1000)).getHours()+ 0)+':'+((new Date(Eventlog[i].time*1000)).getMinutes()+ 0)+':'+((new Date(Eventlog[i].time*1000)).getSeconds()+ 0);\
		td2 = document.createElement('td');\
	        td2.setAttribute('width', '10%'); \
	        td2.innerText = Eventlog[i].kind;\
		td3 = document.createElement('td');\
	        td3.setAttribute('width', '60%'); \
	        td3.innerText = '0x'+(Eventlog[i].event).toString(16);\
	        tr.appendChild(td1); tr.appendChild(td2); tr.appendChild(td3);\
	        table1.appendChild(tr);\
	}	\
}\
function WebSocketPaseDataSetToWeb()\
{\
	sdate.setTime(systemTime);\
	snmp_time_value_text = sdate.getFullYear() + '/' + (sdate.getMonth()+1) + '/' +sdate.getDate() + ' ' + sdate.getHours() +':' + sdate.getMinutes()+':'+sdate.getSeconds();\
	var text ;\
	text = document.getElementById('Year_made_value');\
	text.innerHTML = upsModeBusData.Year_made +'/'+ upsModeBusData.Month_made +'/' 	+ upsModeBusData.Date_made  ;\
	(document.getElementById('Ups_Capacitor_value')).innerHTML = (upsModeBusData.Ups_Capacitor)+ 'KVA';\
	(document.getElementById('Input_Phase_value')).innerHTML = upsModeBusData.Input_Phase+ '상';\
	(document.getElementById('Input_Voltage_value')).innerHTML = upsModeBusData.Input_Voltage+ 'V';\
	(document.getElementById('Output_Voltage_value')).innerHTML = upsModeBusData.Output_Voltage+ 'V';\
	(document.getElementById('Output_Phase_value')).innerHTML = upsModeBusData.Output_Phase+ '상';\
	(document.getElementById('default_Installed_BAT_value')).innerHTML = upsModeBusData.Installed_Battery_Cells+ '셀';\
	if( upsModeBusData.Input_Phase == 1 ) (document.getElementById('Input_rst_volt_rms_value')).innerHTML = upsModeBusData.Input_r_volt_rms + 'V';\
	else (document.getElementById('Input_rst_volt_rms_value')).innerHTML = upsModeBusData.Input_r_volt_rms+ '/'+ upsModeBusData.Input_s_volt_rms+ '/'+ upsModeBusData.Input_t_volt_rms+ 'V';\
	if(upsModeBusData.Input_r_current_rms>0){\
	if( upsModeBusData.Input_Phase == 1 ) (document.getElementById('Input_r_current_rms_value')).innerHTML = upsModeBusData.Input_r_current_rms+'A';\
	else (document.getElementById('Input_r_current_rms_value')).innerHTML = upsModeBusData.Input_r_current_rms+'/' +  upsModeBusData.Input_s_current_rms+ '/' + upsModeBusData.Input_t_current_rms+'A';\
	}\
	else{\
	 (document.getElementById('Input_r_current_rms_value')).innerHTML = '---A';\
	}\
	(document.getElementById('Input_frequency_value')).innerHTML = upsModeBusData.Input_frequency/10+ 'Hz';\
	(document.getElementById('snmp_time_value')).innerHTML = snmp_time_value_text;\
	if( upsModeBusData.Output_Phase == 1 ) (document.getElementById('Bypass_rst_volt_rms_value')).innerHTML = upsModeBusData.Bypass_r_volt_rms+  'V';\
	else (document.getElementById('Bypass_rst_volt_rms_value')).innerHTML = upsModeBusData.Bypass_r_volt_rms+ '/' + upsModeBusData.Bypass_s_volt_rms+ '/'+ upsModeBusData.Bypass_t_volt_rms+ 'V';\
	if( upsModeBusData.Output_Phase == 1 )(document.getElementById('Bypass_rst_current_rms_value')).innerHTML = upsModeBusData.Bypass_r_current_rms+'A';\
	else (document.getElementById('Bypass_rst_current_rms_value')).innerHTML = upsModeBusData.Bypass_r_current_rms+'/' +  upsModeBusData.Bypass_s_current_rms+ '/' + upsModeBusData.Bypass_t_current_rms+'A';\
	(document.getElementById('Bypass_Frequency_value')).innerHTML = upsModeBusData.Bypass_Frequency/10+ 'Hz';\
	if( upsModeBusData.Output_Phase == 1 ) (document.getElementById('Inverter_uvw_volt_rms_value')).innerHTML = upsModeBusData.Inverter_u_volt_rms+'V';\
	else (document.getElementById('Inverter_uvw_volt_rms_value')).innerHTML = upsModeBusData.Inverter_u_volt_rms+ '/'+ upsModeBusData.Inverter_v_volt_rms+ '/'+ upsModeBusData.Inverter_w_volt_rms+ 'V';\
	if( upsModeBusData.Output_Phase == 1 ) (document.getElementById('Inverter_uvw_curr_rms_value')).innerHTML = upsModeBusData.Inverter_u_curr_rms+'A';\
	else (document.getElementById('Inverter_uvw_curr_rms_value')).innerHTML = upsModeBusData.Inverter_u_curr_rms+'/' +  upsModeBusData.Inverter_V_curr_rms+ '/' + upsModeBusData.Inverter_W_curr_rms+'A';\
	(document.getElementById('Inverter_Frequency_value')).innerHTML = upsModeBusData.Inverter_Frequency/10+ 'Hz';\
	if( upsModeBusData.Output_Phase == 1 ) (document.getElementById('Output_rms_volt_rms_value')).innerHTML = upsModeBusData.Output_r_volt_rms+'V';\
	else (document.getElementById('Output_rms_volt_rms_value')).innerHTML = upsModeBusData.Output_r_volt_rms+ '/'+ upsModeBusData.Output_s_volt_rms+ '/'+ upsModeBusData.Output_t_volt_rms+ 'V';\
	if( upsModeBusData.Output_Phase == 1 )(document.getElementById('Output_uvw_current_rms_value')).innerHTML = upsModeBusData.Output_u_current_rms+'A';\
	else (document.getElementById('Output_uvw_current_rms_value')).innerHTML = upsModeBusData.Output_u_current_rms+'/' +  upsModeBusData.Output_v_current_rms+ '/' + upsModeBusData.Output_w_current_rms+'A';\
	(document.getElementById('Output_frequency_value')).innerHTML = upsModeBusData.Output_frequency/10+ 'Hz';\
	if( upsModeBusData.Output_Phase == 1 ) (document.getElementById('Output_RST_Load_value')).innerHTML = upsModeBusData.Output_R_Load+'%';\
	else (document.getElementById('Output_RST_Load_value')).innerHTML = upsModeBusData.Output_R_Load+'/' +  upsModeBusData.Output_S_Load+ '/' + upsModeBusData.Output_T_Load+'%';\
	(document.getElementById('Bat_volt_rms_value')).innerHTML =  upsModeBusData.Bat_volt_rms+ 'V';\
	(document.getElementById('Bat_current_rms_value')).innerHTML =upsModeBusData.Bat_current_rms + 'A';\
	\	
	if( upsModeBusData.Input_Phase == 1 ) (document.getElementById('Bat_charge_value')).innerHTML = upsModeBusData.BMS_Bat_Voltage+ '%';\
	else (document.getElementById('Bat_charge_value')).innerHTML = '--%';\
	\
	if( (upsModeBusData.Company_code_And_upstype & 0x00ff)  == 0x01) (document.getElementById('Company_code_value')).innerHTML = 'IFTECH';\
	else if( (upsModeBusData.Company_code_And_upstype & 0x00ff) == 0x02) (document.getElementById('Company_code_value')).innerHTML = '한강기전(주)';\
	else if( (upsModeBusData.Company_code_And_upstype & 0x00ff) == 0x03) (document.getElementById('Company_code_value')).innerHTML = '성신전기공업(주)';\
	else if( (upsModeBusData.Company_code_And_upstype & 0x00ff) == 0x04) (document.getElementById('Company_code_value')).innerHTML = '대농산업전기';\
	else if( (upsModeBusData.Company_code_And_upstype & 0x00ff) == 0x05) (document.getElementById('Company_code_value')).innerHTML = '(주)지오닉스';\
	else if( (upsModeBusData.Company_code_And_upstype & 0x00ff) == 0x06) (document.getElementById('Company_code_value')).innerHTML = '대한전력전자(주)';\
	else if( (upsModeBusData.Company_code_And_upstype & 0x00ff) == 0x07) (document.getElementById('Company_code_value')).innerHTML = '(주)영신엔지니어링';\
	else if( (upsModeBusData.Company_code_And_upstype ) == 50) (document.getElementById('Company_code_value')).innerHTML = UpsSystemName;\
	else if( (upsModeBusData.Company_code_And_upstype ) == 51) (document.getElementById('Company_code_value')).innerHTML = UpsSystemName;\
	else if( (upsModeBusData.Company_code_And_upstype ) == 52) (document.getElementById('Company_code_value')).innerHTML = UpsSystemName;\
	\
	if( (upsModeBusData.Company_code_And_upstype >> 8)  == 0x0A)	  (document.getElementById('upstype_value')).innerHTML = 'NEP 3상3상';\
	else if( (upsModeBusData.Company_code_And_upstype >> 8)  == 0x0B) (document.getElementById('upstype_value')).innerHTML = 'NEP 3상1상';\
	else {\
		(document.getElementById('upstype_value')).innerHTML = upsModeBusData.Input_Phase+'상 '+upsModeBusData.Output_Phase+'상';\
	}\
}\
	function onBodyLoadEvent(){ \
	WebSocketToSnmp('UPS_EX_DATA','');\
	};\
</script>\
";
const static char *webHTML_END="";

const static char *webHTML_CSS = 
"\
<style>\
html {\
	background-color: #DDD;\
}\
body {\
	-webkit-box-sizing: content-box;\
	-moz-box-sizing: content-box;\
	box-sizing: content-box;\
	width: 1100px;\
	margin: 0 auto;\
	font-family: Verdana, Helvetica, Arial, sans-serif;\
	-webkit-box-shadow: 0 0 10px 2px rgba(0, 0, 0, .5);\
	-moz-box-shadow: 0 0 10px 2px rgba(0, 0, 0, .5);\
	box-shadow: 0 0 10px 2px rgba(0, 0, 0, .5);\
	padding: 7px 25px 370px;\
}\
#head_div {\
	text-shadow: black 1px 1px 2px , blue 0 0 10px;\
	background: -webkit-radial-gradient(20% 10%, 120px 80px, white, #9ca4af);\
	border-bottom: 3px double black;\
	text-align:center;\
	font-family: Verdana, Helvetica, Arial, sans-serif;\
	font-size: 42px ;\
	font-weight:bold;\
}\
#menu  {\
	float: left;\
	width: 16%;	\
	position: releative;\
	height: 740px;\
	left: 20px;\
	margin: 5px;	\
}\
#topMenuLi{\
	list-style-type:none;\
	text-align:left;\
	margin-left:-2.5em;\
	margin-bottom:2px;\
	width:9em;\
	display:block;\
	font-weight: bold;\
	font-family: 'Trebuchet MS', Dotum, Arial;\
	line-height: 40px;\
	vertical-align: middle;\
	text-align: center;\
	background-color:#dbfff4;\
	box-shadow:5px 5px 5px gray;\
}\
#submenu  {\
	text-align: right;\
	margin-right:0.2em;\
	height: 0px;\
	overflow: hidden;\
	transition: height .6s;\
}\
#submenu li{\
	list-style-type:none;\
	margin-left:-1.0em;\
	box-shadow:1px 1px 1px gray;\
}\
#submenu li:hover{\
	list-style-type:none;\
	background-color:#DDDDEE;\
	box-shadow:3px 3px 3px gray;\
}\
#topMenuLi:hover #submenu  {	\
	background-color:#dbfff4;\
	box-shadow:3px 3px 3px gray;\
	margin-bottom:2px;\
	height: 150px;\
}\
#iftechLogoImg{\
	width:50px;\
	height:40px;\
}\
#company_info_logo{\
	float:left;\
	height:40px;\
	padding:10px 10px 10px 200px;\
	textAlign:right;\
}\
#company_info_name\
{\
	height:40px;\
	float:left;\
	padding:10px 120px 10px 120px;\
	color:blue;\
	textAlign:left;\
}\
#company_info_webaddress\
{\
	float:left;\
	height:40px;\
	padding:10px 00px 10px 120px;\
	textAlign:right;\
	color:blue;\
}\
h1, h2, h3, h4, h5, h6 {\
	font-family: Georgia, 'Times New Roman', serif;\
}\
h2, form {\
	text-align: center;\
}\
.loginputTextArea {\
	width: 800px;\
	height: 180px;\
	margin: 0 auto;\
	display: block;\
}\
#imageDiv  {\
	display:show;\
	border: 1px black solid;\
	float: left;\
	position: releative;\
	width: 805px;\
	height: 225px;\
	left: 200px;\
	top: 0px;\
	margin: 5px;	\
	\
}\
#content  {\
	display:show;\
	border: 1px black solid;\
	float: left;\
	width: 75%\
	position: releative;\
	width: 800px;\
	height: 450px;\
	left: 200px;\
	top: 0px;\
	margin: 5px;	\
}\
#content_left{\
	float:left;\
	width:48%;\
	margin:1px;\
}\
#content_right{\
	left-margin:52%;\
	width:48%;\
	float:left;\
	margin:1px;\
}\
#ups_inverter_label_title\
{\
	\
	background-color: #c6cdff;\
	float: left;\
	width:100%;\
}\
.ups_status\
{\
	float:left;\
	background-color: #DDD;\
	width:100%;\
	margin:1px;\
}\
.ups_inverter_label\
{\
	float:left;\
	background-color: #f0f8ff;\
	width:51%;\
	margin:1px;\
}\
.ups_inverter_value\
{\
	float:left;\
	background-color: #f0f8ff;\
	width:48%;\
	text-align: center;\
}\
#content_log  {\
	border: 1px blue solid;\
	float: left;\
	width: 73%;\
	margin: 5px;	\
}\
#footer{ \
	background: -webkit-radial-gradient(20% 10%, 120px 80px, white, #9ca4af); \
	border-top: 3px double gray;\
	clear:both;\
	text-align: center;\
	height:60px;\
	font-family: Verdana, Helvetica, Arial, sans-serif;\
	font-size: 18px ;\
	font-weight:bold;\
}\
h1, h2, h3, h4, h5, h6 {\
	font-family: Georgia, 'Times New Roman', serif;\
}\
#content_UpsSetup, #content_EmailSetup {\
	float: left;\
	width: 60%;\
	height: 400px;\
	top: 0px;\
	margin: 20px;	\
	\
}\
#SetNetWork_fieldset, #SetMailServer_fieldset,#SetMailUser_fieldset{\
	border: 1px black solid;\
	background-color: #AAAAFF;\
	float:left;\
	width: 600px;\
	margin-left: 50px;\
}\
form{\
}\
legend{\
	font-family: Verdana, Helvetica, Arial, sans-serif;\
	font-size: 25px ;\
	font-weight:bold;\
}\
label{\
	float: left;\
	width: 10em;\
	font-family: Verdana,Helvetica, Arial, sans-serif;\
	font-size: 14px ;\
}\
p {\
	margin: 1em;\
	margin-top: 1em;\
	margin-bottom: 1em\
}\
input {\
	background-color: #CCCCFF;\
	float: left;\
}\
button {\
	float: left;\
	width: 10em;\
	margin-left: 1em;\
	background-color: #45546b;\
color: #FFFFFF;}\
</style>\
";

const static char *webHTML_menu = "\
<body id='MainBody' onLoad='onBodyLoadEvent()'>\
	<div id ='head_div'>UPS관리시스템</div>\
	<div id='all'>\
	<div id ='menu'>	<P></P>\
		<ul>\
		<li id='topMenuLi'>기본정보\
			<ul id='submenu'>\
			<li><a href='../index.html'>UPS상태정보</a></li>\
			<!-- <li><a href='' onClick=goHome()>UPS기본정보</a></li>\
			<li><a href='../index.html'>UPS제어</a></li> -->\
			</ul>\
		</li>\
		<li id='topMenuLi'> 설정\
			<ul id='submenu'>\
			<li><a onClick=menuSelect('content_UpsSetup','','') >UPS설정</a></li>\
			<!-- <li><a onClick=menuSelect('content_EmailSetup','','') >Email설정</a></li>  -->\
			<li><a href='../SETUP_UPS.html'>IP설정</a></li>\
			<li><a href='../CHANGE_PASSWD.html'>비밀번호변경</a></li>\
		</ul>\
		</li>\
		<!-- <li id='topMenuLi'> 로그정보\
			<ul id='submenu'>\
			  <li><a href='../SETUP_UPS.html'>이벤트로그</a></li>\
			  <li><a href='../SETUP_UPS.html'>데이타로그</a></li>\
			  <li><a href='../SETUP_UPS.html'>데이타로그저장</a></li>\
		    </ul>\
	    </li> --> \
	</ul>\
	</div>\
";
const static char *webHTML_EMAILSETUP_content = 
"\
<div id='content_EmailSetup'>\
<form id='mailServerInfoForm' action = './SETUP_EMAIL.html' method='get'>\
<fieldset id='SetMailServer_fieldset'>\
<legend id='SetMailServer_Legend'>E-Mail설정</legend>\
<p><label>E-Mail서버</label> <input type = 'text'  id=mailServer name=mailServer value='ift.iptime.org'/></p><br>\
<p><label>발신자메일주소</label> <input type = 'text' id=userSendMail name=userSendMail value='mvcjhkim@gmail.com'/></p><br>\
<p><label>계정ID</label> <input type = 'text' id=userMailid name=userMailid value='mvcjhkim'/></p><br>\
<p><label>PASSWD</label> <input type = 'text' id=userMailPasswd name=userMailPasswd value='qoffl1260'/></p><br>\
<p><label>서버인증여부</label> <input type='checkbox' id='checkSeverAuthentic' name='checkSeverAuthentic' value='YES' />\
<p><label>이벤트메일전송</label> <input type='checkbox' id='checkEventMail' name='checkEventMail' value='YES' /><br>\
<p><input type='submit' value='적 용'></p><br>\
</fieldset>\
</form>\
<form id='mailUserInfoForm' action = './SETUP_EMAIL.html' method='get'>\
<fieldset id='SetMailUser_fieldset'>\
<legend id='SetMailUser_Legend'>이벤트 전송 메일 리스트</legend>\
<p><label>E-Mail주소</label> <input type = 'text' id=UM1 name=UM value='mvcjhkim@gmail.com'/></p><br>\
<p><label>E-Mail주소</label> <input type = 'text' id=UM2 name=UM value='mvcjhkim@gmail.com'/></p><br>\
<p><label>E-Mail주소</label> <input type = 'text' id=UM3 name=UM value='mvcjhkim@gmail.com'/></p><br>\
<p><label>E-Mail주소</label> <input type = 'text' id=UM4 name=UM value='mvcjhkim@gmail.com'/></p><br>\
<p><label>E-Mail주소</label> <input type = 'text' id=UM5 name=UM value='mvcjhkim@gmail.com'/></p><br>\
<p><input type='submit' value='적 용'></p><br>\
</fieldset>\
</form>\
</div>\
";
const static char *webHTML_UPSSETUPBASIC_script = "\
<script>\
function onBodyLoadEvent()\
{\
menuSelect('content_UpsSetup','','');\
document.getElementById('status_bar').innerHTML = '메뉴선택';\
}\
</script>\
" ;
const static char *webHTML_UPSSETUPBASIC_content = "\
			<div id='content_UpsSetup'>\
			<form action = ''>\
				<fieldset id='SetNetWork_fieldset'>\
		   		<legend id='network_Legend'>	UPS 기본 설정</legend>\
		   		<p><label  style='color:blue;text-align:left;width:200px;'>UPS 종류(통신방식)</label>\
		   		<select name='SB0' id='SB0'  style='color:blue;text-align:left;width:200px;margin-left:-70px;'>\
		   			    <option value='33' >IFTECH 3P3P MODBUS</option>\
					    <option value='31' >IFTECH 3P1P MODBUS</option>\
					    <option selected value='11'>IFTECH 1P1P MODBUS</option>\
					    <option value='50'>MEGA TECH SNMP 3P3P</option>\
					    <option value='51'>MEGA TECH SNMP 3P1P</option>\
					    <option value='52'>MEGA TECH SNMP 1P1P</option>\
					    <option value='80'>SENA SNMP</option>\
					    <option value='00' selected ></option>\
		   		</select> </p>\
				<p><label style='color:blue;text-align:left;width:200px;'>UPS 모델</label><input type = 'text' id=SB1 value='IFU550'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><label style='color:blue;text-align:left;width:200px;'>UPS 정격전압</label><input type = 'text' id=SB2 value='220'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><label style='color:blue;text-align:left;width:200px;'>UPS 충전전압</label><input type = 'text' id=SB3 value='220'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><label style='color:blue;text-align:left;width:200px;'>최종배터리교체일(년/월/일)</label><input type = 'text' id=SB4 value='2018/01/12'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><label style='color:blue;text-align:left;width:200px;'>제조년월일(년/월/일)</label><input type = 'text' id=SBC value='2019/10/21'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><label style='color:blue;text-align:left;width:200px;'>제조회사</label><input type = 'text' id=SBD value='IFTECH'    style='width:200px;color:blue;margin-left:50px;' /></p><br><br>\
				\
				<div  style='text-align:left;background-color:#445c82 ;' >UPS 로그 </div>\
				<p><label  style='color:blue;text-align:left;width:200px;'>로그기록주기</label>\
		   		<select name='SB5' id=SB5 style='color:blue;text-align:left;width:200px;margin-left:-70px;'>\
		   			    <option value='1'>1분</option>\
					    <option value='10'>10분</option>\
					    <option value='30'>30분</option>\
					    <option value='60'>1시간</option>\
					    <option value='0'>없음</option>\
		   		</select> </p>\
				<div  style='text-align:left;background-color:#445c82 ;' >UPS 테스트 </div>\
				<p><label  style='color:blue;text-align:left;width:200px;'>UPS테스트주기</label>\
		   		<select name='SB6' id=SB6 style='color:blue;text-align:left;width:200px;margin-left:-70px;'>\
		   			    <option value='0'>없음</option>\
					    <option value='1'>1주</option>\
					    <option value='2'>2주</option>\
		   		</select> </p>\
				<p><label  style='color:blue;text-align:left;width:200px;'>테스트요일</label>\
		   		<select name='SB7' id=SB7  style='color:blue;text-align:left;width:200px;margin-left:-70px;'>\
		   			    <option value='0'>일</option>\
					    <option value='1'>월</option>\
					    <option value='2'>화</option>\
					    <option value='3'>수</option>\
					    <option value='4'>목</option>\
					    <option value='5'>금</option>\
					    <option value='6'>토</option>\
		   		</select> </p>\
				<p><label  style='color:blue;text-align:left;width:200px;'>테스트시간(시:분)</label><input type = 'text' id=SB8  value='00:30'    style='width:200px;color:blue;margin-left:50px;' /></p><br><br>\
				<div  style='text-align:left;background-color:#445c82 ;' >경고한계치 </div>\
				<p><label  style='color:blue;text-align:left;width:200px;'>한계부하(%)</label><input type = 'text' id=SB9 value='100'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><label  style='color:blue;text-align:left;width:200px;'>한계온도(℃)</label><input type = 'text' id=SBA value='70.0'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><label  style='color:blue;text-align:left;width:200px;'>한계용량(%)</label><input type = 'text' id=SBB value='100'    style='width:200px;color:blue;margin-left:50px;' /></p><br>\
				<p><button type='button' onClick = 'setDataFromSnmp()'>적용</button>	<p>\
				</fieldset>\
			<div><span class='ups_status' id='status_bar' ></span></div>\
			</form>\
		</div>\
" ;


const static char *webHTML_PASSWD_content = 
"\
		<fieldset id='SetNetWork_fieldset'>   \
		<legend id='network_Legend'>SNMP 비밀번호 변경</legend>\
		<p>\
		<label>비밀번호</label>\
		<input type = 'text' id=passwd_text value=''/><button type='button' onClick = WebSocketToSnmp('changePasswd','') >변경</button>\
		</p><br>	\
		<div><span class='ups_status' id='status_bar' ></span></div>\
";
const static char *webHTML_UPSSETUP_content = 
"\
<div id='content_UpsSetup'>\
	<form action = ''>\
		<!--\
		<fieldset id='SetNetWork_fieldset'>\
		<legend id='network_Legend'>UPS 통신설정 </legend>\
		<input type = 'radio' name='radioUpsProtocalkind'  id=radioUpsProtocalkind value='1' checked>IFTECH 3P3P MODBUS<Br>\
		<input type = 'radio' name='radioUpsProtocalkind'  id=radioUpsProtocalkind value='2'        >IFTECH 3P1P MODBUS<Br>\
		<input type = 'radio' name='radioUpsProtocalkind'  id=radioUpsProtocalkind value='3'        >IFTECH 1P1P MODBUS<Br>\
		<input type = 'radio' name='radioUpsProtocalkind'  id=radioUpsProtocalkind value='4'        >IFTECH ST Command<Br>\
		<input type = 'radio' name='radioUpsProtocalkind'  id=radioUpsProtocalkind value='5'        >MEGA TECH SNMP<Br>\
		<input type = 'radio' name='radioUpsProtocalkind'  id=radioUpsProtocalkind value='6'        >SENA SNMP<Br>\
		<button type='button' onClick = 'radioUpsProtocalkindSetup(\"radioUpsProtocalkind\")'>설정</button>\
		</fieldset>\
		-->\
		<fieldset id='SetNetWork_fieldset'>   \
		<legend id='network_Legend'>UPS 장비 IP설정</legend>\
		<p>\
		<label>IP ADDRESS</label>\
		<input type = 'text' id=ipaddress_input_text value='192.168.0.55'/><button type='button' onClick = 'setNetwork_IpSetup(\"ipaddress_input_text\")'>설정</button>\
		</p><br>	\
		<p>\
		<label>GATE WAY</label>\
		<input type = 'text'  id=gateway_input_text value='192.168.0.1'/><button type='button' onClick = 'setNetwork_IpSetup(\"gateway_input_text\")'>설정</button>\
		</p><br>\
		<p>\
		<label>Subnet Mast</label>\
		<input type = 'text'  id=subnetmask_input_text value='255.255.255.0'/><button type='button' onClick = 'setNetwork_IpSetup(\"subnetmask_input_text\")'>설정</button>\
		</p><br>\
		<p>\
		<label>MAC ADDRESS</label>\
		<input type = 'text'  id=macaddress_input_text value='50.56.64.55.53,55'/><button type='button' onClick = 'setNetwork_IpSetup(\"macaddress_input_text\")'>설정</button>\
		</p><br>\
		<p>\
		<label>WEB_PORT</label>\
		<input type = 'text'  id=web_port_input_text value='80'/><button type='button' onClick = 'setNetwork_IpSetup(\"web_port_input_text\")'>설정</button>\
		</p><br>\
		<p>\
		<button type='button' onClick = 'setNetwork_IpSetup(\"ONOFF\")'>적용후 재 시작</button>\
		</p><br>\
		</fieldset>\
		<fieldset id='SetNetWork_fieldset'>   \
		<legend id='network_Legend'>SNMP TRAP ADDRESS</legend>\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_0 value='192.168.0.55'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_0\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_1 value='192.168.0.56'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_1\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_2 value='192.168.0.57'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_2\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_3 value='192.168.0.58'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_3\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_4 value='192.168.0.59'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_4\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_5 value='192.168.0.60'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_5\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_6 value='192.168.0.61'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_6\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_7 value='192.168.0.62'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_7\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_8 value='192.168.0.63'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_8\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TRAP ADDRESS</label>\
			<input type = 'text' id=trap_address_9 value='192.168.0.64'/><button type='button' onClick = 'setNetwork_IpSetup(\"trap_address_9\")'>설정</button>\
			</p><br>	\
		</fieldset>\
		<fieldset id='SetNetWork_fieldset'>   \
		<legend id='network_Legend'>TIME SERVER</legend>\
			<p>\
			<label>TIME SERVER 1</label>\
			<input type = 'text' id=time_server_0 value='192.168.0.55'/><button type='button' onClick = 'setNetwork_IpSetup(\"time_server_0\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>TIME SERVER 2</label>\
			<input type = 'text' id=time_server_1 value='192.168.0.55'/><button type='button' onClick = 'setNetwork_IpSetup(\"time_server_1\")'>설정</button>\
			</p><br>	\
			<p>\
			<label>SNMP TIME</label>\
			<input type = 'text' id=set_systemtime value='20180303121212'/><button type='button' onClick = 'setSystemTime(\"set_systemtime\")'>설정</button>\
			</p><br>	\
		</fieldset>\
		<fieldset id='SetNetWork_fieldset'>\
			<legend id='network_Legend'>화면 자동 갱신 주기 설정</legend>\
			<p>\
			<label >자동로딩시간</label>\
			<input type = 'text' id='reload_time' value='10'/>\
			<button type='button' onClick = 'setNetwork_IpSetup(\"reload_time\")'>설정</button>\
			</p><br>\
		</fieldset>\
	</form>\
</div>\
";

const static char *webHTML_default_START_content = 
"\
<div id='statusInfoDiv'>\
</div>\
<div id='imageDiv'>\
<canvas id='ups_canvas' width='800' height='220' style='border:1px solid #000000;' ></canvas>\
	<script>drawUpsDiagram()</script>\
</div>\
\
<div id='content'>\
	\
	<div id='content_left'>\
	<span id='ups_inverter_label_title'>UPS 기본 정보</span>\
	<span class='ups_inverter_label' id='Year_made_name'> 제조년월일</span>\
	<span class='ups_inverter_value' id='Year_made_value'>2000/01/01</span>\
	<span class='ups_inverter_label' id='Ups_Capacitor_name'>용 량</span>\
	<span class='ups_inverter_value' id='Ups_Capacitor_value'>0KVA</span>\
	<span class='ups_inverter_label' id='Input_Phase_name'>입 력</span>\
	<span class='ups_inverter_value' id='Input_Phase_value'>0상</span>	\
	<span class='ups_inverter_label' id='Input_Voltage_name'>입력전원</span>\
	<span class='ups_inverter_value' id='Input_Voltage_value'>0V</span>	\
	</div>\
	\
	<div id='content_right'>\
	<span id='ups_inverter_label_title'>UPS 기본정보\
	</span>\
	<span  class='ups_inverter_label' id='Output_Voltage_name'>출력전압</span>\
	<span  class='ups_inverter_value' id='Output_Voltage_value'>0V</span>\
	<span class='ups_inverter_label' id='Output_Phase_name'>출력위상 </span>\
	<span class='ups_inverter_value' id='Output_Phase_value'>0상</span>\
	<span class='ups_inverter_label' id='default_Installed_BAT'>축전지셀수</span>\
	<span class='ups_inverter_value' id='default_Installed_BAT_value'>0셀</span>\
	<span class='ups_inverter_label' id='selected_station_name '>설치장소</span>\
	<span class='ups_inverter_value' id='selected_station_value'>-</span>	\
	</div>\
	\
	<div id='content_left'>\
	<span id='ups_inverter_label_title'>입력 상태</span>\
	<span class='ups_inverter_label' id='Input_rst_volt_rms_name'> 전압 R/S/T</span>\
	<span class='ups_inverter_value' id='Input_rst_volt_rms_value'>0.0V</span>\
	<span class='ups_inverter_label' id='Input_r_current_rms_name'>전  류 </span>\
	<span class='ups_inverter_value' id='Input_r_current_rms_value'>0.0/0.0/0.0A</span>\
	<span class='ups_inverter_label' id='Input_frequency_name'>주 파 수 </span>\
	<span class='ups_inverter_value' id='Input_frequency_value'>0Hz</span>\
	</div>\
	\
	<div id='content_right'>\
	<span id='ups_inverter_label_title'>바이패스상태</span>\
	<span  class='ups_inverter_label' id='Bypass_rst_volt_rms_name'>전압 R/S/T</span>\
	<span  class='ups_inverter_value' id='Bypass_rst_volt_rms_value'>0V</span>\
	<span class='ups_inverter_label' id='Bypass_rst_current_rms_name'>전  류 </span>\
	<span class='ups_inverter_value' id='Bypass_rst_current_rms_value'>0Hz</span>\
	<span class='ups_inverter_label' id='Bypass_Frequency_name'>주 파 수 </span>\
	<span class='ups_inverter_value' id='Bypass_Frequency_value'>0Hz</span>\
	</div>\
	<!--  다음 라인 인버터상태  및 출력 정보 -->\
	<div id='content_left'>\
	<span id='ups_inverter_label_title'>인버터상태</span>\
	<span class='ups_inverter_label' id='Inverter_uvw_volt_rms_name'>전압 R/S/T</span>\
	<span class='ups_inverter_value' id='Inverter_uvw_volt_rms_value'>0.0V</span>\
	<span class='ups_inverter_label' id='Inverter_uvw_curr_rms_name'>전류</span>\
	<span class='ups_inverter_value' id='Inverter_uvw_curr_rms_value'>-</span>\
	<span class='ups_inverter_label' id='Inverter_Frequency_name'>주파수</span>\
	<span class='ups_inverter_value' id='Inverter_Frequency_value'>-</span>\
	<span class='ups_inverter_label' >-</span>\
	<span class='ups_inverter_value' >-</span>\
	\
	</div>\
\
	<div id='content_right'>\
	<span id='ups_inverter_label_title'>출력상태</span>\
	<span  class='ups_inverter_label' id='Output_rms_volt_rms_name'>전압 R/S/T</span>\
	<span  class='ups_inverter_value' id='Output_rms_volt_rms_value'>220V</span>\
	<span class='ups_inverter_label' id='Output_uvw_current_rms_name'>전 류 </span>\
	<span class='ups_inverter_value' id='Output_uvw_current_rms_value'>53A</span>\
	<span class='ups_inverter_label' id='Output_frequency_name'>주 파 수</span>\
	<span class='ups_inverter_value' id='Output_frequency_value'>60Hz</span>\
	<span class='ups_inverter_label' id='Output_RST_Load_name' >부 하 율</span>\
	<span class='ups_inverter_value' id='Output_RST_Load_value'>0/0/0%</span>\
	\
	</div>\
\
	<div id='content_left'>\
	<span id='ups_inverter_label_title'>축전지정보</span>\
	<span class='ups_inverter_label' id='Bat_volt_rms_value_name'>축전지 전압</span>\
	<span class='ups_inverter_value' id=Bat_volt_rms_value>0.0V</span>\
	<span class='ups_inverter_label' id='Bat_current_rms_name'>축전지 전류 </span>\
	<span class='ups_inverter_value' id='Bat_current_rms_value'>0A</span>\
	<span class='ups_inverter_label' id='Bat_charge_name'>축전지충전률 </span>\
	<span class='ups_inverter_value' id='Bat_charge_value'>---%</span>\
	\
	</div>\
	\
	<div id='content_right'>\
	<span id='ups_inverter_label_title'>-</span>\
	<span  class='ups_inverter_label' id='Company_code_name'>제조회사</span>\
	<span  class='ups_inverter_value' id='Company_code_value'>-</span>\
	<span class='ups_inverter_label' id='upstype_name'>UPS모델</span>\
	<span class='ups_inverter_value' id='upstype_value'>-</span>\
	\
	<span class='ups_inverter_label' id='upstype_name'>시스템타임</span>	\
	<span class='ups_inverter_value' id='snmp_time_value'>2017-07-01 00:00:00</span>\
	<span class='ups_inverter_label' id='upstype_name'>현재시간</span><span class='ups_inverter_value' id='system_time_value'>2017-07-01 00:00:00</span>\
	</div>\
	<div><span class='ups_status' id='status_bar' ></span></div>\
	\
</div>\
";

const static char *webHTML_default_START_log = "<div id='content_log'></div>";

const static char *webHTML_default_START_footer = 
"</div><div id='footer'>\
<span id='company_info_logo'  ><img  id='iftechLogoImg'  src='data:image/png;base64,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'/></span>\
<span id='company_info_name'>IFTECH </span>\
<span  id='company_info_webaddress'>http://www.iftech.co.kr </span></div></body></html>\
";



#endif

