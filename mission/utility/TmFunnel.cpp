#include <fsfw/ipc/QueueFactory.h>
#include <fsfw/tmtcpacket/pus/TmPacketBase.h>
#include <fsfw/tmtcpacket/pus/TmPacketStored.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <mission/utility/TmFunnel.h>

object_id_t TmFunnel::downlinkDestination = objects::NO_OBJECT;
object_id_t TmFunnel::storageDestination = objects::NO_OBJECT;

TmFunnel::TmFunnel(object_id_t objectId, uint32_t messageDepth):
        SystemObject(objectId), messageDepth(messageDepth) {
	tmQueue = QueueFactory::instance()->createMessageQueue(messageDepth,
	        MessageQueueMessage::MAX_MESSAGE_SIZE);
	storageQueue = QueueFactory::instance()->createMessageQueue(messageDepth,
	        MessageQueueMessage::MAX_MESSAGE_SIZE);
}

TmFunnel::~TmFunnel() {
}

MessageQueueId_t TmFunnel::getReportReceptionQueue(uint8_t virtualChannel) {
	return tmQueue->getId();
}

ReturnValue_t TmFunnel::performOperation(uint8_t operationCode) {
	TmTcMessage currentMessage;
	ReturnValue_t status = tmQueue->receiveMessage(&currentMessage);
	while(status == HasReturnvaluesIF::RETURN_OK)
	{
		status = handlePacket(&currentMessage);
		if(status != HasReturnvaluesIF::RETURN_OK){
			break;
		}
		status = tmQueue->receiveMessage(&currentMessage);
	}

	if (status == MessageQueueIF::EMPTY) {
		return HasReturnvaluesIF::RETURN_OK;
	}
	else {
		return status;
	}
}

ReturnValue_t TmFunnel::handlePacket(TmTcMessage* message) {
	uint8_t* packetData = nullptr;
	size_t size = 0;
	ReturnValue_t result = tmPool->modifyData(message->getStorageId(),
	        &packetData, &size);
	if(result != HasReturnvaluesIF::RETURN_OK){
		return result;
	}
	TmPacketBase packet(packetData);
	packet.setPacketSequenceCount(this->sourceSequenceCount);
	sourceSequenceCount++;
	sourceSequenceCount = sourceSequenceCount %
	        SpacePacketBase::LIMIT_SEQUENCE_COUNT;
	packet.setErrorControl();
	packet.print();
	result = tmQueue->sendToDefault(message);
	if(result != HasReturnvaluesIF::RETURN_OK){
		tmPool->deleteData(message->getStorageId());
		sif::error << "TmFunnel::handlePacket: Error sending to downlink "
				"handler" << std::endl;
		return result;
	}

	if(storageDestination != objects::NO_OBJECT) {
		result = storageQueue->sendToDefault(message);
		if(result != HasReturnvaluesIF::RETURN_OK){
			tmPool->deleteData(message->getStorageId());
			sif::error << "TmFunnel::handlePacket: Error sending to storage "
					"handler" << std::endl;
			return result;
		}
	}
	return result;
}

ReturnValue_t TmFunnel::initialize() {

	tmPool = objectManager->get<StorageManagerIF>(objects::TM_STORE);
	if(tmPool == nullptr) {
		sif::error << "TmFunnel::initialize: TM store not set."
				<< std::endl;
		sif::error << "Make sure the tm store is set up properly"
				" and implements StorageManagerIF" << std::endl;
		return ObjectManagerIF::CHILD_INIT_FAILED;
	}

	AcceptsTelemetryIF* tmTarget =
	        objectManager->get<AcceptsTelemetryIF>(downlinkDestination);
	if(tmTarget == nullptr){
		sif::error << "TmFunnel::initialize: Downlink Destination not set."
				<< std::endl;
		sif::error << "Make sure the downlink destination object is set up "
				"properly and implements AcceptsTelemetryIF" << std::endl;
		return ObjectManagerIF::CHILD_INIT_FAILED;
	}
	tmQueue->setDefaultDestination(tmTarget->getReportReceptionQueue());

	// Storage destination is optional.
	if(storageDestination == objects::NO_OBJECT) {
	    return SystemObject::initialize();
	}

	AcceptsTelemetryIF* storageTarget =
	        objectManager->get<AcceptsTelemetryIF>(storageDestination);
	if(storageTarget != nullptr) {
		storageQueue->setDefaultDestination(
		        storageTarget->getReportReceptionQueue());
	}

	return SystemObject::initialize();
}
