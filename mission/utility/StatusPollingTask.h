/*
 * StatusPollingTask.h
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#ifndef MISSION_UTILITY_STATUSPOLLINGTASK_H_
#define MISSION_UTILITY_STATUSPOLLINGTASK_H_
#include <fsfw/objectmanager/SystemObject.h>

#include <fsfw/tasks/ExecutableObjectIF.h>
#include <fsfw/ipc/MessageQueueIF.h>
#include <fsfw/storagemanager/StorageManagerIF.h>
#include <fsfw/tmtcservices/AcceptsTelecommandsIF.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <vector>

/**
 * @brief 	This class can be used to implement the polling of status and time packets
 * @details
 * status packets are send to a multicast address, the addresses are fixed n
 * in the TMTC interface specifications. After joining the network,
 * the performOperation method gets the packets periodically and send it to
 * packetHandler.

 */
class StatusPollingTask: public SystemObject,
		public ExecutableObjectIF {
	//friend class TmTcUnixUdpBridge;
public:
	static constexpr uint16_t DEFAULT_UDP_SERVER_PORT = 7301;
	//static constexpr uint16_t DEFAULT_UDP_CLIENT_PORT = 7302;

	static constexpr uint8_t STATUS_RECEPTION_QUEUE_DEPTH = 20;
	static constexpr size_t DEFAULT_MAX_FRAME_SIZE = 2048;
	//! 0.5  default milliseconds timeout for now.
	static constexpr timeval DEFAULT_TIMEOUT = {.tv_sec = 2, .tv_usec = 0};

	StatusPollingTask(object_id_t objectId,object_id_t statusDestination, object_id_t tcStoreId
			,uint16_t serverPort = 0xFFFF, size_t frameSize = 0, double timeoutSeconds = -1);
	virtual~ StatusPollingTask();

	/**
	 * Turn on optional timeout for UDP polling. In the default mode,
	 * the receive function will block until a packet is received.
	 * @param timeoutSeconds
	 */
	void setTimeout(double timeoutSeconds);

	virtual ReturnValue_t performOperation(uint8_t opCode) override;
	virtual ReturnValue_t initialize() override;
	virtual ReturnValue_t initializeAfterTaskCreation() override;

protected:
	object_id_t tcStoreId = objects::NO_OBJECT;
	object_id_t StatusDestination = objects::NO_OBJECT;
	StorageManagerIF* tcStore = nullptr;
	MessageQueueIF* StatusReceptionQueue = nullptr;

private:


	MessageQueueId_t targetTcDestination = MessageQueueIF::NO_QUEUE;
	//! Reception flags: https://linux.die.net/man/2/recvfrom.
	int receptionFlags = 0;

	std::vector<uint8_t> receptionBuffer;

	size_t frameSize = 0;
	timeval receptionTimeout;

	int serverSocket = 0;

	const int serverSocketOptions = 0;

	sockaddr_in serverAddress;
	socklen_t serverAddressLen = 0;

	bool ipAddrAnySet = false;

	ReturnValue_t handleSuccessfullStatusMessageRead(size_t bytesRead);
	void handleReadError();
	void handleSocketError();
	void handleBindError();
};




#endif /* MISSION_UTILITY_STATUSPOLLINGTASK_H_ */
