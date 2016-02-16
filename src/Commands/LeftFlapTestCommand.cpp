#include <Stronghold2016Robot.h>

LeftFlapTestCommand::LeftFlapTestCommand() :
		CommandBase("LeftFlapTestCommand_") {
	Requires(shooterSubsystem.get());
	m_ticks = 0;
	done = false;
}

void LeftFlapTestCommand::Initialize() {
	printf("FlapTest initialized\n");
//	m_ticks = 0;
	shooterSubsystem->setFlaps(1+(-1*(shooterSubsystem->getLeftPosition())));
	printf("Flaps inverted");
	done = true;
}

void LeftFlapTestCommand::Execute() {
//	m_ticks++;
//	if (m_ticks == 50) {
//		shooterSubsystem->setFlaps(0.0);
//		printf("LeftFlap at %3.2f\n", shooterSubsystem->getLeftPosition());
//		printf("RightFlap at %3.2f\n", shooterSubsystem->getRightPosition());
//		printf("Flaps set to 0\n");
//	}
//	if (m_ticks == 100) {
//		shooterSubsystem->setFlaps(1.0);
//		printf("LeftFlap at %3.2f\n", shooterSubsystem->getLeftPosition());
//		printf("RightFlap at %3.2f\n", shooterSubsystem->getRightPosition());
//		printf("Flaps set to 1\n");
//		m_ticks = 0;
//	}
}

bool LeftFlapTestCommand::IsFinished() {
	return done;
}

void LeftFlapTestCommand::End() {
	printf("Flap Test Done\n");
}

void LeftFlapTestCommand::Interrupted() {
	printf("FlapTest Interrupted");
}
