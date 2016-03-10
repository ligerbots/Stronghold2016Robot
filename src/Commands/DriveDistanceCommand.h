#ifndef DriveDistanceCommand_H
#define DriveDistanceCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class DriveDistanceCommand: public CommandBase
{
private:
	Parameter<double> driveStraightGain;
public:
	enum SPEED {
		SLOW,
		NORMAL,
		FAST
	};

	enum GEAR {
		LOW,
		HIGH
	};
	explicit DriveDistanceCommand(double distance, SPEED speed = NORMAL, GEAR gear = LOW );
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

protected:
	double m_distance;
	double m_startPositionLeft;
	double m_startPositionRight;
	double m_startAngle;
	double m_speed;
	GEAR m_gear;
	SPEED m_speedRequested;
	GEAR m_gearRequested;

	static constexpr double RAMP_RATE = 0.1;
	static constexpr double SLOW_SPEED = 0.5;
	static constexpr double NORMAL_SPEED = 0.7;
	static constexpr double FAST_SPEED = 1;

	static constexpr double speeds[3] = {.5, .7, 1};

	static constexpr double TICKS_PER_FOOT = 7673; // empirically measured by driving 5 feet
	static constexpr double DRIVE_GAIN = 0.06;	   // 4/100 -- taken from 2014 robot

};

#endif
