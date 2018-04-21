task drivebaseControlGyro(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = -SensorValue(leftEncoder);
	float kP = 0.085;
	float kD = 0.017;
	float kPt = 0.27;
	float kDt = 0.0;
	float kPt2 = 0.105;
	float kDt2 = 0.119;
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
		errorT = desiredTurn - SensorValue(Gyro);
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
		delay(20);
	}
}

string autons[9]={"No Auton", "Three Cones R", "Three Cones L", "10 Pt Zone R", "10 Pt Zone L", "Straight R", "Straight L", "Four Cones R", "Four Cones L"};

void threeConesR(){
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWait(1130);
	mobileGoalIn();
	driveBoth(280);
	moveStage2Wait(60);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(260);
	normalStackCone(2);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(-1380);
	normalStackCone(3);
	getOutOfTheWayMid();
	driveBothWait(0);
	turn45DegreesL();
	driveBothWait(-465);
	turn90DegreesL();
	driveBothWaitUntil(775, 730);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(600);
	driveBothWaitUntil(-815, -205);
	mobileGoal = true;
	driveBothWait(165);
	mobileGoal = false;
}

void threeConesL(){
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWait(1130);
	mobileGoalIn();
	driveBoth(280);
	moveStage2Wait(60);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(260);
	normalStackCone(2);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(-1380);
	normalStackCone(3);
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

void fourConesR(){
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWait(1130);
	mobileGoalIn();
	driveBoth(280);
	moveStage2Wait(60);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(260);
	normalStackCone(2);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(250);
	normalStackCone(3);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 265;
	wait1Msec(100);
	driveBoth(-1640);
	normalStackCone(4);
	getOutOfTheWayMid();
	driveBothWait(0);
	turn45DegreesL();
	driveBothWait(-465);
	turn90DegreesL();
	driveBothWaitUntil(775, 730);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(600);
	driveBothWaitUntil(-815, -205);
	mobileGoal = true;
	driveBothWait(165);
	mobileGoal = false;
}

void fourConesL(){
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWait(1130);
	mobileGoalIn();
	driveBoth(280);
	moveStage2Wait(60);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(260);
	normalStackCone(2);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(250);
	normalStackCone(3);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 265;
	wait1Msec(100);
	driveBoth(-1640);
	normalStackCone(4);
	getOutOfTheWayMid();
	driveBothWait(0);
	turn45DegreesR();
	driveBothWait(-465);
	turn90DegreesR();
	driveBothWaitUntil(775, 730);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(600);
	driveBothWaitUntil(-815, -205);
	mobileGoal = true;
	driveBothWait(165);
	mobileGoal = false;
}

void tenZoneR(){
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWait(1130);
	mobileGoalIn();
	driveBoth(280);
	moveStage2Wait(60);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(260);
	normalStackCone(2);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(250);
	normalStackCone(3);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 265;
	wait1Msec(100);
	driveBoth(-1640);
	normalStackCone(4);
	getOutOfTheWayMid();
	driveBothWait(0);
	turnWait(-2070);
	driveBothWait(380);
	mobileGoal = false;
	wait1Msec(1000);
	driveBothWaitUntil(-570, -410);
	mobileGoal = true;
	driveBothWait(0);
	mobileGoal = false;
}

void tenZoneL(){
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWait(1130);
	mobileGoalIn();
	driveBoth(280);
	moveStage2Wait(60);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(260);
	normalStackCone(2);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(250);
	normalStackCone(3);
	groundSetUpConeWait();
	driveBothWait(0);
	groundPickUpConeWait();
	desiredStage1 = 265;
	wait1Msec(100);
	driveBoth(-1640);
	normalStackCone(4);
	getOutOfTheWayMid();
	driveBothWait(0);
	turnWait(-2070);
	driveBothWait(380);
	mobileGoal = false;
	wait1Msec(1000);
	driveBothWaitUntil(-570, -410);
	mobileGoal = true;
	driveBothWait(0);
	mobileGoal = false;
}

void goStraightR(){
	turnWait(-400);
	stopTask(drivebaseControlGyro);
	drive(127, 127);
	wait1Msec(1700);
	drive(0, 0);
}

void goStraightL(){
	turnWait(400);
	stopTask(drivebaseControlGyro);
	drive(127, 127);
	wait1Msec(1700);
	drive(0, 0);
}

void auton(int autonNumber){
	autonRunning = true;
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
	//startTask(stallCheck);

	if(autonNumber==0){
		stopTask(stage1Control);
		stopTask(stage2Control);
		wait1Msec(1000);
	}
	if(autonNumber==1){
		threeConesR();
	}
	if(autonNumber==2){
		threeConesL();
	}
	if(autonNumber==3){
		tenZoneR();
	}
	if(autonNumber==4){
		tenZoneL();
	}
	if(autonNumber==5){
		goStraightR();
	}
	if(autonNumber==6){
		goStraightL();
	}
	if(autonNumber==7){
		fourConesR();
	}
	if(autonNumber==8){
		fourConesL();
	}
	datalogStop();
	autonRunning = false;
}

void testPID(){
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
}

void testTurn(){
	startTask(drivebaseControlGyro);
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
