#ifndef JPRMATH_H
#define JPRMATH_H
#include "../../image.h"
#include <iostream>
#include <QTime>

#define CAM_DEG (56) //ei ole 75 nagu lubatud, sisse zoomides on veelgi väiksem
#define CAM_HEIGHT (110) // 110 mm
#define BALL_WIDTH (43) // 43 mm
#define IMAGE_WIDTH (320) // 320 pix
#define IMAGE_HEIGHT (240) // 240 pix

class JprStatClass {
private:
	int pos;
public:
	JprStatClass();	
	int arr[20];
	void insert(int nr);
	int avg();	
};

class JprMath {
public:
        double getYwidth(Image::Object*, int a = 0);
        double getYheight(Image::Object *B);

	double laiusToKaugus(int x);			// Y telg ainult
	double laiusToXcm(Image::Object *B);		// X telje suunas

        double getYcm(Image::Object *B, int a = 0);	//arvutab Y-st kauguse

        double getDegree(Image::Object *B);     // palli nurk roboti suhtes
};

#endif
