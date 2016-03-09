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
	// don't bash potential previous values
	flapPositionLeft = flapSubsystem->getLeftFlapFraction();
	flapPositionRight = flapSubsystem->getRightFlapFraction();
}

void FlapCommand::Execute() {
//	int hat = Robot::instance->mp_operatorInterface->pXboxController->GetPOV(0);

	int flapButtons =
			Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(6) ? 0 :
			Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(7) ? 4 :
			Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(8) ? 5 :
			Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(3) ? 6 :
			Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(2) ? 8 : -1;

	if (flapButtons != -1) {
		double fraction = 1;
		if(flapButtons > 0){
			fraction = VisionSubsystem::angles[flapButtons];
		}

		flapPositionLeft = fraction;
		flapPositionRight = fraction;

		// there should be a codan prefs file in the project that removes the
		// pesky eclipse error on this line. If there isn't the error is safe
		// to ignore
		flapSubsystem->setFlapsFraction(fraction);
	} else {

		if(Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(25)){
			if(Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(26)){
				flapPositionLeft -= .01;
				flapPositionRight -= .01;
			}

			if(Robot::instance->mp_operatorInterface->pFarmController->GetRawButton(27)){
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
