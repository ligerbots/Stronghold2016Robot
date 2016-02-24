#include <Stronghold2016Robot.h>

CenterOnTargetCommand::CenterOnTargetCommand() :
		CommandBase("CenterOnTargetCommand"), m_softwarePID((PIDController*) SmartDashboard::GetData("CenterOnTargetPID")) {
	Requires(visionSubsystem.get());
	Requires(driveSubsystem.get());
	if(m_softwarePID == NULL){
		m_softwarePID = new PIDController(5, 0.0, 0.0,
						visionSubsystem.get(), driveSubsystem->turnPIDOutput.get());
	}
	m_softwarePID->SetAbsoluteTolerance(0.02);
	SmartDashboard::PutData("CenterOnTargetPID", m_softwarePID);
}

void CenterOnTargetCommand::Initialize() {
	printf("CenterOnTarget: initialize\n");
	driveSubsystem->zeroMotors();
	SetTimeout(20);
//	PIDController* second = SmartDashboard::GetData("CenterOnTargetPID");
//	m_softwarePID.SetPID(second->GetP(), second->GetI(), second->GetD());
	m_softwarePID->Reset();
	m_softwarePID->SetSetpoint(.5);
	m_softwarePID->Enable();
}

void CenterOnTargetCommand::Execute() {
	m_softwarePID->SetSetpoint(.5);
}

bool CenterOnTargetCommand::IsFinished() {
	return IsTimedOut() || m_softwarePID->OnTarget();
}

void CenterOnTargetCommand::End() {
	printf("CenterOnTarget: end\n");
	m_softwarePID->Disable();
	driveSubsystem->zeroMotors();
	CommandBase::driveJoystickCommand->Start();
}

void CenterOnTargetCommand::Interrupted() {
	printf("CenterOnTarget: interrupted\n");
	m_softwarePID->Disable();
	driveSubsystem->zeroMotors();
	CommandBase::driveJoystickCommand->Start();
}
