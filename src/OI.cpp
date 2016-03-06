#include <Stronghold2016Robot.h>

OI::OI() :
		controllerButtons(), buttonsPressed() {
	pXboxController = new Joystick(0);
	pFarmController = new Joystick(1);
}

void OI::registerCommands() {
	registerButton(pXboxController, 1, PRESSED,
			CommandBase::toggleCameraFeedCommand.get());
//	registerButton(pXboxController, 2, PRESSED,
//			CommandBase::flapCommand.get());
	registerButton(pXboxController, 3, PRESSED,
			CommandBase::shootCommand.get());
	registerButton(pXboxController, 4, PRESSED,
			CommandBase::wedgeToggleCommand.get());
	registerButton(pXboxController, 5, PRESSED,
			CommandBase::gearShiftCommand.get());
	registerButton(pXboxController, 6, PRESSED,
			CommandBase::intakeToggleCommand.get());
	registerButton(pXboxController, 7, PRESSED,
			CommandBase::toggleLedCommand.get());
	registerButton(pXboxController, 8, PRESSED,
			CommandBase::toggleCompressorCommand.get());

	// avoid excessive errors if this joystick isn't connected
	m_secondControllerPresent = pFarmController->GetButtonCount() > 0;

	// we register the buttons even if the controller wasn't present on startup
	// because it might be connected later
	// change these buttons to what's convenient on the joystick
	registerButton(pFarmController, 1, PRESSED,
			CommandBase::toggleCameraFeedCommand.get());
	registerButton(pFarmController, 2, PRESSED,
			CommandBase::wedgeToggleCommand.get()); // if we have enough buttons,
	registerButton(pFarmController, 3, PRESSED,
			CommandBase::intakeToggleCommand.get());// make separate up/down buttons
	// 4, 5 roll in/out (full speed) in IntakeRollerCommand
	registerButton(pFarmController, 3, PRESSED,
				CommandBase::centerOnTargetCommand.get());
	registerButton(pFarmController, 7, PRESSED,
			CommandBase::toggleLedCommand.get());
	registerButton(pFarmController, 8, PRESSED,
			CommandBase::toggleCompressorCommand.get());
	registerButton(pFarmController, 13, PRESSED,
				new AutoSetFlapsCommand());

	SmartDashboard::PutData(CommandBase::centerOnTargetCommand.get());
	SmartDashboard::PutData(new AutonomousShootSequence());

	SmartDashboard::PutData(new RotateIMUCommand(90));
	SmartDashboard::PutData(new DriveDistanceIMUCommand(1, 0.7));
}

bool OI::joystickButtonPressed(Joystick* pJoystick, int buttonNumber) {
	bool pressed = false;

	// find the vector to use
	JoystickPressed_t::iterator buttonsPressedIterator = buttonsPressed.find(
			(uintptr_t) pJoystick);
	if (buttonsPressedIterator == buttonsPressed.end()) {
		std::vector<bool> apButtonsPressed;
		for (int i = 0; i < 30; i++) {
			apButtonsPressed.push_back(false);
		}
		buttonsPressed.insert(
				std::pair<uintptr_t, std::vector<bool>>((uintptr_t) pJoystick,
						apButtonsPressed));
	}

	buttonsPressedIterator = buttonsPressed.find((uintptr_t) pJoystick);

	// check if index is there
	buttonsPressedIterator->second.at(buttonNumber) = pressed;

	bool joystickPressed = pJoystick->GetRawButton(buttonNumber);
	bool returnValue = false;
	if (joystickPressed && !pressed) {
		returnValue = true;
	}

	buttonsPressedIterator->second.at(buttonNumber) = joystickPressed;

	return returnValue;
}

void OI::registerButton(Joystick* pJoystick, int buttonNumber, ButtonEvent when,
		Command* pCommand) {
	JoystickButton* button = NULL;

	// find the vector to use
	JoystickMap_t::iterator cIt = controllerButtons.find((uintptr_t) pJoystick);
	if (cIt == controllerButtons.end()) {
		std::vector<JoystickButton*> apButtons(31);
		controllerButtons.insert(
				std::pair<uintptr_t, std::vector<JoystickButton*>>(
						(uintptr_t) pJoystick, apButtons));
	}

	cIt = controllerButtons.find((uintptr_t) pJoystick);

	// check if joystick is there
	if (cIt->second.at(buttonNumber) != NULL) {
		button = cIt->second.at(buttonNumber);
	} else {
		button = new JoystickButton(pJoystick, buttonNumber);
		cIt->second.at(buttonNumber) = button;
	}

	// attach the command to the correct event
	switch (when) {
	case ACTIVE:
		button->WhenActive(pCommand);
		break;
	case INACTIVE:
		button->WhenInactive(pCommand);
		break;
	case PRESSED:
		button->WhenPressed(pCommand);
		break;
	case RELEASED:
		button->WhenReleased(pCommand);
		break;
	}
}
