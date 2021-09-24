/*
 * MissionMessageTypes.cpp
 *
 *  Created on: 17.06.2016
 *      Author: baetz
 */

#include <fsfw/ipc/CommandMessageCleaner.h>

void messagetypes::clearMissionMessage(CommandMessage* message) {
	switch((message->getCommand()>>8) & 0xff){
	default:
		message->setCommand(CommandMessage::CMD_NONE);
		break;
	}
}
