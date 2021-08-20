/*
 * mib_ident_exp.h
 *
 * Created: 4/29/2021 1:04:28 PM
 *  Author: STELLA
 */ 


#ifndef MIB_IDENT_EXP_H_
#define MIB_IDENT_EXP_H_


static void ups_get_upsIdent_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsIdent_value_exp(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsIdent_test_exp(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsIdent_value_exp(struct obj_def *od, u16_t len, void *value);

static void ups_get_upsIdent_object_def_exp(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	u8_t id;
	/* return to object name, adding index depth (1) */
	ident_len += 1;
	ident -= 1;
	if (ident_len == 2)
	{
		od->id_inst_len = ident_len;
		od->id_inst_ptr = ident;

		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("get_object_def system.%"U16_F".0\n",(u16_t)id));

		//ups_info_t ups_info;
		//flash_read_ups_info(&ups_info);
		setSystemInfoDefault();
		switch (id)
		{
			case 1: /* 모델번호, 소프트웨어 버전 */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentManufacturer) ;
			break;
			case 2: /* upsIdentModel */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentModel) ;
			break;
			default:
			LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("system_get_object_def: no such object\n"));
			od->instance = MIB_OBJECT_NONE;
			break;
		}
	}
	else
	{
		LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("system_get_object_def: no scalar\n"));
		od->instance = MIB_OBJECT_NONE;
	};
	if(iCommErrorCount > 0 )
	od->instance = MIB_OBJECT_NONE;
}
static void ups_get_upsIdent_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;

	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];

	//ups_info_t ups_info;
	//flash_read_ups_info(&ups_info);
	setSystemInfoDefault();
	switch (id)
	{
		case 1: /* upsIdentManufacturer */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentManufacturer[0], len);
		break;
		case 2: /* upsIdentModel */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentModel[0], len);
		break;
	};
}
static u8_t ups_set_upsIdent_test_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	switch (id)
	{
		case 1: /* upsIdentManufacturer */
		set_ok = 1;
		break;
		case 2: /* upsIdentModel */
		set_ok = 1;
		break;
	};
	return set_ok;
}
static void ups_set_upsIdent_value_exp(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	//ups_info_t ups_info;
	//flash_read_ups_info(&ups_info);
	LWIP_ASSERT("invalid len", len <= 0xff);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	switch (id)
	{
		case 1: /* upsIdentManufacturer */
		memset(ups_info.upsIdentManufacturer,0x00,sizeof(ups_info.upsIdentManufacturer));
		ocstrncpy( ups_info.upsIdentManufacturer,(u8_t*)value, len);
		flash_write_ups_info(&ups_info);
		break;
		case 2: /*upsIdentModel */
		memset(ups_info.upsIdentModel,0x00,sizeof(ups_info.upsIdentModel));
		ocstrncpy( ups_info.upsIdentModel,(u8_t*)value, len);
		flash_write_ups_info(&ups_info);
		break;
	}
	
}

const mib_scalar_node ups_ident_scalar_exp = {
	&ups_get_upsIdent_object_def_exp,
	&ups_get_upsIdent_value_exp,
	&ups_set_upsIdent_test_exp,
	&ups_set_upsIdent_value_exp,
	MIB_NODE_SC,
	0
};

const s32_t mib2_ident_exp[2] ={ 1,2};
struct mib_node* const mib2_nodes_ident_exp[2] = {
	(struct mib_node*)&ups_ident_scalar_exp,/* 1모델번호, 소프트웨어 버전 */
	(struct mib_node*)&ups_ident_scalar_exp,/* 2특정한 MIB 정의 */
};
const struct mib_array_node ident_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	mib2_ident_exp,
	mib2_nodes_ident_exp
};




#endif /* MIB_IDENT_EXP_H_ */