#ifndef FlapSetCommand_H
#define FlapSetCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class FlapSetCommand: public CommandBase {
protected:
	double m_fraction;
public:
	explicit FlapSetCommand(double fraction);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
