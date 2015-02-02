#ifndef JPRTEST_H
#define JPRTEST_H

#include "jpr.h"
#include "jprnet.h"

// siia klassi tulevad ainult roboti testimisega seotud kood

class JprTest:public Jpr, public JprNet
{
public:
    JprTest();
    void go();
protected:
    bool flag; // testides
    int kiirus;
    QTime testTime; // ainult testides
    void testing();
    void motorTest();

    void runTest(int i);
    void solenoidiTest0();
    void rulliTest0();
    void rulliTest1();
};

#endif // JPRTEST_H
