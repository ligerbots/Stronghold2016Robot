#include <Stronghold2016Robot.h>

WedgeSubsystem::WedgeSubsystem() :
		Subsystem("WedgeSubsystem") {
	// forward to lower, reverse to lift
	mp_wedgeArmSolenoid.reset(
			new DoubleSolenoid(RobotMap::PCM_CAN, RobotMap::PCM_WEDGES_DOWN,
					RobotMap::PCM_WEDGES_UP));
	mp_wedgeDownSwitch.reset(
			new DigitalInput(RobotMap::LIMIT_SWITCH_WEGDE_DOWN));
}

void WedgeSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

bool WedgeSubsystem::isWedgeDown() {
	// flip value so that it's false if it's not connected
	return !mp_wedgeDownSwitch->Get();
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

DoubleSolenoid::Value WedgeSubsystem::getWedgeValue(){
	return mp_wedgeArmSolenoid->Get();
}

void WedgeSubsystem::liftWedge() {
	mp_wedgeArmSolenoid->Set(DoubleSolenoid::kReverse);
}

void WedgeSubsystem::lowerWedge() {
	mp_wedgeArmSolenoid->Set(DoubleSolenoid::kForward);
}

void WedgeSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("Wedge_Clear", isWedgeDown());
	SmartDashboard::PutBoolean("Wedge/IsClear", isWedgeDown());
	if (mp_wedgeArmSolenoid->GetError().GetCode() != 0) {
		SmartDashboard::PutString("Wedge/Solenoid", "Not Present");
	} else {
		DoubleSolenoid::Value val = mp_wedgeArmSolenoid->Get();
		if (val == DoubleSolenoid::kOff) {
			SmartDashboard::PutString("Wedge/Solenoid", "Off");
		} else if (val == DoubleSolenoid::kForward) {
			SmartDashboard::PutString("Wedge/Solenoid", "Forward/Down");
		} else if (val == DoubleSolenoid::kReverse) {
			SmartDashboard::PutString("Wedge/Solenoid", "Reverse/Up");
		}
	}
}
