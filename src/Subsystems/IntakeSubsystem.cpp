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
		m_rollerTalonPresent = false;
	} else {
		m_rollerTalonPresent = true;
	}

	m_robotTickWhenSetUp = -1;

	mp_ballInShooterSwitch.reset(
			new DigitalInput(RobotMap::LIMIT_SWITCH_INTAKE_BALL_IN_SHOOTER));
//	mp_ballInShooterCounter.reset(new Counter(mp_ballInShooterSwitch.get()));
	mp_ballInDefensePositionSwitch.reset(
			new DigitalInput(
					RobotMap::LIMIT_SWITCH_INTAKE_BALL_DEFENSES_POSITION));
//	mp_ballInDefensePositionCounter.reset(new Counter(mp_ballInDefensePositionSwitch.get()));
	mp_intakeUpSwitch.reset(new DigitalInput(RobotMap::LIMIT_SWITCH_INTAKE_UP));
}

void IntakeSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void IntakeSubsystem::rollIn() {
	if (m_rollerTalonPresent)
		mp_rollers->Set(ROLLER_SPEED);
}

void IntakeSubsystem::rollOut() {
	if (m_rollerTalonPresent)
		mp_rollers->Set(-ROLLER_SPEED);
}

void IntakeSubsystem::rollStop() {
	if (m_rollerTalonPresent)
		mp_rollers->Set(0);
}

void IntakeSubsystem::setRollSpeed(double speed) {
	if (m_rollerTalonPresent)
		mp_rollers->Set(speed);
}

double IntakeSubsystem::getRollerRevolutions() {
	if (!m_rollerTalonPresent)
		return 0.0;
	return mp_rollers->GetPosition();
}

void IntakeSubsystem::setIntakeArmUp() {
	mp_intakeArmSolenoid->Set(DoubleSolenoid::kReverse);

	m_robotTickWhenSetUp = Robot::ticks;
}

void IntakeSubsystem::setIntakeArmDown() {
	mp_intakeArmSolenoid->Set(DoubleSolenoid::kForward);
}

DoubleSolenoid::Value IntakeSubsystem::getIntakeArmValue() {
	if (!m_rollerTalonPresent)
		return DoubleSolenoid::kOff;
	return mp_intakeArmSolenoid->Get();
}

bool IntakeSubsystem::isIntakeArmUp() {
#ifdef ROBOT_2_TEST
#pragma message "Warning: disabling intake arm check. Run without -DROBOT_2_TEST to enable"
	return true;
#else
	// flip value so that it's false if it's not connected
	// TODO: DANGER
	return mp_intakeArmSolenoid->Get() == DoubleSolenoid::kReverse && (Robot::ticks - m_robotTickWhenSetUp) > 30;//!mp_intakeUpSwitch->Get();
#endif
}

bool IntakeSubsystem::isBallInShooterPosition() {
	return mp_ballInShooterSwitch->Get();
}

bool IntakeSubsystem::isBallInDefensesCrossingPosition(){
	return mp_ballInDefensePositionSwitch->Get();
}

bool IntakeSubsystem::isIntakeReadyToFire() {
	return isIntakeArmUp() && isBallInShooterPosition();
}

void IntakeSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("Intake/Arm_Clear", isIntakeArmUp() /*!mp_intakeUpSwitch->Get()*/);
	SmartDashboard::PutBoolean("Intake/Ball_In", isBallInShooterPosition());
	SmartDashboard::PutBoolean("Intake/Ball_Defenses", isBallInDefensesCrossingPosition());
	SmartDashboard::PutNumber("Intake/Roller_Revolutions",
			m_rollerTalonPresent ? getRollerRevolutions() : 0);

	SmartDashboard::PutBoolean("Intake/RollerStatus", m_rollerTalonPresent);
	SmartDashboard::PutNumber("Intake/RollerWatts",
			m_rollerTalonPresent ?
					mp_rollers->GetOutputVoltage()
							* mp_rollers->GetOutputCurrent() :
					0);
	if (mp_intakeArmSolenoid->GetError().GetCode() != 0) {
		SmartDashboard::PutString("Intake/Solenoid", "Not Present");
	} else {
		DoubleSolenoid::Value val = mp_intakeArmSolenoid->Get();
		if (val == DoubleSolenoid::kOff) {
			SmartDashboard::PutString("Intake/Solenoid", "Off");
		} else if (val == DoubleSolenoid::kForward) {
			SmartDashboard::PutString("Intake/Solenoid", "Forward/Down");
		} else if (val == DoubleSolenoid::kReverse) {
			SmartDashboard::PutString("Intake/Solenoid", "Reverse/Up");
		}
	}
}
