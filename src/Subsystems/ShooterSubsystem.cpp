#include <Stronghold2016Robot.h>

ShooterSubsystem::ShooterSubsystem() :
		Subsystem("ShooterSubsystem") {
	mp_leftFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_LEFT));
	mp_rightFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_RIGHT));
}

void ShooterSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void ShooterSubsystem::setLeftFlap(double angle){
	mp_leftFlap->SetAngle(angle);
}

void ShooterSubsystem::setRightFlap(double angle){
	mp_rightFlap->SetAngle(angle);
}

void ShooterSubsystem::sendValuesToSmartDashboard(){
	SmartDashboard::PutNumber("Shooter/LeftFlapAngle", mp_leftFlap->GetAngle());
	SmartDashboard::PutNumber("Shooter/RightFlapAngle", mp_rightFlap->GetAngle());
}
