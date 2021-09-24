/*
 * pollingSequenceCentroid.h
 *
 *  Created on: Jun 7, 2021
 *      Author: mala
 */

#ifndef BSP_LINUX_FSFWCONFIG_POLLINGSEQUENCE_POLLINGSEQUENCECENTROID_H_
#define BSP_LINUX_FSFWCONFIG_POLLINGSEQUENCE_POLLINGSEQUENCECENTROID_H_


#include <fsfw/returnvalues/HasReturnvaluesIF.h>
#include "../../fsfwconfig/OBSWConfig.h"
class FixedTimeslotTaskIF;


ReturnValue_t pollingSequenceCentroidFunction(FixedTimeslotTaskIF *thisSequence);



#endif /* BSP_LINUX_FSFWCONFIG_POLLINGSEQUENCE_POLLINGSEQUENCECENTROID_H_ */
