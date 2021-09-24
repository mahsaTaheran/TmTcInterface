/*
 * DummyPointingProvider.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: mala
 */


#include <datapool/dataPoolInit.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <fsfw/tasks/TaskFactory.h>
#include <fsfw/datapool/DataPool.h>
#include <fsfw/datapool/PoolVariable.h>
#include <fsfw/datapool/DataSet.h>
#include "DummyPointingProvider.h"

DummyPointingProvider::DummyPointingProvider(object_id_t objectId, std::string inputPath, int numberPointings):
		SystemObject(objectId), path(inputPath),numberOfPointings(numberPointings) {

}

DummyPointingProvider::~DummyPointingProvider() {
}

ReturnValue_t DummyPointingProvider::initialize(){
	sif::debug<<"pointing Provider Init"<<std::endl;
	ReturnValue_t result = PopulateInput();
	if (result!=HasReturnvaluesIF::RETURN_OK){
		return result;
	}
	pointingIt= pointingInput.begin();
	return HasReturnvaluesIF::RETURN_OK;
}

ReturnValue_t DummyPointingProvider::performOperation(uint8_t operationCode) {
	DataSet newPointing;

	if (pointingIt==pointingInput.end()){
		pointingIt=pointingInput.begin();
	}

	PoolVector<float,2> pointing(datapool::Pointing_PoolId, &newPointing, PoolVariableIF::VAR_WRITE);

	pointing[0]= (*pointingIt).first;
	pointing[1]=(*pointingIt).second;

	newPointing.commit(PoolVariableIF::VALID);


	//checking the float value just for test
	//TODO: remove in final code
	/*
	sif::debug << " pointing values were x "  << pointing[0] <<"and y "  << pointing[1] << std::endl;
	float f = pointing[0];
	int i = *(reinterpret_cast<int*>(&f));
	printf("%08x\n", i);
	 */

	DataSet myTimeSet;
	timeval Time = Clock::getUptime();
	PoolVector<uint32_t,2> pointingTime(datapool::PointingTime_PoolId, &myTimeSet,PoolVariableIF::VAR_WRITE);
	pointingTime[0]=Time.tv_sec;
	pointingTime[1] = Time.tv_usec;
	myTimeSet.commit(PoolVariableIF::VALID);
	pointingIt++;
	//TODO: in case service 5 is active on ground
	//triggerEvent(WRITE_POINTING,numberOfPointings);
	return POINTING_OPS_OK;
}

ReturnValue_t DummyPointingProvider::PopulateInput(){
	float valueX,valueY;
	uint8_t i;
	char dummy;

	pointingValues.open(path);
	if (numberOfPointings==0){
		std::string line;
		while(getline(pointingValues,line)){
			std::istringstream iss(line);
			iss >> valueX;
			iss >> dummy;
			iss >> valueY;
			pointingInput.push_back(pointingPair_t(valueX,valueY));
		}
	}else{

	//start loop until reaching specific number of images
		for (i=0; i<numberOfPointings; i++){
			pointingValues >> valueX;
			pointingValues >> dummy;
			pointingValues >> valueY;
			pointingInput.push_back(pointingPair_t(valueX,valueY));
		}
	}

	sif::debug<< "size of input" << pointingInput.size()<<std::endl;

	pointingValues.close();

	return HasReturnvaluesIF::RETURN_OK;
}



