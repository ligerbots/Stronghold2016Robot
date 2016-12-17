#include <Stronghold2016Robot.h>

ShootCommand::ShootCommand() :
		CommandBase("ShootCommand_") {
	m_done = false;
	m_ticks = 0;
	Requires(shooterSubsystem.get());
	Requires(wedgeSubsystem.get());
	Requires(intakeSubsystem.get());
	SetInterruptible(false);
}

void ShootCommand::Initialize() {
	m_done = false;
	m_ticks = 0;

	double currentYaw = navXSubsystem->GetYaw();
	double yawError = fabs(currentYaw - Robot::end_of_centering_yaw);
	if(yawError > 180){ // get the shorter angle
		yawError = yawError - 180;
	}

	if (/*!wedgeSubsystem->isWedgeDown()
			&& */!intakeSubsystem->isIntakeArmUp()) {
		m_done = true;
		printf("ShooterCommand: not safe to fire\n");
	} else if(GetGroup() != NULL && yawError > 5.0){
		// if this isn't the "plain" shoot command but part of ShootSequence
		// and we were hit, don't shoot
		// bumps should be well over 5 degrees; we don't want a potential
		// sudden 1 degree drift that we saw on 2nd robot to screw us up
		m_done = true;
		printf("ShooterCommand: robot is hit; not shooting\n");
	} else {
		// turns on the shooting animation on the same tick as the actual shooter fire
		// opponents should have no time to react
		Robot::instance->SetLeds(Robot::SHOOT);
	}
	SetInterruptible(false);
}

void ShootCommand::Execute() {
	m_ticks++;
	Robot::instance->mp_operatorInterface->mp_XboxController->SetRumble(Joystick::RumbleType::kLeftRumble, m_ticks < 15);
	Robot::instance->mp_operatorInterface->mp_XboxController->SetRumble(Joystick::RumbleType::kRightRumble, m_ticks < 15);
	if (/*wedgeSubsystem->isWedgeDown() &&*/ intakeSubsystem->isIntakeArmUp()
			&& m_ticks == 1) {
		printf("ShooterCommand: firing\n");
		shooterSubsystem->firePiston();
	} else if (m_ticks == 1) { // // equivalent to: !wedgeSubsystem->isWedgeDown() && intakeSubsystem->isIntakeReadyToFire()
		m_done = true;
		printf("ShooterCommand: not safe to fire\n");
	}
	if (m_ticks == 25) { // wait for shooting to complete
		m_done = true;
	}
}

bool ShootCommand::IsFinished() {
	return m_done;
}

void ShootCommand::End() {
	shooterSubsystem->retractPiston(); // retract piston once the ball is shot (1/2 second after fire)
	Robot::instance->mp_operatorInterface->mp_XboxController->SetRumble(Joystick::RumbleType::kLeftRumble, 0);
	Robot::instance->mp_operatorInterface->mp_XboxController->SetRumble(Joystick::RumbleType::kRightRumble, 0);

	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL)
		CommandBase::intakeRollerCommand->Start();
}

void ShootCommand::Interrupted() {
	//NO
	Robot::instance->mp_operatorInterface->mp_XboxController->SetRumble(Joystick::RumbleType::kLeftRumble, 0);
	Robot::instance->mp_operatorInterface->mp_XboxController->SetRumble(Joystick::RumbleType::kRightRumble, 0);
}
