#pragma systemFile

int limit(int val, int min = -127, int max = 127){
	if(val>max){
		return max;
	}
	if(val<min){
		return min;
	}
	return val;
}

int min(int num1, int num2) {
	if(num1>num2)
		return num2;
	else
		return num1;
}

int max(int num1, int num2) {
	if(num1>num2)
		return num1;
	else
		return num2;
}

float fabs(float num){
	if(num>0)
		return num;
	else
		return -1.0*num;
}

float fsgn(int num){
	if(num == 0)
		return 0;
    if(num > 0)
    	return 1;
    else
    	return (-1);
}

float sciN(float lead, int power){
	return lead*pow(10,power);
}

float polynom(float x, float zero, float one, float two, float three, float four, float five, float six){
	return zero + one*x + two*x*x + three*x*x*x + four*x*x*x*x + five*x*x*x*x*x + six*x*x*x*x*x*x;
}

bool deadband(int val, int target, int db = 10){
	return val > target + db || val < target - db;
}

bool deadband2(int val, int target, int db){
	return abs(val-target) < abs(db);
}
