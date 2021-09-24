#ifndef PARAMETERSERVICE_H_
#define PARAMETERSERVICE_H_

#include <fsfw/tmtcservices/CommandingServiceBase.h>

class ParameterService : public CommandingServiceBase {

public:

	//static const uint8_t SUBSERVICE_SET_PARAMETER = 3;
	//static const uint8_t SUBSERVICE_REPORT_PARAMETER = 1;
	//static const uint8_t SUBSERVICE_TM_REPORT_PARAMETER = 2;

	ParameterService(object_id_t objectId, uint16_t apid, uint8_t serviceId,
					uint8_t numberOfParallel_Commands=4,
					uint16_t commadnTimeout_Seconds = 60);
	virtual ~ParameterService ();

protected:
// isValidSubservice
  ReturnValue_t isValidSubservice(uint8_t subservice) override;
//getMessageQueueAndObject
  ReturnValue_t getMessageQueueAndObject(uint8_t subservice,
			const uint8_t *tcData, size_t tcDataLen, MessageQueueId_t *id,
			object_id_t *objectId) override;

 //prepareCommand
ReturnValue_t prepareCommand(CommandMessage *message,
			uint8_t subservice, const uint8_t *tcData, size_t tcDataLen,
			uint32_t *state, object_id_t objectId) override;

 // handleReply*/
ReturnValue_t handleReply(const CommandMessage *reply,
			Command_t previousCommand, uint32_t *state,
			CommandMessage *optionalNextCommand, object_id_t objectId,
			bool *isStep) override;


private:
	enum class Subservice {
		SET_PARAMETER= 128,
	};

	ReturnValue_t checkInterfaceAndAcquireMessageQueue(
	        MessageQueueId_t* messageQueueToSet, object_id_t* objectId);
};

#endif
