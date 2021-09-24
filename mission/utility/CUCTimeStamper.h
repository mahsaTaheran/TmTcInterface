/*
 * CUCTimeStamper.h
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#ifndef MISSION_UTILITY_CUCTIMESTAMPER_H_
#define MISSION_UTILITY_CUCTIMESTAMPER_H_

#include <fsfw/timemanager/TimeStamperIF.h>
#include <fsfw/timemanager/CCSDSTime.h>
#include <fsfw/objectmanager/SystemObject.h>

/**
 * @brief   Time stamper which can be used to add a CUC timestamp to tm data buffer
 * @details
 * This time stamper uses the CUC timestamp according to the SSC RAMSES system.
 * This timestamp has a size of 6 bytes. A custom timestamp can be used by
 * overriding the #addTimeStamp function.
 * @ingroup utility
 */
class CUCTimeStamper: public TimeStamperIF, public SystemObject {
public:
	struct CUC{
		uint8_t pField;
		uint32_t seconds;
		uint16_t useconds;
	};
    /**
     * @brief   Default constructor which also registers the time stamper as a
     *          system object so it can be found with the #objectManager.
     * @param objectId
     */
	CUCTimeStamper(object_id_t objectId);

	/**
	 * Adds a CUC 6 byte timestamp to the given buffer.
	 * This function can be overriden to use a custom timestamp.
	 * @param buffer
	 * @param maxSize
	 * @return
	 */
	virtual ReturnValue_t addTimeStamp(uint8_t* buffer, const uint8_t maxSize);
private:
	static const uint32_t DAYS_CCSDS_TO_UNIX_EPOCH = 4383;
	static const uint32_t SECONDS_PER_DAY = 24 * 60 * 60;
	ReturnValue_t convertToCUC(CUC * to, timeval* from);
	uint16_t MicrosecondsToSubseconds(uint32_t microseconds);
};







#endif /* MISSION_UTILITY_CUCTIMESTAMPER_H_ */
