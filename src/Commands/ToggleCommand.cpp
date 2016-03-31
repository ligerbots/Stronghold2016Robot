#include "ToggleCommand.h"

/**
 * This command toggles another command. ie, if the command is not running, it starts the command
 * If the command is running, it cancels it and restarts all commands passed in the vector of
 * commands to restart
 *
 * This is useful for joystick buttons that cancel their long-running command on a second press
 * Currently used for the intake command
 */

ToggleCommand::ToggleCommand(Command* whatToCall) :
		mp_whatToCall(whatToCall) {
}

ToggleCommand::ToggleCommand(Command* whatToCall, std::vector<Command*> whatToRestart) :
		mp_whatToCall(whatToCall), ma_restartCommands(whatToRestart) {
}

void ToggleCommand::Initialize() {
	if(mp_whatToCall->IsRunning()){
		mp_whatToCall->Cancel();
		for(std::vector<Command*>::iterator it = ma_restartCommands.begin(); it != ma_restartCommands.end(); ++it){
			(*it)->Start();
		}
	} else {
		mp_whatToCall->Start();
	}
}

void ToggleCommand::Execute() {
}

bool ToggleCommand::IsFinished() {
	return true;
}

void ToggleCommand::End() {
}

void ToggleCommand::Interrupted() {
}
