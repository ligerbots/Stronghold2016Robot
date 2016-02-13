#include <Stronghold2016Robot.h>

CenterOnTargetCommand::CenterOnTargetCommand() :
		CommandBase("CenterOnTargetCommand"), m_softwarePID(0.0, 0.0, 0.0,
				visionSubsystem.get(), driveSubsystem->turnPIDOutput.get()) {
	Requires(visionSubsystem.get());
	Requires(driveSubsystem.get());
}

void CenterOnTargetCommand::Initialize() {
	printf("CenterOnTarget: initialize\n");
	driveSubsystem->zeroMotors();
	m_softwarePID.SetSetpoint(visionSubsystem->getFrameCenter());
	m_softwarePID.Enable();
}

void CenterOnTargetCommand::Execute() {
	m_softwarePID.SetSetpoint(visionSubsystem->getFrameCenter());
}

bool CenterOnTargetCommand::IsFinished() {
	return m_softwarePID.OnTarget();
}

void CenterOnTargetCommand::End() {
	printf("CenterOnTarget: end\n");
	m_softwarePID.Disable();
}

void CenterOnTargetCommand::Interrupted() {
	printf("CenterOnTarget: interrupted\n");
	m_softwarePID.Disable();
	driveSubsystem->zeroMotors();
}
