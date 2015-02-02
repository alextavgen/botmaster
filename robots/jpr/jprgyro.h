#ifndef JPRGYRO_H
#define JPRGYRO_H

#include <QTime>

class JprGyro
{
public:
    JprGyro();

    void collectCenter(int val);
    void calcCenter();

    void restart();
    void update(int analog);

    int deg();

    double scale(double val);

    int center;

private:
    QTime time;
    double degSum;   
    int sumCount;
};

#endif // JPRGYRO_H
