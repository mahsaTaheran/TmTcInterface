/*
 * StatusPacketHandler.cpp
 *
 *  Created on: May 27, 2021
 *      Author: mala
 */
#include "StatusPacketHandler.h"
#include "StatusPacketStored.h"
#include <fsfw/ipc/QueueFactory.h>

#include <fsfw/tcdistribution/CCSDSDistributorIF.h>
#include<fsfw/timemanager/CCSDSTime.h>

StatusPacketHandler::StatusPacketHandler(uint16_t setApid, object_id_t setObjectId,
		object_id_t setPacketSource):SystemObject(setObjectId),packetSource(setPacketSource), apid(setApid) {

	statusQueue = QueueFactory::instance()->
			createMessageQueue(STATUS_MAX_PACKETS);

}

StatusPacketHandler::~StatusPacketHandler() {
	QueueFactory::instance()->deleteMessageQueue(statusQueue);
}

ReturnValue_t StatusPacketHandler::initialize() {
	/*    currentPacket = new TcPacketStored();
    if(currentPacket == nullptr) {
        // Should not happen, memory allocation failed!
        return ObjectManagerIF::CHILD_INIT_FAILED;
    }*/

	CCSDSDistributorIF* ccsdsDistributor =
			objectManager->get<CCSDSDistributorIF>(packetSource);
	if (ccsdsDistributor == nullptr) {
		return HasReturnvaluesIF::RETURN_FAILED;
	}
	return ccsdsDistributor->registerApplication(this);
}
MessageQueueId_t StatusPacketHandler::getRequestQueue() {
	return statusQueue->getId();
}

uint16_t StatusPacketHandler::getIdentifier() {
	return apid;
}
ReturnValue_t StatusPacketHandler::performOperation(uint8_t opCode){

	//read messageQueue and write it to a packet

	ReturnValue_t status = HasReturnvaluesIF::RETURN_OK;
	for (status = statusQueue->receiveMessage(&currentPacket); status == HasReturnvaluesIF::RETURN_OK;
			status = statusQueue->receiveMessage(&currentPacket)) {
		status = handlePacket(currentPacket);
	}
	if (status == MessageQueueIF::EMPTY) {
		return HasReturnvaluesIF::RETURN_OK;
	} else {
		return status;
	}
}

ReturnValue_t StatusPacketHandler::handlePacket(TmTcMessage currentPacket){

	//TODO:check CRC of the packet??with isSizeCorrect()

	store_address_t address = currentPacket.getStorageId();
	StatusPacketStored packet(address);

	if (apid==1){
		return handleStatusPacket(packet.getApplicationData(), packet.getApplicationDataSize());

	}else if(apid==0){
		return handleTimePacket(packet.getApplicationData(), packet.getApplicationDataSize());
	}else{
		return HasReturnvaluesIF::RETURN_FAILED;
	}

}

ReturnValue_t StatusPacketHandler::handleStatusPacket(const uint8_t* statusData,uint16_t statusSize){
	uint8_t channel = *statusData;
	sif::debug <<"active channel is "<< channel<<std::endl;
	statusData++;
	statusData++;
	//TODO:now parse GNSS data:d
	return HasReturnvaluesIF::RETURN_OK;
}

ReturnValue_t StatusPacketHandler::handleTimePacket(const uint8_t* statusData,uint16_t statusSize){

	timeval currentTime;
	uint32_t size;
	ReturnValue_t result = CCSDSTime::convertFromCUC(&currentTime, statusData,
			&size, statusSize);
	//currentTime can be later sent to timeManager to sync
	sif::debug<<"time received is "<<currentTime.tv_sec<<" and "<<currentTime.tv_usec<<std::endl;
	return HasReturnvaluesIF::RETURN_OK;
}
