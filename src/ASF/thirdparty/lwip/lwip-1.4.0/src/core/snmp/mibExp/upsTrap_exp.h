/*
 * upsTrap_exp.h
 *
 * Created: 4/29/2021 3:15:47 PM
 *  Author: STELLA
 */ 


#ifndef UPSTRAP_EXP_H_
#define UPSTRAP_EXP_H_
/*
1	SNMP Trap: SENA-UPSLINK-MIB::senaUpsPowerRestored	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.26\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
2	SNMP Trap: SENA-UPSLINK-MIB::senaUpsAwaitingPower	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.21\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
3	SNMP Trap: SENA-UPSLINK-MIB::senaUpsUserSpecificAlarm	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.25\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
4	SNMP Trap: SENA-UPSLINK-MIB::senaUpsTestInProg	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.24\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
5	SNMP Trap: SENA-UPSLINK-MIB::senaUpsShutdownPending	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.22\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
6	SNMP Trap: SENA-UPSLINK-MIB::senaUpsCommLost	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.20\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
7	SNMP Trap: SENA-UPSLINK-MIB::senaUpsFanFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.16\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
8	SNMP Trap: SENA-UPSLINK-MIB::senaUpsDiagTestFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.19\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
9	SNMP Trap: SENA-UPSLINK-MIB::senaUpslinkUPSTraps#	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
10	SNMP Trap: SENA-UPSLINK-MIB::senaUpsGenFault	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.18\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
11	SNMP Trap: SENA-UPSLINK-MIB::senaUpsFuseFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.17\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
12	SNMP Trap: SENA-UPSLINK-MIB::senaUpsSysOff	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.15\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
13	SNMP Trap: SENA-UPSLINK-MIB::senaUpsShutdownImminent	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.23\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
14	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutOff	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.14\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
15	SNMP Trap: SENA-UPSLINK-MIB::senaUpsUpsoffAr	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.12\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
16	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutputBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.7\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
17	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutoffAr	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.11\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
18	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBypassBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.10\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
19	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOutOverload	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.8\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
20	SNMP Trap: SENA-UPSLINK-MIB::senaUpsInputBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.6\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
21	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOnBattery	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.2\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
22	SNMP Trap: SENA-UPSLINK-MIB::senaUpsTempBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.5\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
23	SNMP Trap: SENA-UPSLINK-MIB::senaUpsChargerFail	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.13\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
24	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryDeprecated	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.4\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
25	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryLow	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.3\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
26	SNMP Trap: SENA-UPSLINK-MIB::senaUpsBatteryBad	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.1\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
27	SNMP Trap: SENA-UPSLINK-MIB::senaUpsOnBypass	snmptrap["\s\.1\.3\.6\.1\.4\.1\.12236\.1\.1\.11\.0\.9\s"]		7d		SNMP ∆Æ∑¶	SENA-UPSLINK-MIB::senaUpslinkUPSTraps	∫Ò»∞º∫
*/
const s32_t mib2_upsSpecial_exp[27] ={ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
struct mib_node* const mib2_nodes_upsSpecial_exp[27] = {
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep,
	(struct mib_node*)&ups_scalar_kep 
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
	1,  // 8∞≥
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
	1,  // 8∞≥
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
	1,  // 8∞≥
	mib2_companyCode_exp,
	mib2_nodes_companyCode_exp
};



#endif /* UPSTRAP_EXP_H_ */