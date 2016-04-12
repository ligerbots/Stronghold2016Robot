#ifndef DifferentialFlapShootCommand_H
#define DifferentialFlapShootCommand_H

#include "../CommandBase.h"
#include "WPILib.h"
#include "RollBallToIntakePositionCommand.h"
#include "RotateToTarget.h"

class DifferentialFlapShootCommand: public CommandBase {
protected:
	RollBallToIntakePositionCommand* mp_rollerCommand;
	bool m_rollFinished;
	RotateToTarget* mp_centeringCommand;
	int m_ticksSinceFire;

	enum State {
		GET_FRAME,
		CENTERING,
		INTAKE_UP,
		SHOOTING
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
