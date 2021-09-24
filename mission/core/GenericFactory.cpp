#include "GenericFactory.h"

#include <OBSWConfig.h>
#include <bsp_linux/fsfwconfig/datapool/dataPoolInit.h>
#include <fsfw/devicehandlers/CookieIF.h>
#include <bsp_linux/fsfwconfig/objects/systemObjectList.h>
#include <fsfw/devicehandlers/CookieIF.h>
#include <objects/systemObjectList.h>
#include <bsp_linux/fsfwconfig/tmtc/apid.h>
#include <bsp_linux/fsfwconfig/tmtc/pusIds.h>
#include <bsp_linux/fsfwconfig/datapool/deviceTestPool.h>
#include <bsp_linux/fsfwconfig/datapool/PointingTestPool.h>
#include <bsp_linux/fsfwconfig/hk/hkInit.h>

#include <fsfw/events/EventManager.h>
#include <fsfw/health/HealthTable.h>
#include <fsfw/internalError/InternalErrorReporter.h>
#include <fsfw/pus/CService200ModeCommanding.h>
#include <fsfw/pus/Service17Test.h>
#include <fsfw/pus/Service1TelecommandVerification.h>
#include <fsfw/pus/Service2DeviceAccess.h>
#include <fsfw/pus/Service5EventReporting.h>
#include <fsfw/pus/Service8FunctionManagement.h>
#include <fsfw/pus/Service9TimeManagement.h>
#include <fsfw/tcdistribution/CCSDSDistributor.h>
#include <fsfw/tcdistribution/PUSDistributor.h>

//#include <fsfw/timemanager/TimeStamper.h>

#include <mission/test/DummyPointingProvider.h>
#include <mission/test/BubbleDataInjector.h>
#include <mission/utility/TmFunnel.h>
#include <mission/utility/StatusPollingTask.h>
#include <mission/utility/StatusPacketHandler.h>
#include <mission/utility/ESBOTmBridge.h>
#include <mission/utility/ESBOTcPolling.h>
#include <mission/utility/CUCTimeStamper.h>
#include <mission/pus/Service3HousekeepingPSB.h>
#include <mission/pus/ParameterManagement/ParameterManagement.h>


void ObjectFactory::produceGenericObjects() {
    /* Framework objects */
    new EventManager(objects::EVENT_MANAGER);
    new HealthTable(objects::HEALTH_TABLE);
    new InternalErrorReporter(objects::INTERNAL_ERROR_REPORTER, 0, 0, 0);
    new CUCTimeStamper(objects::CUC_TIME_STAMPER);
    new CCSDSDistributor(apid::APID, objects::CCSDS_DISTRIBUTOR);
    new PUSDistributor(apid::APID, objects::PUS_DISTRIBUTOR,
            objects::CCSDS_DISTRIBUTOR);
    new TmFunnel(objects::TM_FUNNEL);

    /* PUS stack */
    new Service1TelecommandVerification(objects::PUS_SERVICE_1_VERIFICATION,
            apid::APID, pus::PUS_SERVICE_1, objects::TM_FUNNEL);
    new Service2DeviceAccess(objects::PUS_SERVICE_2_DEVICE_ACCESS,
            apid::APID, pus::PUS_SERVICE_2, 3, 10);
    new Service5EventReporting(objects::PUS_SERVICE_5_EVENT_REPORTING,
            apid::APID, pus::PUS_SERVICE_5, 50);
    new Service8FunctionManagement(objects::PUS_SERVICE_8_FUNCTION_MGMT,
            apid::APID, pus::PUS_SERVICE_8, 3, 10);
    new Service9TimeManagement(objects::PUS_SERVICE_9_TIME_MGMT, apid::APID,
            pus::PUS_SERVICE_9);
    new Service17Test(objects::PUS_SERVICE_17_TEST, apid::APID,
            pus::PUS_SERVICE_17);
    new CService200ModeCommanding(objects::PUS_SERVICE_200_MODE_MGMT,
            apid::APID, pus::PUS_SERVICE_200);


    /* Status and Time packet, tmtc link*/


    new StatusPollingTask(objects::STATUS_POLLING,objects::CCSDS_DISTRIBUTOR, objects::TC_STORE
    			,30020);

    new StatusPollingTask(objects::TIME_POLLING,objects::CCSDS_DISTRIBUTOR, objects::TC_STORE
        			,30030);

	new StatusPacketHandler(apid::TIME_APID, objects::TIME_HANDLER,
				objects::CCSDS_DISTRIBUTOR);
	new StatusPacketHandler(apid::STATUS_APID, objects::STATUS_HANDLER,
					objects::CCSDS_DISTRIBUTOR);

	new ESBOTmBridge(objects::ESBO_TM, objects::CCSDS_DISTRIBUTOR,
				objects::TM_STORE, objects::TC_STORE,30010,30001);
	new ESBOTcPolling(objects::ESBO_TC, objects::ESBO_TM);


	/*dummy pointing data provider*/

	 new DummyPointingProvider(objects::POINTING_PROVIDER, "pointing-RA-Dec.csv");
	 new BubbleDataInjector(objects::SENSOR_DATA_PROVIDER,"sensorData.csv");
	 /* mission PUS services*/

	Service3HousekeepingPSB* housekeepingServicePSB =
	    			new Service3HousekeepingPSB(objects::PUS_SERVICE_3_PSB,
	    			apid::APID, pus::PUS_SERVICE_3);

	ParameterService* parameterService =
	    			new ParameterService(objects::PUS_PARAMETER_SERVICE,
	    			apid::APID, pus::PUS_PARAMETER_SERVICE);

	/* Housekeeping structures */


	struct pointingTestInit::pointingHkStruct dummyPointing;
	pointingTestInit::pointingHkStructInit (&dummyPointing);
	struct hk::hkIdStruct HkIdStruct;
	hk::initHkStruct(&HkIdStruct,dummyPointing);
	hk::hkInit(housekeepingServicePSB, HkIdStruct);



}

