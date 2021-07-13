/*
 * mib_rfc1628_32.h
 *
 * Created: 4/29/2021 3:31:50 PM
 *  Author: STELLA
 */ 


#ifndef MIB_RFC1628_32_H_
#define MIB_RFC1628_32_H_

const struct mib_array_node ups = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	61,
	ups_ids,
	ups_nodes
};


//struct mib_node* const upsObjects_group[1] = {
//	(struct mib_node*)&snmp,
//};
// upsObjects
/* .1.3.6.1.2.1.32.1.1 */
const s32_t upsObjects_ids[1] = { 1};
struct mib_node* const upsObjects_nodes[1] = { (struct mib_node*)&ups };
const struct mib_array_node upsObjects = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 1°³
	upsObjects_ids,
	upsObjects_nodes
};

//upsMIB
/* .1.3.6.1.2.1.32.1 */
const s32_t upsMIB_ids[1] = { 1};
struct mib_node* const upsMIB_nodes[1] = { (struct mib_node*)&upsObjects };

const struct mib_array_node upsMIB = {
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,  // 1°³
	upsMIB_ids,
	upsMIB_nodes
};



#endif /* MIB_RFC1628_32_H_ */