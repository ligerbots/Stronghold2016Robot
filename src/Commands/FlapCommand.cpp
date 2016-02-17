#include <Stronghold2016Robot.h>

FlapCommand::FlapCommand() :
		CommandBase("FlapCommand_") {
	Requires(shooterSubsystem.get());
	m_ticks = 0;
	m_position = 0.3;
	printf("Flaps %f\n", m_position);
    done = false;
}

void FlapCommand::Initialize() {
	printf("FlapTest initialized\n");
	m_ticks = 0;
	done = false;
	shooterSubsystem->setFlaps(1-m_position);
	printf("Flaps inverted %f\n", m_position);
	done = true;
}

void FlapCommand::Execute() {
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
