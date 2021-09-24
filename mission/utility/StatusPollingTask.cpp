/*
 * StatusPollingTask.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#include "StatusPollingTask.h"
#include <fsfw/globalfunctions/arrayprinter.h>
#include <fsfw/ipc/QueueFactory.h>
#include <fsfw/tmtcservices/TmTcMessage.h>

#include <errno.h>
#include <arpa/inet.h>

StatusPollingTask::StatusPollingTask(object_id_t objectId,object_id_t statusDestination, object_id_t tcStoreId,
		uint16_t serverPort,size_t frameSize,double timeoutSeconds
): SystemObject(objectId),tcStoreId(tcStoreId)
,StatusDestination(statusDestination) {

	StatusReceptionQueue = QueueFactory::instance()->
			createMessageQueue(STATUS_RECEPTION_QUEUE_DEPTH);

	uint16_t setServerPort = DEFAULT_UDP_SERVER_PORT;
	if(serverPort != 0xFFFF) {
		setServerPort = serverPort;
	}

	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(serverSocket < 0) {
		sif::error << "StatusPollingTask:: Could not open"
				" UDP socket!" << std::endl;
		handleSocketError();
		return;
	}

	serverAddress.sin_family = AF_INET;

	//serverAddress.sin_addr.s_addr = inet_addr("192.168.197.150");
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	//serverAddress.sin_addr.s_addr=inet_addr("239.255.197.20");
	serverAddress.sin_port = htons(setServerPort);
	serverAddressLen = sizeof(serverAddress);

	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &serverSocketOptions,
			sizeof(serverSocketOptions));
	int result = bind(serverSocket,
			reinterpret_cast<struct sockaddr*>(&serverAddress),
			serverAddressLen);
	if(result == -1) {
		sif::error << "StatusPollingTask:: Could not bind "
				"local port " << setServerPort << " to server socket!"
				<< std::endl;
		handleBindError();
		return;
	}


	if(frameSize > 0) {
		this->frameSize = frameSize;
	}
	else {
		this->frameSize = DEFAULT_MAX_FRAME_SIZE;
	}

	//join multicast
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr("239.255.197.20");
	//mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	mreq.imr_interface.s_addr = inet_addr("192.168.197.150");
	setsockopt(serverSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));

	// Set up reception buffer with specified frame size.
	// For now, it is assumed that only one frame is held in the buffer!
	receptionBuffer.reserve(this->frameSize);
	receptionBuffer.resize(this->frameSize);
	setTimeout(timeoutSeconds);
}

StatusPollingTask::~StatusPollingTask() {}

ReturnValue_t StatusPollingTask::performOperation(uint8_t opCode) {
	// Poll for new UDP datagrams in permanent loop.
	while(1) {


		char testBuffer[2048];

		//TODO: caching the sender address is here unnecessary.Remove
		struct sockaddr_in senderAddress;
		socklen_t senderAddressLen = sizeof(senderAddress);
		ssize_t bytesReceived = recvfrom(serverSocket,
				testBuffer, frameSize, receptionFlags,
				reinterpret_cast<sockaddr*>(&senderAddress), &senderAddressLen);
		if(bytesReceived < 0) {

			sif::error << "StatusPollingTask:: Reception"
					"error." << std::endl;
			handleReadError();

			continue;
		}

		ReturnValue_t result = handleSuccessfullStatusMessageRead(bytesReceived);
		if(result != HasReturnvaluesIF::RETURN_FAILED) {

		}
	}
	return HasReturnvaluesIF::RETURN_OK;
}


ReturnValue_t StatusPollingTask::handleSuccessfullStatusMessageRead(size_t bytesRead) {

	store_address_t storeId;
	ReturnValue_t result = tcStore->addData(&storeId,
			receptionBuffer.data(), bytesRead);

	if (result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "StatusPollingTask::transferPusToSoftwareBus: Data "
				"storage failed" << std::endl;
		sif::error << "Packet size: " << bytesRead << std::endl;
		return HasReturnvaluesIF::RETURN_FAILED;
	}

	TmTcMessage message(storeId);

	result  = MessageQueueSenderIF::sendMessage(targetTcDestination, &message);
	if (result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "StatusPollingTask:: Sending message to queue failed"
				<< std::endl;
		tcStore->deleteData(storeId);
	}
	return result;
}

ReturnValue_t StatusPollingTask::initialize() {
	tcStore = objectManager->get<StorageManagerIF>(objects::TC_STORE);
	if (tcStore == nullptr) {
		sif::error << "StatusPollingTask::initialize: TC Store uninitialized!"
				<< std::endl;
		return ObjectManagerIF::CHILD_INIT_FAILED;
	}
	AcceptsTelecommandsIF* StatusDistributor =
			objectManager->get<AcceptsTelecommandsIF>(StatusDestination);
	if (StatusDistributor == nullptr) {
		sif::error << "StatusPollingTask:: TC Distributor invalid"
				<< std::endl;
		return ObjectManagerIF::CHILD_INIT_FAILED;
	}

	StatusReceptionQueue->setDefaultDestination(StatusDistributor->getRequestQueue());
	return HasReturnvaluesIF::RETURN_OK;
}

ReturnValue_t StatusPollingTask::initializeAfterTaskCreation() {
	targetTcDestination = StatusReceptionQueue->getDefaultDestination();
	return HasReturnvaluesIF::RETURN_OK;
}

void StatusPollingTask::setTimeout(double timeoutSeconds) {

	if(timeoutSeconds == -1) {
		receptionTimeout = DEFAULT_TIMEOUT;
	}else{

		receptionTimeout = timevalOperations::toTimeval(timeoutSeconds);
	}
	int result = setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO,
			&receptionTimeout, sizeof(receptionTimeout));
	if(result == -1) {
		sif::error << "StatusPollingTask:: Setting "
				"receive timeout failed with " << strerror(errno) << std::endl;
	}
}

// TODO: sleep after error detection to prevent spam
void StatusPollingTask::handleReadError() {
	switch(errno) {
	case(EAGAIN): {
		// todo: When working in timeout mode, this will occur more often
		// and is not an error.
		sif::error << "StatusPollingTask::handleReadError: Timeout."
				<< std::endl;
		break;
	}
	default: {
		sif::error << "StatusPollingTask::handleReadError: "
				<< strerror(errno) << std::endl;
	}
	}
}

void StatusPollingTask::handleSocketError() {
	// See: https://man7.org/linux/man-pages/man2/socket.2.html
	switch(errno) {
	case(EACCES):
	case(EINVAL):
	case(EMFILE):
	case(ENFILE):
	case(EAFNOSUPPORT):
	case(ENOBUFS):
	case(ENOMEM):
	case(EPROTONOSUPPORT):
	sif::error << "StatusPollingTask:: Socket creation failed"
	<< " with " << strerror(errno) << std::endl;
	break;
	default:
		sif::error << "StatusPollingTask:: handleSocketError:Unknown error"
		<< std::endl;
		break;
	}
}

void StatusPollingTask::handleBindError() {
	// See: https://man7.org/linux/man-pages/man2/bind.2.html
	switch(errno) {
	case(EACCES): {
		/*
		 Ephermeral ports can be shown with following command:
		 sysctl -A | grep ip_local_port_range
		 */
		sif::error << "StatusPollingTask::handleBindError: Port access issue."
				"Ports 1-1024 are reserved on UNIX systems and require root "
				"rights while ephermeral ports should not be used as well."
				<< std::endl;
	}
	break;
	case(EADDRINUSE):
	case(EBADF):
	case(EINVAL):
	case(ENOTSOCK):
	case(EADDRNOTAVAIL):
	case(EFAULT):
	case(ELOOP):
	case(ENAMETOOLONG):
	case(ENOENT):
	case(ENOMEM):
	case(ENOTDIR):
	case(EROFS): {
		sif::error << "StatusPollingTask::handleBindError: Socket creation failed"
				<< " with " << strerror(errno) << std::endl;
		break;
	}
	default:
		sif::error << "StatusPollingTask::handleBindError: Unknown error"
		<< std::endl;
		break;
	}
}



