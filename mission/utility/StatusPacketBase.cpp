/*
 * StatusPacketBase.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */
#include "StatusPacketBase.h"

#include <fsfw/globalfunctions/CRC.h>
#include <fsfw/globalfunctions/arrayprinter.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>

#include <cstring>

StatusPacketBase::StatusPacketBase(const uint8_t* setData) :
		SpacePacketBase(setData) {
	statusData = reinterpret_cast<StatusPacketPointer*>(const_cast<uint8_t*>(setData));
}

StatusPacketBase::~StatusPacketBase() {

}
const uint8_t* StatusPacketBase::getApplicationData() const {
	return &statusData->appData;
}
uint16_t StatusPacketBase::getApplicationDataSize() {
	return getPacketDataLength() - CRC_SIZE + 1;
}

uint16_t StatusPacketBase::getErrorControl() {
	uint16_t size = getApplicationDataSize() + CRC_SIZE;
	uint8_t* p_to_buffer = &statusData->appData;
	return (p_to_buffer[size - 2] << 8) + p_to_buffer[size - 1];
}

void StatusPacketBase::setErrorControl() {
	uint32_t full_size = getFullSize();
	uint16_t crc = CRC::crc16ccitt(getWholeData(), full_size - CRC_SIZE);
	uint32_t size = getApplicationDataSize();
	(&statusData->appData)[size] = (crc & 0XFF00) >> 8;	// CRCH
	(&statusData->appData)[size + 1] = (crc) & 0X00FF; 		// CRCL
}

void StatusPacketBase::setData(const uint8_t* pData) {
	SpacePacketBase::setData(pData);
	statusData = (StatusPacketPointer*) pData;
}

size_t StatusPacketBase::calculateFullPacketLength(size_t appDataLen) {
	return sizeof(CCSDSPrimaryHeader) +
			appDataLen + StatusPacketBase::CRC_SIZE;
}


