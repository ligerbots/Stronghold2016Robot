#ifndef DummySubsystem_H
#define DummySubsystem_H

#include "Commands/Subsystem.h"
#include "WPILib.h"

class DummySubsystem: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	DummySubsystem();
	void InitDefaultCommand();
};

#endif
