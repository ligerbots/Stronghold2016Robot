#include <Stronghold2016Robot.h>

CenterOnTargetCommand::CenterOnTargetCommand(double centerTo) :
		CommandBase("CenterOnTargetCommand"), mp_softwarePID(
				(PIDController*) SmartDashboard::GetData("CenterOnTargetPID")), centerTo(centerTo) {
	Requires(visionSubsystem.get());
	Requires(driveSubsystem.get());
	if (mp_softwarePID == NULL) {
		mp_softwarePID = new PIDController(
				Preferences::GetInstance()->GetDouble("CenterOnTargetP", 5),
				Preferences::GetInstance()->GetDouble("CenterOnTargetI", 0),
				Preferences::GetInstance()->GetDouble("CenterOnTargetD", 0),
				visionSubsystem.get(), driveSubsystem->turnPIDOutput.get());
	}
	mp_softwarePID->SetAbsoluteTolerance(0.02);
	SmartDashboard::PutData("CenterOnTargetPID", mp_softwarePID);
}

void CenterOnTargetCommand::Initialize() {
	printf("CenterOnTarget: initialize\n");
	driveSubsystem->zeroMotors();
	SetTimeout(20);

	Preferences::GetInstance()->PutDouble("CenterOnTargetP", mp_softwarePID->GetP());
	Preferences::GetInstance()->PutDouble("CenterOnTargetI", mp_softwarePID->GetI());
	Preferences::GetInstance()->PutDouble("CenterOnTargetD", mp_softwarePID->GetD());

	mp_softwarePID->Reset();
	mp_softwarePID->SetSetpoint(centerTo);
	mp_softwarePID->Enable();
}

void CenterOnTargetCommand::Execute() {
	mp_softwarePID->SetSetpoint(.5);
}

bool CenterOnTargetCommand::IsFinished() {
	return IsTimedOut() || mp_softwarePID->OnTarget();
}

void CenterOnTargetCommand::End() {
	printf("CenterOnTarget: end\n");
	mp_softwarePID->Disable();
	driveSubsystem->zeroMotors();
	CommandBase::driveJoystickCommand->Start();
}

void CenterOnTargetCommand::Interrupted() {
	printf("CenterOnTarget: interrupted\n");
	mp_softwarePID->Disable();
	driveSubsystem->zeroMotors();
	CommandBase::driveJoystickCommand->Start();
}
