#ifndef FSFWCONFIG_OBJECTS_SYSTEMOBJECTLIST_H_
#define FSFWCONFIG_OBJECTS_SYSTEMOBJECTLIST_H_

#include <fsfw/objectmanager/frameworkObjects.h>

namespace objects {
enum mission_objects {
	//UDP_BRIDGE = 0x50000300,
	//UDP_POLLING_TASK = 0x50000400,
	TM_FUNNEL = 0x50000500,
	STATUS_POLLING = 0x50000600,
	TIME_POLLING = 0x50000700,
	ESBO_TM=0x50000800,
	ESBO_TC = 0x50000900,
	TIME_HANDLER =0x60000100,
	STATUS_HANDLER =0x60000200,

	/****************Assembly********************/

	SOFTWARE = 0x1,


	/****************Controller******************/


	/****************Device Handler**************/

	/****************MISC*********************/
	CUC_TIME_STAMPER = 0x16,


	/**************TC Handling****************/
	CCSDS_DISTRIBUTOR = 0x10,
	PUS_DISTRIBUTOR = 0x11,
	PUS_SERVICE_3_PSB = 0x1500FFFF,
	PUS_PARAMETER_SERVICE = 0x1501FFFF,

	/****** 0x49 ('I') for Communication Interfaces *****/

	/**************** Test *********************/

	CENTROID_INJECTOR=0x14,
	POINTING_PROVIDER=0x17,
	SENSOR_DATA_PROVIDER=0x18,
};
}

#endif /* FSFWCONFIG_OBJECTS_SYSTEMOBJECTLIST_H_ */
