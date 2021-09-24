/*
 * classIds.h
 *
 *  Created on: 30.06.2016
 *      Author: baetz
 */

#ifndef FSFWCONFIG_RETURNVALUES_CLASSIDS_H_
#define FSFWCONFIG_RETURNVALUES_CLASSIDS_H_

#include <fsfw/returnvalues/FwClassIds.h>

namespace CLASS_ID {
enum {
	DUMMY_HANDLER = FW_CLASS_ID_COUNT, //DDH
	PUS_SERVICE_3,
	CENTROID_WRITER_CLASS,
	POINTING_WRITER_CLASS,
	SENSOR_WRITER_CLASS,
};
}


#endif /* FSFWCONFIG_RETURNVALUES_CLASSIDS_H_ */
