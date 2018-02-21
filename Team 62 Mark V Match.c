#pragma systemFile

task drivebaseControlGyroSkills(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = SensorValue(leftEncoder);
	float kP = 0.115;
	float kD = 0.081;
	float kPt = 0.8201;
	float kDt = 0.0;
	float kPt2 = 0.1114;
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

void twentyPointAuton(){
	mobileGoalOut();
	wait1Msec(450);
	driveBothWait(1500);
	mobileGoalIn();
	driveBothWait(200);
	groundSetUpConeWait();
	groundPickUpConeWait();
	wait1Msec(450);
	normalStackCone(1);
	driveBothWait(200);
	groundSetUpConeWait();
	groundPickUpConeWait();
	wait1Msec(450);
	normalStackCone(2);
	driveBothWait(200);
	groundSetUpConeWait();
	groundPickUpConeWait();
	wait1Msec(450);
	normalStackCone(3);
	wait1Msec(450);
	driveBothWait(-1850);
	wait1Msec(550);
	turn45DegreesR();
	wait1Msec(450);
	driveBothWait(-840);
	wait1Msec(450);
	turn90DegreesR();
	wait1Msec(450);
	driveBothWaitUntil(1100, 800);
	mobileGoalOut();
	wait1Msec(300);
}

void tenPointAuton(){

}

void auton(){
	resetEncoders();
	resetLiftEncoders();
	startTask(drivebaseControlGyro);
	autonRan = true;
	startTask(stage1Control);
	startTask(stage2Control);
	unfoldRobotAuton();

	twentyPointAuton():
	//tenPointAutuon();
}

void testPID(){
	resetEncoders();
	startTask(drivebaseControlGyro);
	straight = true;
	driveBothWait(1700);
	wait1Msec(2500);
	straight = false;
	turnWait(1800);
	wait1Msec(2500);
	straight = true;
	driveBothWait(1700);
	wait1Msec(2500);
	straight = false;
	turnWait(3600);
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
