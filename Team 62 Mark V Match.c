task drivebaseControlGyro(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = -SensorValue(leftEncoder);
	float kP = 0.105;
	float kD = 0.081;
	float kPt = 0.33;
	float kDt = 0.0;
	float kPt2 = 0.13;
	float kDt2 = 0.58762;
	float error = 0;
	float errorT = 0;
	float derivative = 0;
	float derivativeT = 0;
	float driveSpeed = 0;
	float turnSpeed = 0;
	float old = 0;
	float oldT = 0;
	int slewVal = 15;
	while(true){
		errorT = desiredTurn - gyroLeftIsPositive()*SensorValue(Gyro);
		derivativeT = errorT - oldT;
		oldT = errorT;
		if(slewResetDrive){
			slewVal = 15;
			slewResetDrive = false;
		}
		if(!straight){
			drive(slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT), 6), -slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT), 6));
		}
		else{
			//avgEncoder = (SensorValue(rightEncoder) + SensorValue(leftEncoder)) / 2.0;
			//error = desiredDrive - avgEncoder;
			error = desiredDrive + SensorValue(leftEncoder);
			derivative = error - old;
			old = error;
			driveSpeed = slew(&slewVal, limit(kP*(error) + kD*derivative), 10);
			turnSpeed = limit(kPt*errorT + kDt*derivativeT, -1.75*abs(driveSpeed), 1.75*abs(driveSpeed));
			drive(driveSpeed + turnSpeed, driveSpeed - turnSpeed);
		}
		delay(5);
	}
}

int stepWait = 120;

void resetGyro10PtBar(){
	float oldDesiredDrive = desiredDrive;
	stopTask(drivebaseControlGyro);
	drive(35, 35);
	float oldVal = SensorValue(leftEncoder);
	while(!deadband2(oldVal, SensorValue(leftEncoder), 10)){
		wait1Msec(60);
		oldVal = SensorValue(leftEncoder);
	}
	wait1Msec(600);
	SensorValue(Gyro) = 0;
	desiredTurn = 0;
	drive(0, 0);
	startTask(drivebaseControlGyro);
	desiredDrive = -SensorValue(leftEncoder);
}

void goal1(){
	mobileGoal = false;
	wait1Msec(stepWait);
	driveBothWait(1500);
	mobileGoalIn();
	moveStage2WaitUntil(90, 60);
	wait1Msec(300);
	getOutOfTheWayLow();
	driveBothWait(-1300);
	turn45DegreesR();
	wait1Msec(stepWait);
	driveBothWait(-640);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWaitUntil(850, 250);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(stepWait+100);
	mobileGoal = true;
	driveBothWaitUntil(-1000, -500);
	mobileGoal = false;
}

void auton(){
	resetEncoders();
	resetLiftEncoders();
	datalogClear();
	datalogStart();
	startTask(dataLog);
	startTask(drivebaseControlGyro);
	startTask(mobileGoalAuton);
	autonRan = true;
	startTask(stage1Control);
	startTask(stage2Control);
	unfoldRobotAuton();

	goal1();

	datalogStop();
}

void testPID(){
	resetEncoders();
	startTask(drivebaseControlGyro);
	driveBothWait(1200);
	wait1Msec(1100);
	turnWait(1800);
	wait1Msec(1100);
	driveBothWait(1200);
	wait1Msec(1100);
	turnWait(1800);
	wait1Msec(1100);
	driveBothWait(1200);
	//driveBothWait(100000000);
}

void testTurn(){
	startTask(drivebaseControlGyro);
	resetEncoders();
	turn90DegreesR();
	wait1Msec(1000);
	turn90DegreesL();
	wait1Msec(1000);
	turn45DegreesR();
	wait1Msec(1000);
	turn45DegreesL();
	wait1Msec(1000);
	turn180DegreesL();
}
