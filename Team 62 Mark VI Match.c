int maxSpeed = 127;

task drivebaseControlGyro(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = SensorValue(leftEncoder);
	float kP = 0.12;
	float kD = 0.021;
	float kPt = 0.34;
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
			drive(slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT, -maxSpeed, maxSpeed), 6, 1), -slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT, -maxSpeed, maxSpeed), 6, 1));
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

string autons[4][10]={{"20-Pt ","One C","Two C","Three C","Four C","","","","",""},{"10-Pt ","One C","Two C","Three C","Four C","","","","",""},{"D-f ","Straight","Back Push","","","","","","",""},{"Other ","C-L 20","C-L 10","","","","","","",""}};
void twentyPointZone(bool right){
	if(right){
		turn45DegreesR();
	}
	else{
		turn45DegreesL();
	}
	driveBothWait(-680);
	if(right){
		turn90DegreesR();
	}
	else{
		turn90DegreesL();
	}
	//driveBothWaitUntil(940, 800);
	stopTask(drivebaseControlGyro);
	drive(70, 70);
	wait1Msec(500);
	mobileGoal = false;
	wait1Msec(150);
	drive(0,0);
	wait1Msec(600);
	//driveBothWaitUntil(-1005, -285);
	drive(-70,-70);
	wait1Msec(200);
	mobileGoal = true;
	wait1Msec(300);
	drive(0,0);
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

void oneCones(){
	driveBoth(1680);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, 0);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(730);
	moveStage1WaitUntil(165, 180);
	moveStage2WaitUntil(60, -10);
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(-1970);
	normalStackCone(2);
	getOutOfTheWayMid();
	driveBothWait(0);
}

void twoCones(){
	driveBoth(1680);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, 0);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(730);
	moveStage1WaitUntil(165, 180);
	moveStage2WaitUntil(60, -10);
	driveBothWait(0);
	groundPickUpCone();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(-1970);
	normalStackCone(2);
	getOutOfTheWayMid();
	driveBothWait(0);
}

void threeCones(){
	driveBoth(1680);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, -60);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(730);
	moveStage1WaitUntil(165, 180);
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
	currentConeStack = 4;
}

void fourConesVersioned(){
	driveBoth(1650);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, -60);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(730);
	moveStage1WaitUntil(165, 180);
	moveStage2WaitUntil(60, -10);
	driveBothWait(0);
	groundSetUpConeWait();
	wait1Msec(50);
	groundPickUpCone();
	wait1Msec(100);
	desiredStage1 = 470;
	wait1Msec(220);
	driveBoth(410);
	normalStackCone(2);
	groundSetUpConeWait();
	wait1Msec(200);
	groundPickUpCone();
	wait1Msec(100);
	desiredStage1 = 690;
	wait1Msec(220);
	driveBoth(305);
	normalStackCone(3);
	groundSetUpConeWait();
	wait1Msec(200);
	groundPickUpCone();
	wait1Msec(100);
	desiredStage1 = 265;
	wait1Msec(70);
	driveBoth(-2685);
	normalStackCone(4);
	groundSetUpConeWait();
	wait1Msec(170);
	getOutOfTheWayMid();
	driveBothWait(0);
	currentConeStack = 4;
}

void fourCones(){
	driveBoth(1650);
	unfoldRobotAuton();
	mobileGoal = false;
	driveBothWaitUntil(0, -60);
	mobileGoal = true;
	driveBoth(460);
	wait1Msec(730);
	moveStage1WaitUntil(165, 180);
	moveStage2WaitUntil(60, -10);
	groundSetUpConeWait();
	wait1Msec(50);
	driveBothWait(0);
	stopTask(drivebaseControlGyro);
	drive(30,30);
	while(!SensorValue(coneButtonSensor)){
		wait1Msec(20);
	}
	drive(0,0);
	startTask(drivebaseControlGyro);
	groundPickUpCone();
	wait1Msec(100);
	desiredStage1 = 470;
	wait1Msec(220);
	driveBoth(410);
	normalStackCone(2);
	groundSetUpConeWait();
	driveBothWait(0);
	stopTask(drivebaseControlGyro);
	drive(30,30);
	while(!SensorValue(coneButtonSensor)){
		wait1Msec(20);
	}
	drive(0,0);
	startTask(drivebaseControlGyro);
	groundPickUpCone();
	wait1Msec(100);
	desiredStage1 = 690;
	wait1Msec(220);
	driveBoth(150);
	normalStackCone(3);
	groundSetUpConeWait();
	driveBothWait(0);
	stopTask(drivebaseControlGyro);
	drive(30,30);
	while(!SensorValue(coneButtonSensor)){
		wait1Msec(20);
	}
	drive(0,0);
	wait1Msec(30);
	drive(-20,-20);
	wait1Msec(70);
	startTask(drivebaseControlGyro);
	groundPickUpCone();
	wait1Msec(100);
	desiredStage1 = 265;
	wait1Msec(70);
	driveBoth(-2685);
	normalStackCone(4);
	groundSetUpConeWait();
	wait1Msec(170);
	getOutOfTheWayMid();
	driveBothWait(0);
	currentConeStack = 4;
}

void defence(bool right){
	maxSpeed = 80;
	driveBoth(-3410); //-3300
	unfoldRobotAuton();
	mobileGoal = false;
	//driveBothWaitUntil(0, 1650);
	//desiredTurn = -40;
	//driveBothWaitUntil(0, 2550);
	//desiredTurn = 50;
	driveBothWait(0);
	if(right){
		turnWait(-350); //350
	}
	else{
		turnWait(350);
	}
	driveBothWaitUntil(900, 750); //900
	mobileGoalIn();
	if(right){
		turnWait(750);
	}
	else{
		turnWait(-750);
	}
	driveBoth(900);
	moveStage1WaitUntil(165, 180);
	moveStage2WaitUntil(60, -10);
	driveBothWait(0);
	if(right){
		turnWait(-500); //470
	}
	else{
		turnWait(500);
	}
	driveBothWaitUntil(1760, 1260); //1700
	getOutOfTheWayMid();
	mobileGoalOut();
	driveBothWait(0);
	driveBothWaitUntil(-990, -150); //950
	mobileGoalIn();
	if(right){
		desiredTurn+=-100;
	}
	else{
		desiredTurn+=100;
	}
	driveBothWait(0);
	if(right){
		turnWait(900);
	}
	else{
		turnWait(-900);
	}
	driveBothWait(-300);
}

void goStraight(bool right){
	/**if(right){
		turnWait(-400);
	}
	else{
		turnWait(400);
	}*/
	stopTask(drivebaseControlGyro);
	drive(127, 127);
	wait1Msec(1700);
	drive(0, 0);
}

void coneLaunchTwo(bool right){
	desiredStage1=220;
	desiredStage2=-25;
	mobileGoal = false;
	wait1Msec(700);
	SensorValue(Gyro) = 0;
	if(right){
		desiredTurn = -205;
	}
	else{
		desiredTurn = 265;
	}
	driveBoth(1790);
	getOutOfTheWayLow();
	driveBothWaitUntil(0, -60);
	mobileGoal = true;
	driveBothWait(535);
	moveBothStagesWait(215, -25, 25);
	wait1Msec(200);
	groundPickUpConeWaitAuto();
	desiredStage1 = 470;
	wait1Msec(100);
	driveBoth(415);
	normalStackCone(1);
	moveBothStagesWait(215, -25, 25);
	wait1Msec(200);
	groundPickUpConeWaitAuto();
	desiredStage1 = 690;
	wait1Msec(100);
	driveBoth(-2390);
	moveStage1WaitUntil(505, 375);
	moveStage2WaitUntil(-200, -50);
	moveBothStagesWait(200, -450, 40);
	groundSetUpConeWait();
	getOutOfTheWayMid();
	driveBothWait(0);
	currentConeStack = 4;
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
	SensorValue(Gyro)=0;
	//startTask(stallCheck);

	if(autonNumber==0){
		stopTask(stage1Control);
		stopTask(stage2Control);
		wait1Msec(1000);
	}
	else if(autonCatagory==0){
		if(autonNumber==1)
			oneCones();
		else if(autonNumber==2)
			twoCones();
		else if(autonNumber==3)
			threeCones();
		else if(autonNumber==4)
			fourConesVersioned();
		twentyPointZone(autonRight);
	}
	else if(autonCatagory==1){
		if(autonNumber==1)
			oneCones();
		else if(autonNumber==2)
			twoCones();
		else if(autonNumber==3)
			threeCones();
		else if(autonNumber==4)
			fourConesVersioned();
		tenPointZone(autonRight);
	}
	else if(autonCatagory==2){
		if(autonNumber==1){
			goStraight(autonRight);
		}
		else if(autonNumber==2){
			defence(autonRight);
		}
	}
	else if(autonCatagory==3){
		if(autonNumber==1){
			coneLaunchTwo(autonRight);
			twentyPointZone(autonRight);
		}
		else if(autonNumber==2){
			coneLaunchTwo(autonRight);
			tenPointZone(autonRight);
		}
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
