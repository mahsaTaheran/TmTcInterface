/*
* dataPoolInit.cpp
*
* 		 brief: Initialisation function for all variables in the data pool.
*      		This file was auto-generated by getDataPoolFromFLPmib.py via
*      		the flpmib database at 2018-06-04T12:02:46+00:00.
*/ 
#include "dataPoolInit.h"

void dataPoolInit(std::map<uint32_t, PoolEntryIF*>* pool_map) {


	float t_Pointing_PoolId[2] = {0,0};
		pool_map->insert(
				std::pair<uint32_t, PoolEntryIF*>(datapool::Pointing_PoolId,
						new PoolEntry <float>(t_Pointing_PoolId, 2)));

	uint32_t t_PointingTime_PoolId[2] = {0,0};
	pool_map->insert(
			std::pair<uint32_t, PoolEntryIF*>(datapool::PointingTime_PoolId,
					new PoolEntry<uint32_t>(t_PointingTime_PoolId, 2)));

	float t_SensorData_PoolId[3] = {0,0,0};
			pool_map->insert(
					std::pair<uint32_t, PoolEntryIF*>(datapool::SensorData_PoolId,
							new PoolEntry <float>(t_SensorData_PoolId, 3)));
}
