#include <Stronghold2016Robot.h>

LeftFlapTestCommand::LeftFlapTestCommand() :
		CommandBase("LeftFlapTestCommand_") {
	Requires(shooterSubsystem.get());
	m_ticks = 0;
	done = false;
}

void LeftFlapTestCommand::Initialize() {
	printf("FlapTest initialized\n");
	m_ticks = 0;
	shooterSubsystem->setLeftFlap(0.3);
	printf("LeftFlap set to 0\n");
}

void LeftFlapTestCommand::Execute() {
	if (m_ticks++>=100) {
		shooterSubsystem->setLeftFlap(80);
		printf("LeftFlap set to 1\n");
		done = true;
	}
	printf("1 cycle of the command run\n");
}

bool LeftFlapTestCommand::IsFinished() {
	return done;
}

void LeftFlapTestCommand::End() {
}

void LeftFlapTestCommand::Interrupted() {
}
