#ifndef FSFW_RETURNVALUES_FWCLASSIDS_H_
#define FSFW_RETURNVALUES_FWCLASSIDS_H_

namespace CLASS_ID {
enum {
	OPERATING_SYSTEM_ABSTRACTION = 1,  //OS
	OBJECT_MANAGER_IF, //OM
	DEVICE_HANDLER_BASE, //DHB
	RMAP_CHANNEL, //RMP
	POWER_SWITCH_IF, //PS
	HAS_MEMORY_IF, //PP
	DEVICE_STATE_MACHINE_BASE, //DSMB
	DATA_SET_CLASS, //DPS
	POOL_RAW_ACCESS_CLASS, //DPR
	CONTROLLER_BASE, //CTR
	SUBSYSTEM_BASE, //SB
	MODE_STORE_IF, //MS
	SUBSYSTEM, //SS
	HAS_MODES_IF, //HM
	COMMAND_MESSAGE, //CM
	CCSDS_TIME_HELPER_CLASS, //TIM
	ARRAY_LIST, //AL
	ASSEMBLY_BASE, //AB
	MEMORY_HELPER, //MH
	SERIALIZE_IF, //SE
	FIXED_MAP, //FM
	FIXED_MULTIMAP, //FMM
	HAS_HEALTH_IF, //HHI
	FIFO_CLASS, //FF
	MESSAGE_PROXY, //MQP
	TRIPLE_REDUNDACY_CHECK, //TRC
	TC_PACKET_CHECK, //TCC
	PACKET_DISTRIBUTION, //TCD
	ACCEPTS_TELECOMMANDS_IF, //PUS
	DEVICE_SERVICE_BASE, //DSB
	COMMAND_SERVICE_BASE, //CSB
	TM_STORE_BACKEND_IF, //TMB
	TM_STORE_FRONTEND_IF, //TMF
	STORAGE_AND_RETRIEVAL_SERVICE, //SR
	MATCH_TREE_CLASS, //MT
	EVENT_MANAGER_IF, //EV
	HANDLES_FAILURES_IF, //FDI
	DEVICE_HANDLER_IF, //DHI
	STORAGE_MANAGER_IF, //SM
	THERMAL_COMPONENT_IF, //TC
	INTERNAL_ERROR_CODES, //IEC
	TRAP, //TRP
	CCSDS_HANDLER_IF, //CCS
	PARAMETER_WRAPPER, //PAW
	HAS_PARAMETERS_IF, //HPA
	ASCII_CONVERTER, //ASC
	POWER_SWITCHER, //POS
	LIMITS_IF, //LIM
	COMMANDS_ACTIONS_IF, //CF
	HAS_ACTIONS_IF, //HF
	DEVICE_COMMUNICATION_IF, //DC
	BSP, //BSP
	TIME_STAMPER_IF, //TSI 53
	SGP4PROPAGATOR_CLASS, //SGP4 54
	MUTEX_IF, //MUX 55
	MESSAGE_QUEUE_IF,//MQI 56
	SEMAPHORE_IF, //SPH 57
	LOCAL_POOL_OWNER_IF, //LPIF 58
	POOL_VARIABLE_IF, //PVA 59
	HOUSEKEEPING_MANAGER, //HKM 60
	DLE_ENCODER, //DLEE 61
	PUS_SERVICE_9, //PUS9 62
	FW_CLASS_ID_COUNT //is actually count + 1 !

};
}

#endif /* FSFW_RETURNVALUES_FWCLASSIDS_H_ */
