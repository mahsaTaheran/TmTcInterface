/*
 * PointingTestPool.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: mala
 */

#include "PointingTestPool.h"
#include <hk/sid.h>
#include "dataPoolInit.h"

void pointingTestInit::pointingHkStructInit(pointingHkStruct * hkStruct) {
	hkStruct->p.test_pointing = datapool::Pointing_PoolId;
	hkStruct->p.test_time=datapool::PointingTime_PoolId;
	hkStruct->p.test_sensorData = datapool::SensorData_PoolId;

	float TEST_COLLECTION_INTERVAL = 1;
	hkStruct->sid = sid::pointingHkTest;
	hkStruct->collectionInterval = TEST_COLLECTION_INTERVAL;
	hkStruct->isDiagnosticsPacket = false;
}



