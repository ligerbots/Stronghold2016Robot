#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <AHRS.h>

// Unfortunately, the AHRS (NavX sensor class) zeroYaw assumes the robot
// is actually in the zero Yaw position. Sometimes we'll be setting up our
// robot facing backwards. If we zero'd the Yaw of a backwards facing robot,
// all yaw coordinates for that match would be 180 degrees offset from
// our normal coordinate system.

// Rather than do that, we maintain our own offset here. So when we set
// up the robot 180 degrees from the usual position, we keep track of
// that in m_yawOffset and always add m_yawOffset to our results

class NavXSubsystem: public Subsystem, public AHRS  {
private:
	double m_yawOffset;
public:
	explicit NavXSubsystem(SPI::Port);
	void InitDefaultCommand();
	double getYaw();
	void zeroYaw(double yawOffset = 0.0);
	void sendValuesToSmartDashboard();
	bool isRobotAboutToTip(float maxPitchAngle);

	// PIDSource
	void SetPIDSourceType(PIDSourceType pidSource);
	PIDSourceType GetPIDSourceType() const;
	double PIDGet();
};
