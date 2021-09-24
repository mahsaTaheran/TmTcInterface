/*
 * pollingSequenceCentroid.cpp
 *
 *  Created on: Jun 7, 2021
 *      Author: mala
 */

#include "pollingSequenceCentroid.h"
#include <objects/systemObjectList.h>
#include <fsfw/objectmanager/ObjectManagerIF.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <fsfw/tasks/FixedTimeslotTaskIF.h>
#include <fsfw/devicehandlers/DeviceHandlerIF.h>
#include <OBSWConfig.h>
ReturnValue_t pollingSequenceCentroidFunction(
		FixedTimeslotTaskIF* thisSequence){
	uint32_t length = thisSequence->getPeriodMs();

	thisSequence->addSlot(objects::CENTROID_INJECTOR, length * 0, 2);
	//thisSequence->addSlot(objects::CENTROID_WRITER, length * 0.1, 2);

	if (thisSequence->checkSequence() == HasReturnvaluesIF::RETURN_OK) {
		return HasReturnvaluesIF::RETURN_OK;
	}
	else {
		sif::error << "pollingSequenceCentroidFunction::initialize has errors!"
				<< std::endl;
		return HasReturnvaluesIF::RETURN_FAILED;
	}
}


