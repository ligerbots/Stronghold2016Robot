#include <Stronghold2016Robot.h>

FlapCommand::FlapCommand() :
		CommandBase("FlapCommand_") {
	Requires(shooterSubsystem.get());
	m_ticks = 0;
	m_position = 0.2;
    done = false;
}

void FlapCommand::Initialize() {
	printf("FlapTest initialized\n");
	m_ticks = 0;
	shooterSubsystem->setFlaps(0.0);
	printf("Flaps to 0\n");
	done = false;
}

void FlapCommand::Execute() {
	if (m_ticks++==100) {
		shooterSubsystem->setFlaps(0.9);
		printf("Flaps to 1\n");
		done = true;
	}
}

bool FlapCommand::IsFinished() {
	return done;
}

void FlapCommand::End() {
	printf("Flap Test Done\n");
}

void FlapCommand::Interrupted() {
	printf("FlapTest Interrupted");
}
