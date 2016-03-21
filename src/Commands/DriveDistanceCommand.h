#ifndef DriveDistanceCommand_H
#define DriveDistanceCommand_H

#include "../CommandBase.h"
#include "WPILib.h"
#include "../FieldInfo.h"

class DriveDistanceCommand: public CommandBase
{
private:
	Parameter<double> driveStraightGain;
public:
	enum GEAR {
		LOW,
		HIGH
	};
	explicit DriveDistanceCommand(double distance, FieldInfo::Speeds speed = FieldInfo::NORMAL, GEAR gear = LOW );
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
	FieldInfo::Speeds m_speedRequested;
	GEAR m_gearRequested;

	static constexpr double RAMP_RATE = 0.1;
	static constexpr double SLOW_SPEED = 0.5;
	static constexpr double NORMAL_SPEED = 0.7;
	static constexpr double FAST_SPEED = 1;

	// This table is indexed by the FieldInfo::Speeds enum !!!!!!!
	static constexpr double speeds[] = {0.0,
										 SLOW_SPEED,
										 NORMAL_SPEED,
										 FAST_SPEED};
	static constexpr double TICKS_PER_FOOT = 7673;
	static constexpr double DRIVE_GAIN = 0.06;	   // 4/100 -- taken from 2014 robot

};

#endif
