#include "InitMission.h"
#include <bsp_linux/fsfwconfig/OBSWConfig.h>
#include <objects/systemObjectList.h>
#include <pollingsequence/pollingSequenceInit.h>
#include <pollingsequence/pollingSequenceInit2.h>
#include <pollingsequence/pollingSequenceCentroid.h>
#include <fsfw/returnvalues/HasReturnvaluesIF.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <fsfw/tasks/FixedTimeslotTaskIF.h>
#include <fsfw/tasks/PeriodicTaskIF.h>
#include <fsfw/tasks/TaskFactory.h>
#include <iostream>

void InitMission::createTasks(){

	/* TMTC Distribution */
	PeriodicTaskIF* distributerTask = TaskFactory::instance()->
			createPeriodicTask("DIST", 40, PeriodicTaskIF::MINIMUM_STACK_SIZE,
					0.100, nullptr);
	ReturnValue_t result = distributerTask->addComponent(
			objects::CCSDS_DISTRIBUTOR);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}
	result = distributerTask->addComponent(objects::PUS_DISTRIBUTOR);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}
	result = distributerTask->addComponent(objects::TM_FUNNEL);
	if(result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "Object add component failed" << std::endl;
	}

	/*** interface to TMTC ***/

	PeriodicTaskIF* udpBridgeTask = TaskFactory::instance()->createPeriodicTask(
			"UDP_UNIX_BRIDGE", 80, PeriodicTaskIF::MINIMUM_STACK_SIZE,
			0.5, nullptr);
	result = udpBridgeTask->addComponent(objects::ESBO_TM);
	if(result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "Add component UDP Unix Bridge failed" << std::endl;
	}

	PeriodicTaskIF* udpPollingTask = TaskFactory::instance()->
			createPeriodicTask("UDP_POLLING", 80,
					PeriodicTaskIF::MINIMUM_STACK_SIZE, 1.0, nullptr);
	result = udpPollingTask->addComponent(objects::ESBO_TC);
	if(result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "Add component UDP Polling failed" << std::endl;
	}

	PeriodicTaskIF* statusPollingTask = TaskFactory::instance()->
				createPeriodicTask("UDP_STATUS_POLLING", 80,
						PeriodicTaskIF::MINIMUM_STACK_SIZE, 1.0, nullptr);
	result = statusPollingTask->addComponent(objects::STATUS_POLLING);
	if(result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "Add component UDP Polling failed" << std::endl;
	}

	PeriodicTaskIF* timePollingTask = TaskFactory::instance()->
				createPeriodicTask("UDP_TIME_POLLING", 80,
						PeriodicTaskIF::MINIMUM_STACK_SIZE, 1.0, nullptr);

	result = timePollingTask->addComponent(objects::TIME_POLLING);
	if(result != HasReturnvaluesIF::RETURN_OK) {
		sif::error << "Add component UDP Polling failed" << std::endl;
	}

	PeriodicTaskIF* packetHandler = TaskFactory::instance()->
			createPeriodicTask("PACKET_HANDLER", 40,
					PeriodicTaskIF::MINIMUM_STACK_SIZE, 1, nullptr);
	result = packetHandler->addComponent(objects::STATUS_HANDLER);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}

	result = packetHandler->addComponent(objects::TIME_HANDLER);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}

/****event Task ***/

	PeriodicTaskIF* eventTask = TaskFactory::instance()->
			createPeriodicTask("EVENT", 20,
					PeriodicTaskIF::MINIMUM_STACK_SIZE, 0.100, nullptr);
	result = eventTask->addComponent(objects::EVENT_MANAGER);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}



	/*** PUS Services ***/

	PeriodicTaskIF* pusVerification = TaskFactory::instance()->
			createPeriodicTask("PUS_VERIF_1", 40,
					PeriodicTaskIF::MINIMUM_STACK_SIZE, 0.200, nullptr);
	result = pusVerification->addComponent(objects::PUS_SERVICE_1_VERIFICATION);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}

	PeriodicTaskIF* pusEvents = TaskFactory::instance()->
			createPeriodicTask("PUS_VERIF_1", 60,
					PeriodicTaskIF::MINIMUM_STACK_SIZE, 0.200, nullptr);
	result = pusVerification->addComponent(objects::PUS_SERVICE_5_EVENT_REPORTING);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}

	PeriodicTaskIF* pusHighPrio = TaskFactory::instance()->
			createPeriodicTask("PUS_HIGH_PRIO", 50,
					PeriodicTaskIF::MINIMUM_STACK_SIZE,
					0.200, nullptr);
	result = pusHighPrio->addComponent(objects::PUS_SERVICE_2_DEVICE_ACCESS);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}
	result = pusHighPrio->addComponent(objects::PUS_SERVICE_9_TIME_MGMT);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}

	PeriodicTaskIF* PusService03 = TaskFactory::instance()-> createPeriodicTask(
			"PUS_HOUSEKEEPING_3", 80, PeriodicTaskIF::MINIMUM_STACK_SIZE, 1, nullptr);
	result = PusService03->addComponent(objects::PUS_SERVICE_3_PSB/*_PSB*/);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Add component PUS Housekeeping Service 3 failed" << std::endl;
	}

#if ADD_EXTRA_SERVICES ==1

	PeriodicTaskIF* PusService20 = TaskFactory::instance()-> createPeriodicTask(
			"PUS_PARAMETER_20", 4, PeriodicTaskIF::MINIMUM_STACK_SIZE, 1, nullptr);
	result = PusService20->addComponent(objects::PUS_PARAMETER_SERVICE);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Add component PUS Parameter Service failed" << std::endl;
	}
#endif

	PeriodicTaskIF* pusMedPrio = TaskFactory::instance()->
			createPeriodicTask("PUS_HIGH_PRIO", 40,
					PeriodicTaskIF::MINIMUM_STACK_SIZE,
					0.8, nullptr);
	result = pusMedPrio->addComponent(objects::PUS_SERVICE_8_FUNCTION_MGMT);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}
	result = pusMedPrio->addComponent(objects::PUS_SERVICE_200_MODE_MGMT);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}

	PeriodicTaskIF* pusLowPrio = TaskFactory::instance()->
			createPeriodicTask("PUSB", 30, PeriodicTaskIF::MINIMUM_STACK_SIZE,
					1.6, nullptr);
	result = pusLowPrio->addComponent(objects::PUS_SERVICE_17_TEST);
	if(result!=HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}


#if OBSW_ADD_TEST_CODE == 1
	/*** data injector tasks***/

	PeriodicTaskIF* pointingInjectionTask = TaskFactory::instance()->
			createPeriodicTask("POINTING_PROVIDER", 40,
					PeriodicTaskIF::MINIMUM_STACK_SIZE, 1, nullptr);
	result = pointingInjectionTask->addComponent(objects::POINTING_PROVIDER);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}
	result = pointingInjectionTask->addComponent(objects::SENSOR_DATA_PROVIDER);
	if(result != HasReturnvaluesIF::RETURN_OK){
		sif::error << "Object add component failed" << std::endl;
	}
#endif

	//Main thread sleep
	sif::debug << "Starting Tasks in 2 seconds" << std::endl;
	TaskFactory::delayTask(2000);
	distributerTask->startTask();

	udpBridgeTask->startTask();
	//udpPollingTask->startTask();
	//statusPollingTask->startTask();
	//timePollingTask->startTask();
	//packetHandler->startTask();

	pointingInjectionTask->startTask();
	PusService03->startTask();

	#if ADD_EXTRA_SERVICES == 1
	PusService20->startTask();
#endif
	eventTask->startTask();

	pusVerification->startTask();
	pusEvents->startTask();
	pusHighPrio->startTask();
	pusMedPrio->startTask();
	pusLowPrio->startTask();

#if OBSW_ADD_TEST_CODE == 1
	pointingInjectionTask->startTask();
#endif
	sif::debug << "Tasks started.." << std::endl;
}


