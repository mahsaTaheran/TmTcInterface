/*
 * ESBOTcPolling.h
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#ifndef MISSION_UTILITY_ESBOTCPOLLING_H_
#define MISSION_UTILITY_ESBOTCPOLLING_H_

#include <fsfw/objectmanager/SystemObject.h>
#include "ESBOTmBridge.h"
#include <fsfw/tasks/ExecutableObjectIF.h>

#include <sys/socket.h>
#include <vector>

/**
 * @brief 	This class can be used to implement the polling of a Unix socket,
 * 			using UDP, for receiving TC packets sent from TMTC board.
 * @details
 * The task will be blocked while the specified number of bytes has not been
 * received, so TC reception is handled inside a separate task.
 * This class caches the IP address of the sender. It is assumed there
 * is only one sender for now.
 */
class ESBOTcPolling: public SystemObject,
		public ExecutableObjectIF {
	friend class ESBOTmBridge;
public:
	static constexpr size_t DEFAULT_MAX_FRAME_SIZE = 2048;

	//! 0.5  default milliseconds timeout for now.
	static constexpr timeval DEFAULT_TIMEOUT = {.tv_sec = 0, .tv_usec = 500};

	ESBOTcPolling(object_id_t objectId, object_id_t tmtcUnixUdpBridge,
			size_t frameSize = 0, double timeoutSeconds = -1);

	virtual~ ESBOTcPolling();

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
	StorageManagerIF* tcStore = nullptr;

private:
	//! TMTC bridge is cached.
	object_id_t tmtcBridgeId = objects::NO_OBJECT;
	ESBOTmBridge* tmtcBridge = nullptr;
	MessageQueueId_t targetTcDestination = MessageQueueIF::NO_QUEUE;
	//! Reception flags: https://linux.die.net/man/2/recvfrom.
	int receptionFlags = 0;

	//! Server socket, which is member of TMTC bridge and is assigned in
	//! constructor
	int serverUdpSocket = 0;

	std::vector<uint8_t> receptionBuffer;

	size_t frameSize = 0;
	timeval receptionTimeout;

	ReturnValue_t handleSuccessfullTcRead(size_t bytesRead);
	void handleReadError();
};

#endif /* MISSION_UTILITY_ESBOTCPOLLING_H_ */
