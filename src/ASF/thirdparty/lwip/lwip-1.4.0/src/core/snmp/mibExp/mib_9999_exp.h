/*
 * mib_9999_exp.h
 *
 * Created: 4/29/2021 3:10:36 PM
 *  Author: STELLA
 */ 


#ifndef MIB_9999_EXP_H_
#define MIB_9999_EXP_H_





const s32_t mib2_d1_exp[10] ={1,2,3,4,5,6,7,8,9,10 };
struct mib_node* const mib2_nodes_d1_exp[10] = {
	(struct mib_node*)&ident_exp, (struct mib_node*)&battery_exp,
	(struct mib_node*)&input_exp, (struct mib_node*)&output_exp,
	(struct mib_node*)&bypass_exp, (struct mib_node*)&status_exp,
	(struct mib_node*)&upsAlarm_exp, (struct mib_node*)&upsTestBattery_exp,
(struct mib_node*)&upsSchedule_exp, (struct mib_node*)&upsConfig_exp};
const struct mib_array_node upsMIB_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	10,
	mib2_d1_exp,
	mib2_nodes_d1_exp
};

const s32_t mib2_ids_exp[1] ={ 9999 };
struct mib_node* const mib2_nodes_exp[1] = {(struct mib_node*)&upsMIB_exp};
const struct mib_array_node mgmt_exp = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,
	mib2_ids_exp,
	mib2_nodes_exp
};



#endif /* MIB_9999_EXP_H_ */