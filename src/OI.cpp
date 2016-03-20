#include <Stronghold2016Robot.h>

OI::OI() :
		controllerButtons(), buttonsPressed(), m_secondControllerButtonCount(0) {
	pXboxController = new Joystick(0);
	pFarmController = new Joystick(1);
}

void OI::registerCommands() {
	// XBox A command
	registerButton(pXboxController, 1, PRESSED, new ToggleCommand(new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::PICKUP)));
	// XBox B command
	registerButton(pXboxController, 2, PRESSED, new AutonomousShootSequence());
	// XBox X command
	registerButton(pXboxController, 3, PRESSED, CommandBase::shootCommand.get());
	// XBox Y command
	registerButton(pXboxController, 4, PRESSED, CommandBase::wedgeToggleCommand.get());

	// Left bumper
	registerButton(pXboxController, 5, PRESSED, CommandBase::gearShiftCommand.get());
	// Right bumper
	registerButton(pXboxController, 6, PRESSED, CommandBase::intakeToggleCommand.get());

	// Back command (XBox 360)
	registerButton(pXboxController, 7, PRESSED, CommandBase::toggleLedCommand.get());
	// Start command (XBox 360)
	registerButton(pXboxController, 8, PRESSED, CommandBase::toggleCompressorCommand.get());

	registerSecondControllerButtons();

	SmartDashboard::PutData(new AutonomousShootSequence());

}

void OI::registerSecondControllerButtons() {
	// avoid excessive errors if this joystick isn't connected
	// Commented out -- the second controller is now essential. We don't want to just silently fail if it's not there.
	m_secondControllerButtonCount = pFarmController->GetButtonCount();
	printf("Controller 2 has %d buttons.\n", m_secondControllerButtonCount);

	if (true) {
		// we register the buttons even if the controller wasn't present on startup
		// because it might be connected later

		// section 1 - flaps
		registerButton(pFarmController, 1, PRESSED,	CommandBase::autoSetFlapsCommand.get());
		// 2-6 handled by FlapCommand

		// section 2 - intake
		registerButton(pFarmController, 4, PRESSED,
				new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::CROSSING_POSITION));
		registerButton(pFarmController, 5, PRESSED,
				new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::SHOOTING_POSITION));
		registerButton(pFarmController, 9, PRESSED,
				new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::LOW_GOAL));
		registerButton(pFarmController, 10, PRESSED,
				new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::BACK_TO_SHOOTING_POSITION));

		// section 3 - auto commands
		registerButton(pFarmController, 11, PRESSED, new PrepareForCrossingSequence());
		registerButton(pFarmController, 13, PRESSED, new CenterOnTargetCommand());

		// test commands
		registerButton(pFarmController, 15, PRESSED, new DriveDistanceCommand(9 * 12));


		registerButton(pFarmController, 12, PRESSED, new AutonomousDriveSequence(
				FieldInfo::POS_THREE,
				FieldInfo::DEF_MOAT,
				FieldInfo::TARGET_CENTER));
		registerButton(pFarmController, 14, PRESSED, new AutonomousShootSequence());

		// if the farm controller isn't available, we might be using a Logitech Thurstmaster or something
		// that likely gives out around 16 buttons
		if (m_secondControllerButtonCount > 16) {
			registerButton(pFarmController, 16, PRESSED, new RotateIMUCommand(0));
			// section 4 - intake & wedge up/down
			registerButton(pFarmController, 17, PRESSED, new IntakeToggleCommand(true));
			registerButton(pFarmController, 18, PRESSED, new IntakeToggleCommand(false));
			registerButton(pFarmController, 19, PRESSED, new WedgeToggleCommand(true));
			registerButton(pFarmController, 20, PRESSED, new WedgeToggleCommand(false));

			// the big button
	//		registerButton(pFarmController, 21, PRESSED, new AutonomousShootSequence());

			// section 5 - cameras
			registerButton(pFarmController, 22, PRESSED, new ToggleCameraFeedCommand(0));
			registerButton(pFarmController, 23, PRESSED, new ToggleCameraFeedCommand(1));
	//		registerButton(pFarmController, 24, PRESSED, new ToggleCameraFeedCommand(2));
		}
	}
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

bool OI::get2ndControllerButton(int num) {
	if (num <= m_secondControllerButtonCount) return pFarmController->GetRawButton(num);
	return false;
}
