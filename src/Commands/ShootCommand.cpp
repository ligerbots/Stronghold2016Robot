#include <Stronghold2016Robot.h>

ShootCommand::ShootCommand() :
		CommandBase("ShootCommand_") {
	done = false;
	m_ticks = 0;
	Requires(shooterSubsystem.get());
	Requires(wedgeSubsystem.get());
	Requires(intakeSubsystem.get());
	SetInterruptible(false);
}

void ShootCommand::Initialize() {
	done = false;
	m_ticks = 0;
	if (/*!wedgeSubsystem->isWedgeDown()
			&& */!intakeSubsystem->isIntakeArmUp()) {
		done = true;
		printf("ShooterCommand: not safe to fire\n");
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
		done = true;
		printf("ShooterCommand: not safe to fire\n");
	}
	if (m_ticks == 100) {
		done = true;
	}
}

bool ShootCommand::IsFinished() {
	return done;
}

void ShootCommand::End() {
	shooterSubsystem->retractPistons();
	CommandBase::intakeRollerCommand->Start();
}

void ShootCommand::Interrupted() {
	//NO
}
