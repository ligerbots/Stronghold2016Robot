#include "NavXSubsystem.h"
#include "Robot.h"

NavXSubsystem::NavXSubsystem(SPI::Port port) :
	Subsystem("NavXSubsystem"),
	m_yawOffset(0.0)
{
}

void NavXSubsystem::InitDefaultCommand() {
}

void NavXSubsystem::zeroYaw(double yawOffset) {
	m_yawOffset = yawOffset;
}

bool NavXSubsystem::isRobotAboutToTip(float maxPitchAngle) {
	return GetRoll() > maxPitchAngle;
}

void NavXSubsystem::sendValuesToSmartDashboard() {

}

void NavXSubsystem::SetPIDSourceType(PIDSourceType pidSource) {
	// do nothing
}

PIDSourceType NavXSubsystem::GetPIDSourceType() const {
	return PIDSourceType::kDisplacement;
}

double NavXSubsystem::PIDGet() {
	return GetYaw();
}
