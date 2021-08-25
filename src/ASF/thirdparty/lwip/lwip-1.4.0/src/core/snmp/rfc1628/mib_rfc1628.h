/*
 * mib_rfc1268.h
 *
 * Created: 4/29/2021 3:21:00 PM
 *  Author: STELLA
 */ 


#ifndef MIB_RFC1268_H_
#define MIB_RFC1268_H_

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

const mib_scalar_node rfc1628_ups_scalar = {
	&ups_get_object_def,
	&ups_get_value,
	&ups_set_test,
	&ups_set_value,
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

//ups base indet scalar
// 1: upsIdentManufacturer
// 2:upsIdentModel
// 3:upsIdentUPSSoftwareVersion
// 4:upsAgentSoftwareVersion
// 5:upsIdentName
// 6:upsIdentAttachedDevices
//
const mib_scalar_node rfc1628_upsBaseIdent_scalar = {
	&ups_get_upsBaseIdent_object_def,
	&ups_get_upsBaseIdent_value,
	&ups_set_upsBaseIdent_test,
	&ups_set_upsBaseIdent_value,
	MIB_NODE_SC,
	0
};
//
const mib_scalar_node rfc1628_upsSmartIdent_scalar = {
	&ups_get_upsSmartIdent_object_def,
	&ups_get_upsSmartIdent_value,
	&ups_set_upsSmartIdent_test,
	&ups_set_upsSmartIdent_value,
	MIB_NODE_SC,
	0
};

const s32_t rfc1628_upsBaseIdent_ids[6] = {
	1,  2,  3,  4  ,5 ,6
};
struct mib_node* const rfc1628_upsBaseIdent_nodes[6] = {
	(struct mib_node*)&rfc1628_upsBaseIdent_scalar ,
	(struct mib_node*)&rfc1628_upsBaseIdent_scalar ,
	(struct mib_node*)&rfc1628_upsBaseIdent_scalar ,
	(struct mib_node*)&rfc1628_upsBaseIdent_scalar ,
	(struct mib_node*)&rfc1628_upsBaseIdent_scalar ,
	(struct mib_node*)&rfc1628_upsBaseIdent_scalar
};
//.iso.org.dod.internet.private.enterprises.ppc.products.hardware.ups.upsBaseIdentp 
const struct mib_array_node  upsBaseIdent = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	6,
	rfc1628_upsBaseIdent_ids,
	rfc1628_upsBaseIdent_nodes
};

const s32_t rfc1628_upsSmartIdent_ids[4] = {
	1,  2,  3,  4  
};
struct mib_node* const rfc1628_upsSmartIdent_nodes[4] = {
	(struct mib_node*)&rfc1628_upsSmartIdent_scalar ,
	(struct mib_node*)&rfc1628_upsSmartIdent_scalar ,
	(struct mib_node*)&rfc1628_upsSmartIdent_scalar ,
	(struct mib_node*)&rfc1628_upsSmartIdent_scalar ,
};
//.iso.org.dod.internet.private.enterprises.ppc.products.hardware.ups.upsSmartIdentp 
const struct mib_array_node  upsSmartIdent = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	rfc1628_upsSmartIdent_ids,
	rfc1628_upsSmartIdent_nodes
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

const mib_scalar_node rfc1628_upsBaseBattery_scalar = {
	&ups_get_upsBaseBattery_object_def,
	&ups_get_upsBaseBattery_value,
	&ups_set_upsBaseBattery_test,
	&ups_set_upsBaseBattery_value,
	MIB_NODE_SC,
	0
};
const mib_scalar_node rfc1628_upsSmartBattery_scalar = {
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
	/*
	upsSmartBatteryCapacity(1)  
	upsSmartBatteryVoltage(2)  
	upsSmartBatteryTemperature(3)  
	upsSmartBatteryRunTimeRemaining(4)  
	upsSmartBatteryReplaceIndicator(5)  
	upsSmartBatteryFullChargeVoltage(6)  
	upsSmartBatteryCurrent(7) 
	*/
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


const s32_t rfc1628_upsBaseBattery_ids[7] = {
	1,  2,  3,  4  , 5, 6, 7
};
struct mib_node* const rfc1628_upsBaseBattery_nodes[7] = {
	(struct mib_node*)&rfc1628_upsBaseBattery_scalar , (struct mib_node*)&rfc1628_upsBaseBattery_scalar,
	(struct mib_node*)&rfc1628_upsBaseBattery_scalar, (struct mib_node*)&rfc1628_upsBaseBattery_scalar,
	(struct mib_node*)&rfc1628_upsBaseBattery_scalar, (struct mib_node*)&rfc1628_upsBaseBattery_scalar,
	(struct mib_node*)&rfc1628_upsBaseBattery_scalar
};
const struct mib_array_node  upsBaseBattery= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	7,
	rfc1628_upsBaseBattery_ids,
	rfc1628_upsBaseBattery_nodes
};
const s32_t rfc1628_upsSmartBattery_ids[7] = {
	1,  2,  3,  4  , 5, 6, 7
};
struct mib_node* const rfc1628_upsSmartBattery_nodes[7] = {
	(struct mib_node*)&rfc1628_upsSmartBattery_scalar , (struct mib_node*)&rfc1628_upsSmartBattery_scalar,
	(struct mib_node*)&rfc1628_upsSmartBattery_scalar, (struct mib_node*)&rfc1628_upsSmartBattery_scalar,
	(struct mib_node*)&rfc1628_upsSmartBattery_scalar, (struct mib_node*)&rfc1628_upsSmartBattery_scalar,
	(struct mib_node*)&rfc1628_upsSmartBattery_scalar
};
const struct mib_array_node  upsSmartBattery= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	7,
	rfc1628_upsSmartBattery_ids,
	rfc1628_upsSmartBattery_nodes
};



const s32_t rfc1628_upsBatteryp_ids[2] = {
	1,  2
};
struct mib_node* const rfc1628_upsBatteryp_nodes[2] = {
	(struct mib_node*)&upsBaseBattery, (struct mib_node*)&upsSmartBattery
};


const struct mib_array_node  upsBatteryp= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	rfc1628_upsBatteryp_ids,
	rfc1628_upsBatteryp_nodes
};

//----upsBaseInput------------------
//--------------------upsBaseInputEntry
static void ups_get_upsBaseInputTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseInputTable_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseInputTable_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseInputTable_value(struct obj_def *od, u16_t len, void *value);
//  upsInput get and set
static void ups_get_upsBaseInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseInput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseInput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseInput_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsBaseInputTable_scalar = {
	&ups_get_upsBaseInputTable_object_def,
	&ups_get_upsBaseInputTable_value,
	&ups_set_upsBaseInputTable_test,
	&ups_set_upsBaseInputTable_value,
	MIB_NODE_SC,
	0
};
static void ups_get_upsBaseInputTable_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
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

static void ups_get_upsBaseInputTable_value(struct obj_def *od, u16_t len, void *value)
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

static u8_t ups_set_upsBaseInputTable_test(struct obj_def *od, u16_t len, void *value)
{return 0;};
static void ups_set_upsBaseInputTable_value(struct obj_def *od, u16_t len, void *value)
{
	u8_t id;
	id = (u8_t)od->id_inst_ptr[0];
};

const mib_scalar_node rfc1628_upsBaseInput_scalar = {
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



const s32_t rfc1628_upsBaseInputEntry_ids[5] = {
	1,  2,  3, 4, 5
};
struct mib_node* const rfc1628_upsBaseInputEntry_nodes[5] = {
	(struct mib_node*)&rfc1628_upsBaseInputTable_scalar , (struct mib_node*)&rfc1628_upsBaseInputTable_scalar,
	(struct mib_node*)&rfc1628_upsBaseInputTable_scalar , (struct mib_node*)&rfc1628_upsBaseInputTable_scalar,
	(struct mib_node*)&rfc1628_upsBaseInputTable_scalar
};
const struct mib_array_node  upsBaseInputEntry= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	rfc1628_upsBaseInputEntry_ids,
	rfc1628_upsBaseInputEntry_nodes
};
const s32_t rfc1628_upsBaseInputTable_ids = 1;
struct mib_node* const rfc1628_upsBaseInputTable_nodes =
(struct mib_node*)&upsBaseInputEntry;

const struct mib_array_node  upsBaseInputTable= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_RA,
	1,
	&rfc1628_upsBaseInputTable_ids,
	&rfc1628_upsBaseInputTable_nodes
};

const s32_t rfc1628_upsBaseInput_ids[3] = {
	1,  2,  3
};
struct mib_node* const rfc1628_upsBaseInput_nodes[3] = {
	(struct mib_node*)&rfc1628_upsBaseInput_scalar , 
	(struct mib_node*)&rfc1628_upsBaseInput_scalar,
	(struct mib_node*)&rfc1628_upsBaseInput_scalar
};
//(struct mib_node*)&upsBaseInputTable
const struct mib_array_node  upsBaseInput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	rfc1628_upsBaseInput_ids,
	rfc1628_upsBaseInput_nodes
};
//----upsBaseInput------------------

//----upsSmartInput------------------
//  upsSmartInput get and set
static void ups_get_upsSmartInput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartInput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartInput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartInput_value(struct obj_def *od, u16_t len, void *value);


const mib_scalar_node rfc1628_upsSmartInput_scalar = {
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


const s32_t rfc1628_upsSmartInput_ids[5] = {
	1,  2,  3,4,5
};
struct mib_node* const rfc1628_upsSmartInput_nodes[5] = {
	(struct mib_node*)&rfc1628_upsSmartInput_scalar , (struct mib_node*)&rfc1628_upsSmartInput_scalar,
	(struct mib_node*)&rfc1628_upsSmartInput_scalar , (struct mib_node*)&rfc1628_upsSmartInput_scalar,
	(struct mib_node*)&rfc1628_upsSmartInput_scalar 
};

const struct mib_array_node  upsSmartInput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	5,
	rfc1628_upsSmartInput_ids,
	rfc1628_upsSmartInput_nodes
};
//----upsSmartInput------------------

const s32_t rfc1628_upsInputp_ids[2] = {
	1,  2  
};


struct mib_node* const rfc1628_upsInputp_nodes[] = {
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
	rfc1628_upsInputp_ids,
	rfc1628_upsInputp_nodes
};



////UPS_BASE_OUTPUT
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
static void ups_get_upsBaseOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseOutput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseOutput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseOutput_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsBaseOutput_scalar = {
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


const s32_t rfc1628_upsBaseOutput_ids[4] = {
	1,  2,  3 , 4
};
struct mib_node* const rfc1628_upsBaseOutput_nodes[4] = {
	(struct mib_node*)&rfc1628_upsBaseOutput_scalar,
	(struct mib_node*)&rfc1628_upsBaseOutput_scalar,
	(struct mib_node*)&rfc1628_upsBaseOutput_scalar,
	(struct mib_node*)&rfc1628_upsBaseOutput_scalar
};
const struct mib_array_node  upsBaseOutput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	rfc1628_upsBaseOutput_ids,
	rfc1628_upsBaseOutput_nodes
};
////UPS_BASE_OUTPUT
////UPS_SMART_OUTPUT
static void ups_get_upsSmartOutput_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsSmartOutput_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsSmartOutput_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsSmartOutput_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsSmartOutput_scalar = {
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



const s32_t rfc1628_upsSmartOutput_ids[3] = {
	1,  2,  3 
};
struct mib_node* const rfc1628_upsSmartOutput_nodes[3] = {
	(struct mib_node*)&rfc1628_upsSmartOutput_scalar,
	(struct mib_node*)&rfc1628_upsSmartOutput_scalar,
	(struct mib_node*)&rfc1628_upsSmartOutput_scalar
};
const struct mib_array_node  upsSmartOutput= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	3,
	rfc1628_upsSmartOutput_ids,
	rfc1628_upsSmartOutput_nodes
};
////UPS_SMART_OUTPUT

const s32_t rfc1628_upsOutputp_ids[2] = {
	1,  2
};
struct mib_node* const rfc1628_upsOutputp_nodes[2] = {
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
	rfc1628_upsOutputp_ids,
	rfc1628_upsOutputp_nodes
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
	//LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
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
//  upsBaseConfig get and set
static void ups_get_upsBaseConfig_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ups_get_upsBaseConfig_value(struct obj_def *od, u16_t len, void *value);
static u8_t ups_set_upsBaseConfig_test(struct obj_def *od, u16_t len, void *value);
static void ups_set_upsBaseConfig_value(struct obj_def *od, u16_t len, void *value);

const mib_scalar_node rfc1628_upsBaseConfig_scalar = {
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

const mib_scalar_node rfc1628_upsSmartConfig_scalar = {
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
const s32_t rfc1628_upsBaseConfig_ids[10] = {
	1,  2,  3 ,4,5,6,7,8,9,10
};
struct mib_node* const rfc1628_upsBaseConfig_nodes[10] = {
	(struct mib_node*)&rfc1628_upsBaseConfig_scalar, (struct mib_node*)&rfc1628_upsBaseConfig_scalar,
	(struct mib_node*)&rfc1628_upsBaseConfig_scalar, (struct mib_node*)&rfc1628_upsBaseConfig_scalar,
	(struct mib_node*)&rfc1628_upsBaseConfig_scalar, (struct mib_node*)&rfc1628_upsBaseConfig_scalar,
	(struct mib_node*)&rfc1628_upsBaseConfig_scalar, (struct mib_node*)&rfc1628_upsBaseConfig_scalar,
	(struct mib_node*)&rfc1628_upsBaseConfig_scalar, (struct mib_node*)&rfc1628_upsBaseConfig_scalar
};

const struct mib_array_node  upsBaseConfig= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	10,
	rfc1628_upsBaseConfig_ids,
	rfc1628_upsBaseConfig_nodes
};

// upsBaseConfigp
// upsSmartConfigp
const s32_t rfc1628_upsSmartConfig_ids[10] = {
	1,  2,  3 ,4,5,6,7,8,9,10
};
struct mib_node* const rfc1628_upsSmartConfig_nodes[10] = {
	(struct mib_node*)&rfc1628_upsSmartConfig_scalar, (struct mib_node*)&rfc1628_upsSmartConfig_scalar,
	(struct mib_node*)&rfc1628_upsSmartConfig_scalar, (struct mib_node*)&rfc1628_upsSmartConfig_scalar,
	(struct mib_node*)&rfc1628_upsSmartConfig_scalar, (struct mib_node*)&rfc1628_upsSmartConfig_scalar,
	(struct mib_node*)&rfc1628_upsSmartConfig_scalar, (struct mib_node*)&rfc1628_upsSmartConfig_scalar,
	(struct mib_node*)&rfc1628_upsSmartConfig_scalar, (struct mib_node*)&rfc1628_upsSmartConfig_scalar
};

const struct mib_array_node  upsSmartConfig= {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	10,
	rfc1628_upsSmartConfig_ids,
	rfc1628_upsSmartConfig_nodes
};

// upsSmartConfigp

// upsConfigp

const s32_t rfc1628_upsConfigp_ids[2] = {
	1,  2
};
struct mib_node* const rfc1628_upsConfigp_nodes[2] = {
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
	rfc1628_upsConfigp_ids,
	rfc1628_upsConfigp_nodes
};
// upsConfigp

//(struct mib_node*)&upsAlarm,
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
const s32_t rfc1628_upsMIB_ids[9] = { 1,2,3,4,5,6,7,8,9};//{ 1};
struct mib_node* const rfc1628_upsMIB_nodes[9] = {
	//(struct mib_node*)&rfc1628_upsObjects
	(struct mib_node*)&upsIdentp,
	(struct mib_node*)&upsBatteryp,
	(struct mib_node*)&upsInputp,
	(struct mib_node*)&upsOutputp,
	(struct mib_node*)&upsConfigp,
	(struct mib_node*)&upsTest,
	(struct mib_node*)&upsTest,
	(struct mib_node*)&upsControl,
	(struct mib_node*)&upsBypass
};

const struct mib_array_node rfc1628_upsMIB = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	9,  // 1개
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

extern Bool bModebusSuccess;
static void ups_get_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  /* return to object name, adding index depth (1) */

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
    //LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("snmp_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}



// .1.3.6.1.2.1.33.1.1.1.0 ~ 57까지의 데이타를 채운다.


static void ups_get_value(struct obj_def *od, u16_t len, void *value)
{

  u32_t *uint_ptr = (u32_t*)value;
  u8_t id=0;
  uint16_t * pData =(uint16_t *)&upsModeBusData ;	
 
  
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
    //LWIP_DEBUGF_UDP(SNMP_MIB_DEBUG,("od->id_inst_ptr[0]=%d \n",id));
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
//

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