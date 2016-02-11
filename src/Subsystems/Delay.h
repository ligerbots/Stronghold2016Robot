#pragma once

#include "WPILib.h"

/**
 * Dummy "subsystem" that lets WPILib handle DelayCommand correctly (eg two DelayCommands don't run at the same time)
 */
class Delay: public Subsystem {
private:

public:
	Delay();
};
