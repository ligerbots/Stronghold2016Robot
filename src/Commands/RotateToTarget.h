#pragma once
#include "../CommandBase.h"
#include "WPILib.h"

class RotateToTarget: public RotateIMUCommand {
public:
	explicit RotateToTarget();
	void Initialize();
};

