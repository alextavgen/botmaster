#ifndef HALTEST_H
#define HALTEST_H

#include <abstractrobot.h>
#include <iostream>
#include "halmath.h"

#define MAX_KIIRUS 250
#define MIN_KIIRUS 50
#define KAUGEL 20
#define POORAMISKIIRUS 95

class HalTest : public AbstractRobot
{
public:
    HalTest();

    void go();
    void omni(float, float, int, float, float);
    void loo();
    void vaata_pilti();
    void stop();
    void anduriteTest();
    void anduriteTest2();
    void mootoriteTest();
    void loomiseTest();
    void pallipealeKeeramiseTest();
    void pallipealekeeramiseTest2();
    void palliAndmeteTest();
    void pealeSoitmiseAndmeteTest();
    void varavaAndmeteTest();
    HalMath* halMath;

    Image::Object* pall;
    Image::Object* goal;
    Image::Object* obj;

    int pwms[3];

    inline bool isBall() {
        if (analog[0] < 300) return false;
        return true;
    }

    inline bool vasakulEes() {
        if (analog[1] > 250) return true;
        return false;
    }

    inline bool paremalEes() {
        if (analog[2] > 250) return true;
        return false;
    }

    inline bool vasakul() {
        if (analog[3] > 250) return true;
        return false;
    }

    inline bool paremal() {
        if (analog[4] > 250) return true;
        return false;
    }

    inline bool kaugelEes() {
        if (analog[5] > 400) return true;
        return false;
    }

    inline void viivitus( long t ) {
        usleep( t * 1000 );
    }

};

#endif // HALTEST_H
