#ifndef HALMATH_H
#define HALMATH_H

#include <math.h>
#include <cstdlib>

class HalMath
{
public:
    HalMath();
    void omni( int pwms[], float dirDeg, float velocityBody, int velocityMax, float velocityAngular, float battery);
    int palliKaugus( int y );
    int varavakaugus( int y );
};

#endif // HALMATH_H
