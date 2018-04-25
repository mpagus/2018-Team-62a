int maxSpeed = 127;

task drivebaseControlGyro(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = SensorValue(leftEncoder);
	float kP = 0.12;
	float kD = 0.021;
	float kPt = 0.23;
	float kDt = 0.0;
	float kPt2 = 0.225;
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

string autons[4][10]={{"20-Pt ", "One C", "Two C", "Three C", "Four C", "", "", "", "", ""},{"10-Pt ", "One C", "Two C", "Three C", "Four C", "", "", "", "", ""},{"Defence ", "Straight", "Back Push", "", "", "", "", ""},{"Other ", "None", "", "", "", "", "", "", "", "", ""}};

void twentyPointZone(bool right){
	if(right){
		turn45DegreesR();
	}
	else{
		turn45DegreesL();
	}
	driveBothWait(-665);
	if(right){
		turn90DegreesR();
	}
	else{
		turn90DegreesL();
	}
	driveBothWaitUntil(940, 840);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(600);
	driveBothWaitUntil(-1005, -285);
	mobileGoal = true;
	driveBothWait(165);
}

void tenPointZone(bool right){
	if(right){
		turnWait(-2070);
	}
	else{
		turnWait(2070);
	}
	driveBothWait(380);
	mobileGoal = false;
	wait1Msec(1000);
	driveBothWaitUntil(-570, -410);
	mobileGoal = true;
	driveBothWait(0);
	mobileGoal = false;
}

void cones(int num){
	driveBoth(1680);
	unfoldRobotAuton();
	mobileGoal = false;
	if(cones>=1){
		driveBothWaitUntil(0, -60);
		mobileGoal = true;
	}
	if(cones>=2){
		driveBoth(450);
		wait1Msec(670);
	}
	else if(cones==1){
		wait1Msec(600);
		driveBoth(-1555);
	}
	if(cones>=1){
		moveStage1WaitUntil(170, 200);
		moveStage2WaitUntil(60, -10);
		driveBothWait(0);
	}
	if(cones>=2){
		groundPickUpCone();
		desiredStage1 = 470;
		wait1Msec(100);
	}
	if(counes>=3)
		driveBoth(410);
	else if(cones==2){
		driveBoth(-2005);
	}
	if(cones>=2){
		normalStackCone(2);
		groundSetUpCone();
		wait1Msec(200);
	}
	if(cones>=3){
		groundPickUpCone();
		desiredStage1 = 690;
		wait1Msec(100);
	}
	if(cones>=4)
		driveBoth(305);
	else if(cones==3){
		driveBoth(-2415);
	}
	if(cones>=3){
		normalStackCone(3);
		groundSetUpCone();
		wait1Msec(200);
	}
	if(cones>=4){
		groundPickUpCone();
		desiredStage1 = 265;
		wait1Msec(100);
		driveBoth(-2720);
	}
	if(cone>=4){
		normalStackCone(4);
		groundSetUpCone();
		wait1Msec(170);
	}
	getOutOfTheWayMid();
	driveBothWait(0);
	currentConeStack = 4;
}

void defence(bool right){
	driveBoth(-2200);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWait(0);
	if(right){
		turnWait(325);
	}
	else{
		turnWait(-325);
	}
	driveBothWaitUntil(500, 400);
	mobileGoalIn();
	if(right){
		turnWait(-600);
	}
	else{
		turnWait(600);
	}
	driveBothWait(600);
	if(right){
		turnWait(500);
	}
	else{
		turnWait(-500);
	}
	driveBothWait(1500);
	mobileGoalOut();
	driveBothWaitUntil(-500, -150);
	mobileGoalIn();
	if(right){
		turnWait(-1600);
	}
	else{
		turnWait(1600);
	}
	driveBothWait(300);
}

void goStraight(bool right){
	if(right){
		turnWait(-400);
	}
	else{
		turnWait(400);
	}
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
	else if(autonCatagory==0){
		cones(autonNumber);
		twentyPointZone(autonRight);
	}
	else if(autonCatagory==1){
		cones(autonNumber);
		tenPointZone(autonRight);
	}
	else if(autonCatagory==2){
		if(autonChoice==1){
			goStraight(autonRight);
		}
		else if(autonChoice==2){
			defence(autonRight);
		}
	}
	else if(autonCatagory==3){
		stopTask(stage1Control);
		stopTask(stage2Control);
		wait1Msec(1000);
	}
	datalogStop();
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
