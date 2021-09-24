#ifndef MISSION_CORE_GENERICFACTORY_H_
#define MISSION_CORE_GENERICFACTORY_H_


namespace ObjectFactory {

/**
 * @brief   Produce hardware independant objects. Called by bsp specific
 *          object factory.
 */
void produceGenericObjects();

}


#endif /* MISSION_CORE_GENERICFACTORY_H_ */
