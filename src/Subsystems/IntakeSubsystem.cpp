#include <Stronghold2016Robot.h>

IntakeSubsystem::IntakeSubsystem() :
		Subsystem("IntakeSubsystem") {
	mp_intakeArmSolenoid.reset(
			new DoubleSolenoid(RobotMap::PCM_CAN, RobotMap::PCM_INTAKE_DOWN,
					RobotMap::PCM_INTAKE_UP));
	mp_rollers.reset(new CANTalon(RobotMap::CT_INTAKE_ROLLER));
	mp_rollers->SetControlMode(CANTalon::kPercentVbus);
	// don't auto-disable if rollIn/rollOut isn't repeatedly called
	mp_rollers->SetSafetyEnabled(false);
	mp_intakeDownSwitch.reset(
			new DigitalInput(RobotMap::LIMIT_SWITCH_INTAKE_DOWN));
	mp_intakeUpSwitch.reset(new DigitalInput(RobotMap::LIMIT_SWITCH_INTAKE_UP));
}

void IntakeSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void IntakeSubsystem::rollIn() {
	mp_rollers->Set(ROLLER_SPEED);
}

void IntakeSubsystem::rollOut() {
	mp_rollers->Set(-ROLLER_SPEED);
}

void IntakeSubsystem::rollStop() {
	mp_rollers->Set(0);
}

double IntakeSubsystem::rollerRevolutions(){
	return mp_rollers->GetPosition();
}

void IntakeSubsystem::intakeArmUp(){
	mp_intakeArmSolenoid->Set(DoubleSolenoid::kReverse);
}

void IntakeSubsystem::intakeArmDown(){
	mp_intakeArmSolenoid->Set(DoubleSolenoid::kForward);
}

void IntakeSubsystem::sendValuesToSmartDashboard(){

}
