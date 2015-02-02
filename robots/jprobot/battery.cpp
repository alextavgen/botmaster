#include "battery.h"


using namespace std;

Battery::Battery(Jprobot* v){
	jpr = v;
}

int Battery::getAsus(){

	FILE *handle;
	char path[1024];
	int val;

	handle = popen("acpi |cut -d \",\" -f2 |cut -d \"%\" -f1", "r" );

	if(handle == NULL){
		return 0;
	}

	fgets(path, 100, handle);

	val = atoi(path);
	
	//cout << val << endl;
	
	pclose(handle);

	return val;
};

int Battery::getBattery(){
	jpr->getSensors();

	return jpr->analog[4];	
}
