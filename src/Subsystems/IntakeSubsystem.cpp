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

	mp_rollers->GetOutputVoltage();
	if (mp_rollers->GetError().GetCode() != 0) {
		printf("Intake: Roller talon not present!\n");
		rollerTalonPresent = false;
	} else {
		rollerTalonPresent = true;
	}

	mp_ballInSwitch.reset(
			new DigitalInput(RobotMap::LIMIT_SWITCH_INTAKE_BALL_IN));
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

double IntakeSubsystem::getRollerRevolutions() {
	return mp_rollers->GetPosition();
}

void IntakeSubsystem::setIntakeArmUp() {
	mp_intakeArmSolenoid->Set(DoubleSolenoid::kReverse);
}

void IntakeSubsystem::setIntakeArmDown() {
	mp_intakeArmSolenoid->Set(DoubleSolenoid::kForward);
}

bool IntakeSubsystem::isIntakeArmUp() {
	return mp_intakeUpSwitch->Get();
}

bool IntakeSubsystem::isBallInPosition() {
	return mp_ballInSwitch->Get();
}

bool IntakeSubsystem::isIntakeReadyToFire() {
	return isIntakeArmUp() && isBallInPosition();
}

void IntakeSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("Intake/Arm_Up", isIntakeArmUp());
	SmartDashboard::PutBoolean("Intake/Ball_In", isBallInPosition());
	SmartDashboard::PutNumber("Intake/Roller_Revolutions",
			getRollerRevolutions());

	SmartDashboard::PutBoolean("Intake/RollerStatus", rollerTalonPresent);
	SmartDashboard::PutNumber("Intake/RollerWatts",
			rollerTalonPresent ?
					mp_rollers->GetOutputVoltage()
							* mp_rollers->GetOutputCurrent() :
					0);
	if (mp_intakeArmSolenoid->GetError().GetCode() != 0) {
		SmartDashboard::PutBoolean("Intake/Solenoid", "Not Present");
	} else {
		DoubleSolenoid::Value val = mp_intakeArmSolenoid->Get();
		if (val == DoubleSolenoid::kOff) {
			SmartDashboard::PutBoolean("Intake/Solenoid", "Off");
		} else if (val == DoubleSolenoid::kForward) {
			SmartDashboard::PutBoolean("Intake/Solenoid", "Forward/Down");
		} else if (val == DoubleSolenoid::kReverse) {
			SmartDashboard::PutBoolean("Intake/Solenoid", "Reverse/Up");
		}
	}
}
