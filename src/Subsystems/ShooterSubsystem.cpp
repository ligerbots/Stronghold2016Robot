#include <Stronghold2016Robot.h>

ShooterSubsystem::ShooterSubsystem() :
		Subsystem("ShooterSubsystem") {
	mp_shooterSolenoid.reset(
			new DoubleSolenoid(RobotMap::PCM_HIGH_FLOW_CAN, RobotMap::PCM_SHOOTER_FIRE,
					RobotMap::PCM_SHOOTER_RETRACT));
}

void ShooterSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void ShooterSubsystem::firePiston() {
	mp_shooterSolenoid->Set(DoubleSolenoid::kForward);
}

void ShooterSubsystem::retractPiston() {
	mp_shooterSolenoid->Set(DoubleSolenoid::kOff);
}

void ShooterSubsystem::sendValuesToSmartDashboard() {
	if (mp_shooterSolenoid->GetError().GetCode() != 0) {
		SmartDashboard::PutString("Shooter/Solenoid", "Not Present");
	} else {
		DoubleSolenoid::Value val = mp_shooterSolenoid->Get();
		if (val == DoubleSolenoid::kOff) {
			SmartDashboard::PutString("Shooter/Solenoid", "Off");
		} else if (val == DoubleSolenoid::kForward) {
			SmartDashboard::PutString("Shooter/Solenoid", "Forward/Fire");
		} else if (val == DoubleSolenoid::kReverse) {
			SmartDashboard::PutString("Shooter/Solenoid", "Reverse/Retract");
		}
	}
}
