#include "RecordValuesCommand.h"
#include <ctime>

RecordValuesCommand::RecordValuesCommand() {
	dataFile = fopen("/home/lvuser/data.csv", "a");
	time_t rawtime;
	tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
	fprintf(dataFile, "X\tY\tDistance\tRough Angle\tFine Angle\tLeft Flap\tRight Flap\t%s\n", buffer);
	fflush(dataFile);
}

void RecordValuesCommand::Initialize() {
}

void RecordValuesCommand::Execute() {
	double x = SmartDashboard::GetNumber("Target X Pos", 0);
	double y = SmartDashboard::GetNumber("Target Y Pos", 0);
	double dist = SmartDashboard::GetNumber("NewVision Target Distance", 0);
	double roughAngle = SmartDashboard::GetNumber("NewVision Target Angle", 0);
	double fineAngle = SmartDashboard::GetNumber("Small (10 degree) angle", 0);
	double flapLeft = SmartDashboard::GetNumber("FlapCommandPositionLeft", 0);
	double flapRight = SmartDashboard::GetNumber("FlapCommandPositionRight", 0);

	fprintf(dataFile, "%f\t%f\t%f\t%f\t%f\t%f\t%f\n", x, y, dist, roughAngle, fineAngle, flapLeft, flapRight);
	fflush(dataFile); // make sure we write, since robot program always ends by being force killed
}

bool RecordValuesCommand::IsFinished() {
	return true;
}

void RecordValuesCommand::End() {
}

void RecordValuesCommand::Interrupted() {
}
