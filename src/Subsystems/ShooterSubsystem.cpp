#include <Stronghold2016Robot.h>

ShooterSubsystem::ShooterSubsystem() :
		Subsystem("ShooterSubsystem") {
	mp_shooterSolenoid.reset(new DoubleSolenoid(RobotMap::PCM_SHOOTER_FIRE, RobotMap::PCM_SHOOTER_RETRACT));
}

void ShooterSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void ShooterSubsystem::firePistons() {
	mp_shooterSolenoid->Set(DoubleSolenoid::kForward);
}

void ShooterSubsystem::retractPistons() {
	mp_shooterSolenoid->Set(DoubleSolenoid::kReverse);
}

void ShooterSubsystem::sendValuesToSmartDashboard(){

}
