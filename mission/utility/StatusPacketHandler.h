/*
 * StatusPacketHandler.h
 *
 *  Created on: May 27, 2021
 *      Author: mala
 */

#ifndef MISSION_UTILITY_STATUSPACKETHANDLER_H_
#define MISSION_UTILITY_STATUSPACKETHANDLER_H_

#include <fsfw/tmtcservices/AcceptsTelecommandsIF.h>
#include <fsfw/ipc/MessageQueueIF.h>
#include <fsfw/tasks/ExecutableObjectIF.h>
#include <fsfw/tmtcservices/TmTcMessage.h>
#include <fsfw/objectmanager/SystemObject.h>

/**
 * @brief   The class receiving status packets from TMTC board
 *
 * @details
 * The performOpertaion method is called periodically to read the packets
 * depending on apid in the packet, it is a status or time, and would be handled separately
 *
 * @ingroup utility
 */
class StatusPacketHandler: public SystemObject,public AcceptsTelecommandsIF, public ExecutableObjectIF {

public:

	StatusPacketHandler(uint16_t setApid, object_id_t setObjectId,
			object_id_t setPacketSource);
	virtual ~StatusPacketHandler();
	MessageQueueId_t getRequestQueue() override;
	ReturnValue_t initialize() override;
	uint16_t getIdentifier() override;
	ReturnValue_t performOperation(uint8_t opCode);


private:
	TmTcMessage currentPacket;
	const object_id_t packetSource;
	MessageQueueIF* statusQueue = nullptr;
	uint16_t apid;
	static constexpr uint8_t STATUS_MAX_PACKETS = 16;
	ReturnValue_t handlePacket(TmTcMessage packet);
	ReturnValue_t handleStatusPacket(const uint8_t* statusData,uint16_t statusSize);
	ReturnValue_t handleTimePacket(const uint8_t* statusData,uint16_t statusSize);
};



#endif /* MISSION_UTILITY_STATUSPACKETHANDLER_H_ */
