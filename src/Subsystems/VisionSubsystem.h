#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <Utils/Parameter.h>

class VisionSubsystem: public Subsystem, public PIDSource {
private:
	Parameter<int> exposure;
	Parameter<bool> showVision;
	Parameter<bool> paintTarget;
	Parameter<double> color;
	Parameter<double> boundingBoxWidth;
	Parameter<double> boundingBoxHeight;
	Parameter<double> convexHullSize;
	Parameter<double> convexHullPerArea;

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

	double getCenterOfMassX();
	double getCenterOfMassY();
	double getBoundingBoxWidth();
	double getBoundingBoxHeight();

	/**
	 * Gives distance to target in feet using an equation calculated from test data.
	 * Assumes the target is visible, otherwise it probably returns the last known distance
	 * @return Distance, in feet
	 */
	double getDistanceToTarget();

	void sendValuesToSmartDashboard();

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
};
