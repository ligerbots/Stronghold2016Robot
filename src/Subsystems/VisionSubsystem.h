#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <Utils/Parameter.h>

class VisionSubsystem: public Subsystem, public PIDSource {
private:
	Parameter<int> exposure;
	Parameter<bool> showVision;

	std::thread m_processingThread;
	std::mutex m_frameMutex;
	Image* mp_currentFrame;
	Image* mp_processingFrame;

	double frameCenterX;
	Parameter<int> frameCenterXParam;

	// managed hardware object
	std::unique_ptr<Relay> ledRingSpike;

	void visionProcessingThread();

public:
	// Subsystem
	VisionSubsystem();
	void InitDefaultCommand();
	void camerasOn();
	void updateVision(int ticks);
	bool isLedRingOn();
	void setLedRingOn(bool on);

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
};
