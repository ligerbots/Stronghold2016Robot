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
	IntakePosition m_targetIntakePosition;
	bool m_sensorFlag;
	bool m_moveUp;
	int m_ticks;
	int m_shooterSwitchState;
	int m_waitingTicks;
	bool m_needsToWaitForFlaps;
	bool m_switchAlreadyPressed;

	int m_ticksSinceCrossingPosition;
	static constexpr int STATE_UP = 0;
	static constexpr int STATE_WAIT_FOR_RELEASE = 1;
	static constexpr int STATE_BACK = 2;
	static constexpr int STATE_WAIT = 3;
public:
	explicit RollBallToIntakePositionCommand(IntakePosition targetIntakePositon);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
