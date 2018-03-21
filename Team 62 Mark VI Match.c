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

void threeCones(){
	mobileGoal = false;
	driveBothWait(1390);
	mobileGoalIn();
	driveBoth(345);
	desiredStage2 = 90;
	wait1Msec(100);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 450;
	wait1Msec(100);
	driveBoth(320);
	moveStage1WaitUntil(435, 405);
	moveBothStagesWait(465, -160);
	moveBothStagesWait(215, -215);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 600;
	wait1Msec(100);
	driveBoth(-1700);
	moveStage1WaitUntil(640, 575);
	moveBothStagesWait(640, -85);
	moveBothStagesWait(235, -235);
	groundSetUpConeWait();
	getOutOfTheWayMid();
	driveBothWait(0);
	turn45DegreesR();
	driveBothWait(-570);
	turn90DegreesR();
	driveBothWaitUntil(955, 900);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(600);
	driveBothWaitUntil(-1000, -250);
	mobileGoal = true;
	driveBothWait(200);
	mobileGoal = false;
}

void fourCones(){
	mobileGoal = false;
	driveBothWait(1390);
	mobileGoalIn();
	driveBoth(315);
	desiredStage2 = 90;
	wait1Msec(100);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 450;
	wait1Msec(100);
	driveBoth(320);
	moveStage1WaitUntil(435, 405);
	moveBothStagesWait(465, -160);
	moveBothStagesWait(215, -215);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 600;
	wait1Msec(100);
	driveBoth(305);
	moveStage1WaitUntil(595, 515);
	moveBothStagesWait(595, -85);
	moveBothStagesWait(235, -235);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	driveBoth(-320);
	groundSetUpConeWait();
	groundPickUpCone();
	moveStage1WaitUntil(275, 130);
	driveBoth(-1870);
	moveBothStagesWait(287, 755);
	desiredStage1 = 110;
	wait1Msec(250);
	moveStage1Wait(140);
	groundSetUpCone();
	desiredStage1 = 120;
	wait1Msec(200);
	groundSetUpConeWait();
	getOutOfTheWayMid();
	driveBothWait(0);
	turn45DegreesR();
	driveBothWait(-650);
	turn90DegreesR();
	driveBothWaitUntil(960, 250);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(200);
	mobileGoal = true;
	driveBothWaitUntil(-1000, -500);
	mobileGoal = false;
}

void tenZone(){
	mobileGoal = false;
	driveBothWait(1390);
	mobileGoalIn();
	wait1Msec(150);
	driveBoth(345);
	desiredStage2 = 90;
	wait1Msec(100);
	groundSetUpConeWait();
	wait1Msec(150);
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 450;
	wait1Msec(100);
	driveBoth(335);
	moveStage1WaitUntil(435, 405);
	moveBothStagesWait(465, -160);
	moveBothStagesWait(215, -215);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 600;
	wait1Msec(100);
	driveBoth(-1715);
	moveStage1WaitUntil(650, 575);
	moveBothStagesWait(650, -85);
	moveBothStagesWait(235, -235);
	groundSetUpConeWait();
	getOutOfTheWayMid();
	driveBothWait(0);
	//turn180DegreesL();
	turnWait(2070);
	driveBothWait(470);
	mobileGoal = false;
	wait1Msec(1000);
	driveBothWaitUntil(-700, -500);
	mobileGoal = true;
	driveBothWait(0);
	mobileGoal = false;
}

void goStraight(){
	stopTask(drivebaseControlGyro);
	drive(127, 127);
	wait1Msec(1700);
	drive(0, 0);
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
	startTask(stallCheck);
	unfoldRobotAuton();

	//threeCones();
	tenZone();
	//goStraight();

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