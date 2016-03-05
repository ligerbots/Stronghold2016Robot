#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <AHRS.h>

class NavXSubsystem: public Subsystem, public PIDSource {
private:
	std::unique_ptr<AHRS> mp_navX;

public:
	NavXSubsystem();
	void InitDefaultCommand();
	double getRoll();
	double getPitch();
	double getYaw();
	AHRS* getNavX();
	void sendValuesToSmartDashboard();
	bool isRobotAboutToTip();

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
};
