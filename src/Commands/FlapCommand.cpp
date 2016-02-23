#include <Stronghold2016Robot.h>

FlapCommand::FlapCommand() :
		CommandBase("FlapCommand_"), leftLowLimit("FlapLeftLowLimit"), leftHighLimit(
				"FlapLeftHighLimit"), rightLowLimit("FlapRightLowLimit"), rightHighLimit(
				"FlapRightHighLimit") {
	Requires(flapSubsystem.get());
	SetInterruptible(true);
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

		double leftFlapPosition = (leftHighLimit.get() - leftLowLimit.get()) * fraction
				+ leftLowLimit.get();
		double rightFlapPosition = (rightHighLimit.get() - rightLowLimit.get()) * fraction
				+ rightLowLimit.get();
		flapSubsystem->setLeftFlapAngle(leftFlapPosition);
		flapSubsystem->setRightFlapAngle(rightFlapPosition);
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
