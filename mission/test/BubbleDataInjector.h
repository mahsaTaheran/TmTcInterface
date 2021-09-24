/*
 * BubbleDataInjector.h
 *
 *  Created on: Sep 1, 2021
 *      Author: mala
 */

#ifndef MISSION_TEST_BUBBLEDATAINJECTOR_H_
#define MISSION_TEST_BUBBLEDATAINJECTOR_H_

#include <vector>
#include<fstream>
#include <fsfw/tasks/ExecutableObjectIF.h>
#include <fsfw/objectmanager/SystemObject.h>
#include <bsp_linux/fsfwconfig/returnvalues/classIds.h>
#include <bsp_linux/fsfwconfig/events/subsystemIdRanges.h>
class BubbleDataInjector : public ExecutableObjectIF, public SystemObject {

/**********************
 * This class is mostly used for tests, and in particular for tests of the visualizer ground
 * component. Given that the TMTC board keeps packets until buffer is filled, these data is written
 * also to the same packet as pointing data
 * data is from the previous bubble launch, and includes alt.temp. and pressure
 */

public:
	BubbleDataInjector(object_id_t objectID, std::string inputPath, int numberData=0);
	ReturnValue_t initialize();
	virtual ~BubbleDataInjector();
	virtual ReturnValue_t performOperation(uint8_t operationCode = 0);
	static const uint8_t INTERFACE_ID = CLASS_ID::SENSOR_WRITER_CLASS;
	static const uint8_t SUBSYSTEM_ID = SUBSYSTEM_ID::SENSOR_WRITER;
	static const Event WRITE_POINTING = MAKE_EVENT (0, SEVERITY::INFO);
	static const ReturnValue_t DATA_INJECTION_OPS_OK= MAKE_RETURN_CODE(0x010);
private:
	//???
	uint16_t numberOfData;
	std::ifstream sensorValues;
	ReturnValue_t PopulateInput();
	struct sensorData_t{
		sensorData_t(float x, float y, float z):temp(x), pressure(y), alt(z){}
		float pressure;
		float temp;
		float alt;
	};

	std::vector<sensorData_t> dataInput;
	std::vector<sensorData_t>::iterator dataIt;
	std::string path;

};




#endif /* MISSION_TEST_BUBBLEDATAINJECTOR_H_ */
