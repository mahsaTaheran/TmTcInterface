#include "ObjectFactory.h"

#include <OBSWConfig.h>
#include <objects/systemObjectList.h>
#include <datapool/dataPoolInit.h>
#include <tmtc/apid.h>
#include <tmtc/pusIds.h>

#include <mission/utility/TmFunnel.h>
#include <mission/core/GenericFactory.h>
#include <fsfw/monitoring/MonitoringMessageContent.h>

#include <fsfw/storagemanager/PoolManager.h>
#include <fsfw/tmtcpacket/pus/TmPacketStored.h>
#include <fsfw/tmtcservices/CommandingServiceBase.h>
#include <fsfw/tmtcservices/PusServiceBase.h>

#if OBSW_ADD_TEST_CODE == 1
#include <mission/test/TestTask.h>
#endif

void Factory::setStaticFrameworkObjectIds(){
	MonitoringReportContent<float>::timeStamperId = objects::CUC_TIME_STAMPER;
	MonitoringReportContent<double>::timeStamperId = objects::CUC_TIME_STAMPER;
	MonitoringReportContent<uint32_t>::timeStamperId = objects::CUC_TIME_STAMPER;
	MonitoringReportContent<int32_t>::timeStamperId = objects::CUC_TIME_STAMPER;
	MonitoringReportContent<int16_t>::timeStamperId = objects::CUC_TIME_STAMPER;
	MonitoringReportContent<uint16_t>::timeStamperId = objects::CUC_TIME_STAMPER;

	TmFunnel::downlinkDestination = objects::ESBO_TM;
	// No storage object for now.
	TmFunnel::storageDestination = objects::NO_OBJECT;

	PusServiceBase::packetSource = objects::PUS_DISTRIBUTOR;
	PusServiceBase::packetDestination = objects::TM_FUNNEL;

	CommandingServiceBase::defaultPacketSource = objects::PUS_DISTRIBUTOR;
	CommandingServiceBase::defaultPacketDestination = objects::TM_FUNNEL;

	VerificationReporter::messageReceiver = objects::PUS_SERVICE_1_VERIFICATION;

	TmPacketStored::timeStamperId = objects::CUC_TIME_STAMPER;
}



void ObjectFactory::produce(){
	Factory::setStaticFrameworkObjectIds();

	{
		static constexpr uint8_t NUMBER_OF_POOLS = 5;
		const uint16_t element_sizes[NUMBER_OF_POOLS] = {16, 32, 64, 128, 1024};
		const uint16_t n_elements[NUMBER_OF_POOLS] = {100, 50, 25, 15, 5};
		new PoolManager<NUMBER_OF_POOLS>(objects::TC_STORE, element_sizes,
				n_elements);
	}

	{
		static constexpr uint8_t NUMBER_OF_POOLS = 5;
		const uint16_t element_sizes[NUMBER_OF_POOLS] = {16, 32, 64, 128, 1024};
		const uint16_t n_elements[NUMBER_OF_POOLS] = {100, 50, 25, 15, 5};
		new PoolManager<NUMBER_OF_POOLS>(objects::TM_STORE, element_sizes,
				n_elements);
	}

	{
		static constexpr uint8_t NUMBER_OF_POOLS = 6;
		const uint16_t element_sizes[NUMBER_OF_POOLS] = {32, 64, 512,
				1024, 2048, 4096};
		const uint16_t n_elements[NUMBER_OF_POOLS] = {200, 100, 50, 25, 15, 5};
		new PoolManager<NUMBER_OF_POOLS>(objects::IPC_STORE, element_sizes,
				n_elements);
	}

	ObjectFactory::produceGenericObjects();

#if OBSW_ADD_TEST_CODE == 1
	new TestTask(objects::TEST_TASK, false);
#endif

}
