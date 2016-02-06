#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class ShooterSubsystem: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	ShooterSubsystem();
	void InitDefaultCommand();
	void shoot();
	void flapUp();
	void flapDown();
	void varyFlaps(int leftDirection, int rightDirection);

};
