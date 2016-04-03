#include <Stronghold2016Robot.h>

CenterOnTargetCommand::CenterOnTargetCommand() :
		CommandBase("CenterOnTargetCommand"), mp_softwarePID(NULL), m_centerTo(0), centerMediumZone(
				"CenterMediumZone"), centerSlowZone("CenterSlowZone"), fastSpeed(
				"CenterFastSpeed"), mediumSpeed("CenterMediumSpeed"), slowSpeed(
				"CenterSlowSpeed") {
	Requires(visionSubsystem.get());
	Requires(driveSubsystem.get());
//	if (mp_softwarePID == NULL) {
//		mp_softwarePID = new PIDController(
//				Preferences::GetInstance()->GetDouble("CenterOnTargetP", 5),
//				Preferences::GetInstance()->GetDouble("CenterOnTargetI", 0),
//				Preferences::GetInstance()->GetDouble("CenterOnTargetD", 0),
//				visionSubsystem.get(), driveSubsystem->turnPIDOutput.get());
//	}
//	mp_softwarePID->SetAbsoluteTolerance(0.02);
//	SmartDashboard::PutData("CenterOnTargetPID", mp_softwarePID);

	m_ticksSinceCentered = 0;
	m_isCentered = false;
}

void CenterOnTargetCommand::Initialize() {
	printf("CenterOnTarget: initialize\n");
	visionSubsystem->setVisionEnabled(true);
	visionSubsystem->setLedRingState(true);
	driveSubsystem->zeroMotors();
	driveSubsystem->shiftDown(); // untested in high gear

	SmartDashboard::PutNumber("AngleBeforeFineCenter", navXSubsystem->GetYaw());

	SetTimeout(5);

//	Preferences::GetInstance()->PutDouble("CenterOnTargetP",
//			mp_softwarePID->GetP());
//	Preferences::GetInstance()->PutDouble("CenterOnTargetI",
//			mp_softwarePID->GetI());
//	Preferences::GetInstance()->PutDouble("CenterOnTargetD",
//			mp_softwarePID->GetD());
//
//	mp_softwarePID->SetPID(mp_softwarePID->GetP(), 0, mp_softwarePID->GetD());
//
//	mp_softwarePID->Reset();
//	mp_softwarePID->SetSetpoint(centerTo);
//	mp_softwarePID->Enable();
}

void CenterOnTargetCommand::Execute() {
//	mp_softwarePID->SetSetpoint(centerTo);
//	printf("%f %f\n", izone.get(), mp_softwarePID->GetI());
//	if (fabs(mp_softwarePID->GetError()) < izone.get()) {
//		mp_softwarePID->SetPID(
//				Preferences::GetInstance()->GetDouble("CenterOnTargetP"),
//				Preferences::GetInstance()->GetDouble("CenterOnTargetI"),
//				Preferences::GetInstance()->GetDouble("CenterOnTargetD"));
//	} else {
//		mp_softwarePID->SetPID(mp_softwarePID->GetP(), 0,
//				mp_softwarePID->GetD());
//	}

	if (!visionSubsystem->isTargetVisible()) {
		driveSubsystem->zeroMotors();
		return;
	}

	// continuously run vision for this command, since it's closed loop
	// see RotateToTarget for centering without continuous vision
	visionSubsystem->requestVisionFrame();

	m_centerTo = visionSubsystem->getCorrectedFrameCenter(
			visionSubsystem->getDistanceToTarget());

	double error = m_centerTo - visionSubsystem->PIDGet();
	double sign = error < 0 ? -1 : 1;
	if (!m_isCentered) {
		if (fabs(error) > centerMediumZone.get()) {
			driveSubsystem->turnPIDOutput->PIDWrite(fastSpeed.get() * sign); // .7
		} else if (fabs(error) > centerSlowZone.get()) {
			driveSubsystem->turnPIDOutput->PIDWrite(mediumSpeed.get() * sign); // .4
		} else {
			driveSubsystem->turnPIDOutput->PIDWrite(slowSpeed.get() * sign); // .32
		}
	} else {
		driveSubsystem->zeroMotors();
	}
}

bool CenterOnTargetCommand::IsFinished() {
	if (Robot::instance->mp_operatorInterface->getSecondControllerRawButton(28)) {
		printf("Center: ending command\n");
		return true;
	}

	m_isCentered = fabs(m_centerTo - visionSubsystem->PIDGet())
			< ACCEPTABLE_ERROR;
	if (m_isCentered) {
		m_ticksSinceCentered++;
	} else {
		m_ticksSinceCentered = 0;
	}

	return IsTimedOut() || (m_isCentered && m_ticksSinceCentered > 15);
}

void CenterOnTargetCommand::End() {
	printf("CenterOnTarget: end\n");
//	printf("\tOn target: %d\n", mp_softwarePID->OnTarget());
//	mp_softwarePID->Disable();
	driveSubsystem->zeroMotors();

	SmartDashboard::PutNumber("AngleAfterFineCenter", navXSubsystem->GetYaw());
	Robot::end_of_centering_yaw = navXSubsystem->GetYaw();

//	SmartDashboard::PutData("CenterOnTargetPID", mp_softwarePID);
	if (DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL)
		CommandBase::driveJoystickCommand->Start();
}

void CenterOnTargetCommand::Interrupted() {
//	printf("CenterOnTarget: interrupted\n");
//	mp_softwarePID->Disable();
	driveSubsystem->zeroMotors();
//	SmartDashboard::PutData("CenterOnTargetPID", mp_softwarePID);
}
