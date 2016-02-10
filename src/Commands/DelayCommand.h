#pragma once

#include "WPILib.h"
#include "../Subsystems/Delay.h"

/**
 * Command for use in chaining that delays for the specified number of seconds
 */
class DelayCommand: public Command {
public:
	explicit DelayCommand(double delaySeconds);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();

private:
	/**
	 * The number of ticks that have passed
	 */
	int m_ticks;
	/**
	 * The number of seconds to delay
	 */
	double m_delaySeconds;
};


