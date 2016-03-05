#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <Utils/Parameter.h>



class VisionSubsystem: public Subsystem, public PIDSource {
private:
	Parameter<int> exposure;
	Parameter<bool> showVision;
	Parameter<bool> paintTarget;
	Parameter<bool> enableVision;
	Parameter<double> color;
	Parameter<double> boundingBoxWidth;
	Parameter<double> boundingBoxHeight;
	Parameter<double> convexHullSize;
	Parameter<double> convexHullPerArea;
	Parameter<double> feretDiameter;
	Parameter<double> feretStartX;
	Parameter<double> feretStartY;
	Parameter<double> feretEndX;
	Parameter<double> feretEndY;
	// Define the indexes to our MeasuremeParticleReport
	enum Measures {
		COMX,	// IMAQ_MT_CENTER_OF_MASS_X
		COMY,	// IMAQ_MT_CENTER_OF_MASS_Y
		CHA, 	// IMAQ_MT_CONVEX_HULL_AREA
		AREA,	// IMAQ_MT_AREA
		BRW, 	// IMAQ_MT_BOUNDING_RECT_WIDTH
		BRH,	// IMAQ_MT_BOUNDING_RECT_HEIGHT
		AVSL,	// IMAQ_MT_AVERAGE_VERT_SEGMENT_LENGTH
		AHSL,	// IMAQ_MT_AVERAGE_HORIZ_SEGMENT_LENGTH
		MFD, 	// IMAQ_MT_MAX_FERET_DIAMETER
		ERLS,	// IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE
		ERSS,	// IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE
		ERSSF, 	// IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE_FERET
		MFDO, 	// IMAQ_MT_MAX_FERET_DIAMETER_ORIENTATION
		MAXVAL
	};

	std::mutex m_frameMutex;
	Image* mp_currentFrame;
	Image* mp_processingFrame;

	bool m_inVision;

	double m_frameCenterX;
	double m_frameCenterY;
	int m_numParticles;
	MeasurementType mT[MAXVAL];

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
	/**
	 * Bad implementation that uses lookup table values from the test data
	 */
	double getFlapsFractionForDistance(double distance);

	void sendValuesToSmartDashboard();

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
};
