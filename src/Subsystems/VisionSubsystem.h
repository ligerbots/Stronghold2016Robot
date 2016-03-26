#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <Utils/Parameter.h>
#include <pthread.h>
#include "DriveSubsystem.h"

// We believe the "float" color in the imaqDraw calls is BGR (Blue Green Red)
// with ranges of 0.0 to just under 256.0 for each color
#define COLOR(R, G, B) (16384.0*B + 256.0*G + R)
#define BLUE COLOR(255, 0, 0)
#define GREEN COLOR(0, 255, 0)
#define RED COLOR(0, 0, 255)
#define YELLOW COLOR(255, 255, 0)
#define CYAN COLOR(0, 255, 255)
#define MAGENTA COLOR(255, 0, 255)
#define DODGERBLUE COLOR(30, 144, 255)
#define ORANGERED COLOR(255, 69, 0)

class VisionSubsystem: public Subsystem, public PIDSource {
private:
	Parameter<int> exposure;
	Parameter<bool> showVision;
	Parameter<bool> paintTarget;
	Parameter<bool> enableVision;
	Parameter<double> color;

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
		MFDSX,	// IMAQ_MT_MAX_FERET_DIAMETER_START_X
		MFDSY,	// IMAQ_MT_MAX_FERET_DIAMETER_START_Y
		MFDEX,	// IMAQ_MT_MAX_FERET_DIAMETER_END_X
		MFDEY,	// IMAQ_MT_MAX_FERET_DIAMETER_END_Y
		ORIENT, // IMAQ_MT_ORIENTATION
		MAXVAL
	};

	// Max's regression coefficients
	static constexpr double regcoef_a1 = -2.448E-01;
	static constexpr double regcoef_a2 =  1.979E-05;
	static constexpr double regcoef_a3 = -1.217E-01;
	static constexpr double regcoef_a4 = -3.020E-05;
	static constexpr double regcoef_a5 =  4.378E-04;
	static constexpr double regcoef_a6 =  7.033E+01;
	static constexpr double regcoef_d1 = -2.09E-01;
	static constexpr double regcoef_d2 =  3.46E-04;
	static constexpr double regcoef_d3 =  3.58E-02;
	static constexpr double regcoef_d4 =  9.40E-04;
	static constexpr double regcoef_d5 =  0.00E+00;
	static constexpr double regcoef_d6 =  6.51E+01;

	std::mutex m_frameMutex;
	Image* mp_currentFrame;
	Image* mp_processingFrame;

	double m_frameCenterX;
	double m_frameCenterY;
	/**
	 * Distance and angle, calculated from feret midpoint using the lookup table
	 * Inches / Degrees
	 */
	double m_distance;
	double m_angle;

	double m_frameWidth;
	int m_numParticles;

	std::thread m_processingThread;
	bool m_visionBusy;
	bool m_visionRequested;
	pthread_cond_t m_threadCond;
	pthread_mutex_t m_threadMutex;
	int m_lastVisionTick;

	DriveSubsystem::Position m_robotPos;
	bool m_firstFrame;

	// Take all the particple measurements in one call
	// Q: Does taking extra measurements slow things down?
	MeasurementType mT[MAXVAL];

	int m_activeCamera;

	// managed hardware object
	std::unique_ptr<Relay> ledRingSpike;
	double *m_pM;	// pixel measurement


	void visionProcessingThread();
	void measureTarget(Image *image);
	void markTarget(Image *image);

public:
	// Subsystem
	VisionSubsystem();
	void InitDefaultCommand();
	void camerasOn();
	void updateVision(int ticks);
	bool isLedRingOn();
	void setLedRingOn(bool on);
	void toggleCameraFeed();
	void setCameraFeed(int whichCamera);
	void setVisionEnabled(bool enabled);

	// empirical values for flap angle vs distance and curve fit for distance vs target y
	static double angles[13];
	static constexpr double distance_a = 2.333;
	static constexpr double distance_b = 1.0052;
	static constexpr double camera_offset = 3.25; // in
	static constexpr double horizontal_field_of_view = 78.442;
	static constexpr double tan_half_horizontal_field_of_view = 0.8162;

	void runVision();
	double getCorrectedFrameCenter(double distInches);
	bool isTargetVisible();
	bool isVisionCalculationDirty();
	bool isVisionBusy();

	double getTargetCenterX();
	double getTargetCenterY();
	double getBoundingBoxWidth();
	double getBoundingBoxHeight();

	/**
	 * Gives distance to target in feet using an equation calculated from test data.
	 * Assumes the target is visible, otherwise it probably returns the last known distance
	 * @return Distance, in feet
	 */
	double getDistanceToTarget();

	void calculateDistanceAndAngle(double xpos, double ypos, double* distance, double* angle);
	void calculateDistanceAndAngle_FromRegression(double xpos, double ypos, double* distance, double* angle);

	/**
	 * Bad implementation that uses lookup table values from the test data
	 */
	double getFlapsFractionForDistance(double distance);

	void sendValuesToSmartDashboard();

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
	double TargetAngle();
};
