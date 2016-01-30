#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <Utils/Parameter.h>

class VisionSubsystem: public Subsystem, public PIDSource {
private:
	Parameter<int> exposure;
	Parameter<bool> runVision;

	double frameCenterX;

	// managed hardware object
	std::unique_ptr<Relay> ledRingSpike;

public:
	// Subsystem
	VisionSubsystem();
	void InitDefaultCommand();
	void updateVision(int ticks);

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
};
