#ifndef DifferentialFlapShootCommand_H
#define DifferentialFlapShootCommand_H

#include "../CommandBase.h"
#include "WPILib.h"
#include "RollBallToIntakePositionCommand.h"
#include "RotateToTarget.h"

class DifferentialFlapShootCommand: public CommandBase {
protected:
	RollBallToIntakePositionCommand* mp_rollerCommand;
	bool m_intakeFinished;
	RotateToTarget* mp_centeringCommand;
	int m_ticksSinceFire;
	bool m_die;
	double m_RTCWhenFlapSet;
	double m_yawAtCenteringEnd;
	bool m_haveFlapsBeenSet;

	enum State {
		GET_FRAME, // waiting for the next vision frame
		CENTERING, // running the centering command
		INTAKE_FLAPS, // waiting for intake to complete while continuously checking vision and setting flaps
		SHOOTING // taking the shot
	};

	State m_state;
public:
	DifferentialFlapShootCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
