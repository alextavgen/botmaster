/**
 * @file main.cpp
 * @brief Daemon program thats reads and writes LPT port to print debug information
 *
 *
 * @details Holds the abstract class AbstractRobot which all the robots inherit.
 *
 * Copyright: See COPYING file that comes with this distribution
 *
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2011
 * thanks http://eagerfish.eu/lpt-programming-example-in-c-for-ubuntu-linux
 * thanks http://tldp.org/HOWTO/IO-Port-Programming.html
 */


#include <QtCore/QCoreApplication>
#include "parallelport.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    ParallelPort port;

    port.go();

    return a.exec();
}
