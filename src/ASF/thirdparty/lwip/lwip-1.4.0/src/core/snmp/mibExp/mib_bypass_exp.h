/*
 * mib_bypass_exp.h
 *
 * Created: 4/29/2021 12:08:47 PM
 *  Author: STELLA
 */ 


#ifndef MIB_BYPASS_EXP_H_
#define MIB_BYPASS_EXP_H_


static void ups_get_upsBypass_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBypass_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBypass_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBypass_value_exp(struct obj_def *od, u16_t len, void *value);

static void ups_get_upsBypassTable_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBypassTable_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBypassTable_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBypassTable_value_exp(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsBypass_scalar_exp = {
	&ups_get_upsBypass_object_def_exp,
	&ups_get_upsBypass_value_exp,
	&ups_set_upsBypass_test_exp,
	&ups_set_upsBypass_value_exp,
	MIB_NODE_SC,
	0
};

const mib_scalar_node rfc1628_upsBypassTable_scalar_exp = {
	&ups_get_upsBypassTable_object_def_exp,
	&ups_get_upsBypassTable_value_exp,
	&ups_set_upsBypassTable_test_exp,
	&ups_set_upsBypassTable_value_exp,
	MIB_NODE_SC,
	0
};



static void ups_get_upsBypass_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsBypass_value_exp(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue=0;
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:// 우회 주파수. (측정 단위 : mHz)
			lValue = upsModeBusData.Bypass_Frequency;
			*uint_ptr =(u32_t)lValue;
			break;
		case 2: // 이 장치에서 사용되는 우회 위상의 수. 이 항목은 입력 테이블의 행 수
			lValue = 25;
			*uint_ptr =(u32_t)lValue;
			break;
		default:
			*uint_ptr = upsModeBusData.Input_Phase;
			break;
	}
}

static u8_t ups_set_upsBypass_test_exp(struct obj_def *od, u16_t len, void *value)
{return 1;};
static void ups_set_upsBypass_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


static void ups_get_upsBypassTable_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
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

static void ups_get_upsBypassTable_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
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

static u8_t ups_set_upsBypassTable_test_exp(struct obj_def *od, u16_t len, void *value)
{return 1;};
static void ups_set_upsBypassTable_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};

const s32_t mib2_bypass_s6sub[3] ={ 1,2,3};
struct mib_node* const mib2_nodes_bypass_s6sub[3] = {
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep
};
const struct mib_array_node bypass_s6sub = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	mib2_bypass_s6sub,
	mib2_nodes_bypass_s6sub
};
// end bypass_s6sub
// bypass_s6_exp
const s32_t mib2_bypass_s6_exp[1] ={1 };
struct mib_node* const mib2_nodes_bypass_s6_exp[1] = {
	(struct mib_node*)&bypass_s6sub
};
const struct mib_array_node ups_bypass_s6_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,
	mib2_bypass_s6_exp,
	mib2_nodes_bypass_s6_exp
};
// bypass_exp
const s32_t mib2_bypass_exp[3] ={ 1,2,3};
struct mib_node* const mib2_nodes_bypass_exp[3] = {
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_bypass_s6_exp
};
//(struct mib_node*)&ups_bypass_s6_exp
const struct mib_array_node bypass_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	mib2_bypass_exp,
	mib2_nodes_bypass_exp
};
//end bypass

#endif /* MIB_BYPASS_EXP_H_ */