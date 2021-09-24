/*
 * hkInit.h
 *
 *  Created on: Jun 7, 2021
 *      Author: mala
 */

#ifndef BSP_LINUX_FSFWCONFIG_HK_HKINIT_H_
#define BSP_LINUX_FSFWCONFIG_HK_HKINIT_H_


#include <mission/pus/Service3HousekeepingPSB.h>
#include <datapool/deviceTestPool.h>
#include <datapool/PointingTestPool.h>

namespace hk {

struct hkIdStruct {
	//GpsInit::navDataIdStruct gps0;
	//GpsInit::navDataIdStruct gps1;
	//TestInit::TestIdStruct test;
	//deviceHkTestInit::TestHkIdStruct deviceHkTest;
	pointingTestInit::pointingHkStruct pointingHkTest;
};
/*
*
 * Initialise all pre-defined housekeeping definitions
 * @param housekeepingService Housekeeping Service 3 instance
 * @param hkIdStruct Contains all HK definitions as structs
*/



void initHkStruct(struct hkIdStruct * HkIdStruct, pointingTestInit::pointingHkStruct PointingHkTest);
void hkInit(Service3HousekeepingPSB * housekeepingService,
		struct hk::hkIdStruct hkIdStruct);
}

/*
 * Initiate HK ID struct with local ID structs
 * @param hkIdStruct
 * @param gps0
 * @param gps1
*/
/*
void initHkStruct(struct hkIdStruct * hkIdStruct,
		deviceHkTestInit::TestHkIdStruct deviceHkTest, pointingTestInit::pointingHkStruct PointingHkTest);
*/

//void initHkStruct(struct hkIdStruct * hkIdStruct, pointingTestInit::pointingHkAzimuthStruct PointingHkAzimuthTest, pointingTestInit::pointingHkElevationStruct PointingHkElevationTest);






#endif /* BSP_LINUX_FSFWCONFIG_HK_HKINIT_H_ */
