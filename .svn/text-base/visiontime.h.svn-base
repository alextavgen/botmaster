/**
 * @file visiontime.h
 * @version 1.2
 * @brief C++ Interface: visiontime
 *
 * @details A tool designed for measuring robot computer vision performance + analyze & sum up results
 *
 * Copyright: See COPYING file that comes with this distribution
 *
 */
#ifndef VISIONTIME
#define VISIONTIME

#include <iostream>
#include <sys/time.h>

#include <QFile>
#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QVector>
#include <QMutex>


class VisionTime
{
public:

    static VisionTime * DO();
    void turnOn();

    void setMeasureNumberOfFrames(int frameNumber);
    void setStartMeasuringFromFrame(int frameNumber);
    void newFrame();
    /**
     * @brief Mark the begin of the first event.
     */
    void markBegin(const std::string & );
    /**
     * @brief Mark the end of the second event.
     */
    void markEnd(const std::string& );
    ~VisionTime();

private:
    VisionTime(); //constructor is private
    static VisionTime* visionTimerInstance; //creates a pointer to an instance of Timer
    static bool turnedOn;

    bool measure;
    timespec begin, end, temp, moment;
    QFile* file;
    QTextStream* out;
    int startMeasuringFrame, currentFrame, measureNrOfFrames, finalFrame;

    /**
    * @brief Begin saveing the measuring results to file.
    */
     void beginSaveToFile();
    /**
    * @brief End saving the measuring results to file.
    */
    void endSaveToFile();

    int nanoDifference(int begin, int end);

    void CalculateResults();
    void printMeasurementsData(int numberOfRecords);

    QVector<long> timeMemory;
    QVector<std::string> procMemory;
    QMutex memoryMutex;
};

#endif // VisionTime
