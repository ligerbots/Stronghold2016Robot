#include <Stronghold2016Robot.h>

FlapCommand::FlapCommand() :
		CommandBase("FlapCommand_"), leftLowLimit("FlapLeftLowLimit"),
								    leftHighLimit("FlapLeftHighLimit"),
									rightLowLimit("FlapRightLowLimit"),
									rightHighLimit("FlapRightHighLimit")
{
	Requires(flapSubsystem.get());
	m_direction = false;
    m_done = false;
}

void FlapCommand::Initialize() {
	m_direction = !m_direction;
	printf("FlapTest initialized, direction = %s\n", m_direction ? "true" : "false");
	m_done = false;
}

void FlapCommand::Execute() {
	if (m_direction) {
		printf("Move flaps %s to %4.2f, %4.2f\n", m_direction ? "true" : "false",
				leftLowLimit.get(), rightHighLimit.get());
		flapSubsystem->setLeftFlapAngle(leftLowLimit.get());
		flapSubsystem->setRightFlapAngle(rightHighLimit.get());
	}
	else {
		printf("Move flaps to %s to %4.2f, %4.2f\n", m_direction ? "true" : "false",
				leftHighLimit.get(), rightLowLimit.get());
		flapSubsystem->setLeftFlapAngle(leftHighLimit.get());
		flapSubsystem->setRightFlapAngle(rightLowLimit.get());
	}
	m_done = true;
}

bool FlapCommand::IsFinished() {
	return m_done;
}

void FlapCommand::End() {
	printf("Flap Test Done\n");
}

void FlapCommand::Interrupted() {
	printf("FlapTest Interrupted");
}
