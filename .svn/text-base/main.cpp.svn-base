//#include <QtCore/QCoreApplication>
#include <QApplication>
#include "server.h"
#include <iostream>
#include "abstractrobot.h"
#include "robotfactory.h"
#include "config.h"
#include "visiontime.h"

using namespace std;

int main(int argc, char *argv[])
{

    //Visiontime init
    //VisionTime::DO()->turnOn();
    VisionTime::DO()->setStartMeasuringFromFrame(100);
    VisionTime::DO()->setMeasureNumberOfFrames(30);


   QCoreApplication a(argc, argv);
//    QApplication a(argc,argv);

    Config  & conf  = Config::getConfig(/*argn,argv*/);
    conf.setConfig(argc,argv);

    // ?? cout << "new QTcpServer" << endl;

    AbstractRobot * robot = RobotFactory::getRobot(conf.getRobotName());


    robot->go();

    return a.exec();
}
