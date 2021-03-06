#ifndef FRAMEWORK_OSAL_LINUX_TIMER_H_
#define FRAMEWORK_OSAL_LINUX_TIMER_H_

#include <signal.h>
#include <time.h>
#include <stdint.h>

/**
 * This class is a helper for the creation of a Clock Monotonic timer which does not trigger a signal
 */
class Timer {
public:
	/**
	 * Creates the Timer sets the timerId Member
	 */
	Timer();
	/**
	 * Deletes the timer
	 *
	 * Careful! According to POSIX documentation:
	 * The treatment of any pending signal generated by the deleted timer is unspecified.
	 */
	virtual ~Timer();

	/**
	 * Set the timer given in timerId to the given interval
	 *
	 * @param intervalMs Interval in ms to be set
	 * @return 0 on Success 1 else
	 */
	int setTimer(uint32_t intervalMs);

	/**
	 * Get the remaining time of the timer
	 *
	 * @param remainingTimeMs Pointer to integer value which is used to return the remaining time
	 * @return 0 on Success 1 else (see timer_getime documentation of posix function)
	 */
	int getTimer(uint32_t* remainingTimeMs);

private:
	timer_t timerId;
};

#endif /* FRAMEWORK_OSAL_LINUX_TIMER_H_ */
