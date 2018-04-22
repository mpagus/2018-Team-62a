int maxSpeed = 127;

task drivebaseControlGyro(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = SensorValue(leftEncoder);
	float kP = 0.12;
	float kD = 0.021;
	float kPt = 0.23;
	float kDt = 0.0;
	float kPt2 = 0.24;
	float kDt2 = 0.38;
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
			error = desiredDrive - SensorValue(leftEncoder);
			derivative = error - old;
			old = error;
			driveSpeed = slew(&slewVal, limit(kP*(error) + kD*derivative, -maxSpeed, maxSpeed), 10, 4);
			turnSpeed = limit(kPt*errorT + kDt*derivativeT, -1.75*abs(driveSpeed), 1.75*abs(driveSpeed));
			drive(driveSpeed + turnSpeed, driveSpeed - turnSpeed);
		}
		delay(20);
	}
}

string autons[9]={"No Auton", "Three Cones R", "Three Cones L", "10 Pt Zone R", "10 Pt Zone L", "Straight R", "Straight L", "Four Cones R", "Four Cones L"};

void twentyPointZone(bool right){
	if(right){
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
	else{
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
}

void twentyPointZoneL(){
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

void twoCones(bool right){
	driveBoth(1680);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, -60);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(670);
	moveStage1Wait(220);
	moveStage2WaitUntil(60, -10);
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(-1970);
	normalStackCone(2);
	getOutOfTheWayMid();
	driveBothWait(0);
	twentyPointZoneR(right);
}

void threeCones(bool right){
	driveBoth(1680);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, -60);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(670);
	moveStage1Wait(220);
	moveStage2WaitUntil(60, -10);
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(390);
	normalStackCone(2);
	groundSetUpCone();
	wait1Msec(200);
	groundPickUpCone();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(-2360);
	normalStackCone(3);
	getOutOfTheWayMid();
	driveBothWait(0);
	twentyPointZone(right);
}

void fourCones(bool right){
	driveBoth(1680);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, -60);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(670);
	moveStage1Wait(220);
	moveStage2WaitUntil(60, -10);
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(390);
	normalStackCone(2);
	groundSetUpCone();
	wait1Msec(200);
	groundPickUpCone();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(310);
	normalStackCone(3);
	groundSetUpCone();
	wait1Msec(200);
	groundPickUpCone();
	desiredStage1 = 265;
	wait1Msec(100);
	driveBoth(-2670);
	normalStackCone(4);
	groundSetUpCone();
	wait1Msec(170);
	getOutOfTheWayMid();
	driveBothWait(0);
	twentyPointZone(right);
}

void tenZone(bool right){
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
		threeCones(true);
	}
	if(autonNumber==2){
		threeCones(false);
	}
	if(autonNumber==3){
		tenZoneR();
	}
	if(autonNumber==4){
		tenZoneL();
	}
	if(autonNumber==5){
		goStraight(true);
	}
	if(autonNumber==6){
		goStraight(false);
	}
	if(autonNumber==7){
		fourCones(true);
	}
	if(autonNumber==8){
		fourCones(false);
	}
	datalogStop();
	autonRunning = false;
}

void testPID(){
	resetEncoders();
	startTask(drivebaseControlGyro);
	driveBothWait(1400);
	wait1Msec(1100);
	turnWait(1800);
	wait1Msec(1100);
	driveBothWait(1400);
	wait1Msec(1100);
	turnWait(1800);
	wait1Msec(1100);
	driveBothWait(1400);
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
