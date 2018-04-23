#pragma systemFile

#include "Math.h"

bool slewResetDrive = false;

//drive
void drive(int rVal, int lVal);

int slew(int* slewVal, int val, int waitTime = 15, int change = 2){
	*slewVal = *slewVal + change;
	wait1Msec(waitTime);
	return limit(val, -*slewVal, *slewVal);
}

void driveDeadband(int rVal, int lVal, int db = 10){
	if(deadband(rVal, 0, db))
		motor[rightDrive1] = motor[rightDrive2] = motor[rightDrive3] = rVal;
	else
		motor[rightDrive1] = motor[rightDrive2] = motor[rightDrive3] = 0;
	if(deadband(lVal, 0, db))
		motor[leftDrive1] = motor[leftDrive2] = motor[leftDrive3] = lVal;
	else
		motor[leftDrive1] = motor[leftDrive2] = motor[leftDrive3] = 0;
}

task driveControl(){
	while(true){
		driveDeadband(vexRT[Ch2], vexRT[Ch3], 10);
		delay(15);
	}
}

//mobile goal
void intake(int val);

/**void mobileGoalGeneralLess(int originalPower, int finalPower, int val){
	while(SensorValue(intakeEncoder) < val){
		intake(originalPower);
		delay(5);
	}
	intake(finalPower);
}

void mobileGoalGeneralGreater(int originalPower, int finalPower, int val){
	while(SensorValue(intakeEncoder) > val){
		intake(originalPower);
		delay(5);
	}
	intake(finalPower);
}*/

//drive
bool straight = true;
float desiredDrive = 0;
float desiredTurn = 0;
//float avgEncoder = 0;

/**void resetDesired(){
	desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
}

void resetEncoders(){
	desiredDrive = 0;
	SensorValue(rightEncoder) = 0;
	SensorValue(leftEncoder) = 0;
}

void driveBoth(int value){
	slewResetDrive = true;
	desiredDrive = desiredDrive + value;
}

void driveBothWaitUntil(int value, int continueValue, int normalContinue = 180, int derivativeContinue = 100, int tickNum = 10){
	slewResetDrive = true;
	desiredDrive = desiredDrive + value;
	int ticks = 0;
	float oldAvgEncoder = avgEncoder;
	while(ticks<tickNum){
		if(deadband2(avgEncoder, desiredDrive - value + continueValue, normalContinue) && deadband2(avgEncoder-oldAvgEncoder, 0, derivativeContinue))
			ticks++;
		else
			ticks = 0;
		oldAvgEncoder = avgEncoder;
		wait1Msec(20);
	}
}*/

void resetEncoders(){
	desiredDrive = 0;
	SensorValue(leftEncoder) = 0;
}

void driveBoth(int value){
	straight = true;
	slewResetDrive = true;
	desiredDrive = desiredDrive + value;
}

void driveBothWaitUntil(int value, int continueValue, int normalContinue = 50){
	straight = true;
	slewResetDrive = true;
	desiredDrive = desiredDrive + value;
	while(!deadband2(SensorValue(leftEncoder), desiredDrive - value + continueValue, normalContinue)){
		wait1Msec(20);
	}
}

void driveBothWait(int value, int normalContinue = 65, int derivativeContinue = 8, int tickNum = 5){
	straight = true;
	slewResetDrive = true;
	desiredDrive = desiredDrive + value;
	int continueValue = value;
	int ticks = 0;
	int timeOut = 0;
	float oldEncoder = SensorValue(leftEncoder);
	while(ticks<tickNum && timeOut<20){
		if(deadband2(SensorValue(leftEncoder), desiredDrive - value + continueValue, normalContinue) && deadband2(SensorValue(leftEncoder)-oldEncoder, 0, derivativeContinue))
			ticks++;
		else
			ticks = 0;
		if(deadband2(SensorValue(leftEncoder)-oldEncoder, 0, 1))
			timeOut++;
		else
			timeOut = 0;
		oldEncoder = SensorValue(leftEncoder);
		wait1Msec(10);
	}
}

void turn(int value, int normalContinue = 140, int derivativeContinue = 80){
	straight = false;
	slewResetDrive = true;
	desiredTurn += value;
	if(desiredTurn >= 3600)
		desiredTurn = 3600 - desiredTurn;
	desiredDrive = SensorValue(leftEncoder);
}

void turnWait(int value, int normalContinue = 130, int derivativeContinue = 9, int tickNum = 3){
	straight = false;
	slewResetDrive = true;
	desiredTurn += value;
	if(desiredTurn >= 3600)
		desiredTurn = 3600 - desiredTurn;
	int ticks = 0;
	int timeticks = 0;
	float oldGyro = SensorValue(Gyro);
	while(ticks<tickNum || timeticks<2){
		if(deadband2(SensorValue(Gyro), desiredTurn, normalContinue) && deadband2(SensorValue(Gyro) - oldGyro, 0, derivativeContinue))
			ticks++;
		else
			ticks = 0;
		if(deadband2(SensorValue(Gyro) - oldGyro, 0, 4))
			timeticks++;
		else
			timeticks = 0;
		oldGyro = SensorValue(Gyro);
		wait1Msec(20);
	}
	desiredDrive = SensorValue(leftEncoder);
}

void turn180DegreesL(){
	straight = false;
	turnWait(1800);
	straight = true;
}

void turn90DegreesL(){
	straight = false;
	turnWait(900);
	straight = true;
}

void turn90DegreesR(){
	straight = false;
	turnWait(-900);
	straight = true;
}

void turn45DegreesL(){
	straight = false;
	turnWait(450);
	straight = true;
}

void turn45DegreesR(){
	straight = false;
	turnWait(-450);
	straight = true;
}

/**void calibrateGyros(){
	SensorType[Gyro] = sensorNone;
	wait1Msec(250);
	SensorType[Gyro] = sensorGyro;
	wait1Msec(2000);
	SensorScale[Gyro] = 139.8888;
}*/

void calibrateGyros(){
	SensorType[Gyro] = sensorNone;
	wait1Msec(250);
	SensorType[Gyro] = sensorGyro;
	wait1Msec(2000);
	SensorScale[Gyro] = 141.2847;
}

//arm movement
void moveSingleStageWaitUntil(tSensors sensor, float continueValue, int normalContinue){
	if(continueValue > SensorValue(sensor)){
		while(SensorValue(sensor) < continueValue - normalContinue){
			wait1Msec(20);
		}
	}
	else if(continueValue > SensorValue(sensor)){
		while(SensorValue(sensor) > continueValue + normalContinue){
			wait1Msec(20);
		}
	}
}

void moveSingleStageWait(tSensors sensor, float continueValue, int normalContinue, int derivativeContinue, int tickNum = 20){
	int ticks = 0;
	int timeOut = 0;
	float oldEncoder = SensorValue(sensor);
	while(ticks<tickNum && timeOut<200){
		if(deadband2(SensorValue(sensor), continueValue, normalContinue) && deadband2(SensorValue(sensor)-oldEncoder, 0, derivativeContinue))
			ticks++;
		else
			ticks = 0;
		if(deadband2(SensorValue(sensor)-oldEncoder, 0, 30))
			timeOut++;
		oldEncoder = SensorValue(sensor);
		wait1Msec(5);
	}
}

void moveDoubleStageWait(tSensors sensor1, float desiredValue1, int normalContinue1, int derivativeContinue1, tSensors sensor2, float desiredValue2, int normalContinue2, int derivativeContinue2, int tickNum = 10){
	int ticks = 0;
	int timeOut = 0;
	float oldEncoder1 = SensorValue(sensor1);
	float oldEncoder2 = SensorValue(sensor2);
	while(ticks<tickNum  && timeOut<200){
		if(deadband2(SensorValue(sensor1), desiredValue1, normalContinue1) && deadband2(SensorValue(sensor1)-oldEncoder1, 0, derivativeContinue1) && deadband2(SensorValue(sensor2), desiredValue2, normalContinue2) && deadband2(SensorValue(sensor2)-oldEncoder2, 0, derivativeContinue2))
			ticks++;
		else
			ticks = 0;
		if(deadband2(SensorValue(sensor1)-oldEncoder1, 0, 30) || deadband2(SensorValue(sensor2)-oldEncoder2, 0, 30))
			timeOut++;
		oldEncoder1 = SensorValue(sensor1);
		oldEncoder2 = SensorValue(sensor2);
		wait1Msec(5);
	}
}
