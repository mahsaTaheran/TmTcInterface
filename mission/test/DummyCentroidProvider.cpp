/*
 * DummyCentroidProvider.cpp
 *
 *  Created on: May 1, 2021
 *      Author: mala
 */

#include <datapool/dataPoolInit.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <fsfw/tasks/TaskFactory.h>
#include <fsfw/datapool/DataPool.h>
#include <fsfw/datapool/PoolVariable.h>
#include <fsfw/datapool/DataSet.h>
#include "DummyCentroidProvider.h"
DummyCentroidInjector::DummyCentroidInjector(object_id_t objectId, std::string inputPath, uint8_t numberImages):
		SystemObject(objectId), path(inputPath),numberOfImages(numberImages) {

}

DummyCentroidInjector::~DummyCentroidInjector() {
}

ReturnValue_t DummyCentroidInjector::initialize(){
	sif::debug<<"injectorInit"<<std::endl;
	ReturnValue_t result = PopulateInput();
	if (result!=HasReturnvaluesIF::RETURN_OK){
		return result;
	}
	centroidIt= centroidInput.begin();
	return HasReturnvaluesIF::RETURN_OK;
}

ReturnValue_t DummyCentroidInjector::performOperation(uint8_t operationCode) {
	DataSet mySet;
	if (centroidIt==centroidInput.end()){
		centroidIt=centroidInput.begin();
	}
	//PoolVariable<uint32_t> dummy1(datapool::CENTROID_X,&mySet,PoolVariableIF::VAR_WRITE);
	//or pool vector?? depending on what is needed
	PoolVector<float,2> centroidData(datapool::Centroid_PoolId, &mySet, PoolVariableIF::VAR_WRITE);
	centroidData[0]= (*centroidIt).first;
	centroidData[1]=(*centroidIt).second;
	mySet.commit(PoolVariableIF::VALID);
	DataSet myTimeSet;
	timeval Time = Clock::getUptime();
	PoolVector<uint32_t,2> centroidTime(datapool::CentroidTime_PoolId, &myTimeSet,PoolVariableIF::VAR_WRITE);
	centroidTime[0]=Time.tv_sec;
	centroidTime[1] = Time.tv_usec;
	myTimeSet.commit(PoolVariableIF::VALID);
	sif::debug << "values were x "  << (*centroidIt).first <<"and y "  << (*centroidIt).second << std::endl;
	centroidIt++;
	triggerEvent(WRITE_CENTROID,numberOfImages);
	return CENTROID_OPS_OK;
}

ReturnValue_t DummyCentroidInjector::PopulateInput(){
	float valueX,valueY;
	uint8_t i;
	char dummy;
	centroidValues.open(path);

	//if error RETURN_FAILED
	//start loop until reaching empty line, or specfic number of images
	for (i=0; i<numberOfImages; i++){
		centroidValues >> valueX;
		centroidValues >> dummy;
		centroidValues >> valueY;
		centroidInput.push_back(centroidPair_t(valueX,valueY));
		sif::debug << "values were x "  << valueX <<"and y "  << valueY << std::endl;

	}
	centroidValues.close();
	return HasReturnvaluesIF::RETURN_OK;
}




