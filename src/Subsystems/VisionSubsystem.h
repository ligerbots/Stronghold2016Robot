#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <Utils/Parameter.h>

class VisionSubsystem: public Subsystem, public PIDSource {
private:
	Parameter<int> exposure;
	Parameter<bool> showVision;
	Parameter<bool> paintTarget;

	std::mutex m_frameMutex;
	Image* mp_currentFrame;
	Image* mp_processingFrame;

	double m_frameCenterX;
	double m_frameCenterY;
	int m_numParticles;

	int activeCamera;

	// managed hardware object
	std::unique_ptr<Relay> ledRingSpike;

	std::thread m_processingThread;

	void visionProcessingThread();

public:
	// Subsystem
	VisionSubsystem();
	void InitDefaultCommand();
	void camerasOn();
	void updateVision(int ticks);
	bool isLedRingOn();
	void setLedRingOn(bool on);
	void toggleCameraFeed();

	double getFrameCenter();

	void sendValuesToSmartDashboard();

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
};
