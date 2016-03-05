#include <Stronghold2016Robot.h>

AutonomousShootCommand::AutonomousShootCommand() :
		CommandBase("AutonomousShootCommand") {
	Requires(shooterSubsystem.get());
	Requires(wedgeSubsystem.get());
	Requires(intakeSubsystem.get());
	Requires(flapSubsystem.get());
	SetInterruptible(false);
	m_ticks = 0;
	flapAngle = 0;
	isSafe = false;
}

void AutonomousShootCommand::Initialize() {
	m_ticks = 0;
	flapAngle = visionSubsystem->getFlapAngle(
			visionSubsystem->getDistanceToTarget());
	isSafe = intakeSubsystem->isIntakeArmUp();
}

void AutonomousShootCommand::Execute() {
	if (!isSafe)
		return;
	m_ticks++;
	flapSubsystem->setFlapsFraction(flapAngle, flapAngle);
	if (m_ticks == 25) {
		shooterSubsystem->firePistons();
	}
}

bool AutonomousShootCommand::IsFinished() {
	return !isSafe || m_ticks > 50;
}

void AutonomousShootCommand::End() {
	shooterSubsystem->retractPistons();
}

void AutonomousShootCommand::Interrupted() {

}
