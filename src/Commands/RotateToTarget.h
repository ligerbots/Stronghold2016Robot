#pragma once
#include "../CommandBase.h"
#include "WPILib.h"
#include "RotateIMUCommand.h"

class RotateToTarget: public RotateIMUCommand {
public:
	explicit RotateToTarget();
	void Initialize();
};

