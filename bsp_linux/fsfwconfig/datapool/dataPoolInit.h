/*
* dataPoolInit.h
*
* 		 brief: Initialisation function for all variables in the data pool.
*      		This file was auto-generated by getDataPoolFromFLPmib.py via
*      		the flpmib database at 2018-06-04T12:02:46+00:00.
*/ 
#ifndef DATAPOOLINIT_H_
#define DATAPOOLINIT_H_

#include <fsfw/datapool/DataPool.h>

namespace datapool {
	enum opus_variable_id {
		NO_PARAMETER = 0x0,	//This PID is used to denote a non-existing param in the OBSW (NOPARAME), Size: 1
		Pointing_PoolId= 0x100010,
		PointingTime_PoolId= 0x100011,
		SensorData_PoolId= 0x100012
	};
}

void dataPoolInit( std::map<uint32_t, PoolEntryIF*>* pool_map );
#endif /* DATAPOOLINIT_H_ */
