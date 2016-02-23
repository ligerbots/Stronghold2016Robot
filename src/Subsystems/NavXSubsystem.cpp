#include <Stronghold2016Robot.h>

NavXSubsystem::NavXSubsystem() :
		Subsystem("NavXSubsystem") {
	try {
		mp_navX.reset(new AHRS(SPI::Port::kMXP));
	} catch (std::exception& ex) {
		printf("NavXSubsystem: Failed to create AHRS object\n");
		printf("\t%s", ex.what());
	}
}

void NavXSubsystem::InitDefaultCommand() {
}

double NavXSubsystem::getYaw() {
	if (mp_navX.get() == NULL)
		return 0;
	return mp_navX->GetYaw();
}

void NavXSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("NavX/IMU_Connected", mp_navX->IsConnected());
	SmartDashboard::PutNumber("NavX/IMU_Yaw", mp_navX->GetYaw());
	SmartDashboard::PutNumber("NavX/IMU_Pitch", mp_navX->GetPitch());
	SmartDashboard::PutNumber("NavX/IMU_Roll", mp_navX->GetRoll());
	SmartDashboard::PutNumber("NavX/IMU_CompassHeading", mp_navX->GetCompassHeading());
	SmartDashboard::PutNumber("NavX/IMU_Update_Count", mp_navX->GetUpdateCount());
	SmartDashboard::PutNumber("NavX/IMU_Byte_Count", mp_navX->GetByteCount());

	/* These functions are compatible w/the WPI Gyro Class */
	SmartDashboard::PutNumber("NavX/IMU_TotalYaw", mp_navX->GetAngle());
	SmartDashboard::PutNumber("NavX/IMU_YawRateDPS", mp_navX->GetRate());

	SmartDashboard::PutNumber("NavX/IMU_Accel_X", mp_navX->GetWorldLinearAccelX());
	SmartDashboard::PutNumber("NavX/IMU_Accel_Y", mp_navX->GetWorldLinearAccelY());
	SmartDashboard::PutBoolean("NavX/IMU_IsMoving", mp_navX->IsMoving());
	SmartDashboard::PutNumber("NavX/IMU_Temp_C", mp_navX->GetTempC());
	SmartDashboard::PutBoolean("NavX/IMU_IsCalibrating", mp_navX->IsCalibrating());

	SmartDashboard::PutNumber("NavX/Velocity_X", mp_navX->GetVelocityX());
	SmartDashboard::PutNumber("NavX/Velocity_Y", mp_navX->GetVelocityY());
	SmartDashboard::PutNumber("NavX/Displacement_X", mp_navX->GetDisplacementX());
	SmartDashboard::PutNumber("NavX/Displacement_Y", mp_navX->GetDisplacementY());

	/* Display Raw Gyro/Accelerometer/Magnetometer Values                       */
	/* NOTE:  These values are not normally necessary, but are made available   */
	/* for advanced users.  Before using this data, please consider whether     */
	/* the processed data (see above) will suit your needs.                     */

	SmartDashboard::PutNumber("NavX/RawGyro_X", mp_navX->GetRawGyroX());
	SmartDashboard::PutNumber("NavX/RawGyro_Y", mp_navX->GetRawGyroY());
	SmartDashboard::PutNumber("NavX/RawGyro_Z", mp_navX->GetRawGyroZ());
	SmartDashboard::PutNumber("NavX/RawAccel_X", mp_navX->GetRawAccelX());
	SmartDashboard::PutNumber("NavX/RawAccel_Y", mp_navX->GetRawAccelY());
	SmartDashboard::PutNumber("NavX/RawAccel_Z", mp_navX->GetRawAccelZ());
	SmartDashboard::PutNumber("NavX/RawMag_X", mp_navX->GetRawMagX());
	SmartDashboard::PutNumber("NavX/RawMag_Y", mp_navX->GetRawMagY());
	SmartDashboard::PutNumber("NavX/RawMag_Z", mp_navX->GetRawMagZ());
	SmartDashboard::PutNumber("NavX/IMU_Temp_C", mp_navX->GetTempC());
	/* Omnimount Yaw Axis Information                                           */
	AHRS::BoardYawAxis yaw_axis = mp_navX->GetBoardYawAxis();
	SmartDashboard::PutString("NavX/YawAxisDirection", yaw_axis.up ? "Up" : "Down");
	SmartDashboard::PutNumber("NavX/YawAxis", yaw_axis.board_axis);

	/* Sensor Board Information                                                 */
	SmartDashboard::PutString("NavX/FirmwareVersion", mp_navX->GetFirmwareVersion());

	/* Quaternion Data                                                          */
	/* Quaternions are fascinating, and are the most compact representation of  */
	/* orientation data.  All of the Yaw, Pitch and Roll Values can be derived  */
	/* from the Quaternions.  If interested in motion processing, knowledge of  */
	/* Quaternions is highly recommended.                                       */
	SmartDashboard::PutNumber("NavX/QuaternionW", mp_navX->GetQuaternionW());
	SmartDashboard::PutNumber("NavX/QuaternionX", mp_navX->GetQuaternionX());
	SmartDashboard::PutNumber("NavX/QuaternionY", mp_navX->GetQuaternionY());
	SmartDashboard::PutNumber("NavX/QuaternionZ", mp_navX->GetQuaternionZ());
}

void NavXSubsystem::SetPIDSourceType(PIDSourceType pidSource) {
	// do nothing
}

PIDSourceType NavXSubsystem::GetPIDSourceType() const {
	return PIDSourceType::kDisplacement;
}

double NavXSubsystem::PIDGet() {
	return getYaw();
}
