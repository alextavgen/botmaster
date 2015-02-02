#include "jptopsecretmasterclass.h"
#include "../../image.h"
#include "../../comm.h"
//#include <cmath>

StatClass::StatClass(){
	pos = 0;
	for(int i=0;i<20;i++)arr[i]=0;
}

/*TopSecretMasterClass::TopSecretMasterClass(Jprobot* v){
	jpr = v;
}*/

// 2,5-15cm
// 2,5 - 155; 5 - 118; 10 - 79; 15 - 58;

double TopSecretMasterClass::laiusToKaugus(int x){
	if(x < 10){
		x = 10;
	} else if (x > 100){
		x = 100;
	}

	int z = x;
	return (1.34689593735949e-08)*(z*z -z*183.799 +8454.955)*(z*z -z*48.415 +879.989)*(z*z -z*125.759 +4414.899);
}

double TopSecretMasterClass::getYcm(Image::Object *B)
{
	double x = B->rect.y;
	if(x>150)x=150;
	if(x<5)x=5;
	//191,5;173,7;149,10;137,12;121,15;106,20;95,25;78,35;67,50;46,150;
	return 1.4878394321544e-15*(x -16.222)*(x*x -402.953*x +40651.332)*(x*x -340.259*x +29660.601)*(x*x -136.377*x +5410.666)*(x*x -241.514*x +15907.208);
}

double TopSecretMasterClass::getYwidth(Image::Object *B)
{
	double x = B->rect.y;
	if(x>150)x=150;
	if(x<5)x=5;
	//191,95;173,82;149,71;137,62;121,53;106,43;95,35;78,26;20,50;46,8;
	return 2.0353917306713e-15*(x +7.518)*(x*x -75.301*x +1481.087)*(x*x -381.863*x +36650.468)*(x*x -161.136*x +7663.185)*(x*x -286.475*x +22047.054);
}

double TopSecretMasterClass::laiusToXcm(Image::Object *B)
{
	//if(B->rect.width==0)
	return (B->distanceH)/((B->rect.width)/4.28);
}

void StatClass::insert(int nr){
	pos++;
	if(pos>=20)pos=0;
	arr[pos]=nr;
}

int StatClass::avg(){
	int ret=0;
	for(int i=0; i<20;i++)ret+=arr[i];
	return ret/20;
}


