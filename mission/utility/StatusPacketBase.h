/*
 * StatusPacketBase.h
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#ifndef MISSION_UTILITY_STATUSPACKETBASE_H_
#define MISSION_UTILITY_STATUSPACKETBASE_H_

#include <fsfw/tmtcpacket/SpacePacketBase.h>
#include <cstddef>

/**
 * @brief   The base packet format of status and time packet from TMTC
 *
 * @details
 * These packets are sent to payload to sync time and set TMTC link.
 * Their format is close to space packet but the headers have deviations
 *
 * @ingroup utility
 */

struct StatusPacketPointer {
	CCSDSPrimaryHeader primary;
	uint8_t appData;
};

class StatusPacketBase : public SpacePacketBase {
public:


	StatusPacketBase( const uint8_t* setData );

	virtual ~StatusPacketBase();

	const uint8_t* getApplicationData() const;
	uint16_t getApplicationDataSize();
	uint16_t getErrorControl();
	void setErrorControl();
	static size_t calculateFullPacketLength(size_t appDataLen);

protected:
    /**
     * A pointer to a structure which defines the data structure of
     * the packet's data.
     *
     * To be hardware-safe, all elements are of byte size.
     */
    StatusPacketPointer* statusData;

    /* With this method, the packet data pointer can be redirected to another
    * location.
    * This call overwrites the parent's setData method to set both its
    * @c tc_data pointer and the parent's @c data pointer.
    *
    * @param p_data    A pointer to another PUS Telecommand Packet.
    */
   void setData( const uint8_t* pData );
};



#endif /* MISSION_UTILITY_STATUSPACKETBASE_H_ */
