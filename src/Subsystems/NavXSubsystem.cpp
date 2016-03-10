#include "NavXSubsystem.h"

NavXSubsystem::NavXSubsystem(SPI::Port port) :
	Subsystem("NavXSubsystem"),
	AHRS(port),
	m_yawOffset(0.0)
{
}

void NavXSubsystem::InitDefaultCommand() {
}

void NavXSubsystem::zeroYaw(double yawOffset) {
	m_yawOffset = yawOffset;
	AHRS::ZeroYaw();
}

float NavXSubsystem::GetYaw() {
	// The NavX calibration routine sets the Z axis to be pointing down, so  clockwise turn
	// corresponds to a positive change in the yaw angle.
	// Also, we wrap the AHRS base GetYaw class with our own so we can maintain our own
	// starting position offset.
	return (float) (m_yawOffset + AHRS::GetYaw());
}

bool NavXSubsystem::isRobotAboutToTip(float maxPitchAngle) {
	return (GetPitch()>maxPitchAngle);
}

void NavXSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("NavX/IMU_Connected", IsConnected());
	SmartDashboard::PutNumber("NavX/IMU_Yaw", GetYaw());
	// Since the NavX is mounted on the side of the robot, we have
	// swapped the roll and pitch axes.
	SmartDashboard::PutNumber("NavX/IMU_Pitch", GetRoll());
	SmartDashboard::PutNumber("NavX/IMU_Roll", GetPitch());
	SmartDashboard::PutNumber("NavX/IMU_CompassHeading", GetCompassHeading());
	SmartDashboard::PutNumber("NavX/IMU_Update_Count", GetUpdateCount());
	SmartDashboard::PutNumber("NavX/IMU_Byte_Count", GetByteCount());

	/* These functions are compatible w/the WPI Gyro Class */
	SmartDashboard::PutNumber("NavX/IMU_TotalYaw", GetAngle());
	SmartDashboard::PutNumber("NavX/IMU_YawRateDPS", GetRate());

	SmartDashboard::PutNumber("NavX/IMU_Accel_X", GetWorldLinearAccelX());
	SmartDashboard::PutNumber("NavX/IMU_Accel_Y", GetWorldLinearAccelY());
	SmartDashboard::PutBoolean("NavX/IMU_IsMoving", IsMoving());
	SmartDashboard::PutNumber("NavX/IMU_Temp_C", GetTempC());
	SmartDashboard::PutBoolean("NavX/IMU_IsCalibrating", IsCalibrating());

	SmartDashboard::PutNumber("NavX/Velocity_X", GetVelocityX());
	SmartDashboard::PutNumber("NavX/Velocity_Y", GetVelocityY());
	SmartDashboard::PutNumber("NavX/Displacement_X", GetDisplacementX());
	SmartDashboard::PutNumber("NavX/Displacement_Y", GetDisplacementY());
	SmartDashboard::PutNumber("NavX/Displacement_Z", GetDisplacementZ());

	/* Display Raw Gyro/Accelerometer/Magnetometer Values                       */
	/* NOTE:  These values are not normally necessary, but are made available   */
	/* for advanced users.  Before using this data, please consider whether     */
	/* the processed data (see above) will suit your needs.                     */

	SmartDashboard::PutNumber("NavX/RawGyro_X", GetRawGyroX());
	SmartDashboard::PutNumber("NavX/RawGyro_Y", GetRawGyroY());
	SmartDashboard::PutNumber("NavX/RawGyro_Z", GetRawGyroZ());
	SmartDashboard::PutNumber("NavX/RawAccel_X", GetRawAccelX());
	SmartDashboard::PutNumber("NavX/RawAccel_Y", GetRawAccelY());
	SmartDashboard::PutNumber("NavX/RawAccel_Z", GetRawAccelZ());
	SmartDashboard::PutNumber("NavX/RawMag_X", GetRawMagX());
	SmartDashboard::PutNumber("NavX/RawMag_Y", GetRawMagY());
	SmartDashboard::PutNumber("NavX/RawMag_Z", GetRawMagZ());
	SmartDashboard::PutNumber("NavX/IMU_Temp_C", GetTempC());
	/* Omnimount Yaw Axis Information                                           */
	AHRS::BoardYawAxis yaw_axis = GetBoardYawAxis();
	SmartDashboard::PutString("NavX/YawAxisDirection", yaw_axis.up ? "Up" : "Down");
	SmartDashboard::PutNumber("NavX/YawAxis", yaw_axis.board_axis);

	/* Sensor Board Information                                                 */
	SmartDashboard::PutString("NavX/FirmwareVersion", GetFirmwareVersion());

	/* Quaternion Data                                                          */
	/* Quaternions are fascinating, and are the most compact representation of  */
	/* orientation data.  All of the Yaw, Pitch and Roll Values can be derived  */
	/* from the Quaternions.  If interested in motion processing, knowledge of  */
	/* Quaternions is highly recommended.                                       */
	SmartDashboard::PutNumber("NavX/QuaternionW", GetQuaternionW());
	SmartDashboard::PutNumber("NavX/QuaternionX", GetQuaternionX());
	SmartDashboard::PutNumber("NavX/QuaternionY", GetQuaternionY());
	SmartDashboard::PutNumber("NavX/QuaternionZ", GetQuaternionZ());
}

void NavXSubsystem::SetPIDSourceType(PIDSourceType pidSource) {
	// do nothing
}

PIDSourceType NavXSubsystem::GetPIDSourceType() const {
	return PIDSourceType::kDisplacement;
}

double NavXSubsystem::PIDGet() {
	return GetYaw();
}
