/*
 * mib_battery_exp.h
 *
 * Created: 4/29/2021 1:02:20 PM
 *  Author: STELLA
 */ 


#ifndef MIB_BATTERY_EXP_H_
#define MIB_BATTERY_EXP_H_

static void ups_get_upsBattery_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBattery_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBattery_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBattery_value_exp(struct obj_def *od, u16_t len, void *value);

static void ups_get_upsBattery_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2: case 3: case 4: case 5: case 6: case 7:
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

static void ups_get_upsBattery_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;

	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	u8_t istate;
	switch(id){
		case 1:// battery status    exp: 배터리 고갈 예상 시간
		if(ups_info.ups_type  == 52 || ups_info.ups_type  == 51 ||ups_info.ups_type  == 50  ){
			istate=*(pData+13);  // inverter state
			istate = istate & BIT(6);
			istate = istate >> 6;
			*uint_ptr = istate;
		}
		else
		*uint_ptr = 999;
		break;
		case 2: // estimate seconds on battery  exp : 배터리 온도
		*uint_ptr = 25;
		break;
		case 3: //배터리 전압
		*uint_ptr = *(pData+37);
		break;
		case 4:// 배터리 충전전류
		*uint_ptr = *(pData+38);
		break;
		case 5: //배터리 충전량
		*uint_ptr = *(pData+37);
		break;
		case 6: // 사용 가능한 총 배터리 용량
		*uint_ptr = *(pData+38);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBattery_test_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	return set_ok;
}


static void ups_set_upsBattery_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
}

const mib_scalar_node rfc1628_upsBattery_scalar_exp = {
	&ups_get_upsBattery_object_def_exp,
	&ups_get_upsBattery_value_exp,
	&ups_set_upsBattery_test_exp,
	&ups_set_upsBattery_value_exp,
	MIB_NODE_SC,
	0
};

const s32_t mib2_battery_exp[6] ={ 1,2,3,4,5,6};
struct mib_node* const mib2_nodes_battery_exp[6] = {
	(struct mib_node*)&rfc1628_upsBattery_scalar_exp,/* 1배터리 고갈 예상 시간 */
	(struct mib_node*)&rfc1628_upsBattery_scalar_exp,/* 2 배터리 온도*/
	(struct mib_node*)&rfc1628_upsBattery_scalar_exp,/* 3배터리 전압 */
	(struct mib_node*)&rfc1628_upsBattery_scalar_exp,/* 4 배터리 충전전류*/
	(struct mib_node*)&rfc1628_upsBattery_scalar_exp,/* 5 배터리 충전량*/
	(struct mib_node*)&rfc1628_upsBattery_scalar_exp/* 6 사용 가능한 총 배터리 용량*/
};
const struct mib_array_node battery_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	6,
	mib2_battery_exp,
	mib2_nodes_battery_exp
};
// end battery_exp
// input_s6sub



#endif /* MIB_BATTERY_EXP_H_ */