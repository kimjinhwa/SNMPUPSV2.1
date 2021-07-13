/*
 * mib_testbattery_exp.h
 *
 * Created: 4/29/2021 2:43:34 PM
 *  Author: STELLA
 */ 


#ifndef MIB_TESTBATTERY_EXP_H_
#define MIB_TESTBATTERY_EXP_H_


static void ups_get_upsTestBattery_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsTestBattery_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsTestBattery_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsTestBattery_value_exp(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsTestBattery_scalar_exp = {
	&ups_get_upsTestBattery_object_def_exp,
	&ups_get_upsTestBattery_value_exp,
	&ups_set_upsTestBattery_test_exp,
	&ups_set_upsTestBattery_value_exp,
	MIB_NODE_SC,
	0
};




static void ups_get_upsTestBattery_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
			case 6:
			case 7:
			case 8:
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
static void ups_get_upsTestBattery_value_exp(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue=0;
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://각 값은 이 객체의 값을 계산하기 위해 더해진다.
		lValue =  99;
		*uint_ptr =(u32_t)lValue;
		break;
		case 2: //
		*uint_ptr =(u32_t)lValue;
		break;
		case 3: //
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 4: //
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 5: //
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		case 6: //
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		case 7: //
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsTestBattery_test_exp(struct obj_def *od, u16_t len, void *value)
{return 1;};
static void ups_set_upsTestBattery_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};
const s32_t mib2_upsTestBattery_exp[6] ={ 1,2,3,4,5,6};
struct mib_node* const mib2_nodes_upsTestBattery_exp[6] = {
	(struct mib_node*)&rfc1628_upsTestBattery_scalar_exp,
	(struct mib_node*)&rfc1628_upsTestBattery_scalar_exp,
	(struct mib_node*)&rfc1628_upsTestBattery_scalar_exp,
	(struct mib_node*)&rfc1628_upsTestBattery_scalar_exp,
	(struct mib_node*)&rfc1628_upsTestBattery_scalar_exp,
	(struct mib_node*)&rfc1628_upsTestBattery_scalar_exp
};
const struct mib_array_node upsTestBattery_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	6,
	mib2_upsTestBattery_exp,
	mib2_nodes_upsTestBattery_exp
};



#endif /* MIB_TESTBATTERY_EXP_H_ */