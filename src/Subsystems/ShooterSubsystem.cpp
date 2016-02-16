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

void ShooterSubsystem::setLeftFlap(double extent){
	mp_leftFlap->Set(extent);
}

void ShooterSubsystem::setRightFlap(double extent){
	mp_rightFlap->Set(extent);
}

void ShooterSubsystem::setFlaps(double extent) {
	mp_leftFlap->Set(extent);
	mp_rightFlap->Set(extent);
}

double ShooterSubsystem::getLeftPosition() {
	return mp_leftFlap->Get();
}

double ShooterSubsystem::getRightPosition() {
	return mp_rightFlap->Get();
}

void ShooterSubsystem::sendValuesToSmartDashboard(){
	SmartDashboard::PutNumber("Shooter/LeftFlapAngle", mp_leftFlap->Get());
	SmartDashboard::PutNumber("Shooter/RightFlapAngle", mp_rightFlap->Get());
}
