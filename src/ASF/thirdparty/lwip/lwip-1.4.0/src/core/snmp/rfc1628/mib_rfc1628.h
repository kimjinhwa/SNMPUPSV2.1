/*
 * mib_rfc1268.h
 *
 * Created: 4/29/2021 3:21:00 PM
 *  Author: STELLA
 */ 


#ifndef MIB_RFC1268_H_
#define MIB_RFC1268_H_


static void ups_get_upsIdent_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsIdent_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsIdent_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsIdent_value(struct obj_def *od, u16_t len, void *value);

static u8_t ups_set_upsIdent_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	switch (id)
	{
		case 5: /* sysContact */
		set_ok = 1;
		break;
		case 6: /* sysName */
		set_ok = 1;
		break;
	};
	return set_ok;
}


static void ups_set_upsIdent_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	//ups_info_t ups_info;
	flash_read_ups_info(&ups_info);
	LWIP_ASSERT("invalid len", len <= 0xff);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	switch (id)
	{
		case 5: /* upsIdentName */
		memset(ups_info.upsIdentName,0x00,sizeof(ups_info.upsIdentName));
		ocstrncpy( ups_info.upsIdentName,(u8_t*)value, len);
		flash_write_ups_info(&ups_info);
		break;
		case 6: /* upsIdentAttachedDevices*/
		memset(ups_info.upsIdentAttachedDevices,0x00,sizeof(ups_info.upsIdentAttachedDevices));
		ocstrncpy( ups_info.upsIdentAttachedDevices,(u8_t*)value, len);
		flash_write_ups_info(&ups_info);
		break;
	}
	
}

static void ups_get_upsIdent_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
		flash_read_ups_info(&ups_info);
		setSystemInfoDefault();
		switch (id)
		{
			case 1: /* upsIdentManufacturer */
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
			case 3: /* upsIdentUPSSoftwareVersion */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentUPSSoftwareVersion) ;
			break;
			case 4: /* upsIdentManufacturer */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentAgentSoftwareVersion) ;
			break;
			case 5: /* upsIdentName */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentName) ;
			break;
			case 6: /* sysLocation */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentAttachedDevices) ;
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
static void ups_get_upsIdent_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;

	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];

	//ups_info_t ups_info;
	flash_read_ups_info(&ups_info);
	setSystemInfoDefault();
	switch (id)
	{
		case 1: /* upsIdentManufacturer */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentManufacturer[0], len);
		break;
		case 2: /* upsIdentModel */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentModel[0], len);
		break;
		case 3: /* upsIdentUPSSoftwareVersion */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentUPSSoftwareVersion[0], len);
		break;
		case 4: /* upsIdentAgentSoftwareVersion */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentAgentSoftwareVersion[0], len);
		break;
		case 5: /* upsIdentName */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentName[0], len);
		break;
		case 6: /* upsIdentAttachedDevices */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentAttachedDevices[0], len);
		break;
	};
}


const mib_scalar_node rfc1628_ups_scalar = {
	&ups_get_object_def,
	&ups_get_value,
	&ups_set_test,
	&ups_set_value,
	MIB_NODE_SC,
	0
};
const mib_scalar_node rfc1628_upsIdent_scalar = {
	&ups_get_upsIdent_object_def,
	&ups_get_upsIdent_value,
	&ups_set_upsIdent_test,
	&ups_set_upsIdent_value,
	MIB_NODE_SC,
	0
};


// 하나씩 이 부분은 바꿔준다. 임시로 설정 되어 있다
const s32_t rfc1628_ups_ids[4] = {
	1,  2,  3,  4
};
struct mib_node* const rfc1628_ups_nodes[4] = {
	(struct mib_node*)&rfc1628_ups_scalar , (struct mib_node*)&rfc1628_ups_scalar,
	(struct mib_node*)&rfc1628_ups_scalar, (struct mib_node*)&rfc1628_ups_scalar
};
// 1: upsIdentManufacturer
// 2:upsIdentModel
// 3:upsIdentUPSSoftwareVersion
// 4:upsAgentSoftwareVersion
// 5:upsIdentName
// 6:upsIdentAttachedDevices
//
const s32_t rfc1628_upsIdent_ids[6] = {
	1,  2,  3,  4  ,5 ,6
};
struct mib_node* const rfc1628_upsIdent_nodes[6] = {
	(struct mib_node*)&rfc1628_upsIdent_scalar ,
	(struct mib_node*)&rfc1628_upsIdent_scalar ,
	(struct mib_node*)&rfc1628_upsIdent_scalar ,
	(struct mib_node*)&rfc1628_upsIdent_scalar ,
	(struct mib_node*)&rfc1628_upsIdent_scalar ,
	(struct mib_node*)&rfc1628_upsIdent_scalar
};

const struct mib_array_node  upsIdent = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	6,
	rfc1628_upsIdent_ids,
	rfc1628_upsIdent_nodes
};

//  upsBattery get and set
static void ups_get_upsBattery_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBattery_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBattery_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBattery_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsBattery_scalar = {
	&ups_get_upsBattery_object_def,
	&ups_get_upsBattery_value,
	&ups_set_upsBattery_test,
	&ups_set_upsBattery_value,
	MIB_NODE_SC,
	0
};


static void ups_get_upsBattery_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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

static void ups_get_upsBattery_value(struct obj_def *od, u16_t len, void *value)
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
		case 3: //upsEstimateMinute
		*uint_ptr = 0;
		break;
		case 4:// charte remainning
		*uint_ptr = 0;
		break;
		case 5: // ups_battery_voltage
		*uint_ptr = *(pData+37);
		break;
		case 6: // current
		*uint_ptr = *(pData+38);
		break;
		case 7: // ups battery temperature
		*uint_ptr = *(pData+58);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBattery_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	return set_ok;
}


static void ups_set_upsBattery_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
}


const s32_t rfc1628_upsBattery_ids[7] = {
	1,  2,  3,  4  , 5, 6, 7
};
struct mib_node* const rfc1628_upsBattery_nodes[7] = {
	(struct mib_node*)&rfc1628_upsBattery_scalar , (struct mib_node*)&rfc1628_upsBattery_scalar,
	(struct mib_node*)&rfc1628_upsBattery_scalar, (struct mib_node*)&rfc1628_upsBattery_scalar,
	(struct mib_node*)&rfc1628_upsBattery_scalar, (struct mib_node*)&rfc1628_upsBattery_scalar,
	(struct mib_node*)&rfc1628_upsBattery_scalar
};
const struct mib_array_node  upsBattery= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	7,
	rfc1628_upsBattery_ids,
	rfc1628_upsBattery_nodes
};
//--------------------upsInputEntry
static void ups_get_upsInputTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsInputTable_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsInputTable_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsInputTable_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsInputTable_scalar = {
	&ups_get_upsInputTable_object_def,
	&ups_get_upsInputTable_value,
	&ups_set_upsInputTable_test,
	&ups_set_upsInputTable_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsInputTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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

static void ups_get_upsInputTable_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue;
	
	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://  input line identifier
		lValue =  *(pData+5);
		*uint_ptr =(u32_t)lValue;
		break;
		case 2: // frequency
		lValue = (*(pData+22))/10;
		*uint_ptr =(u32_t)lValue;
		break;
		case 3: // RMS voltage
		lValue = *(pData+16);
		*uint_ptr =(u32_t)lValue;
		break;
		case 4: // input current
		lValue = *(pData+19);
		*uint_ptr =(u32_t)lValue;
		break;
		case 5: //  watt
		lValue = (*(pData+40))/10;
		*uint_ptr =(u32_t)lValue;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsInputTable_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsInputTable_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};




//  upsInput get and set
static void ups_get_upsInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsInput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsInput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsInput_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsInput_scalar = {
	&ups_get_upsInput_object_def,
	&ups_get_upsInput_value,
	&ups_set_upsInput_test,
	&ups_set_upsInput_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2:
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
static void ups_get_upsInput_value(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:// utility line fail count
		*uint_ptr = 0;
		break;
		case 2: // input phase
		*uint_ptr = *(pData+4);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsInput_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsInput_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};



const s32_t rfc1628_upsInputEntry_ids[5] = {
	1,  2,  3, 4, 5
};
struct mib_node* const rfc1628_upsInputEntry_nodes[5] = {
	(struct mib_node*)&rfc1628_upsInputTable_scalar , (struct mib_node*)&rfc1628_upsInputTable_scalar,
	(struct mib_node*)&rfc1628_upsInputTable_scalar , (struct mib_node*)&rfc1628_upsInputTable_scalar,
	(struct mib_node*)&rfc1628_upsInputTable_scalar
};
const struct mib_array_node  upsInputEntry= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	rfc1628_upsInputEntry_ids,
	rfc1628_upsInputEntry_nodes
};
const s32_t rfc1628_upsInputTable_ids = 1;
struct mib_node* const rfc1628_upsInputTable_nodes =
(struct mib_node*)&upsInputEntry;

const struct mib_array_node  upsInputTable= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_RA,
	1,
	&rfc1628_upsInputTable_ids,
	&rfc1628_upsInputTable_nodes
};

const s32_t rfc1628_upsInput_ids[3] = {
	1,  2,  3
};
struct mib_node* const rfc1628_upsInput_nodes[3] = {
	(struct mib_node*)&rfc1628_upsInput_scalar , (struct mib_node*)&rfc1628_upsInput_scalar,
	(struct mib_node*)&upsInputTable
};

const struct mib_array_node  upsInput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	rfc1628_upsInput_ids,
	rfc1628_upsInput_nodes
};
//--------------------upsOutputEntry .1.3.6.1.33.1.4.4.1.1~5
//--------------------upsOutputEntry
static void ups_get_upsOutputTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsOutputTable_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsOutputTable_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsOutputTable_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsOutputTable_scalar = {
	&ups_get_upsOutputTable_object_def,
	&ups_get_upsOutputTable_value,
	&ups_set_upsOutputTable_test,
	&ups_set_upsOutputTable_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsOutputTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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

static void ups_get_upsOutputTable_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue;

	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:// upsoutput line identifier
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 2: //RMS Voltage
		lValue = *(pData+43);
		*uint_ptr =(u32_t)lValue;
		break;
		case 3: // RMS  current
		lValue = *(pData+46);
		*uint_ptr =(u32_t)lValue;
		break;
		case 4: // watt
		lValue = (*(pData+51))/10;
		*uint_ptr =(u32_t)lValue;
		break;
		case 5: //
		lValue = *(pData+54);
		*uint_ptr =(u32_t)lValue;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsOutputTable_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsOutputTable_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};

const s32_t rfc1628_upsOutputEntry_ids[5] = {
	1,  2,  3, 4, 5
};
struct mib_node* const rfc1628_upsOutputEntry_nodes[5] = {
	(struct mib_node*)&rfc1628_upsOutputTable_scalar , (struct mib_node*)&rfc1628_upsOutputTable_scalar,
	(struct mib_node*)&rfc1628_upsOutputTable_scalar , (struct mib_node*)&rfc1628_upsOutputTable_scalar,
	(struct mib_node*)&rfc1628_upsOutputTable_scalar
};
const struct mib_array_node  upsOutputEntry= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	rfc1628_upsOutputEntry_ids,
	rfc1628_upsOutputEntry_nodes
};

//--------------------upsOutput .1.3.6.1.33.1.4.1.1
const s32_t rfc1628_upsOutputTable_ids = 1;
struct mib_node* const rfc1628_upsOutputTable_nodes =
(struct mib_node*)&upsOutputEntry;

const struct mib_array_node  upsOutputTable= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_RA,
	1,
	&rfc1628_upsOutputTable_ids,
	&rfc1628_upsOutputTable_nodes
};


//--------------------upsOutput .1.3.6.1.33.1.4.1~4
//  upsOutput get and set
static void ups_get_upsOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsOutput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsOutput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsOutput_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsOutput_scalar = {
	&ups_get_upsOutput_object_def,
	&ups_get_upsOutput_value,
	&ups_set_upsOutput_test,
	&ups_set_upsOutput_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2: case 3:
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
static void ups_get_upsOutput_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://
		*uint_ptr = 0;
		break;
		case 2: //output frequency
		*uint_ptr = (*(pData+49))/10;
		break;
		case 3: //output phase
		*uint_ptr = *(pData+6);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsOutput_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsOutput_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


const s32_t rfc1628_upsOutput_ids[4] = {
	1,  2,  3 , 4
};
struct mib_node* const rfc1628_upsOutput_nodes[4] = {
	(struct mib_node*)&rfc1628_upsOutput_scalar,
	(struct mib_node*)&rfc1628_upsOutput_scalar,
	(struct mib_node*)&rfc1628_upsOutput_scalar,
	(struct mib_node*)&upsOutputTable
};

const struct mib_array_node  upsOutput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	rfc1628_upsOutput_ids,
	rfc1628_upsOutput_nodes
};
//--------------------upsBypassEntry .1.3.6.1.33.1.5.3.1.1~4
//--------------------upsBypassEntry
static void ups_get_upsBypassTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBypassTable_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBypassTable_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBypassTable_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsBypassTable_scalar = {
	&ups_get_upsBypassTable_object_def,
	&ups_get_upsBypassTable_value,
	&ups_set_upsBypassTable_test,
	&ups_set_upsBypassTable_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBypassTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2: case 3: case 4:
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

static void ups_get_upsBypassTable_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue;

	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:// upsBypass line identifier
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 2: //RMS Voltage
		lValue = *(pData+23);
		*uint_ptr =(u32_t)lValue;
		break;
		case 3: // RMS  current
		lValue = *(pData+26);
		*uint_ptr =(u32_t)lValue;
		break;
		case 4: // watt
		lValue = (*(pData+51))/10;
		*uint_ptr =(u32_t)lValue;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBypassTable_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsBypassTable_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


const s32_t rfc1628_upsBypassEntry_ids[4] = {
	1,  2,  3, 4
};
struct mib_node* const rfc1628_upsBypassEntry_nodes[4] = {
	(struct mib_node*)&rfc1628_upsBypassTable_scalar ,
	(struct mib_node*)&rfc1628_upsBypassTable_scalar,
	(struct mib_node*)&rfc1628_upsBypassTable_scalar ,
	(struct mib_node*)&rfc1628_upsBypassTable_scalar
};
const struct mib_array_node  upsBypassEntry= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	rfc1628_upsBypassEntry_ids,
	rfc1628_upsBypassEntry_nodes
};


//--------------------upsBypassTable .1.3.6.1.33.1.5.3.1

const s32_t rfc1628_upsBypassTable_ids = 1;
struct mib_node* const rfc1628_upsBypassTable_nodes =
(struct mib_node*)&upsBypassEntry;

const struct mib_array_node  upsBypassTable= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_RA,
	1,
	&rfc1628_upsBypassTable_ids,
	&rfc1628_upsBypassTable_nodes
};


//--------------------upsBypass .1.3.6.1.33.1.5.1~3
//  upsBypass get and set
static void ups_get_upsBypass_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBypass_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBypass_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBypass_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsBypass_scalar = {
	&ups_get_upsBypass_object_def,
	&ups_get_upsBypass_value,
	&ups_set_upsBypass_test,
	&ups_set_upsBypass_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBypass_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2:
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
static void ups_get_upsBypass_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:// frequency
		*uint_ptr = (*(pData+29))/10;
		break;
		case 2: // number of line , so use input line number
		*uint_ptr = *(pData+4);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBypass_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsBypass_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


const s32_t rfc1628_upsBypass_ids[3] = {
	1,  2,  3
};
struct mib_node* const rfc1628_upsBypass_nodes[3] = {
	(struct mib_node*)&rfc1628_upsBypass_scalar ,
	(struct mib_node*)&rfc1628_upsBypass_scalar ,
	(struct mib_node*)&upsBypassTable
};

const struct mib_array_node  upsBypass= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	rfc1628_upsBypass_ids,
	rfc1628_upsBypass_nodes
};

//--------------------upsAlarmEntry .1.3.6.1.33.1.6.2.1.1~3
const s32_t rfc1628_upsAlarmEntry_ids[3] = {
	1,  2,  3
};
struct mib_node* const rfc1628_upsAlarmEntry_nodes[3] = {
	(struct mib_node*)&rfc1628_ups_scalar ,
	(struct mib_node*)&rfc1628_ups_scalar,
	(struct mib_node*)&rfc1628_ups_scalar
};
const struct mib_array_node  upsAlarmEntry= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	rfc1628_upsAlarmEntry_ids,
	rfc1628_upsAlarmEntry_nodes
};


//--------------------upsAlarmTable .1.3.6.1.33.1.6.2.1
const s32_t rfc1628_upsAlarmTable_ids = 1;
struct mib_node* const rfc1628_upsAlarmTable_nodes =
(struct mib_node*)&upsAlarmEntry;

const struct mib_array_node  rfc1628_upsAlarmTable= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_RA,
	1,
	&rfc1628_upsAlarmTable_ids,
	&rfc1628_upsAlarmTable_nodes
};


//--------------------upsWellKnownAlarms .1.3.6.1.33.1.6.3.1~24
//  upsWellKnownAlarms get and set
static void ups_get_upsWellKnownAlarms_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsWellKnownAlarms_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsWellKnownAlarms_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsWellKnownAlarms_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsWellKnownAlarms_scalar = {
	&ups_get_upsWellKnownAlarms_object_def,
	&ups_get_upsWellKnownAlarms_value,
	&ups_set_upsWellKnownAlarms_test,
	&ups_set_upsWellKnownAlarms_value,
	MIB_NODE_SC,
	0
};


static void ups_get_upsWellKnownAlarms_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		if( id > 0 && id < 25)
		{
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
			od->v_len = sizeof(u32_t);
		}
		else{
			od->instance = MIB_OBJECT_NONE;
		}
	}
}

static void ups_get_upsWellKnownAlarms_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue;

	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	lValue = 0;
	if( id > 0 && id < 25)
	{
		*uint_ptr = *(pData+0);
	}
	else{
		*uint_ptr = *(pData+0);
	}
}

static u8_t ups_set_upsWellKnownAlarms_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	return set_ok;
}


static void ups_set_upsWellKnownAlarms_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
}


const s32_t rfc1628_upsWellKnownAlarms_ids[24] = {
	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,
	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,
	21,	22,	23,	24
};
struct mib_node* const rfc1628_upsWellKnownAlarms_nodes[24] = {
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar, (struct mib_node*)&rfc1628_upsWellKnownAlarms_scalar,
};

const struct mib_array_node  upsWellKnownAlarms= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	24,
	rfc1628_upsWellKnownAlarms_ids,
	rfc1628_upsWellKnownAlarms_nodes
};


//--------------------upsAlarm .1.3.6.1.33.1.6.1~3
const s32_t rfc1628_upsAlarm_ids[3] = {
	1,  2,  3
};
struct mib_node* const rfc1628_upsAlarm_nodes[3] = {
	(struct mib_node*)&rfc1628_ups_scalar,
	(struct mib_node*)&rfc1628_upsAlarmTable,
	(struct mib_node*)&upsWellKnownAlarms
};

const struct mib_array_node  upsAlarm= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	rfc1628_upsAlarm_ids,
	rfc1628_upsAlarm_nodes
};

//--------------------upsWellKnownTest .1.3.6.1.33.1.6.3.1~24
//--------------------upsOutputEntry
static void ups_get_upsWellKnownTest_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsWellKnownTest_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsWellKnownTest_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsWellKnownTest_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsWellKnownTest_scalar = {
	&ups_get_upsWellKnownTest_object_def,
	&ups_get_upsWellKnownTest_value,
	&ups_set_upsWellKnownTest_test,
	&ups_set_upsWellKnownTest_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsWellKnownTest_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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

static void ups_get_upsWellKnownTest_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	uint16_t lValue;

	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:// upsoutput line identifier
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 2: //RMS Voltage
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 3: // RMS  current
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 4: // watt
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		case 5: //
		lValue = 0;
		*uint_ptr =(u32_t)lValue;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsWellKnownTest_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsWellKnownTest_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


const s32_t rfc1628_upsWellKnownTest_ids[5] = {
	1,2,3,4,5
};
struct mib_node* const rfc1628_upsWellKnownTest_nodes[5] = {
	(struct mib_node*)&rfc1628_upsWellKnownTest_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownTest_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownTest_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownTest_scalar,
	(struct mib_node*)&rfc1628_upsWellKnownTest_scalar
};

const struct mib_array_node  upsWellKnownTest= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	rfc1628_upsWellKnownTest_ids,
	rfc1628_upsWellKnownTest_nodes
};


//--------------------upsTest .1.3.6.1.33.1.7.1~7
//  upsTest get and set
static void ups_get_upsTest_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsTest_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsTest_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsTest_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsTest_scalar = {
	&ups_get_upsTest_object_def,
	&ups_get_upsTest_value,
	&ups_set_upsTest_test,
	&ups_set_upsTest_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsTest_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
			od->v_len = sizeof(u32_t);
			break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
			od->v_len = sizeof(u32_t);
			default:
			od->instance = MIB_OBJECT_NONE;
			break;
		}
	}
}
static void ups_get_upsTest_value(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		*uint_ptr = 0;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsTest_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsTest_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};

const s32_t rfc1628_upsTest_ids[7] = {
	1,  2,  3 ,4,5,6,7
};
struct mib_node* const rfc1628_upsTest_nodes[7] = {
	(struct mib_node*)&rfc1628_upsTest_scalar, (struct mib_node*)&rfc1628_upsTest_scalar,
	(struct mib_node*)&rfc1628_upsTest_scalar, (struct mib_node*)&rfc1628_upsTest_scalar,
	(struct mib_node*)&rfc1628_upsTest_scalar, (struct mib_node*)&rfc1628_upsTest_scalar,
	(struct mib_node*)&upsWellKnownTest
};

const struct mib_array_node  upsTest= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	7,
	rfc1628_upsTest_ids,
	rfc1628_upsTest_nodes
};

//--------------------upsControl .1.3.6.1.33.1.8.1~5
//  upsControl get and set
static void ups_get_upsControl_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsControl_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsControl_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsControl_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsControl_scalar = {
	&ups_get_upsControl_object_def,
	&ups_get_upsControl_value,
	&ups_set_upsControl_test,
	&ups_set_upsControl_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsControl_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
			od->v_len = sizeof(u32_t);
			break;
			default:
			od->instance = MIB_OBJECT_NONE;
			break;
		}
	}
}
static void ups_get_upsControl_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:
		*uint_ptr = 0;
		break;
		case 2:
		*uint_ptr = 0;
		break;
		case 3:
		*uint_ptr = 0;
		break;
		case 4:
		*uint_ptr = 0;
		break;
		case 5:
		*uint_ptr = 0;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsControl_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(len);
	set_ok = 0;
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	switch(id){
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		set_ok = 1;
		break;
		default:
		break;
	}
	return set_ok ;
};
static void ups_set_upsControl_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:
		break;
		case 2:
		break;
		case 3:
		break;
		case 4:
		break;
		case 5:
		break;
		default:
		break;
	}
};


const s32_t rfc1628_upsControl_ids[5] = {
	1,  2,  3 ,4,5
};
struct mib_node* const rfc1628_upsControl_nodes[5] = {
	(struct mib_node*)&rfc1628_upsControl_scalar, (struct mib_node*)&rfc1628_upsControl_scalar,
	(struct mib_node*)&rfc1628_upsControl_scalar, (struct mib_node*)&rfc1628_upsControl_scalar,
	(struct mib_node*)&rfc1628_upsControl_scalar
};


const struct mib_array_node  upsControl= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	rfc1628_upsControl_ids,
	rfc1628_upsControl_nodes
};

//--------------------upsConfig .1.3.6.1.33.1.9.1~5
//  upsConfig get and set
static void ups_get_upsConfig_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsConfig_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsConfig_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsConfig_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsConfig_scalar = {
	&ups_get_upsConfig_object_def,
	&ups_get_upsConfig_value,
	&ups_set_upsConfig_test,
	&ups_set_upsConfig_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsConfig_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 2:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 3:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 4:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 5:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 6:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 7:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 8:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 9:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			case 10:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
			od->v_len = sizeof(u32_t);
			break;
			default:
			od->instance = MIB_OBJECT_NONE;
			break;
		}
	}
}
static void ups_get_upsConfig_value(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	
	id = (u8_t)od->id_inst_ptr[0];
	flash_read_ups_info(&ups_info);

	switch(id){
		case 1: //nominal input voltage
		*uint_ptr = ups_info.input_voltage;
		break;
		case 2: //input_frequency
		*uint_ptr = ups_info.input_frequency;
		break;
		case 3: //output voltage
		*uint_ptr = ups_info.out_voltage;
		break;
		case 4://output_frequency
		*uint_ptr = ups_info.output_frequency;
		break;
		case 5:// capacity
		*uint_ptr = ups_info.capacity;
		break;
		case 6: // watt
		*uint_ptr = ups_info.capacity;
		break;
		case 7:
		//ups_info.capacity=(u16_t*)value;
		break;
		case 8: // alarm audible
		*uint_ptr = ups_info.alarm_audible;
		break;
		case 9:// minimum input voltage
		*uint_ptr = ups_info.inputMin_voltage;
		break;
		case 10://maximum input voltage
		*uint_ptr = ups_info.inputMax_voltage;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsConfig_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(len);
	set_ok = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	switch(id){
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		set_ok = 1;
		break;
		default:
		break;
	}
	return set_ok ;
};
static void ups_set_upsConfig_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
	u32_t *uint_ptr = (u32_t*)value;
	//ups_info_t ups_info;
	flash_read_ups_info(&ups_info);
	switch(id){
		case 1: //nominal input voltage
		ups_info.input_voltage=(u16_t)*uint_ptr;//
		flash_write_ups_info(&ups_info);
		break;
		case 2: //input_frequency
		ups_info.input_frequency=(u16_t)*uint_ptr;//(u16_t*)value;
		flash_write_ups_info(&ups_info);
		break;
		case 3: //output voltage
		ups_info.out_voltage=(u16_t)*uint_ptr;//(u16_t*)value;
		flash_write_ups_info(&ups_info);
		break;
		case 4://output_frequency
		ups_info.output_frequency=(u16_t)*uint_ptr;//(u16_t*)value;
		flash_write_ups_info(&ups_info);
		break;
		case 5:// capacity
		ups_info.capacity=(u16_t)*uint_ptr;//(u16_t*)value;
		flash_write_ups_info(&ups_info);
		break;
		case 6: // watt
		ups_info.capacity=(u16_t)*uint_ptr;//(u16_t*)value;
		flash_write_ups_info(&ups_info);
		break;
		case 7:
		//ups_info.capacity=(u16_t)*uint_ptr;//(u16_t*)value;
		//flash_write_ups_info(&ups_info);
		break;
		case 8: // alarm audible
		ups_info.alarm_audible=(u16_t)*uint_ptr;//value;
		flash_write_ups_info(&ups_info);
		break;
		case 9:// minimum input voltage
		ups_info.inputMin_voltage=(u16_t)*uint_ptr;//(u16_t*)value;
		flash_write_ups_info(&ups_info);
		break;
		case 10://maximum input voltage
		ups_info.inputMax_voltage=(u16_t)*uint_ptr;//(u16_t*)value;
		flash_write_ups_info(&ups_info);
		break;
		default:
		break;

	}
};


const s32_t rfc1628_upsConfig_ids[10] = {
	1,  2,  3 ,4,5,6,7,8,9,10
};
struct mib_node* const rfc1628_upsConfig_nodes[10] = {
	(struct mib_node*)&rfc1628_upsConfig_scalar, (struct mib_node*)&rfc1628_upsConfig_scalar,
	(struct mib_node*)&rfc1628_upsConfig_scalar, (struct mib_node*)&rfc1628_upsConfig_scalar,
	(struct mib_node*)&rfc1628_upsConfig_scalar, (struct mib_node*)&rfc1628_upsConfig_scalar,
	(struct mib_node*)&rfc1628_upsConfig_scalar, (struct mib_node*)&rfc1628_upsConfig_scalar,
	(struct mib_node*)&rfc1628_upsConfig_scalar, (struct mib_node*)&rfc1628_upsConfig_scalar
};

const struct mib_array_node  upsConfig= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	10,
	rfc1628_upsConfig_ids,
	rfc1628_upsConfig_nodes
};


//(struct mib_node*)&upsAlarm,
const s32_t rfc1628_upsObjects_ids[9] = { 1,2,3,4,5,6,7,8,9};
struct mib_node* const rfc1628_upsObjects_nodes[9] = {
	(struct mib_node*)&upsIdent,
	(struct mib_node*)&upsBattery,
	(struct mib_node*)&upsInput,
	(struct mib_node*)&upsOutput,
	(struct mib_node*)&upsBypass,
	(struct mib_node*)&upsTest,
	(struct mib_node*)&upsTest,
	(struct mib_node*)&upsControl,
	(struct mib_node*)&upsConfig
};
const struct mib_array_node rfc1628_upsObjects = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	9,  // 1개
	rfc1628_upsObjects_ids,
	rfc1628_upsObjects_nodes
};


/* .1.3.6.1.2.1.34.1 */
const s32_t rfc1628_upsMIB_ids[1] = { 1};
struct mib_node* const rfc1628_upsMIB_nodes[1] = {
	(struct mib_node*)&rfc1628_upsObjects
};

const struct mib_array_node rfc1628_upsMIB = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 1개
	rfc1628_upsMIB_ids,
	rfc1628_upsMIB_nodes
};
///////////////////////////////////////////

/* snmp .1.3.6.1.2.1.11 */
const mib_scalar_node ups_scalar = {
	&ups_get_object_def,
	&ups_get_value,
	&ups_set_test,
	&ups_set_value,
	MIB_NODE_SC,
	0
};




/* .1.3.6.1.2.1.32.1.1   1~60*/
const s32_t ups_ids[61] = {
	1,  2,  3,  4,  5,  6,  7, 8,   9, 10, 11, 12, 13, 14, 15,
	16,	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
	31, 32, 33, 34, 35,	36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
	46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,	57,	58,	59,	60,61
};
struct mib_node* const ups_nodes[61] = {
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,	/* 1 2 */
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,	/* 11 12 */
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,	/* 21 22 */
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,

	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,	/* 31 32 */
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,

	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,	/* 41 42 */
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,

	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,	/* 51 52 */
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar, (struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar,(struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar,(struct mib_node*)&ups_scalar,
	(struct mib_node*)&ups_scalar,
};

static void ups_get_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
	if( id >=1  && id <=59 )
	{
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_WRITE;
        //od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(u32_t);
	}
	else if( id == 60 )
	{
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
		//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG, (" sizeof(&upsModeBusData)=%d\n ",sizeof(&upsModeBusData)) );
        od->v_len = 120;
	}
	else if( id == 61 )
	{
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        //od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
		//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG, (" sizeof(&upsModeBusData)=%d\n ",sizeof(&upsModeBusData)) );
        od->v_len = sizeof(u32_t);
	}
	else
	{
        od->instance = MIB_OBJECT_NONE;
	}
    /*
	switch (id)
    {
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
        od->v_len = sizeof(u32_t);
        break;
      case 30: // snmpEnableAuthenTraps 
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
      default:
        LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("snmp_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
	*/
  }
  else
  {
    LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("snmp_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}



// .1.3.6.1.2.1.33.1.1.1.0 ~ 57까지의 데이타를 채운다.


static void ups_get_value(struct obj_def *od, u16_t len, void *value)
{

  u32_t *uint_ptr = (u32_t*)value;
  u8_t id=0;
  uint16_t * pData =(uint16_t *)&upsModeBusData ;	
 
	while( isModebusRunning)
	{
		vTaskDelay(100);
		if(id>20)break;
		id++;
	};
  
  LWIP_UNUSED_ARG(len);
  LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
  id = (u8_t)od->id_inst_ptr[0];
  
  
  if( id > 0 && id <60)
  {
	  // 아이디와 UPS-MIB를 맞추어 준다..나중에
	  uint16_t lValue=*(pData+id-1);
	  *uint_ptr =(u32_t)lValue;  // id 는 1부터 시작하고배열은 0부터 시작한다.
  }
  if( id == 60) //전체데이타를 한번에 준다.
  {
	  int16_t ups_modbus_data_len=120;
	  //LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG, (" sizeof(&upsModeBusData)=%d\n ",sizeof(&upsModeBusData)) );
	  ocstrncpy((u8_t*)value,(u8_t*)&upsModeBusData, ups_modbus_data_len);
  }
  if( id == 61) 
	  *uint_ptr =0;
}



/**
 * Test snmp object value before setting.
 *
 * @param od is the object definition
 * @param len return value space (in bytes)
 * @param value points to (varbind) space to copy value from.
 */
static u8_t ups_set_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;

	LWIP_UNUSED_ARG(len);
	set_ok = 0;
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
    LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	if (id>=1 && id <= 61)
	{
		/* snmpEnableAuthenTraps */
		s32_t *sint_ptr = (s32_t*)value;
		set_ok = 1;
		/*
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
		*/
	}
	return set_ok;
}


static void ups_set_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;

	LWIP_UNUSED_ARG(len);
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];
	uint16_t * pData =(uint16_t *)&upsModeBusData ;	
	u32_t *ptr = (u32_t*)value;
	if (id >=1  && id <= 59)
	{
		flash_read_ups_info(&ups_info);
		switch(id){
			case 1:
				if((u16_t)*ptr < 2999 ){
					ups_info.install_year=(u16_t)*ptr;
					*(pData+id-1) = (u16_t)*ptr;
				}
				break;
			case 2:
				if((u16_t)*ptr <= 1 && (u16_t)*ptr >= 12  ){
					ups_info.install_month =(u16_t)*ptr;
					*(pData+id-1) = (u16_t)*ptr;
				}
				break;
			case 3:
				if((u16_t)*ptr <= 1 && (u16_t)*ptr >= 31  ){
					ups_info.install_day=(u16_t)*ptr;
					*(pData+id-1) = (u16_t)*ptr;
				}
				break;
			case 4:
				ups_info.capacity=(u16_t)*ptr;
				*(pData+id-1) = (u16_t)*ptr;
				break;
			case 5:
				ups_info.input_frequency=(u16_t)*ptr;
				*(pData+id-1) = (u16_t)*ptr;
				break;
			case 6:
				ups_info.input_voltage=(u16_t)*ptr;
				*(pData+id-1) = (u16_t)*ptr;
				break;
			case 7:
				ups_info.output_frequency=(u16_t)*ptr;
				*(pData+id-1) = (u16_t)*ptr;
				break;
			case 8:
				ups_info.out_voltage=(u16_t)*ptr;
				*(pData+id-1) = (u16_t)*ptr;
				break;
			case 9:
				ups_info.company_code=(u16_t)*ptr;
				*(pData+id-1) = (u16_t)*ptr;
				break;
			case 10:
				ups_info.installed_battery=(u16_t)*ptr;
				*(pData+id-1) = (u16_t)*ptr;
				break;
			default:
				break;
		}
		flash_write_ups_info(&ups_info);
		//*snmpenableauthentraps_ptr = *ptr;
	}
	/*
	if (id == 30)
	{
		u8_t *ptr = (u8_t*)value;
		*snmpenableauthentraps_ptr = *ptr;
	}
	*/
	if (id == 61)
	{
		if( (u16_t)*ptr == 1) 
		{
			while(1);
		}
	}
}


#endif /* MIB_RFC1268_H_ */