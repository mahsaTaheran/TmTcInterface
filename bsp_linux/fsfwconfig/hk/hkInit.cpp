/*
 * hkInit.cpp
 *
 *  Created on: Jun 7, 2021
 *      Author: mala
 */
//#include "hkInit.h"

#include <datapool/dataPoolInit.h>
#include <datapool/PointingTestPool.h>
#include <fsfw/returnvalues/HasReturnvaluesIF.h>
#include <mission/pus/Service3PsbPackets.h>
#include "hkInit.h"
//// TODO: Generate this file automatically from header file



void hk::initHkStruct(struct hkIdStruct* HkIdStruct, pointingTestInit::pointingHkStruct PointingHkTest)
{

	//hkIdStruct->gps0 = gps0;
	//hkIdStruct->gps1 = gps1;
	//hkIdStruct->test = test;
	//hkIdStruct->deviceHkTest = deviceHkTest;
	HkIdStruct->pointingHkTest = PointingHkTest;
}

void hk::hkInit(Service3HousekeepingPSB * housekeepingService,
		struct hk::hkIdStruct hkIdStruct) {
	HkPacketDefinition hkPacket;
	ReturnValue_t initResult;
	 //GPS 0
/*	hkPacket.initialize(
			hkIdStruct.gps0.sid,
			hkIdStruct.gps0.collectionInterval,
			hkIdStruct.gps0.numberOfParameters,
			reinterpret_cast<uint32_t *>(&hkIdStruct.gps0.p),
			hkIdStruct.gps0.enablePacket
	);
	initResult = housekeepingService->addPacketDefinitionToStore(&hkPacket,
			hkIdStruct.gps0.isDiagnosticsPacket);
	if(initResult != HasReturnvaluesIF::RETURN_OK) {
		sif::debug << "Inserting GPS0 HK definition failed with ID " <<
				initResult << std::endl;
	}
	 //GPS 1
	hkPacket.initialize(
			hkIdStruct.gps1.sid,
			hkIdStruct.gps1.collectionInterval,
			hkIdStruct.gps1.numberOfParameters,
			reinterpret_cast<uint32_t *>(&hkIdStruct.gps1.p),
			hkIdStruct.gps1.enablePacket
	);
	initResult = housekeepingService->addPacketDefinitionToStore(&hkPacket,
			hkIdStruct.gps1.isDiagnosticsPacket);
	if(initResult != HasReturnvaluesIF::RETURN_OK) {
		sif::debug << "Inserting GPS1 HK definition failed with ID " <<
				initResult << std::endl;
	}
	 //Test
	hkPacket.initialize(
			hkIdStruct.test.sid,
			hkIdStruct.test.collectionInterval,
			hkIdStruct.test.numberOfParameters,
			reinterpret_cast<uint32_t *>(&hkIdStruct.test.p),
			hkIdStruct.test.enablePacket
	);
	initResult = housekeepingService->addPacketDefinitionToStore(&hkPacket,
			hkIdStruct.test.isDiagnosticsPacket);
	if(initResult != HasReturnvaluesIF::RETURN_OK) {
		sif::debug << "Inserting Test HK definition failed with ID " <<
				initResult << std::endl;
	}*/

	// deviceTest

/*	hkPacket.initialize(
			hkIdStruct.deviceHkTest.sid,
			hkIdStruct.deviceHkTest.collectionInterval,
			hkIdStruct.deviceHkTest.numberOfParameters,
			reinterpret_cast<uint32_t *>(&hkIdStruct.deviceHkTest.p),
			hkIdStruct.deviceHkTest.enablePacket);
	initResult = housekeepingService->addPacketDefinitionToStore(&hkPacket,
			hkIdStruct.deviceHkTest.isDiagnosticsPacket);
	if(initResult != HasReturnvaluesIF::RETURN_OK) {
		sif::debug << "Inserting Test HK definition failed with ID " <<
				initResult << std::endl;
	}*/

	hkPacket.initialize(
				hkIdStruct.pointingHkTest.sid,
				hkIdStruct.pointingHkTest.collectionInterval,
				hkIdStruct.pointingHkTest.numberOfParameters,
				reinterpret_cast<uint32_t *>(&hkIdStruct.pointingHkTest.p),
				hkIdStruct.pointingHkTest.enablePacket
		);
		initResult = housekeepingService->addPacketDefinitionToStore(&hkPacket,
				hkIdStruct.pointingHkTest.isDiagnosticsPacket);
		if (initResult != HasReturnvaluesIF::RETURN_OK) {
			sif::debug << "Inserting Test HK definition failed with ID " <<
					initResult << std::endl;
	}
	/*hkPacket.initialize(
			hkIdStruct.pointingHkAzimuthTest.sid,
			hkIdStruct.pointingHkAzimuthTest.collectionInterval,
			hkIdStruct.pointingHkAzimuthTest.numberOfParameters,
			reinterpret_cast<uint32_t *>(&hkIdStruct.pointingHkAzimuthTest.p),
			hkIdStruct.pointingHkAzimuthTest.enablePacket
	);
	initResult = housekeepingService->addPacketDefinitionToStore(&hkPacket,
			hkIdStruct.pointingHkAzimuthTest.isDiagnosticsPacket);
	if (initResult != HasReturnvaluesIF::RETURN_OK) {
		sif::debug << "Inserting Test HK definition failed with ID " <<
				initResult << std::endl;
}
	hkPacket.initialize(
					hkIdStruct.pointingHkElevationTest.sid,
					hkIdStruct.pointingHkElevationTest.collectionInterval,
					hkIdStruct.pointingHkElevationTest.numberOfParameters,
					reinterpret_cast<uint32_t *>(&hkIdStruct.pointingHkElevationTest.p),
					hkIdStruct.pointingHkElevationTest.enablePacket
			);
			initResult = housekeepingService->addPacketDefinitionToStore(&hkPacket,
					hkIdStruct.pointingHkElevationTest.isDiagnosticsPacket);
			if (initResult != HasReturnvaluesIF::RETURN_OK) {
				sif::debug << "Inserting Test HK definition failed with ID " <<
						initResult << std::endl;
}*/

}



