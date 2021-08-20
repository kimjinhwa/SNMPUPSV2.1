/*
 * mibKep.h
 *
 * Created: 4/29/2021 3:39:09 PM
 *  Author: STELLA
 */ 

#ifndef MIBKEP_H_
#define MIBKEP_H_

extern Bool getDataFromSerial();
extern int16_t charging_method;
extern Bool bModebusSuccess;
extern Bool stopModebusGet;
//kep_mgmt  이렇게 하면 .1.3.6.1.2---과 .1.3.6.1.4---는 동일한 값을 표현한다.
const mib_scalar_node ups_scalar_kep = {
	&ups_get_object_def_kep,
	&ups_get_value_kep,
	&ups_set_test_kep,
	&ups_set_value_kep,
	MIB_NODE_SC,
	0
};
const s32_t mib2_upsSpecial_kep[23] ={ 1,2,3,4,5,6,7,8,9,200,201,202,203,210,211,212,213,214,215,216,217,224,225 };
struct mib_node* const mib2_nodes_upsSpecial_kep[23] = {
	(struct mib_node*)&ups_scalar_kep,/* 1 */
	(struct mib_node*)&ups_scalar_kep,/* 2 */
	(struct mib_node*)&ups_scalar_kep,/* 3 */
	(struct mib_node*)&ups_scalar_kep,/* 4 */
	(struct mib_node*)&ups_scalar_kep,/* 5 */
	(struct mib_node*)&ups_scalar_kep,/* 6 */
	(struct mib_node*)&ups_scalar_kep,/* 7 */
	(struct mib_node*)&ups_scalar_kep,/* 8 */
	(struct mib_node*)&ups_scalar_kep,/* 9 */
	(struct mib_node*)&ups_scalar_kep,/* 200 */
	(struct mib_node*)&ups_scalar_kep,/* 201 */
	(struct mib_node*)&ups_scalar_kep,/* 202 */
	(struct mib_node*)&ups_scalar_kep,/* 203 */
	(struct mib_node*)&ups_scalar_kep,/* 210 */
	(struct mib_node*)&ups_scalar_kep,/* 211 */
	(struct mib_node*)&ups_scalar_kep,/* 212 */
	(struct mib_node*)&ups_scalar_kep,/* 213 */
	(struct mib_node*)&ups_scalar_kep,/* 214 */
	(struct mib_node*)&ups_scalar_kep,/* 215 */
	(struct mib_node*)&ups_scalar_kep,/* 216 */
	(struct mib_node*)&ups_scalar_kep,/* 217 */
	(struct mib_node*)&ups_scalar_kep,/* 224 */
	(struct mib_node*)&ups_scalar_kep /* 225 */
};

const struct mib_array_node upsSpecial_Value_kep = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	23,  // 23개
	mib2_upsSpecial_kep,
	mib2_nodes_upsSpecial_kep
};

const s32_t mib2_companyCode_kep[1] ={12082 };
struct mib_node* const mib2_nodes_companyCode_kep[1] = {(struct mib_node*)&upsSpecial_Value_kep};
const struct mib_array_node upsSpecial_kep = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 2개
	mib2_companyCode_kep,
	mib2_nodes_companyCode_kep
};

const s32_t mib2_company_kep[2] ={ 1,2 };
extern const struct mib_array_node upsSpecial_exp;
struct mib_node* const mib2_nodes_company_kep[2] = {(struct mib_node*)&upsSpecial_exp,(struct mib_node*)&upsSpecial_kep};

const struct mib_array_node upsCompany_kep = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	mib2_company_kep,
	mib2_nodes_company_kep
};
//----------------------------------------


const s32_t mib2_ups_xppc[1] ={ 1 };
struct mib_node* const mib2_nodes_ups_xppc[1] = {(struct mib_node*)&rfc1628_upsMIB};
const struct mib_array_node ups_xppc = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 1개
	mib2_ups_xppc,
	mib2_nodes_ups_xppc
};

const s32_t mib2_hardware_xppc[1] ={ 1 };
struct mib_node* const mib2_nodes_hardware_xppc[1] = {(struct mib_node*)&ups_xppc};
const struct mib_array_node hardware_xppc = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 1개
	mib2_hardware_xppc,
	mib2_nodes_hardware_xppc
};

const s32_t mib2_product_xppc[1] ={ 1 };
struct mib_node* const mib2_nodes_product_xppc[1] = {(struct mib_node*)&hardware_xppc,};
const struct mib_array_node product_xppc = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 1개
	mib2_product_xppc,
	mib2_nodes_product_xppc
};
//----------------------
const s32_t mib2_device_kep[2] ={ 935,12236 };
//(struct mib_node*)&rfc1628_upsMIB,
struct mib_node* const mib2_nodes_device_kep[2] = {(struct mib_node*)&product_xppc,(struct mib_node*)&upsCompany_kep};


const struct mib_array_node upsMIB_kep = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,  // 2개
	mib2_device_kep,
	mib2_nodes_device_kep
};


const s32_t mib2_ids_kep[1] ={ 1 };
struct mib_node* const mib2_nodes_kep[1] = {(struct mib_node*)&upsMIB_kep};
const struct mib_array_node mgmt_kep = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,
	mib2_ids_kep,
	mib2_nodes_kep
};
static void ups_get_object_def_kep(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	/* return to object name, adding index depth (1) */
	int breakCount = 150;
	while( isModebusRunning)
	{
		vTaskDelay(10); //if(lValue>500)break; //lValue++;
		if( breakCount-- == 0){
			od->instance = MIB_OBJECT_NONE;
			return ;	
		}
	};
	/*
	if(bModebusSuccess ==  false){
		od->instance = MIB_OBJECT_NONE;
		return ;	
	}
	*/
	ident_len += 1;
	ident -= 1;
	if (ident_len == 2)
	{
		u8_t id;

		od->id_inst_len = ident_len;
		od->id_inst_ptr = ident;

		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch (id)
		{
			case 1:// Input_r_volt_rms
			case 2: // Input_s_volt_rms
			case 3: // Input_t_volt_rms
			case 4:	//Output_r_volt_rms
			case 5://Output_u_current_rms
			case 6://Bat_volt_rms
			case 7://Bat_current_rms
				od->instance = MIB_OBJECT_SCALAR;
				od->access = MIB_OBJECT_READ_ONLY;
				od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
				od->v_len = sizeof(u32_t);
				break;
			case 8:// 부동충전 or 균등 충전 Converter Status
				od->instance = MIB_OBJECT_SCALAR;
				od->access = MIB_OBJECT_READ_ONLY;
				od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
				if(charging_method == 0 ) od->v_len = 8;
				else od->v_len = 10;
				break;
			case 9://Output_frequency 10으로 나누어 준다
			case 200:
			case 201:
			case 202:
			case 203:
			case 210:
			case 211:
			case 212:
			case 213:
			case 214:
			case 215:
			case 216:
			case 217:
			case 224:
			case 225:
				od->instance = MIB_OBJECT_SCALAR;
				od->access = MIB_OBJECT_READ_ONLY;
				od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
				od->v_len = sizeof(u32_t);
			break;
			default:
				od->instance = MIB_OBJECT_NONE;
				break;
		}
	}
	else
	{
		LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("snmp_get_object_def: no scalar\n"));
		od->instance = MIB_OBJECT_NONE;
	}
	// 통신 에러가 있다면 데이타를 송출하지 않는다.
	if(iCommErrorCount > 0 )
	od->instance = MIB_OBJECT_NONE;
}

static void ups_get_value_kep(struct obj_def *od, u16_t len, void *value)
{

	/*{ 1,2,3,4,5,6,7,8,9,200,201,202,203,210,211,212,213,214,215,216,217,224,225 };*/
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	
	id = (u8_t)od->id_inst_ptr[0];
	// 아이디와 UPS-MIB를 맞추어 준다..나중에
	//id = id+1;
	uint16_t lValue=0;
	stopModebusGet = true;
	/*
	*/
	//portENTER_CRITICAL();
	//if(upsModeBusData.Bat_volt_rms == 0 ){
	//		while(getDataFromSerial());
		 //return;
	//}
	//if(upsModeBusData.Bat_volt_rms == 0 ){
		//portEXIT_CRITICAL();
	//	return;
	//}
	lValue=0;
	switch (id){
		case 1:// Input_r_volt_rms
			*uint_ptr =(u32_t)( *(pData+16) );
			break;
		case 2: // Input_s_volt_rms
			*uint_ptr=(u32_t)(*(pData+4))==1 ?  0:(u32_t)( *(pData+17)) ;
			break;  
		case 3: // Input_t_volt_rms
			*uint_ptr=(u32_t)(*(pData+4))==1 ?  0:(u32_t)( *(pData+18)) ;
			break;
		case 4:	//Output_r_volt_rms
			*uint_ptr =(u32_t)( *(pData+43));
			break;
		case 5://Output_u_current_rms
			*uint_ptr =(u32_t)( *(pData+46))+(u32_t)( *(pData+47))+(u32_t)( *(pData+48));
			break;
		case 6://Bat_volt_rms
			*uint_ptr =(u32_t)( *(pData+37));
			break;
		case 7://Bat_current_rms
			*uint_ptr =(u32_t)( *(pData+38));
			break;
		case 8://부동충전 or 균등 충전 Converter Status  Floating or Equalizing
			*uint_ptr =charging_method;//(u32_t)( (*(pData+12) & BIT(1)) >> 1  );
			if(charging_method == 0 ) ocstrncpy((u8_t*)value,  "Floating  ", len);
			else ocstrncpy((u8_t*)value, "Equalizing", len);
			break;
		case 9://Output_frequency 10으로 나누어 준다
			*uint_ptr =((u32_t)( *(pData+49)))/10;
			break;
		case 200:
			*uint_ptr =(u32_t)( *(pData+12)); 
			break;  //power module fail or bypass CONVERTER D12,INVERTER D13,D14
		case 201:
			*uint_ptr =(u32_t)( *(pData+13)); 
			break; // normal
		case 202:
			*uint_ptr =(u32_t)( *(pData+14));
			 break; // 정전 또는 입력전원이상 CONVERTER OPERATION FAULT 0X2EE0
		case 203:
			*uint_ptr =(u32_t)( *(pData+15)) ; 
			break; // 정상
		case 210:
			*uint_ptr =0; 
			break; // MODULE#1 장애
		case 211:
			*uint_ptr =0; 
			break;
		case 212:
			*uint_ptr =0; 
			break;// MODULE#2장애
		case 213:
			*uint_ptr =0; 
			break;
		case 214:
			*uint_ptr =0; 
			break;// MODULE#3 장애
		case 215:
			*uint_ptr =0; 
			break;
		case 216:	
			*uint_ptr =0; 
			break;// MODULE#4 장애
		case 217:
			*uint_ptr =0; 
			break;
		case 224:
			*uint_ptr =0; 
			break;//찬단기 켜짐
		case 225:
			*uint_ptr =0; 
			break;
		default:
		break;
	}
	stopModebusGet = false;
	//portEXIT_CRITICAL();
}
static u8_t ups_set_test_kep(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;

	LWIP_UNUSED_ARG(len);
	set_ok = 0;
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	/*
	if (id == 30)
	{
		// snmpEnableAuthenTraps 
		s32_t *sint_ptr = (s32_t*)value;

		if (snmpenableauthentraps_ptr != &snmpenableauthentraps_default)
		{
			// we should have writable non-volatile mem here 
			if ((*sint_ptr == 1) || (*sint_ptr == 2))
			{
				set_ok = 1;
			}
		}
		else
		{
			// const or hardwired value 
			if (*sint_ptr == snmpenableauthentraps_default)
			{
				set_ok = 1;
			}
		}
	}
	*/
				set_ok = 1;
	return set_ok;
}
static void ups_set_value_kep(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;

	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	if (id == 30)
	{
		/* snmpEnableAuthenTraps */
		/* @todo @fixme: which kind of pointer is 'value'? s32_t or u8_t??? */
		u8_t *ptr = (u8_t*)value;
		*snmpenableauthentraps_ptr = *ptr;
	}
}
#endif /* MIBKEP_H_ */