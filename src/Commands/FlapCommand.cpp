#include <Stronghold2016Robot.h>

FlapCommand::FlapCommand() :
		CommandBase("FlapCommand_"), flapPositionLeft("FlapCommandPositionLeft",
				false), flapPositionRight("FlapCommandPositionRight", false) {
	Requires(flapSubsystem.get());
	SetInterruptible(true);
	oi = Robot::instance->mp_operatorInterface;

	flapPositionLeft = 0;
	flapPositionRight = 0;
}

void FlapCommand::Initialize() {
	printf("FlapCommand initialized\n");
	// don't bash potential previous values
	flapPositionLeft = flapSubsystem->getLeftFlapFraction();
	flapPositionRight = flapSubsystem->getRightFlapFraction();
}

void FlapCommand::Execute() {
//	int hat = Robot::instance->mp_operatorInterface->pXboxController->GetPOV(0);

	int flapButtons =
			oi->get2ndControllerButton(6) ? 0 :
					oi->get2ndControllerButton(7) ? 4 :
							oi->get2ndControllerButton(8) ? 5 :
									oi->get2ndControllerButton(3) ? 6 :
											oi->get2ndControllerButton(2) ? 8 : -1;

	if (flapButtons != -1) {
		double fraction = 1;
		if(flapButtons > 0){
			fraction = VisionSubsystem::angles[flapButtons];
		}

		printf("Setting flaps to %f\n", fraction);

		flapPositionLeft = fraction;
		flapPositionRight = fraction;

		// there should be a codan prefs file in the project that removes the
		// pesky eclipse error on this line. If there isn't the error is safe
		// to ignore
		flapSubsystem->setFlapsFraction(fraction);
	} else {
		if (oi->get2ndControllerButton(25)) {
			if (oi->get2ndControllerButton(26)) {
				flapPositionLeft -= .01;
				flapPositionRight -= .01;
			}

			if (oi->get2ndControllerButton(27)) {
				flapPositionLeft += .01;
				flapPositionRight += .01;
			}
		}

		flapSubsystem->setFlapsFraction(/*flapPositionLeft.get(), */flapPositionRight.get());
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
