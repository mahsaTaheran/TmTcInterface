/*
 * sid.cpp
 *
 *  Created on: Jun 7, 2021
 *      Author: mala
 */


#include "sid.h"
#include <cmath>
uint32_t sid::INTERVAL_SECONDS_TO_INTERVAL(bool isDiagnostics,
		float collectionInterval_seconds) {
	if(isDiagnostics) {
		return 	ceil(collectionInterval_seconds/sid::DIAG_MIN_INTERV_DIAGNOSTICS_SECONDS);
	}
	else {
		return ceil(collectionInterval_seconds/sid::DIAG_MIN_INTERV_HK_SECONDS);
	}
}

float sid::INTERVAL_TO_INTERVAL_SECONDS(bool isDiagnostics,
		uint32_t collectionInterval) {
	if(isDiagnostics) {
		return 	(float)collectionInterval * sid::DIAG_MIN_INTERV_DIAGNOSTICS_SECONDS;
	}
	else {
		return 	(float)collectionInterval * sid::DIAG_MIN_INTERV_HK_SECONDS;
	}
}

