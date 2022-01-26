/*
 * upsTrap_exp.h
 *
 * Created: 4/29/2021 3:15:47 PM
 *  Author: STELLA
 */ 


#ifndef UPSTRAP_EXP_H_
#define UPSTRAP_EXP_H_
/*
1	SNMP Trap: SENA-UPSLINK-MIB::senaUpsPowerRestored	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.26\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
2	SNMP Trap: SENA-UPSLINK-MIB::senaUpsAwaitingPower	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.21\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
3	SNMP Trap: SENA-UPSLINK-MIB::senaUpsUserSpecificAlarm	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.25\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
4	SNMP Trap: SENA-UPSLINK-MIB::senaUpsTestInProg	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.24\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
5	SNMP Trap: SENA-UPSLINK-MIB::senaUpsShutdownPending	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.22\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
6	SNMP Trap: SENA-UPSLINK-MIB::senaUpsCommLost	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.20\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
7	SNMP Trap: SENA-UPSLINK-MIB::senaUpsFanFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.16\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
8	SNMP Trap: SENA-UPSLINK-MIB::senaUpsDiagTestFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.19\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
9	SNMP Trap: SENA-UPSLINK-MIB::senaUpslinkUPSTraps#	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
10	SNMP Trap: SENA-UPSLINK-MIB::senaUpsGenFault	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.18\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
11	SNMP Trap: SENA-UPSLINK-MIB::senaUpsFuseFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.17\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
12	SNMP Trap: SENA-UPSLINK-MIB::senaUpsSysOff	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.15\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
13	SNMP Trap: SENA-UPSLINK-MIB::senaUpsShutdownImminent	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.23\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
14	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutOff	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.14\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
15	SNMP Trap: SENA-UPSLINK-MIB::senaUpsUpsoffAr	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.12\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
16	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutputBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.7\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
17	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutoffAr	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.11\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
18	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBypassBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.10\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
19	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutOverload	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.8\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
20	SNMP Trap: SENA-UPSLINK-MIB::senaUpsInputBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.6\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
21	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOnBattery	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.2\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
22	SNMP Trap: SENA-UPSLINK-MIB::senaUpsTempBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.5\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
23	SNMP Trap: SENA-UPSLINK-MIB::senaUpsChargerFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.13\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
24	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryDeprecated	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.4\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
25	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryLow	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.3\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
26	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.1\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
27	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOnBypass	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.9\s"]		7d		SNMP 트랩	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	비활성
*/
static void ups_get_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_value_exp(struct obj_def *od, u16_t len, void *value);


const mib_scalar_node ups_scalar_exp = {
	&ups_get_object_def_exp,
	&ups_get_value_exp,
	&ups_set_test_exp,
	&ups_set_value_exp,
	MIB_NODE_SC,
	0
};

static void ups_get_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	/* return to object name, adding index depth (1) */

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
			case 7: case 8: case 9: case 10:
			case 11: case 12: case 13: case 14:
			case 15: case 16: case 17: case 18:
			case 19: case 20: case 21: case 22:
			case 23: case 24: case 25: case 26:
			case 27:
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

static void ups_get_value_exp(struct obj_def *od, u16_t len, void *value)
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
			*uint_ptr =(u32_t)( (*(pData+12) & BIT(1)) >> 1  );
			break;
		case 9://Output_frequency 10으로 나누어 준다
			*uint_ptr =((u32_t)( *(pData+49)))/10;
			break;
		case 10:
		case 11: case 12: case 13: case 14:case 15:
		case 16: case 17: case 18: case 19: case 20: 
		case 21: case 22: case 23: case 24: case 25: 
		case 26: case 27: 
			*uint_ptr =((u32_t)( *(pData+49)))/10;
			break;
		default:
			break;
	}
}

static u8_t ups_set_test_exp(struct obj_def *od, u16_t len, void *value)
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

static void ups_set_value_exp(struct obj_def *od, u16_t len, void *value)
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
const s32_t mib2_upsSpecial_exp[27] ={ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
struct mib_node* const mib2_nodes_upsSpecial_exp[27] = {
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp,
	(struct mib_node*)&ups_scalar_exp 
};

const struct mib_array_node upsSpecial_Value_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	27,  
	mib2_upsSpecial_exp,
	mib2_nodes_upsSpecial_exp
};

const s32_t mib2_companyCode_d2_exp[1] ={ 0};  //.1.3.6.1.4.1.12236.1.1.11
struct mib_node* const mib2_nodes_d2_companyCode_exp[1] = {(struct mib_node*)&upsSpecial_Value_exp};
const struct mib_array_node upsSpecial_d2_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 8개
	mib2_companyCode_d2_exp,
	mib2_nodes_d2_companyCode_exp
};

const s32_t mib2_companyCode_d1_exp[1] ={ 11};  //.1.3.6.1.4.1.12236.1.1.11
struct mib_node* const mib2_nodes_d1_companyCode_exp[1] = {(struct mib_node*)&upsSpecial_d2_exp};
const struct mib_array_node upsSpecial_d1_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 8개
	mib2_companyCode_d1_exp,
	mib2_nodes_d1_companyCode_exp
};

const s32_t mib2_companyCode_exp[1] ={ 1};  //.1.3.6.1.4.1.12236.1.1
struct mib_node* const mib2_nodes_companyCode_exp[1] = {(struct mib_node*)&upsSpecial_d1_exp};
const struct mib_array_node upsSpecial_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 8개
	mib2_companyCode_exp,
	mib2_nodes_companyCode_exp
};



#endif /* UPSTRAP_EXP_H_ */