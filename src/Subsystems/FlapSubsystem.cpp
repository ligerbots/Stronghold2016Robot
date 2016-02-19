/*
 * ServoSubsystem.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: Ligerbots
 */
#include <Subsystems/FlapSubsystem.h>
#include "WPILib.h"
#include <RobotMap.h>

FlapSubsystem::FlapSubsystem() : Subsystem("FlapSubsystem") {
	mp_leftFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_LEFT));
	mp_rightFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_RIGHT));
}

void FlapSubsystem::InitDefaultCommand() {

}

void FlapSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutNumber("LeftFlapAngle", mp_leftFlap->GetAngle());
	SmartDashboard::PutNumber("RightFlapAngle", mp_rightFlap->GetAngle());
}

void FlapSubsystem::setLeftFlapAngle(double angle) {
	mp_leftFlap->SetAngle(angle);
}

void FlapSubsystem::setRightFlapAngle(double angle) {
	mp_rightFlap->SetAngle(angle);
}

double FlapSubsystem::getLeftFlapAngle() {
	return mp_leftFlap->GetAngle();
}

double FlapSubsystem::getRightFlapAngle() {
	return mp_rightFlap->GetAngle();
}
