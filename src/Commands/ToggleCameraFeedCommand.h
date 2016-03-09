#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class ToggleCameraFeedCommand: public CommandBase {
	int whichCamera;
public:
	ToggleCameraFeedCommand();
	explicit ToggleCameraFeedCommand(int whichCamera);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
