#pragma systemFile

task drivebaseControlGyro(){
	desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	float kP = 0.05499;
	float kD = 0.0254;
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
	int slewVal = 10;
	while(true){
		errorT = desiredTurn - gyroLeftIsPositive()*SensorValue(Gyro);
		derivativeT = errorT - oldT;
		oldT = errorT;
		if(slewResetDrive){
			slewVal = 15;
			slewResetDrive = false;
		}
		if(!straight){
			drive(slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT), 7), -slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT), 7));
		}
		else{
			avgEncoder = (SensorValue(rightEncoder) + SensorValue(leftEncoder)) / 2.0;
			error = desiredDrive - avgEncoder;
			derivative = error - old;
			old = error;
			driveSpeed = limit(kP*(error) + kD*derivative);
			turnSpeed = limit(kPt*errorT + kDt*derivativeT, -1.75*abs(driveSpeed), 1.75*abs(driveSpeed));
			drive(slew(&slewVal, driveSpeed + turnSpeed, 15), slew(&slewVal, driveSpeed - turnSpeed, 15));
		}
		delay(5);
	}
}

void twentyPointAuton(){
	mobileGoalOut();
	wait1Msec(450);
	driveBothWait(4150);
	mobileGoalIn();
	wait1Msec(450);
	driveBothWait(-3200);
	wait1Msec(550);
	resetEncoders();
	turn45DegreesR();
	wait1Msec(450);
	driveBothWait(-2520);
	wait1Msec(450);
	turn90DegreesR();
	wait1Msec(450);
	driveBothWaitUntil(3400, 2400);
	mobileGoalOut();
	wait1Msec(300);
}

void tenPointAuton(){

}

void auton(){
	resetEncoders();
	startTask(drivebaseControlGyro);

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
