#include <Stronghold2016Robot.h>

RollBallPickupThenShooter::RollBallPickupThenShooter() {
	AddSequential(
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::PICKUP));
	AddSequential(
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::SHOOTING_POSITION));
	m_alreadyInShooter = false;
}

void RollBallPickupThenShooter::Initialize(){
	m_alreadyInShooter = CommandBase::intakeSubsystem->isBallInShooterPosition();
}

bool RollBallPickupThenShooter::IsFinished(){
	if(m_alreadyInShooter){
		return true;
	} else{
		return CommandGroup::IsFinished();
	}
}

void RollBallPickupThenShooter::End(){
	CommandGroup::End();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::flapCommand->Start();
	}
}
