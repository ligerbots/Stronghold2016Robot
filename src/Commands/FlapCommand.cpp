#include <Stronghold2016Robot.h>

FlapCommand::FlapCommand() :
		CommandBase("LeftFlapTestCommand_") {
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
	shooterSubsystem->setFlaps(m_position);
	printf("Flaps inverted %f\n", m_position);
}

void FlapCommand::Execute() {
	m_ticks++;
	if (m_ticks < 50) {
		shooterSubsystem->setFlaps(m_position);
	}
	else done = true;
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
