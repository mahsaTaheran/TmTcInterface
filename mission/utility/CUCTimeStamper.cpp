/*
 * CUCTimeStamper.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#include "CUCTimeStamper.h"
#include <fsfw/timemanager/Clock.h>
#include <fsfw/serialize/EndianConverter.h>
#include <cstring>

CUCTimeStamper::CUCTimeStamper(object_id_t objectId): SystemObject(objectId) {}


ReturnValue_t CUCTimeStamper::addTimeStamp(uint8_t* buffer,
		const uint8_t maxSize) {
	if(maxSize < TimeStamperIF::MISSION_TIMESTAMP_SIZE){
		return HasReturnvaluesIF::RETURN_FAILED;
	}

	timeval now;
	CUC cucTimeNow;
	Clock::getClock_timeval(&now);
	//convert it to CUC- CCSDS epoch is used
	ReturnValue_t result = convertToCUC(&cucTimeNow,&now);
	if(result != HasReturnvaluesIF::RETURN_OK){
		return result;
	}

	//The P.Field is not used in this version of TM format

    uint32_t secs= EndianConverter::convertBigEndian(cucTimeNow.seconds);
	std::memcpy(buffer,&secs,sizeof(secs));
	buffer= buffer+sizeof(secs);
	//conversion necessary as some test systems were littleEndian
	uint16_t usecs= EndianConverter::convertBigEndian(cucTimeNow.useconds);
	std::memcpy(buffer,&usecs,sizeof(usecs));

	return result;
}

ReturnValue_t CUCTimeStamper::convertToCUC(CUC * to, timeval* from){
	//should be 00011110 here
	to->pField = CCSDSTime::P_FIELD_CUC_6B_CCSDS;

	//both should be uint32_t
	to->seconds= from->tv_sec;

	//change epoch to CCSDS from UNIX
	to->seconds+= DAYS_CCSDS_TO_UNIX_EPOCH * SECONDS_PER_DAY;

	//check the conversion!
	to->useconds= MicrosecondsToSubseconds(from->tv_usec);

	return HasReturnvaluesIF::RETURN_OK;

}

uint16_t CUCTimeStamper::MicrosecondsToSubseconds(uint32_t microseconds) {
	uint16_t temp = microseconds * (1 << (sizeof(uint16_t) * 8))/1000000;
	return temp;
}

