#include <Stronghold2016Robot.h>

WedgeSubsystem::WedgeSubsystem() :
		Subsystem("WedgeSubsystem")
{
	// forward to lower, reverse to lift
	mp_wedgeArmSolenoid.reset(new DoubleSolenoid(RobotMap::PCM_WEDGES_DOWN, RobotMap::PCM_WEDGES_UP));
	mp_wedgeDownSwitch.reset(new DigitalInput(RobotMap::LIMIT_SWITCH_WEGDE_DOWN));
}

void WedgeSubsystem::InitDefaultCommand()
{
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

bool WedgeSubsystem::isWedgeDown() {
	return mp_wedgeDownSwitch->Get();
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

void WedgeSubsystem::liftWedge() {
	mp_wedgeArmSolenoid->Set(DoubleSolenoid::kReverse);
}

void WedgeSubsystem::lowerWedge() {
	mp_wedgeArmSolenoid->Set(DoubleSolenoid::kForward);
}

void WedgeSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("Wedge_Clear", isWedgeDown());
}
