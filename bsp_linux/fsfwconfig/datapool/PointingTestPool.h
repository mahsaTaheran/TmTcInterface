/*
 * pointingTestPool.h
 *
 *  Created on: Jul 21, 2021
 *      Author: mala
 */

#ifndef BSP_LINUX_FSFWCONFIG_DATAPOOL_POINTINGTESTPOOL_H_
#define BSP_LINUX_FSFWCONFIG_DATAPOOL_POINTINGTESTPOOL_H_

#include <stdint.h>
#include <hk/sid.h>

namespace pointingTestInit{

struct pointingHkStruct { //!< [EXPORT] : [DATA]
	sid32_t sid;
	struct ParameterList {
		uint32_t test_pointing;
		uint32_t test_time;
		uint32_t test_sensorData;
	};
	ParameterList p;
	uint8_t numberOfParameters = sizeof(p) / 4;
	float collectionInterval = 1;
	bool isDiagnosticsPacket = true;
	bool enablePacket = true;
};




/**
 * Initiate Pointing pool structs with datapool IDs for HK service

 */
void pointingHkStructInit(pointingHkStruct * hkStruct);

}







#endif /* BSP_LINUX_FSFWCONFIG_DATAPOOL_POINTINGTESTPOOL_H_ */
