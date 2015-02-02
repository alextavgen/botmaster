/** aasta 2009 fail, hetkel ei kasuta */
#include "jprmath.h"

JprStatClass::JprStatClass() {
	pos = 0;

	for(int i = 0; i < 20; i++)
		arr[i] = 0;
}

// 2,5-15cm
// 2,5 - 155; 5 - 118; 10 - 79; 15 - 58;

double JprMath::laiusToKaugus(int x) {
        if(x < 10) {
                x = 10;
        } else if (x > 100) {
                x = 100;
        }

        int z = x;
        return (1.34689593735949e-08) * (z * z -z * 183.799 + 8454.955) * (z * z - z * 48.415 + 879.989) * (z * z - z * 125.759 + 4414.899);
}

double JprMath::getYcm(Image::Object *B, int a) {

    if(a == 1){
        double x = B->rect.y;
        if(x > 150) x = 150;
        if(x < 5) x = 5;
        // 194,2; 152,6; 126,10; 105,15; 88,20; 68,30;
        return 30*(x -88)*(x -105)*(x -126)*(x -152)*(x -194) / (( -20)*( -37)*( -58)*( -84)*( -126)) +20*(x -68)*(x -105)*(x -126)*(x -152)*(x -194) / ((20)*( -17)*( -38)*( -64)*( -106)) +15*(x -68)*(x -88)*(x -126)*(x -152)*(x -194) / ((37)*(17)*( -21)*( -47)*( -89)) +10*(x -68)*(x -88)*(x -105)*(x -152)*(x -194) / ((58)*(38)*(21)*( -26)*( -68)) +6*(x -68)*(x -88)*(x -105)*(x -126)*(x -194) / ((84)*(64)*(47)*(26)*( -42)) +2*(x -68)*(x -88)*(x -105)*(x -126)*(x -152) / ((126)*(106)*(89)*(68)*(42));
        //return 1.4878394321544e-15 * (x - 16.222) * (x * x - 402.953 * x + 40651.332) * (x * x - 340.259 * x + 29660.601) * (x * x - 136.377 * x + 5410.666) * (x * x - 241.514 * x + 15907.208);
    }else if(a != 1){
        double y = B->rect.y;
        double w = B->rect.width;
        return (240.0-y)/w*BALL_WIDTH/0.7185/10.0; // täpsus +- 0.5 (3..20 cm); +-2 (30..60 cm); -5..-50 (80..200 cm)
    }
}


double JprMath::getYwidth(Image::Object *B, int a) {
    if(a == 1){
        double x = B->rect.y;
	if(x > 150) x = 150;
	if(x < 5) x = 5;
	//191,95;173,82;149,71;137,62;121,53;106,43;95,35;78,26;20,50;46,8;
	return 2.0353917306713e-15 * (x + 7.518) * (x * x - 75.301 * x + 1481.087) * (x * x - 381.863 * x + 36650.468) * (x * x - 161.136 * x + 7663.185) * (x * x - 286.475 * x + 22047.054);
    }else if(a != 1){
        double y = B->rect.y;
        return 0.474058791208791*y+2.80413571428571; // väga täpne, eksimus lähedal max +3 pikslit, kaugel +-1
        // kaamera asukoha muutmisel üle kontrollida
    }
}

double JprMath::getYheight(Image::Object *B) {
    double y = B->rect.y;
    return 0.528571428571429*y+0.0714285714285694;
    // kaamera asukoha muutmisel üle kontrollida
}

double JprMath::laiusToXcm(Image::Object *B) {
	//if(B->rect.width==0)
	return (B->distanceH) / ((B->rect.width) / 4.28);
}

double JprMath::getDegree(Image::Object *B){
    double dh = B->distanceH;
    return dh/120.0*(CAM_DEG/2.0);
}

void JprStatClass::insert(int nr) {
	pos++;
	if(pos >= 20) pos = 0;
	arr[pos] = nr;
}

int JprStatClass::avg() {
	int ret = 0;
	for(int i = 0; i < 20; i++)
		ret += arr[i];

	return ret / 20;
}
