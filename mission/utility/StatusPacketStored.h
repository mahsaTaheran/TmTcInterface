/*
 * StatusPacketStored.h
 *
 *  Created on: May 31, 2021
 *      Author: mala
 */

#ifndef MISSION_UTILITY_STATUSPACKETSTORED_H_
#define MISSION_UTILITY_STATUSPACKETSTORED_H_


#include "StatusPacketBase.h"
#include <fsfw/storagemanager/StorageManagerIF.h>

/**
 * @brief  This class represents a stored status packet
 *
 * @details
 * to send the status packets from an object to another,
 *  this class would be used
 *
 * @ingroup utility
 */

class StatusPacketStored : public StatusPacketBase {
public:
	/**
	 * This is a default constructor which does not set the data pointer.
	 * However, it does try to set the packet store.
	 */
	StatusPacketStored();
	/**
	 * With this constructor, the class instance is linked to an existing
	 * packet in the packet store.
	 * The packet content is neither checked nor changed with this call. If
	 * the packet could not be found, the data pointer is set to NULL.
	 */
	StatusPacketStored( store_address_t setAddress );
	ReturnValue_t getData(const uint8_t ** dataPtr,
			size_t* dataSize);
	ReturnValue_t deletePacket();
	void setStoreAddress( store_address_t setAddress );
	bool isSizeCorrect();

private:
    /**
     * This is a pointer to the store all instances of the class use.
     * If the store is not yet set (i.e. @c store is NULL), every constructor
     * call tries to set it and throws an error message in case of failures.
     * The default store is objects::TC_STORE.
     */
    static StorageManagerIF* store;
    /**
     * The address where the packet data of the object instance is stored.
     */
    store_address_t storeAddress;
    /**
     * A helper method to check if a store is assigned to the class.
     * If not, the method tries to retrieve the store from the global
     * ObjectManager.
     * @return  @li @c true if the store is linked or could be created.
     *          @li @c false otherwise.
     */
    bool checkAndSetStore();
};

#endif /* MISSION_UTILITY_STATUSPACKETSTORED_H_ */
