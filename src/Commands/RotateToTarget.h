#pragma once
#include "../CommandBase.h"
#include "WPILib.h"
#include "RotateIMUCommand.h"

class RotateToTarget: public RotateIMUCommand {
protected:
	bool m_useFineAngle;
public:
	explicit RotateToTarget();
	void Initialize();
	void SetUseFineAngle(bool useFineAngle);
};

