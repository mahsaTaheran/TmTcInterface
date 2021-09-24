/*
 * DummyCentroidProvider.h
 *
 *  Created on: May 1, 2021
 *      Author: mala
 */

#ifndef DUMMYCENTROIDPROVIDER_H_
#define DUMMYCENTROIDPROVIDER_H_

#include <vector>
#include<fstream>
#include <fsfw/tasks/ExecutableObjectIF.h>
#include <fsfw/objectmanager/SystemObject.h>
#include <bsp_linux/fsfwconfig/returnvalues/classIds.h>
#include <bsp_linux/fsfwconfig/events/subsystemIdRanges.h>
class DummyCentroidInjector : public ExecutableObjectIF, public SystemObject {

public:
	DummyCentroidInjector(object_id_t objectID, std::string inputPath, uint8_t numberImages);
	ReturnValue_t initialize();
	virtual ~DummyCentroidInjector();
	virtual ReturnValue_t performOperation(uint8_t operationCode = 0);
	static const uint8_t INTERFACE_ID = CLASS_ID::CENTROID_WRITER_CLASS;
	static const uint8_t SUBSYSTEM_ID = SUBSYSTEM_ID::CENTROID_WRITER;
	static const Event WRITE_CENTROID = MAKE_EVENT (0, SEVERITY::INFO);
	static const ReturnValue_t CENTROID_OPS_OK= MAKE_RETURN_CODE(0x010);
private:
	//???
	uint8_t numberOfImages;
	std::ifstream centroidValues;
	ReturnValue_t PopulateInput();
	typedef std::pair<float,float> centroidPair_t;
	std::vector<centroidPair_t> centroidInput;
	std::vector<centroidPair_t>::iterator centroidIt;
	std::string path;

};

#endif /* DUMMYCENTROIDPROVIDER_H_ */
