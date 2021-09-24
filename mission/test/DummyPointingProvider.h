/*
 * DummyPointingProvider.h
 *
 *  Created on: Jul 21, 2021
 *      Author: mala
 */

#ifndef MISSION_TEST_DUMMYPOINTINGPROVIDER_H_
#define MISSION_TEST_DUMMYPOINTINGPROVIDER_H_


#include <vector>
#include<fstream>
#include <fsfw/tasks/ExecutableObjectIF.h>
#include <fsfw/objectmanager/SystemObject.h>
#include <bsp_linux/fsfwconfig/returnvalues/classIds.h>
#include <bsp_linux/fsfwconfig/events/subsystemIdRanges.h>

/***************************************
 * This class is only for test, mostly for testing the pointing visualizer system connected
 * to RAMSES. The pointing values are in RA/Dec , data, read from csv, simulates the move
 * from one target star to another.
 *
 */

class DummyPointingProvider : public ExecutableObjectIF, public SystemObject {

public:
	DummyPointingProvider(object_id_t objectID, std::string inputPath, int numberImages = 0);
	ReturnValue_t initialize();
	virtual ~DummyPointingProvider();
	virtual ReturnValue_t performOperation(uint8_t operationCode = 0);

	static const uint8_t INTERFACE_ID = CLASS_ID::POINTING_WRITER_CLASS;
	static const uint8_t SUBSYSTEM_ID = SUBSYSTEM_ID::POINTING_WRITER;
	static const Event WRITE_POINTING = MAKE_EVENT (0, SEVERITY::INFO);
	static const ReturnValue_t POINTING_OPS_OK= MAKE_RETURN_CODE(0x010);

private:

	uint16_t numberOfPointings;
	std::ifstream pointingValues;
	ReturnValue_t PopulateInput();
	typedef std::pair<double,double> pointingPair_t;
	std::vector<pointingPair_t> pointingInput;
	std::vector<pointingPair_t>::iterator pointingIt;
	std::string path;

};


#endif /* MISSION_TEST_DUMMYPOINTINGPROVIDER_H_ */
