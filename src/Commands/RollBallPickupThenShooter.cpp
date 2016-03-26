#include <Stronghold2016Robot.h>

RollBallPickupThenShooter::RollBallPickupThenShooter() {
	AddSequential(
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::PICKUP));
	AddSequential(
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::SHOOTING_POSITION));
}

void RollBallPickupThenShooter::End(){
	CommandGroup::End();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::flapCommand->Start();
	}
}
