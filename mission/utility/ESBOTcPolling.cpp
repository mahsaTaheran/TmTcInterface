/*
 * ESBOTcPolling.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#include "ESBOTcPolling.h"
#include <fsfw/globalfunctions/arrayprinter.h>

ESBOTcPolling::ESBOTcPolling(object_id_t objectId,
		object_id_t tmtcUnixUdpBridge, size_t frameSize,
		double timeoutSeconds): SystemObject(objectId),
		tmtcBridgeId(tmtcUnixUdpBridge) {

	if(frameSize > 0) {
		this->frameSize = frameSize;
	}
	else {
		this->frameSize = DEFAULT_MAX_FRAME_SIZE;
	}

	// Set up reception buffer with specified frame size.
	// For now, it is assumed that only one frame is held in the buffer!
	receptionBuffer.reserve(this->frameSize);
	receptionBuffer.resize(this->frameSize);

	if(timeoutSeconds == -1) {
		receptionTimeout = DEFAULT_TIMEOUT;
	}
	else {
		receptionTimeout = timevalOperations::toTimeval(timeoutSeconds);
	}
}

ESBOTcPolling::~ESBOTcPolling() {}

ReturnValue_t ESBOTcPolling::performOperation(uint8_t opCode) {
	// Poll for new UDP datagrams in permanent loop.
	while(1) {

		//! Sender Address is cached here.
		struct sockaddr_in senderAddress;
		char testBuffer[1024];
		socklen_t senderSockLen = sizeof(senderAddress);
		ssize_t bytesReceived = recvfrom(serverUdpSocket,
				testBuffer, frameSize, receptionFlags,
				reinterpret_cast<sockaddr*>(&senderAddress), &senderSockLen);

		/*receptionBuffer.data()*/
		if(bytesReceived < 0) {
			// handle error
			sif::error << "ESBOTcPolling::performOperation: Reception"
					"error." << std::endl;
			handleReadError();

			continue;
		}

		ReturnValue_t result = handleSuccessfullTcRead(bytesReceived);
		if(result != HasReturnvaluesIF::RETURN_FAILED) {

		}
		//tmtcBridge->registerCommConnect();
		//tmtcBridge->checkAndSetClientAddress(senderAddress);
	}
	return HasReturnvaluesIF::RETURN_OK;
}


ReturnValue_t ESBOTcPolling::handleSuccessfullTcRead(size_t bytesRead) {

	//store message first

	store_address_t storeId;
	ReturnValue_t result = tcStore->addData(&storeId,
			receptionBuffer.data(), bytesRead);
	// arrayprinter::print(receptionBuffer.data(), bytesRead);

	if (result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "ESBOTcPolling::transferPusToSoftwareBus: Data "
				"storage failed" << std::endl;
		sif::error << "Packet size: " << bytesRead << std::endl;
		return HasReturnvaluesIF::RETURN_FAILED;
	}

	//send message to distributor

	TmTcMessage message(storeId);
	result  = MessageQueueSenderIF::sendMessage(targetTcDestination, &message);
	if (result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "ESBBoTcPolling: Sending message to queue failed"
				<< std::endl;
		tcStore->deleteData(storeId);
	}
	return result;
}

ReturnValue_t ESBOTcPolling::initialize() {
	tcStore = objectManager->get<StorageManagerIF>(objects::TC_STORE);
	if (tcStore == nullptr) {
		sif::error << "ESBBoTcPolling::TC Store uninitialized!"
				<< std::endl;
		return ObjectManagerIF::CHILD_INIT_FAILED;
	}

	tmtcBridge = objectManager->get<ESBOTmBridge>(tmtcBridgeId);
	if(tmtcBridge == nullptr) {
		sif::error << "ESBBoTcPolling::Invalid"
				" TMTC bridge object!" << std::endl;
		return ObjectManagerIF::CHILD_INIT_FAILED;
	}

	serverUdpSocket = tmtcBridge->serverSocket;

	return HasReturnvaluesIF::RETURN_OK;
}

ReturnValue_t ESBOTcPolling::initializeAfterTaskCreation() {
	// Initialize the destination after task creation. This ensures
	// that the destination will be set in the TMTC bridge.
	targetTcDestination = tmtcBridge->getRequestQueue();
	return HasReturnvaluesIF::RETURN_OK;
}

void ESBOTcPolling::setTimeout(double timeoutSeconds) {
	timeval tval;
	tval = timevalOperations::toTimeval(timeoutSeconds);
	int result = setsockopt(serverUdpSocket, SOL_SOCKET, SO_RCVTIMEO,
			&tval, sizeof(receptionTimeout));
	if(result == -1) {
		sif::error << "ESBBoTcPolling: Setting "
				"receive timeout failed with " << strerror(errno) << std::endl;
	}
}

// TODO: sleep after error detection to prevent spam
void ESBOTcPolling::handleReadError() {
	switch(errno) {
	case(EAGAIN): {
		// todo: When working in timeout mode, this will occur more often
		// and is not an error.
		sif::error << "ESBBoTcPolling::handleReadError: Timeout."
				<< std::endl;
		break;
	}
	default: {
		sif::error << "ESBBoTcPolling::handleReadError: "
				<< strerror(errno) << std::endl;
	}
	}
}



