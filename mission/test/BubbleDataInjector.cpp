/*
 * BubbleDataInjector.cpp
 *
 *  Created on: Sep 1, 2021
 *      Author: mala
 */

#include <datapool/dataPoolInit.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <fsfw/tasks/TaskFactory.h>
#include <fsfw/datapool/DataPool.h>
#include <fsfw/datapool/PoolVariable.h>
#include <fsfw/datapool/DataSet.h>

#include "BubbleDataInjector.h"

BubbleDataInjector::BubbleDataInjector(object_id_t objectID, std::string inputPath, int numberData):
	SystemObject(objectID), path(inputPath),numberOfData(numberData){

}

 BubbleDataInjector::~BubbleDataInjector(){

}

ReturnValue_t BubbleDataInjector::initialize(){

	ReturnValue_t result = PopulateInput();
	if (result!=HasReturnvaluesIF::RETURN_OK){
		return result;
	}

	dataIt = dataInput.begin();
	return HasReturnvaluesIF::RETURN_OK;
}

 ReturnValue_t BubbleDataInjector::performOperation(uint8_t operationCode){

	DataSet newData;


	if (dataIt==dataInput.end()){
		dataIt=dataInput.begin();
	}

	PoolVector<float,3> sensorData(datapool::SensorData_PoolId, &newData, PoolVariableIF::VAR_WRITE);

	sensorData[0]= (*dataIt).temp;
	sensorData[1]=(*dataIt).pressure;
	sensorData[2]= (*dataIt).alt;
	newData.commit(PoolVariableIF::VALID);

	//just for test, print the float
	//TODO: remove them later
	/*
	sif::debug << " sensor values were temp "  << sensorData[0] <<"and pressure "  << sensorData[1] << std::endl;
	float f = sensorData[0];
	int i = *(reinterpret_cast<int*>(&f));
	printf("%08x\n", i);
	 */
	dataIt++;

	return DATA_INJECTION_OPS_OK;
}

 ReturnValue_t BubbleDataInjector::PopulateInput(){
	//TODO: does not include error checking now, so always returns RETURN_OK
	 float valueX,valueY, valueZ;
	 uint8_t i;
	 char dummy;

	 sensorValues.open(path);
	 if (numberOfData==0){
		 std::string line;
		 while(getline(sensorValues,line)){
			 std::istringstream iss(line);
			 iss >> valueX;
			 iss >> dummy;
			 iss >> valueY;
			 iss >> dummy;
			 iss >> valueZ;
			 dataInput.push_back(sensorData_t(valueX,valueY,valueZ));
		 }
	 }else{
		 //start loop until reaching specific number of images
		 for (i=0; i<numberOfData; i++){
			 sensorValues >> valueX;
			 sensorValues >> dummy;
			 sensorValues >> valueY;
			 sensorValues >> dummy;
			 sensorValues >> valueZ;
			 dataInput.push_back(sensorData_t(valueX,valueY,valueZ));
		 }
	 }
	 sensorValues.close();
	 return HasReturnvaluesIF::RETURN_OK;
 }
