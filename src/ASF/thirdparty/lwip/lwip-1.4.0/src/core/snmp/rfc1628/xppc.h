/*
 * xppc.h
 *
 * Created: 2021-08-25 오후 2:44:06
 *  Author: STELLA
 */ 

#ifndef XPPC_H_
#define XPPC_H_

static void ups_get_upsBaseIdent_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseIdent_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseIdent_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseIdent_value(struct obj_def *od, u16_t len, void *value);

static void ups_get_upsSmartIdent_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartIdent_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartIdent_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartIdent_value(struct obj_def *od, u16_t len, void *value);

static u8_t ups_set_upsBaseIdent_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
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
static u8_t ups_set_upsSmartIdent_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	switch (id)
	{
		case 1: case 2: case 3: case 4: 
		set_ok = 0;
		break;
	};
	return set_ok;
}


static void ups_set_upsBaseIdent_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	//ups_info_t ups_info;
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
static void ups_set_upsSmartIdent_value(struct obj_def *od, u16_t len, void *value)
{
	return;	
}

static void ups_get_upsBaseIdent_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
		//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("get_object_def system.%"U16_F".0\n",(u16_t)id));

		//ups_info_t ups_info;
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
			//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("system_get_object_def: no such object\n"));
			od->instance = MIB_OBJECT_NONE;
			break;
		}
	}
	else
	{
		//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("system_get_object_def: no scalar\n"));
		od->instance = MIB_OBJECT_NONE;
	};
	if(iCommErrorCount > 0 )
	od->instance = MIB_OBJECT_NONE;
}

static void ups_get_upsSmartIdent_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
		//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("get_object_def system.%"U16_F".0\n",(u16_t)id));

		//ups_info_t ups_info;
		switch (id)
		{
			case 1: /* upsIdentUPSSoftwareVersion */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentUPSSoftwareVersion) ;
			break;
			case 2: /* upsIdentManufacturer */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentAgentSoftwareVersion) ;
			break;
			case 3: /* upsIdentName */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentName) ;
			break;
			case 4: /* sysLocation */
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_WRITE;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.upsIdentAttachedDevices) ;
			break;
			default:
				od->instance = MIB_OBJECT_NONE;
				break;
		}
	}
	else
	{
		//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("system_get_object_def: no scalar\n"));
		od->instance = MIB_OBJECT_NONE;
	};
	if(iCommErrorCount > 0 )
	od->instance = MIB_OBJECT_NONE;
}
static void ups_get_upsBaseIdent_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;

	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];

	//ups_info_t ups_info;
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

static void ups_get_upsSmartIdent_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;

	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	id = (u8_t)od->id_inst_ptr[0];

	//ups_info_t ups_info;
	switch (id)
	{
		case 1: /* upsIdentUPSSoftwareVersion */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentUPSSoftwareVersion[0], len);
		break;
		case 2: /* upsIdentAgentSoftwareVersion */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentAgentSoftwareVersion[0], len);
		break;
		case 3: /* upsIdentName */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentName[0], len);
		break;
		case 4: /* upsIdentAttachedDevices */
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.upsIdentAttachedDevices[0], len);
		break;
	};
}

const mib_scalar_node xppcups_scalar = {
	&ups_get_object_def,
	&ups_get_value,
	&ups_set_test,
	&ups_set_value,
	MIB_NODE_SC,
	0
};

// 하나씩 이 부분은 바꿔준다. 임시로 설정 되어 있다
const s32_t xppc_ups_ids[4] = {
	1,  2,  3,  4
};
struct mib_node* const xppc_ups_nodes[4] = {
	(struct mib_node*)&xppcups_scalar , (struct mib_node*)&xppcups_scalar,
	(struct mib_node*)&xppcups_scalar, (struct mib_node*)&xppcups_scalar
};

//ups base indet scalar
// 1: upsIdentManufacturer
// 2:upsIdentModel
// 3:upsIdentUPSSoftwareVersion
// 4:upsAgentSoftwareVersion
// 5:upsIdentName
// 6:upsIdentAttachedDevices
//
const mib_scalar_node xppc_upsBaseIdent_scalar = {
	&ups_get_upsBaseIdent_object_def,
	&ups_get_upsBaseIdent_value,
	&ups_set_upsBaseIdent_test,
	&ups_set_upsBaseIdent_value,
	MIB_NODE_SC,
	0
};
//
const mib_scalar_node xppc_upsSmartIdent_scalar = {
	&ups_get_upsSmartIdent_object_def,
	&ups_get_upsSmartIdent_value,
	&ups_set_upsSmartIdent_test,
	&ups_set_upsSmartIdent_value,
	MIB_NODE_SC,
	0
};

const s32_t xppc_upsBaseIdent_ids[6] = {
	1,  2,  3,  4  ,5 ,6
};
struct mib_node* const xppc_upsBaseIdent_nodes[6] = {
	(struct mib_node*)&xppc_upsBaseIdent_scalar ,
	(struct mib_node*)&xppc_upsBaseIdent_scalar ,
	(struct mib_node*)&xppc_upsBaseIdent_scalar ,
	(struct mib_node*)&xppc_upsBaseIdent_scalar ,
	(struct mib_node*)&xppc_upsBaseIdent_scalar ,
	(struct mib_node*)&xppc_upsBaseIdent_scalar
};
//.iso.org.dod.internet.private.enterprises.ppc.products.hardware.ups.upsBaseIdentp 
const struct mib_array_node  upsBaseIdent = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	6,
	xppc_upsBaseIdent_ids,
	xppc_upsBaseIdent_nodes
};

const s32_t xppc_upsSmartIdent_ids[4] = {
	1,  2,  3,  4  
};
struct mib_node* const xppc_upsSmartIdent_nodes[4] = {
	(struct mib_node*)&xppc_upsSmartIdent_scalar ,
	(struct mib_node*)&xppc_upsSmartIdent_scalar ,
	(struct mib_node*)&xppc_upsSmartIdent_scalar ,
	(struct mib_node*)&xppc_upsSmartIdent_scalar ,
};
//.iso.org.dod.internet.private.enterprises.ppc.products.hardware.ups.upsSmartIdentp 
const struct mib_array_node  upsSmartIdent = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	xppc_upsSmartIdent_ids,
	xppc_upsSmartIdent_nodes
};


//  upsBattery get and set
static void ups_get_upsBaseBattery_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseBattery_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseBattery_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseBattery_value(struct obj_def *od, u16_t len, void *value);

static void ups_get_upsSmartBattery_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartBattery_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartBattery_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartBattery_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node xppc_upsBaseBattery_scalar = {
	&ups_get_upsBaseBattery_object_def,
	&ups_get_upsBaseBattery_value,
	&ups_set_upsBaseBattery_test,
	&ups_set_upsBaseBattery_value,
	MIB_NODE_SC,
	0
};
const mib_scalar_node xppc_upsSmartBattery_scalar = {
	&ups_get_upsSmartBattery_object_def,
	&ups_get_upsSmartBattery_value,
	&ups_set_upsSmartBattery_test,
	&ups_set_upsSmartBattery_value,
	MIB_NODE_SC,
	0
};


static void ups_get_upsBaseBattery_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id)
		{
			case 1: case 2: 
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
			od->v_len = sizeof(u32_t);
			break;
			case 3:
			od->instance = MIB_OBJECT_SCALAR;
			od->access = MIB_OBJECT_READ_ONLY;
			od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
			od->v_len = sizeof(ups_info.batLastReplaceDate) ;
			// ocstrncpy((u8_t*)value, (u8_t*)&ups_info.batLastReplaceDate[0], len);
			case 4: case 5: case 6: case 7:
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
static void ups_get_upsSmartBattery_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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

extern xppc_data_t	xppc_data;
static void ups_get_upsBaseBattery_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;

	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	u8_t istate;
	switch(id){
		case 1://unknown(1), batteryNormal(2), batteryLow(3) 
			xppc_data.upsBaseBatteryStatus = 2;
			*uint_ptr  = xppc_data.upsBaseBatteryStatus;
		break;
		case 2: 
		*uint_ptr = 0;
		break;
		case 3: //upsEstimateMinute
		ocstrncpy((u8_t*)value, (u8_t*)&ups_info.batLastReplaceDate[0], len);
		break;
		case 4:// 
		*uint_ptr = 0;
		break;
		case 5: //
		*uint_ptr = *(pData+37);
		break;
		case 6: //
		*uint_ptr = *(pData+38);
		break;
		case 7:
		*uint_ptr = *(pData+58);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static void ups_get_upsSmartBattery_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;

	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	u8_t istate;
	switch(id){
		case 1:// 
		// The remaining battery capacity expressed in percent of full capacity.
			*uint_ptr = (u32_t)xppc_data.upsSmartBatteryCapacity;
		break;
		case 2: 
			*uint_ptr = (u32_t)xppc_data.upsSmartBatteryVoltage;
		break;
		case 3: //upsEstimateMinute
			*uint_ptr = (u32_t)xppc_data.upsSmartBatteryTemperature;
		break;
		case 4:// 
			*uint_ptr = (u32_t)xppc_data.upsSmartBatteryRunTimeRemaining;
		break;
		case 5: // 
		*uint_ptr =  1;
		break;
		case 6: // current
			*uint_ptr = (u32_t)xppc_data.upsSmartBatteryFullChargeVoltage;
		break;
		case 7: // ups battery temperature
			*uint_ptr = (u32_t)xppc_data.upsSmartBatteryCurrent;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBaseBattery_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	return set_ok;
}

static u8_t ups_set_upsSmartBattery_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(value);
	set_ok = 0; id = 0;
	id = (u8_t)od->id_inst_ptr[0];
	//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
	return set_ok;
}

static void ups_set_upsBaseBattery_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
}
static void ups_set_upsSmartBattery_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
}


const s32_t xppc_upsBaseBattery_ids[7] = {
	1,  2,  3,  4  , 5, 6, 7
};
struct mib_node* const xppc_upsBaseBattery_nodes[7] = {
	(struct mib_node*)&xppc_upsBaseBattery_scalar , (struct mib_node*)&xppc_upsBaseBattery_scalar,
	(struct mib_node*)&xppc_upsBaseBattery_scalar, (struct mib_node*)&xppc_upsBaseBattery_scalar,
	(struct mib_node*)&xppc_upsBaseBattery_scalar, (struct mib_node*)&xppc_upsBaseBattery_scalar,
	(struct mib_node*)&xppc_upsBaseBattery_scalar
};
const struct mib_array_node  upsBaseBattery= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	7,
	xppc_upsBaseBattery_ids,
	xppc_upsBaseBattery_nodes
};
const s32_t xppc_upsSmartBattery_ids[7] = {
	1,  2,  3,  4  , 5, 6, 7
};
struct mib_node* const xppc_upsSmartBattery_nodes[7] = {
	(struct mib_node*)&xppc_upsSmartBattery_scalar , (struct mib_node*)&xppc_upsSmartBattery_scalar,
	(struct mib_node*)&xppc_upsSmartBattery_scalar, (struct mib_node*)&xppc_upsSmartBattery_scalar,
	(struct mib_node*)&xppc_upsSmartBattery_scalar, (struct mib_node*)&xppc_upsSmartBattery_scalar,
	(struct mib_node*)&xppc_upsSmartBattery_scalar
};
const struct mib_array_node  upsSmartBattery= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	7,
	xppc_upsSmartBattery_ids,
	xppc_upsSmartBattery_nodes
};



const s32_t xppc_upsBatteryp_ids[2] = {
	1,  2
};
struct mib_node* const xppc_upsBatteryp_nodes[2] = {
	(struct mib_node*)&upsBaseBattery, (struct mib_node*)&upsSmartBattery
};


const struct mib_array_node  upsBatteryp= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	xppc_upsBatteryp_ids,
	xppc_upsBatteryp_nodes
};

//----upsBaseInput------------------
//  upsInput get and set
static void ups_get_upsBaseInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseInput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseInput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseInput_value(struct obj_def *od, u16_t len, void *value);


const mib_scalar_node xppc_upsBaseInput_scalar = {
	&ups_get_upsBaseInput_object_def,
	&ups_get_upsBaseInput_value,
	&ups_set_upsBaseInput_test,
	&ups_set_upsBaseInput_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBaseInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2:case 3:
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
static void ups_get_upsBaseInput_value(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1:// utility line fail count
		*uint_ptr = upsModeBusData.Input_Phase;
		break;
		case 2: // input phase
		*uint_ptr = *(pData+4);
		break;
		case 3: // input phase
		*uint_ptr = *(pData+4);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBaseInput_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsBaseInput_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


const s32_t xppc_upsBaseInput_ids[3] = {
	1,  2,  3
};
struct mib_node* const xppc_upsBaseInput_nodes[3] = {
	(struct mib_node*)&xppc_upsBaseInput_scalar , 
	(struct mib_node*)&xppc_upsBaseInput_scalar,
	(struct mib_node*)&xppc_upsBaseInput_scalar
};

const struct mib_array_node  upsBaseInput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	xppc_upsBaseInput_ids,
	xppc_upsBaseInput_nodes
};
//----upsBaseInput------------------

//----upsSmartInput------------------
//  upsSmartInput get and set
static void ups_get_upsSmartInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartInput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartInput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartInput_value(struct obj_def *od, u16_t len, void *value);


const mib_scalar_node xppc_upsSmartInput_scalar = {
	&ups_get_upsSmartInput_object_def,
	&ups_get_upsSmartInput_value,
	&ups_set_upsSmartInput_test,
	&ups_set_upsSmartInput_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsSmartInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsSmartInput_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://upsSmartInputLineVoltage(1)  
		*uint_ptr = upsModeBusData.Input_r_volt_rms;
		break;
		case 2: // upsSmartInputMaxLineVoltage
		*uint_ptr = upsModeBusData.Input_r_volt_rms;
		break;
		case 3: // upsSmartInputMinLineVoltage
		*uint_ptr = upsModeBusData.Input_r_volt_rms;
		break;
		case 4: // upsSmartInputFrequency
		*uint_ptr = upsModeBusData.Input_frequency;
		break;
		case 5: // upsSmartInputLineFailCause
		*uint_ptr = upsModeBusData.Converter_State;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsSmartInput_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsSmartInput_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


const s32_t xppc_upsSmartInput_ids[5] = {
	1,  2,  3,4,5
};
struct mib_node* const xppc_upsSmartInput_nodes[5] = {
	(struct mib_node*)&xppc_upsSmartInput_scalar , (struct mib_node*)&xppc_upsSmartInput_scalar,
	(struct mib_node*)&xppc_upsSmartInput_scalar , (struct mib_node*)&xppc_upsSmartInput_scalar,
	(struct mib_node*)&xppc_upsSmartInput_scalar 
};

const struct mib_array_node  upsSmartInput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	xppc_upsSmartInput_ids,
	xppc_upsSmartInput_nodes
};
//----upsSmartInput------------------

const s32_t xppc_upsInputp_ids[2] = {
	1,  2  
};


struct mib_node* const xppc_upsInputp_nodes[] = {
	(struct mib_node*)&upsBaseInput,
	(struct mib_node*)&upsSmartInput
};
const struct mib_array_node  upsInputp= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	xppc_upsInputp_ids,
	xppc_upsInputp_nodes
};



//--------------------upsOutput .1.3.6.1.33.1.4.1~4
//  upsOutput get and set
static void ups_get_upsBaseOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseOutput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseOutput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseOutput_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node xppc_upsBaseOutput_scalar = {
	&ups_get_upsBaseOutput_object_def,
	&ups_get_upsBaseOutput_value,
	&ups_set_upsBaseOutput_test,
	&ups_set_upsBaseOutput_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBaseOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2: case 3:case 4:
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
static void ups_get_upsBaseOutput_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://  upsBaseOutputStatus(1)  
		*uint_ptr =upsModeBusData.Inverter_State;
		break;
		case 2: //  upsBaseOutputPhase(2)  
		*uint_ptr = upsModeBusData.Output_frequency;
		break;
		case 3: //output phase
		*uint_ptr = *(pData+6);
		break;
		case 4: //output phase
		*uint_ptr = *(pData+6);
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBaseOutput_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsBaseOutput_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};


const s32_t xppc_upsBaseOutput_ids[4] = {
	1,  2,  3 , 4
};
struct mib_node* const xppc_upsBaseOutput_nodes[4] = {
	(struct mib_node*)&xppc_upsBaseOutput_scalar,
	(struct mib_node*)&xppc_upsBaseOutput_scalar,
	(struct mib_node*)&xppc_upsBaseOutput_scalar,
	(struct mib_node*)&xppc_upsBaseOutput_scalar
};
const struct mib_array_node  upsBaseOutput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	xppc_upsBaseOutput_ids,
	xppc_upsBaseOutput_nodes
};
////UPS_BASE_OUTPUT
////UPS_SMART_OUTPUT
static void ups_get_upsSmartOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartOutput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartOutput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartOutput_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node xppc_upsSmartOutput_scalar = {
	&ups_get_upsSmartOutput_object_def,
	&ups_get_upsSmartOutput_value,
	&ups_set_upsSmartOutput_test,
	&ups_set_upsSmartOutput_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsSmartOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsSmartOutput_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1://  upsSmartOutputVoltage(1)  
		*uint_ptr = upsModeBusData.Output_r_volt_rms;
		break;
		case 2: //upsSmartOutputFrequency(2) 
		*uint_ptr = upsModeBusData.Output_frequency;
		break;
		case 3: //  upsSmartOutputLoad(3)  
		*uint_ptr = upsModeBusData.Output_R_Load;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsSmartOutput_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsSmartOutput_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};



const s32_t xppc_upsSmartOutput_ids[3] = {
	1,  2,  3 
};
struct mib_node* const xppc_upsSmartOutput_nodes[3] = {
	(struct mib_node*)&xppc_upsSmartOutput_scalar,
	(struct mib_node*)&xppc_upsSmartOutput_scalar,
	(struct mib_node*)&xppc_upsSmartOutput_scalar
};
const struct mib_array_node  upsSmartOutput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	xppc_upsSmartOutput_ids,
	xppc_upsSmartOutput_nodes
};
////UPS_SMART_OUTPUT

const s32_t xppc_upsOutputp_ids[2] = {
	1,  2
};
struct mib_node* const xppc_upsOutputp_nodes[2] = {
	(struct mib_node*)&upsBaseOutput,
	(struct mib_node*)&upsSmartOutput
};
const struct mib_array_node  upsOutputp= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	xppc_upsOutputp_ids,
	xppc_upsOutputp_nodes
};


//  upsEnvironment get and set
static void ups_get_upsEnvironment_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsEnvironment_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsEnvironment_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsEnvironment_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node upsEnvironment_scalar = {
	&ups_get_upsEnvironment_object_def,
	&ups_get_upsEnvironment_value,
	&ups_set_upsEnvironment_test,
	&ups_set_upsEnvironment_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsEnvironment_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsEnvironment_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1: case 2: case 3: case 4:
		*uint_ptr = *(pData+4);
		break;
		default:
		*uint_ptr = *(pData+4);
		break;
	}
}

static u8_t ups_set_upsEnvironment_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsEnvironment_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};
//  xppc_upsEnvStatus get and set
static void ups_get_upsEnvStatus_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsEnvStatus_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsEnvStatus_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsEnvStatus_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node upsEnvStatus_scalar = {
	&ups_get_upsEnvStatus_object_def,
	&ups_get_upsEnvStatus_value,
	&ups_set_upsEnvStatus_test,
	&ups_set_upsEnvStatus_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsEnvStatus_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2: case 3: case 4:
			case 5: case 6: case 7: case 8:
			case 9: case 10: case 11: 
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
static void ups_get_upsEnvStatus_value(struct obj_def *od, u16_t len, void *value)
{
	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1: case 2: case 3: case 4:
		case 5: case 6: case 7: case 8:
		case 9: case 10: case 11: 
		*uint_ptr = 0;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsEnvStatus_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsEnvStatus_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};
//upsEnvStatus_ids

const s32_t upsEnvStatus_ids[11] = {
	1,  2,  3,4,5,6,7,8,9,10,11
};
struct mib_node* const upsEnvStatus_nodes[11] = {
	(struct mib_node*)&upsEnvStatus_scalar , (struct mib_node*)&upsEnvStatus_scalar ,
	(struct mib_node*)&upsEnvStatus_scalar , (struct mib_node*)&upsEnvStatus_scalar ,
	(struct mib_node*)&upsEnvStatus_scalar , (struct mib_node*)&upsEnvStatus_scalar ,
	(struct mib_node*)&upsEnvStatus_scalar , (struct mib_node*)&upsEnvStatus_scalar ,
	(struct mib_node*)&upsEnvStatus_scalar , (struct mib_node*)&upsEnvStatus_scalar ,
	(struct mib_node*)&upsEnvStatus_scalar
};

const struct mib_array_node  xppc_upsEnvStatus= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	11,
	upsEnvStatus_ids,
	upsEnvStatus_nodes
};
//-------------
//upsSetting_ids

const s32_t upsSetting_ids[4] = {
	1,  2,  3,4
};
struct mib_node* const upsSetting_nodes[4] = {
	(struct mib_node*)&upsEnvironment_scalar , (struct mib_node*)&upsEnvironment_scalar ,
	(struct mib_node*)&upsEnvironment_scalar , (struct mib_node*)&upsEnvironment_scalar
};

const struct mib_array_node  xppcupsSetting= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	upsSetting_ids,
	upsSetting_nodes
};
//-------------
const s32_t upsEnvironment_ids[2] = {
	1,  2
};
struct mib_node* const upsEnvironment_nodes[2] = {
	(struct mib_node*)&xppc_upsEnvStatus ,
	(struct mib_node*)&xppcupsSetting
};

const struct mib_array_node  upsEnvironment= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	upsEnvironment_ids,
	upsEnvironment_nodes
};
//---------------

//--------------------upsBasicTest .1.3.6.1.33.1.7.1~7
//  upsBasicTest get and set
static void ups_get_upsBasicTest_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBasicTest_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBasicTest_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBasicTest_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node upsBasicTest_scalar = {
	&ups_get_upsBasicTest_object_def,
	&ups_get_upsBasicTest_value,
	&ups_set_upsBasicTest_test,
	&ups_set_upsBasicTest_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBasicTest_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
	ident_len += 1; ident -= 1;
	if (ident_len == 2)
	{
		u8_t id; od->id_inst_len = ident_len; od->id_inst_ptr = ident;
		LWIP_ASSERT("invalid id", (ident[0] >= 0) && (ident[0] <= 0xff));
		id = (u8_t)ident[0];
		switch(id){
			case 1: case 2: case 3: case 4: 
			case 5: case 6: case 7: case 8:
			case 9: case 10: case 11: case 12:
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
static void ups_get_upsBasicTest_value(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	//uint16_t lValue=*(pData+id-1);
	id = (u8_t)od->id_inst_ptr[0];
	switch(id){
		case 1: case 2: case 3: case 4: 
		case 5: case 6: case 7: case 8:
		case 9: case 10: case 11: case 12:
		 *uint_ptr = 0;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsBasicTest_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsBasicTest_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};

const s32_t upsBasicTest_ids[12] = {
	1,  2,  3 ,4,5,6,7,8,9,10,11,12
};
struct mib_node* const upsBasicTest_nodes[12] = {
	(struct mib_node*)&upsBasicTest_scalar, (struct mib_node*)&upsBasicTest_scalar,
	(struct mib_node*)&upsBasicTest_scalar, (struct mib_node*)&upsBasicTest_scalar,
	(struct mib_node*)&upsBasicTest_scalar, (struct mib_node*)&upsBasicTest_scalar,
	(struct mib_node*)&upsBasicTest_scalar, (struct mib_node*)&upsBasicTest_scalar
};

const struct mib_array_node  upsBasicTest= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	12,
	upsBasicTest_ids,
	upsBasicTest_nodes
};
const s32_t upsBasicTestp_ids[2] = {
	1,  2
};
struct mib_node* const upsBasicTestp_nodes[2] = {
	(struct mib_node*)&upsBasicTest, 
	(struct mib_node*)&upsBasicTest
};

const struct mib_array_node  upsBasicTestp= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	upsBasicTestp_ids,
	upsBasicTestp_nodes
};

const s32_t upsThreePhasep_ids[8] = {
	1,  2,3,4,5,6,7,8,
};
struct mib_node* const upsThreePhasep_nodes[8] = {
	(struct mib_node*)&upsBasicTest, (struct mib_node*)&upsBasicTest,
	(struct mib_node*)&upsBasicTest, (struct mib_node*)&upsBasicTest,
	(struct mib_node*)&upsBasicTest, (struct mib_node*)&upsBasicTest,
	(struct mib_node*)&upsBasicTest, (struct mib_node*)&upsBasicTest
};

const struct mib_array_node  upsThreePhasep= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	8,
	upsThreePhasep_ids,
	upsThreePhasep_nodes
};
//--------------------upsControl .1.3.6.1.33.1.8.1~5
//  upsControl get and set
static void ups_get_upsBaseControl_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseControl_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseControl_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseControl_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node upsBaseControl_scalar = {
	&ups_get_upsBaseControl_object_def,
	&ups_get_upsBaseControl_value,
	&ups_set_upsBaseControl_test,
	&ups_set_upsBaseControl_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBaseControl_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsBaseControl_value(struct obj_def *od, u16_t len, void *value)
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

static u8_t ups_set_upsBaseControl_test(struct obj_def *od, u16_t len, void *value)
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
		set_ok = 1;
		break;
		default:
		break;
	}
	return set_ok ;
};
static void ups_set_upsBaseControl_value(struct obj_def *od, u16_t len, void *value)
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

//  upsControl get and set
static void ups_get_upsSmartControl_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartControl_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartControl_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartControl_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node upsSmartControl_scalar = {
	&ups_get_upsSmartControl_object_def,
	&ups_get_upsSmartControl_value,
	&ups_set_upsSmartControl_test,
	&ups_set_upsSmartControl_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsSmartControl_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsSmartControl_value(struct obj_def *od, u16_t len, void *value)
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
		case 6:
		*uint_ptr = 0;
		break;
		default:
		*uint_ptr = 0;
		break;
	}
}

static u8_t ups_set_upsSmartControl_test(struct obj_def *od, u16_t len, void *value)
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
		set_ok = 1;
		break;
		default:
		break;
	}
	return set_ok ;
};
static void ups_set_upsSmartControl_value(struct obj_def *od, u16_t len, void *value)
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

const s32_t upsBaseControl_ids[5] = {
	1,  2,  3 ,4,5
};
struct mib_node* const upsBaseControl_nodes[5] = {
	(struct mib_node*)&upsBaseControl_scalar, (struct mib_node*)&upsBaseControl_scalar,
	(struct mib_node*)&upsBaseControl_scalar, (struct mib_node*)&upsBaseControl_scalar,
	(struct mib_node*)&upsBaseControl_scalar
};


const struct mib_array_node  upsBaseControl= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	upsBaseControl_ids,
	upsBaseControl_nodes
};

const s32_t upsSmartControl_ids[6] = {
	1,  2,  3 ,4,5,6
};
struct mib_node* const upsSmartControl_nodes[6] = {
	(struct mib_node*)&upsSmartControl_scalar, (struct mib_node*)&upsSmartControl_scalar,
	(struct mib_node*)&upsSmartControl_scalar, (struct mib_node*)&upsSmartControl_scalar,
	(struct mib_node*)&upsSmartControl_scalar, (struct mib_node*)&upsSmartControl_scalar
};


const struct mib_array_node  upsSmartControl= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	6,
	upsSmartControl_ids,
	upsSmartControl_nodes
};
const s32_t xppc_upsControlp_ids[2] = {
	1,  2
};
struct mib_node* const xppc_upsControlp_nodes[2] = {
	(struct mib_node*)&upsBaseControl,
	(struct mib_node*)&upsSmartControl
};
const struct mib_array_node  upsControlp= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	xppc_upsControlp_ids,
	xppc_upsControlp_nodes
};
//--------------------upsConfig .1.3.6.1.33.1.9.1~5
//  upsBaseConfig get and set
static void ups_get_upsBaseConfig_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseConfig_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseConfig_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseConfig_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node xppc_upsBaseConfig_scalar = {
	&ups_get_upsBaseConfig_object_def,
	&ups_get_upsBaseConfig_value,
	&ups_set_upsBaseConfig_test,
	&ups_set_upsBaseConfig_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBaseConfig_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsBaseConfig_value(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	
	id = (u8_t)od->id_inst_ptr[0];

	flash_read_ups_info(&ups_info);
	switch(id){
		case 1: //nominal input voltage
		*uint_ptr =(u32_t)ups_info.input_voltage;
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

static u8_t ups_set_upsBaseConfig_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(len);
	set_ok = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	switch(id){
		case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
		set_ok = 1;
		break;
		default:
		break;
	}
	return set_ok ;
};
static void ups_set_upsBaseConfig_value(struct obj_def *od, u16_t len, void *value)
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

//  upsSmartConfig get and set
static void ups_get_upsSmartConfig_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartConfig_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartConfig_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartConfig_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node xppc_upsSmartConfig_scalar = {
	&ups_get_upsSmartConfig_object_def,
	&ups_get_upsSmartConfig_value,
	&ups_set_upsSmartConfig_test,
	&ups_set_upsSmartConfig_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsSmartConfig_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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
static void ups_get_upsSmartConfig_value(struct obj_def *od, u16_t len, void *value)
{

	u32_t *uint_ptr = (u32_t*)value;
	u8_t id;
	uint16_t * pData =(uint16_t *)&upsModeBusData ;
	
	id = (u8_t)od->id_inst_ptr[0];
	flash_read_ups_info(&ups_info);
	switch(id){
		case 1: //nominal input voltage
		*uint_ptr =(u32_t)ups_info.input_voltage;
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

static u8_t ups_set_upsSmartConfig_test(struct obj_def *od, u16_t len, void *value)
{
	u8_t id, set_ok;
	LWIP_UNUSED_ARG(len);
	set_ok = 0;
	id = (u8_t)od->id_inst_ptr[0];
	LWIP_ASSERT("invalid id", (od->id_inst_ptr[0] >= 0) && (od->id_inst_ptr[0] <= 0xff));
	switch(id){
		case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
		set_ok = 1;
		break;
		default:
		break;
	}
	return set_ok ;
};
static void ups_set_upsSmartConfig_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
	u32_t *uint_ptr = (u32_t*)value;
	//ups_info_t ups_info;
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


// upsBaseConfigp
const s32_t xppc_upsBaseConfig_ids[10] = {
	1,  2,  3 ,4,5,6,7,8,9,10
};
struct mib_node* const xppc_upsBaseConfig_nodes[10] = {
	(struct mib_node*)&xppc_upsBaseConfig_scalar, (struct mib_node*)&xppc_upsBaseConfig_scalar,
	(struct mib_node*)&xppc_upsBaseConfig_scalar, (struct mib_node*)&xppc_upsBaseConfig_scalar,
	(struct mib_node*)&xppc_upsBaseConfig_scalar, (struct mib_node*)&xppc_upsBaseConfig_scalar,
	(struct mib_node*)&xppc_upsBaseConfig_scalar, (struct mib_node*)&xppc_upsBaseConfig_scalar,
	(struct mib_node*)&xppc_upsBaseConfig_scalar, (struct mib_node*)&xppc_upsBaseConfig_scalar
};

const struct mib_array_node  upsBaseConfig= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	10,
	xppc_upsBaseConfig_ids,
	xppc_upsBaseConfig_nodes
};

// upsBaseConfigp
// upsSmartConfigp
const s32_t xppc_upsSmartConfig_ids[10] = {
	1,  2,  3 ,4,5,6,7,8,9,10
};
struct mib_node* const xppc_upsSmartConfig_nodes[10] = {
	(struct mib_node*)&xppc_upsSmartConfig_scalar, (struct mib_node*)&xppc_upsSmartConfig_scalar,
	(struct mib_node*)&xppc_upsSmartConfig_scalar, (struct mib_node*)&xppc_upsSmartConfig_scalar,
	(struct mib_node*)&xppc_upsSmartConfig_scalar, (struct mib_node*)&xppc_upsSmartConfig_scalar,
	(struct mib_node*)&xppc_upsSmartConfig_scalar, (struct mib_node*)&xppc_upsSmartConfig_scalar,
	(struct mib_node*)&xppc_upsSmartConfig_scalar, (struct mib_node*)&xppc_upsSmartConfig_scalar
};

const struct mib_array_node  upsSmartConfig= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	10,
	xppc_upsSmartConfig_ids,
	xppc_upsSmartConfig_nodes
};

// upsSmartConfigp

// upsConfigp

const s32_t xppc_upsConfigp_ids[2] = {
	1,  2
};
struct mib_node* const xppc_upsConfigp_nodes[2] = {
	(struct mib_node*)&upsBaseConfig, 
	(struct mib_node*)&upsSmartConfig, 
};
const struct mib_array_node  upsConfigp= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	xppc_upsConfigp_ids,
	xppc_upsConfigp_nodes
};
// upsConfigp

const s32_t upsIdentp_ids[2] = { 1,2};
struct mib_node* const upsIdentp_nodes[2] = {
	(struct mib_node*)&upsBaseIdent,
	(struct mib_node*)&upsSmartIdent,
};

const struct mib_array_node upsIdentp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,  // 9개
	upsIdentp_ids,
	upsIdentp_nodes
};


/* .1.3.6.1.2.1.34.1 */
const s32_t xppc_upsMIB_ids[9] = { 1,2,3,4,5,6,7,8,9};//{ 1};
struct mib_node* const xppc_upsMIB_nodes[9] = {
	//(struct mib_node*)&xppc_upsObjects
	(struct mib_node*)&upsIdentp,
	(struct mib_node*)&upsBatteryp,
	(struct mib_node*)&upsInputp,
	(struct mib_node*)&upsOutputp,
	(struct mib_node*)&upsConfigp,
	(struct mib_node*)&upsControlp,
	(struct mib_node*)&upsBasicTestp,
	(struct mib_node*)&upsThreePhasep,
	(struct mib_node*)&upsEnvironment
};

const struct mib_array_node xppc_upsMIB = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	9,  // 1개
	xppc_upsMIB_ids,
	xppc_upsMIB_nodes
};
///////////////////////////////////////////





#endif /* XPPC_H_ */