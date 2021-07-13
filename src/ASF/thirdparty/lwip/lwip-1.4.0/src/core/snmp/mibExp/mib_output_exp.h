/*
 * mib_output_exp.h
 *
 * Created: 4/29/2021 11:39:26 AM
 *  Author: STELLA
 */ 


#ifndef MIB_OUTPUT_EXP_H_
#define MIB_OUTPUT_EXP_H_

static void ups_get_upsOutput_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsOutput_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsOutput_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsOutput_value_exp(struct obj_def *od, u16_t len, void *value);

static void ups_get_upsOutputTable_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsOutputTable_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsOutputTable_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsOutputTable_value_exp(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsOutput_scalar_exp = {
	&ups_get_upsOutput_object_def_exp,
	&ups_get_upsOutput_value_exp,
	&ups_set_upsOutput_test_exp,
	&ups_set_upsOutput_value_exp,
	MIB_NODE_SC,
	0
};



static void ups_get_upsOutput_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
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
}
static void ups_get_upsOutput_value_exp(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	//uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue=0;
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://  현재 입력 주파수
		lValue = upsModeBusData.Bypass_Frequency/10;
		*uint_ptr =(u32_t)lValue;
		break;
		case 2: // 에이전트가 시작된 이후의 절전 상태 수
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 3: // 에이전트가 시작된 이후의 블랙아웃 조건 수
		lValue =  0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 4: // 에이전트가 시작된 이후의 일시적인 조건 수
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 5: //  장치에서 사용되는 입력 단계의 수
		lValue =upsModeBusData.Input_Phase;
		*uint_ptr =(u32_t)lValue;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsOutput_test_exp(struct obj_def *od, u16_t len, void *value)
{return 1;};
static void ups_set_upsOutput_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};
const mib_scalar_node rfc1628_upsOutputTable_scalar_exp = {
	&ups_get_upsOutputTable_object_def_exp,
	&ups_get_upsOutputTable_value_exp,
	&ups_set_upsOutputTable_test_exp,
	&ups_set_upsOutputTable_value_exp,
	MIB_NODE_SC,
	0
};
static void ups_get_upsOutputTable_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2: case 3: case 4: case 5:
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
}

static void ups_get_upsOutputTable_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	//uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue;
	
	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://  출력 단계.
		lValue =  upsModeBusData.Output_Phase;
		*uint_ptr =(u32_t)lValue;
		break;
		case 2: // 출력 전압. (측정 단위 : 볼트)
		lValue =upsModeBusData.Output_r_volt_rms;
		*uint_ptr =(u32_t)lValue;
		break;
		case 3: //출력 전류. (측정 단위 : Amp / 10)
		lValue =  upsModeBusData.Output_u_current_rms;
		*uint_ptr =(u32_t)lValue ;
		break;
		case 4: // 출력 볼트 - 앰프.
		lValue = upsModeBusData.Output_r_volt_rms * upsModeBusData.Output_u_current_rms;
		*uint_ptr =(u32_t)lValue;
		break;
		case 5: //  출력 전력. (측정 단위 : 와트)
		lValue = upsModeBusData.Output_kw_KW;
		*uint_ptr =(u32_t)lValue;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsOutputTable_test_exp(struct obj_def *od, u16_t len, void *value)
{return 1;};
static void ups_set_upsOutputTable_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};




const s32_t mib2_output_s6sub[5] ={ 1,2,3,4,5};
struct mib_node* const mib2_nodes_output_s6sub[5] = {
	(struct mib_node*)&rfc1628_upsOutputTable_scalar_exp,
	(struct mib_node*)&rfc1628_upsOutputTable_scalar_exp,
	(struct mib_node*)&rfc1628_upsOutputTable_scalar_exp,
	(struct mib_node*)&rfc1628_upsOutputTable_scalar_exp,
	(struct mib_node*)&rfc1628_upsOutputTable_scalar_exp
};
const struct mib_array_node output_s6sub = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	mib2_output_s6sub,
	mib2_nodes_output_s6sub
};
// end input_s6sub
// input_s6_exp
const s32_t mib2_output_s6_exp[1] ={1 };
struct mib_node* const mib2_nodes_output_s6_exp[1] = {
	(struct mib_node*)&input_s6sub
};
const struct mib_array_node ups_output_s6_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,
	mib2_output_s6_exp,
	mib2_nodes_output_s6_exp
};
// end input_s6_exp




const s32_t mib2_output_exp[5] ={ 1,2,3,4,5};
struct mib_node* const mib2_nodes_output_exp[5] = {
	(struct mib_node*)&rfc1628_upsOutput_scalar_exp,
	(struct mib_node*)&rfc1628_upsOutput_scalar_exp,
	(struct mib_node*)&rfc1628_upsOutput_scalar_exp,
	(struct mib_node*)&rfc1628_upsOutput_scalar_exp,
	(struct mib_node*)&ups_output_s6_exp,
};
const struct mib_array_node output_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	mib2_output_exp,
	mib2_nodes_output_exp
};

#endif /* MIB_OUTPUT_EXP_H_ */