#include <Stronghold2016Robot.h>

ShootCommand::ShootCommand() :
		CommandBase("ShootCommand_") {
	done = false;
	m_ticks = 0;
	Requires(shooterSubsystem.get());
	Requires(wedgeSubsystem.get());
	Requires(intakeSubsystem.get());
}

void ShootCommand::Initialize() {
	done = false;
	m_ticks = 0;
	if (not (wedgeSubsystem->isWedgeDown() && intakeSubsystem->isIntakeReadyToFire())) {
		done = true;
	}
}

void ShootCommand::Execute() {
	m_ticks++;
	if (wedgeSubsystem->isWedgeDown() && intakeSubsystem->isIntakeReadyToFire() && m_ticks == 1) {
			shooterSubsystem->firePistons();
		}
	// equivalent to: not (wedgeSubsystem->isWedgeDown() && intakeSubsystem->isIntakeReadyToFire())
	else if (m_ticks == 1) {
		done = true;
	}
	if (m_ticks == 100) {
		shooterSubsystem->retractPistons();
		done = true;
	}
}

bool ShootCommand::IsFinished() {
	return done;
}

void ShootCommand::End() {

}

void ShootCommand::Interrupted() {

}
