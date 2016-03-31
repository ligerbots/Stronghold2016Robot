#ifndef PrepareForAutoCommand_H
#define PrepareForAutoCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class PrepareForAutoCommand: public CommandBase {
protected:
	bool m_setWedgesUp;
	bool m_setIntakeUp;
public:
	PrepareForAutoCommand(bool wedgesUp, bool intakeUp);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
