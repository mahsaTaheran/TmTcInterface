/*
 * StatusPacketStored.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */


#include "StatusPacketStored.h"
#include <fsfw/objectmanager/ObjectManagerIF.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>

#include <cstring>

StorageManagerIF* StatusPacketStored::store = nullptr;

StatusPacketStored::StatusPacketStored(store_address_t setAddress) :
		StatusPacketBase(nullptr), storeAddress(setAddress) {
	setStoreAddress(storeAddress);
}
StatusPacketStored::StatusPacketStored(): StatusPacketBase(nullptr) {
	this->storeAddress.raw = StorageManagerIF::INVALID_ADDRESS;
	this->checkAndSetStore();

}

ReturnValue_t StatusPacketStored::deletePacket() {
	ReturnValue_t result = this->store->deleteData(this->storeAddress);
	this->storeAddress.raw = StorageManagerIF::INVALID_ADDRESS;
	this->setData(nullptr);
	return result;
}

bool StatusPacketStored::checkAndSetStore() {
	if (this->store == nullptr) {
		this->store = objectManager->get<StorageManagerIF>(objects::TC_STORE);
		if (this->store == nullptr) {
			sif::error << "StatusPacketStored:: TC Store not found!"
					<< std::endl;
			return false;
		}
	}
	return true;
}

void StatusPacketStored::setStoreAddress(store_address_t setAddress) {
	this->storeAddress = setAddress;
	const uint8_t* tempData = nullptr;
	size_t temp_size;
	ReturnValue_t status = StorageManagerIF::RETURN_FAILED;
	if (this->checkAndSetStore()) {
		status = this->store->getData(this->storeAddress, &tempData,
				&temp_size);
	}
	if (status == StorageManagerIF::RETURN_OK) {
		this->setData(tempData);
	} else {
		this->setData(nullptr);
		this->storeAddress.raw = StorageManagerIF::INVALID_ADDRESS;
	}
}

ReturnValue_t StatusPacketStored::getData(const uint8_t ** dataPtr,
		size_t* dataSize) {
	auto result = this->store->getData(storeAddress, dataPtr, dataSize);
	if(result != HasReturnvaluesIF::RETURN_OK) {
		sif::warning << "StatusPacketStored: Could not get data!" << std::endl;
	}
	return result;
}
bool StatusPacketStored::isSizeCorrect() {
	const uint8_t* temp_data = nullptr;
	size_t temp_size;
	ReturnValue_t status = this->store->getData(this->storeAddress, &temp_data,
			&temp_size);
	if (status == StorageManagerIF::RETURN_OK) {
		if (this->getFullSize() == temp_size) {
			return true;
		}
	}
	return false;
}
