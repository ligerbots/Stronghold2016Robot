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
	if (/*!wedgeSubsystem->isWedgeDown()
			&& */!intakeSubsystem->isIntakeArmUp()) {
		m_done = true;
		printf("ShooterCommand: not safe to fire\n");
	} else {
		// turns on the shooting animation on the same tick as the actual shooter fire
		// opponents should have no time to react
		Robot::instance->SetLeds(Robot::SHOOT);
	}
	SetInterruptible(false);
}

void ShootCommand::Execute() {
	m_ticks++;
	if (/*wedgeSubsystem->isWedgeDown() &&*/ intakeSubsystem->isIntakeArmUp()
			&& m_ticks == 1) {
		printf("ShooterCommand: firing\n");
		shooterSubsystem->firePistons();
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
	shooterSubsystem->retractPistons(); // retract piston once the ball is shot (1/2 second after fire)

	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL)
		CommandBase::intakeRollerCommand->Start();
}

void ShootCommand::Interrupted() {
	//NO
}
