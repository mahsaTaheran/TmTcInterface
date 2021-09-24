/*
 * sid.h
 *
 *  Created on: Jun 7, 2021
 *      Author: mala
 */

#ifndef BSP_LINUX_FSFWCONFIG_HK_SID_H_
#define BSP_LINUX_FSFWCONFIG_HK_SID_H_

#include <stdint.h>
/**
 * A set of data pool entries may be identified by a unique
 * structure ID called SID.
 */
typedef uint32_t sid32_t;

namespace sid {
	/**
	 * This number is the minimum sampling time and
	 * specifies the minimum unit used to specify how often HK or diagnostics packets
	 * are generated. Smaller size derived from HK Service periodicity of 200 ms.
	 * HK packets have a specified minimum sampling time of 1 second.
	 */
	static const float DIAG_MIN_INTERV_DIAGNOSTICS_SECONDS = 0.2;
	static const float DIAG_MIN_INTERV_HK_SECONDS = 1.0;
	static const uint8_t HK_INTERV_FACTOR = 5;

	static const uint8_t MAX_NUMBER_OF_LARGE_HK_DEFINITIONS = 10;
	static const uint8_t MAX_NUMBER_OF_SMALL_HK_DEFINITIONS = 50;
	static const uint8_t MAX_NUMBER_OF_DIAGNOSTICS_DEFINITIONS = 15;

	static const uint8_t MAX_NUMBER_OF_HK_DEFINITIONS = MAX_NUMBER_OF_LARGE_HK_DEFINITIONS
													  + MAX_NUMBER_OF_SMALL_HK_DEFINITIONS;

	static const uint8_t MAX_LARGE_HK_BUFFER_SIZE = 210;
	static const uint8_t MAX_SMALL_HK_BUFFER_SIZE = 50;

	enum structureIds {

		pointingHkTest = 0x4600,
	};

	uint32_t INTERVAL_SECONDS_TO_INTERVAL(bool isDiagnostics, float collectionInterval_seconds);
	float INTERVAL_TO_INTERVAL_SECONDS(bool isDiagnostics, uint32_t collectionInterval);
}




#endif /* BSP_LINUX_FSFWCONFIG_HK_SID_H_ */
