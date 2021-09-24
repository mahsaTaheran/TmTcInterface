/*
 * ESBOTmBridge.h
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#ifndef MISSION_UTILITY_ESBOTMBRIDGE_H_
#define MISSION_UTILITY_ESBOTMBRIDGE_H_

#include <fsfw/tmtcservices/AcceptsTelecommandsIF.h>
#include <fsfw/tmtcservices/TmTcBridge.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>


/**
 * @brief   Tm Bridge that sends TM packets over UDP to TMTC
 *
 * @details
 * This bridge uses the interface specification defined by SSC , the addresses
 * are fixed, and only change in case the LOS is lost and BLOS is connected
 * this change can be done either by checking the client address from TC link,
 * or by getting an event from the status packet handler.
 *
 * @ingroup utility
 */
class ESBOTmBridge: public TmTcBridge {
	friend class ESBOTcPolling;
public:
	// The ports chosen here should not be used by any other process.
	// List of used ports on Linux: /etc/services
	static constexpr uint16_t DEFAULT_UDP_SERVER_PORT = 7301;
	static constexpr uint16_t DEFAULT_UDP_CLIENT_PORT = 7302;

	ESBOTmBridge(object_id_t objectId, object_id_t tcDestination,
			object_id_t tmStoreId, object_id_t tcStoreId,
			uint16_t serverPort = 0xFFFF,uint16_t clientPort = 0xFFFF);
	virtual~ ESBOTmBridge();

	void checkAndSetClientAddress(sockaddr_in& clientAddress);

	void setClientAddressToAny(bool ipAddrAnySet);
protected:
	virtual ReturnValue_t sendTm(const uint8_t * data, size_t dataLen) override;

private:
	int serverSocket = 0;

	const int serverSocketOptions = 0;

	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen = 0;

	struct sockaddr_in serverAddress;
	socklen_t serverAddressLen = 0;

	bool ipAddrAnySet = false;

	//! Access to the client address is mutex protected as it is set
	//! by another task.
	MutexIF* mutex;

	void handleSocketError();
	void handleBindError();
	void handleSendError();
};



#endif /* MISSION_UTILITY_ESBOTMBRIDGE_H_ */
