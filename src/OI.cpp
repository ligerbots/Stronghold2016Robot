#include <Stronghold2016Robot.h>

OI::OI() :
		m_controllerButtons(), m_buttonsPressed(), m_secondControllerButtonCount(0) {
	mp_XboxController = new Joystick(0);
	mp_FarmController = new Joystick(1);
}

void OI::registerCommands() {
	// XBox A command
	std::vector<Command*> rollBallRestartCommands;
	rollBallRestartCommands.push_back(CommandBase::intakeRollerCommand.get());
	rollBallRestartCommands.push_back(CommandBase::flapCommand.get());
	registerButton(mp_XboxController, 1, PRESSED, new ToggleCommand(new RollBallPickupThenShooter(), rollBallRestartCommands));
	// XBox B command
	registerButton(mp_XboxController, 2, PRESSED, CommandBase::wedgeToggleCommand.get()/*new DriveDistanceCommand(12.0, FieldInfo::SLOW)*/ /*new AutonomousShootSequence()*/);
	// XBox X command
	registerButton(mp_XboxController, 3, PRESSED, CommandBase::shootCommand.get());
	// XBox Y command
	registerButton(mp_XboxController, 4, PRESSED, /*CommandBase::wedgeToggleCommand.get()*/
			CommandBase::diffShootCommand.get());

	// Left bumper
	registerButton(mp_XboxController, 5, PRESSED, CommandBase::gearShiftCommand.get());
	// Right bumper
	registerButton(mp_XboxController, 6, PRESSED, CommandBase::intakeToggleCommand.get());

	// Back command (XBox 360)
	registerButton(mp_XboxController, 7, PRESSED, CommandBase::toggleLedCommand.get());
	// Start command (XBox 360)
	registerButton(mp_XboxController, 8, PRESSED, CommandBase::toggleCompressorCommand.get());

	registerSecondControllerButtons();

	SmartDashboard::PutData(new AutonomousShootSequence());

}

void OI::registerSecondControllerButtons() {
	// avoid excessive errors if this joystick isn't connected
	// Commented out -- the second controller is now essential. We don't want to just silently fail if it's not there.
//	m_secondControllerButtonCount = mp_FarmController->GetButtonCount();
//	printf("Controller 2 has %d buttons.\n", m_secondControllerButtonCount);
	m_secondControllerButtonCount = 28; // having issues with buttons not being bound

	// we register the buttons even if the controller wasn't present on startup
	// because it might be connected later

	// section 1 - flaps
	registerButton(mp_FarmController, 1, PRESSED,
			CommandBase::autoSetFlapsCommand.get());
	// 2-6 handled by FlapCommand

	// section 2 - intake
	registerButton(mp_FarmController, 4, PRESSED,
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::CROSSING_POSITION));
	registerButton(mp_FarmController, 5, PRESSED,
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::SHOOTING_POSITION));
	registerButton(mp_FarmController, 9, PRESSED,
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::LOW_GOAL));
	registerButton(mp_FarmController, 10, PRESSED,
			new RollBallToIntakePositionCommand(
					RollBallToIntakePositionCommand::BACK_TO_SHOOTING_POSITION));

	// section 3 - auto-related and test commands
	registerButton(mp_FarmController, 11, PRESSED,
			new PrepareForCrossingSequence());
	registerButton(mp_FarmController, 12, PRESSED,
			CommandBase::diffShootCommand.get());
	registerButton(mp_FarmController, 13, PRESSED,
			new AcquireTarget(true, true));
	registerButton(mp_FarmController, 14, PRESSED,
			new AutonomousShootSequence());
	registerButton(mp_FarmController, 15, PRESSED, new CenterOnTargetCommand());
	registerButton(mp_FarmController, 16, PRESSED,
			new RotateIMUCommand(90, false));

	// if the farm controller isn't available, we might be using a Logitech Thurstmaster or something
	// that likely gives out around 16 buttons
	if (m_secondControllerButtonCount > 16) {
		// section 4 - intake & wedge up/down
		registerButton(mp_FarmController, 17, PRESSED,
				new IntakeToggleCommand(true));
		registerButton(mp_FarmController, 18, PRESSED,
				new IntakeToggleCommand(false));
		registerButton(mp_FarmController, 19, PRESSED,
				new WedgeToggleCommand(true));
		registerButton(mp_FarmController, 20, PRESSED,
				new WedgeToggleCommand(false));

		// section 5 - cameras

		// the big button (21)
		registerButton(mp_FarmController, 21, PRESSED,
				new AutoSetFlapsCommand(true));

		registerButton(mp_FarmController, 22, PRESSED,
				new ToggleCameraFeedCommand(0));
		registerButton(mp_FarmController, 23, PRESSED,
				new ToggleCameraFeedCommand(1));
		// 24 turns vision off - handled somewhere else
	}
}

bool OI::joystickButtonPressed(Joystick* pJoystick, int buttonNumber) {
	bool pressed = false;

	// find the vector to use
	JoystickPressed_t::iterator buttonsPressedIterator = m_buttonsPressed.find(
			(uintptr_t) pJoystick);
	if (buttonsPressedIterator == m_buttonsPressed.end()) {
		std::vector<bool> apButtonsPressed;
		for (int i = 0; i < 30; i++) {
			apButtonsPressed.push_back(false);
		}
		m_buttonsPressed.insert(
				std::pair<uintptr_t, std::vector<bool>>((uintptr_t) pJoystick,
						apButtonsPressed));
	}

	buttonsPressedIterator = m_buttonsPressed.find((uintptr_t) pJoystick);

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
	JoystickMap_t::iterator cIt = m_controllerButtons.find((uintptr_t) pJoystick);
	if (cIt == m_controllerButtons.end()) {
		std::vector<JoystickButton*> apButtons(31);
		m_controllerButtons.insert(
				std::pair<uintptr_t, std::vector<JoystickButton*>>(
						(uintptr_t) pJoystick, apButtons));
	}

	cIt = m_controllerButtons.find((uintptr_t) pJoystick);

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

bool OI::getSecondControllerRawButton(int num) {
	if (num <= m_secondControllerButtonCount){
		return mp_FarmController->GetRawButton(num);
	}
	return false;
}
