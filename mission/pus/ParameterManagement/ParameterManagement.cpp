#include <fsfw/parameters/ParameterMessage.h>
#include <fsfw/parameters/ReceivesParameterMessagesIF.h>
#include <fsfw/serialize/SerializeAdapter.h>
#include "ParameterManagement.h"

ParameterService::ParameterService(object_id_t objectId, uint16_t apid, uint8_t serviceId,
		uint8_t numberOfParallel_Commands,uint16_t commandTimeout_Seconds):
			CommandingServiceBase(objectId, apid, serviceId,
					numberOfParallel_Commands, commandTimeout_Seconds){
}

ParameterService::~ParameterService(){

	}

ReturnValue_t ParameterService::isValidSubservice(uint8_t subservice){

	switch(static_cast<Subservice>(subservice)) {
		case Subservice::SET_PARAMETER:
			return HasReturnvaluesIF::RETURN_OK;
		default:
			return AcceptsTelecommandsIF::INVALID_SUBSERVICE;
	}
}


ReturnValue_t ParameterService::getMessageQueueAndObject(uint8_t subservice,
				const uint8_t *tcData, size_t tcDataLen, MessageQueueId_t *id,
				object_id_t *objectId){

	if(tcDataLen < sizeof(object_id_t)) {
		return CommandingServiceBase::INVALID_TC;
	}
	SerializeAdapter::deSerialize(objectId, &tcData,&tcDataLen, SerializeIF::Endianness::BIG);
	return checkInterfaceAndAcquireMessageQueue(id,objectId);
}

ReturnValue_t ParameterService::checkInterfaceAndAcquireMessageQueue(
		MessageQueueId_t* messageQueueToSet, object_id_t* objectId) {
	ReceivesParameterMessagesIF *object = objectManager->get<ReceivesParameterMessagesIF>(*objectId);
	sif::debug<<"service 20 called"<<std::endl;
	if (object == nullptr) {
		return CommandingServiceBase::INVALID_OBJECT;
	}
	*messageQueueToSet = object->getCommandQueue();
	return HasReturnvaluesIF::RETURN_OK;
}
 //prepareCommand
ReturnValue_t ParameterService:: prepareCommand(CommandMessage *message,
			uint8_t subservice, const uint8_t *tcData, size_t tcDataLen,
			uint32_t *state, object_id_t objectId){

    if(tcDataLen < sizeof(object_id_t) + sizeof(ParameterId_t)) {
        sif::debug << "ParameterService::prepareDirectCommand:"
                << " TC size smaller thant minimum size of command."
                << std::endl;
        return CommandingServiceBase::INVALID_TC;
    }

    	const uint8_t *pData = tcData + sizeof(object_id_t);
    	size_t size = tcDataLen - sizeof (object_id_t);

    	ParameterId_t parameterId;
    	ReturnValue_t result = SerializeAdapter::deSerialize(
    				&parameterId, &pData, &size,  SerializeIF::Endianness::BIG);
    		if (result != RETURN_OK) {
    			return result;
    		}

    	store_address_t parameterAddress;

    	result = IPCStore->addData(&parameterAddress, pData, size);
    	if(result != HasReturnvaluesIF::RETURN_OK){
    		return result;
    	}

    	ParameterMessage ::setParameterLoadCommand(message,parameterId,parameterAddress);
return result;
}

 ReturnValue_t ParameterService::handleReply(const CommandMessage *reply,
			Command_t previousCommand, uint32_t *state,
			CommandMessage *optionalNextCommand, object_id_t objectId,
			bool *isStep){
return INVALID_REPLY;
 }



