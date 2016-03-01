#include <Stronghold2016Robot.h>

FlapCommand::FlapCommand() :
		CommandBase("FlapCommand_"), flapPositionLeft("FlapCommandPositionLeft",
				false), flapPositionRight("FlapCommandPositionRight", false) {
	Requires(flapSubsystem.get());
	SetInterruptible(true);

	flapPositionLeft = 0;
	flapPositionRight = 0;
}

void FlapCommand::Initialize() {
	printf("FlapCommand initialized\n");
}

void FlapCommand::Execute() {
	int hat = Robot::instance->mp_operatorInterface->pXboxController->GetPOV(0);
	if (hat != -1) {
		double fraction = 0; // angle = 0 or 360
		if (hat == 90)
			fraction = 1.0 / 3.0;
		else if (hat == 180)
			fraction = 2.0 / 3.0;
		else if (hat == 270)
			fraction = 1;

		flapPositionLeft = fraction;
		flapPositionRight = fraction;

		// there should be a codan prefs file in the project that removes the
		// pesky eclipse error on this line. If there isn't the error is safe
		// to ignore
		flapSubsystem->setFlapsFraction(fraction);
	} else {
		flapSubsystem->setFlapsFraction(flapPositionLeft.get(), flapPositionRight.get());
	}
}

bool FlapCommand::IsFinished() {
	return false;
}

void FlapCommand::End() {
	printf("FlapCommand: Done\n");
}

void FlapCommand::Interrupted() {
	printf("FlapCommand: Interrupted\n");
}
