#include <datapool/dataPoolInit.h>

#include <fsfw/datapool/DataPool.h>
#include <fsfw/objectmanager/ObjectManager.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <fsfw/tasks/TaskFactory.h>
#include <bsp_linux/core/InitMission.h>
#include <bsp_linux/core/ObjectFactory.h>

//#include <mission/test/MutexExample.h>
//#include <mission/utility/PusPacketCreator.h>

#include <iostream>

#ifdef WIN32
static const char* COMPILE_PRINTOUT = "Windows";
#elif LINUX
static const char* COMPILE_PRINTOUT = "Linux";
#else
static const char* COMPILE_PRINTOUT = "unknown OS";
#endif

// This is configured for linux without \cr
ServiceInterfaceStream sif::debug("DEBUG", false);
ServiceInterfaceStream sif::info("INFO", false);
ServiceInterfaceStream sif::warning("WARNING", false);
ServiceInterfaceStream sif::error("ERROR", false, false, true);

ObjectManagerIF *objectManager = nullptr;

//Initialize Data Pool
DataPool dataPool(dataPoolInit);

#include <array>

int main() {

	std::cout << "-- FSFW Example (Hosted) --" << std::endl;
	std::cout << "-- Compiled for " << COMPILE_PRINTOUT << " --" << std::endl;
	std::cout << "-- " <<  __DATE__ << " " << __TIME__ << " --" << std::endl;

	objectManager = new ObjectManager(ObjectFactory::produce);
	objectManager->initialize();
	InitMission::createTasks();


	// Permanent loop.
	for(;;) {
		// sleep main thread, not needed anymore.
		TaskFactory::delayTask(5000);
	}
	return 0;
}
