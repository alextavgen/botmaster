#include "jprgyro.h"

JprGyro::JprGyro()
{
    time.start();
    degSum = 0;
    sumCount = 0;
}

void JprGyro::collectCenter(int val){
    degSum += val;
    sumCount++;
}

void JprGyro::calcCenter(){
    center = degSum/sumCount;
    degSum = 0;
    sumCount = 0;
}

void JprGyro::update(int analog){
    degSum += float(analog - center)*(float(time.elapsed())/1000.0);
    sumCount++;
    time.restart();
}

double JprGyro::scale(double val){
    //return val/225.0*360.0;
    return val;
}

int JprGyro::deg(){
    return int(scale(degSum));
}

void JprGyro::restart(){
    time.restart();
    sumCount = 0;
    degSum = 0;
}
