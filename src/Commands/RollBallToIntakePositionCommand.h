#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class RollBallToIntakePositionCommand: public CommandBase {
public:
	enum IntakePosition {
		PICKUP,
		LOW_GOAL, // shoot out max speed
		CROSSING_POSITION, // if shooter switch pressed, pull down. Otherwise pull up
		SHOOTING_POSITION, // pull up to shooter
		BACK_TO_SHOOTING_POSITION // if we overshot the shooter, pull down
	};
protected:
	IntakePosition where;
	bool sensorFlag;
	bool moveUp;
	int ticks;
	int ticks_since_shooter_switch;
public:
	explicit RollBallToIntakePositionCommand(IntakePosition where);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
