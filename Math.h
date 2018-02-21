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

bool deadband(int val, int target, int db = 10){
	return val > target + db || val < target - db;
}

bool deadband2(int val, int target, int db){
	return abs(val-target) < abs(db);
}
