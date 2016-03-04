#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class ToggleCameraFeedCommand: public CommandBase {
public:
	ToggleCameraFeedCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
