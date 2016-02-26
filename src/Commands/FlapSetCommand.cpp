#include <Stronghold2016Robot.h>

FlapSetCommand::FlapSetCommand(double fraction) :
		CommandBase("FlapSetCommand"), fraction(fraction) {
	Requires(flapSubsystem.get());
}

void FlapSetCommand::Initialize() {
	SetTimeout(1); // give enough time for flaps to get into position
	printf("FlapSetCommand: settings to %4.2f\n", fraction);
}

void FlapSetCommand::Execute() {
	flapSubsystem->setFlapsFraction(fraction);
}

bool FlapSetCommand::IsFinished() {
	return IsTimedOut();
}

void FlapSetCommand::End() {
	printf("FlapSetCommand: end\n");
}

void FlapSetCommand::Interrupted() {
	printf("FlapSetCommand: interrupted\n");
}
