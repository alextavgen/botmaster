// VisionTime v1.2
// Authors: Silver Kuusik(Timer.cpp), Erik Kaju (C) 2010, 2012
// A tool designed for measuring robot computer vision performance + analyze & sum up results

#include "visiontime.h"
#include <string>
#include <QHash>
#include <fstream>
#include <sstream>
#include <QDebug>
#include <QVector>


// VisionTime Instance
// by default NULL
VisionTime * VisionTime::visionTimerInstance = NULL;
bool VisionTime::turnedOn = false;

//
// Public functions
//

VisionTime *VisionTime::DO()
{
    //if(turnedOn){
        // An object gets created only at first time of DO() execution
        if(visionTimerInstance == NULL){
            visionTimerInstance = new VisionTime();
            qDebug("New VisionTime instance!!!");
        }
        return visionTimerInstance;
    //} //else return NULL;
}

void VisionTime::turnOn()
{
    turnedOn=true;
}

void VisionTime::setStartMeasuringFromFrame(int frameNumber){
    startMeasuringFrame = frameNumber;
    procMemory.reserve(100000);
    timeMemory.reserve(100000);

    if(startMeasuringFrame != NULL && measureNrOfFrames != NULL)
        finalFrame = startMeasuringFrame + measureNrOfFrames;
}

void VisionTime::setMeasureNumberOfFrames(int frameNumber){
    measureNrOfFrames = frameNumber;

    if(startMeasuringFrame != NULL && measureNrOfFrames != NULL)
        finalFrame = startMeasuringFrame + measureNrOfFrames;
}

void VisionTime::newFrame(){

    if(turnedOn){
    ++currentFrame;

    if(!measure){
        if(currentFrame == startMeasuringFrame){
            measure = true;
            markBegin("frame");
        }
    }else{

        markEnd("frame");
        if(currentFrame == finalFrame){
            measure = false;
            qDebug("Locking mutex");
            memoryMutex.lock();
            qDebug("Mutex locked");
            CalculateResults();
            return;
        }
        markBegin("frame");
    }
    }
}

void VisionTime::markBegin(const std::string& action) {
    //QMutexLocker(& memoryMutex);
    if(measure){
        memoryMutex.lock();
        clock_gettime(CLOCK_MONOTONIC, &moment);
        procMemory.append(action + ";b");
        timeMemory.append(moment.tv_nsec);
        memoryMutex.unlock();
    }
}

void VisionTime::markEnd(const std::string& action) {
    //QMutexLocker(& memoryMutex);
    if(measure){
        memoryMutex.lock();
        clock_gettime(CLOCK_MONOTONIC, &moment);
        procMemory.append(action + ";e");
        timeMemory.append(moment.tv_nsec);
        memoryMutex.unlock();
    }
}

VisionTime::~VisionTime()
{
    endSaveToFile();
}

//
// Private functions
//

VisionTime::VisionTime()
{
    currentFrame = 0;
    measure = false;
    turnedOn = false;
}

void VisionTime::beginSaveToFile() {

    if( true /* !file || !file->isOpen() */) {
        QString logName("Debug/log_");
        logName += QString( QDateTime::currentDateTime().toString( QString("dd:MMM:yyyy_hh:mm:ss") ) ) + QString( ".csv" );
        file = new QFile( logName );

        if( file->open( QFile::WriteOnly | QFile::Truncate ) ) {
            out = new QTextStream( file );
            qDebug("VisionTime:Just opened time count file!");
        }else{
            qDebug("VisionTime: ERROR! Couldn't open time count file!");
            exit(1);
        }
    }
}

void VisionTime::endSaveToFile() {

    if( file->isOpen() ){
        out->flush();
        file->close();

        qDebug("VisionTime:Stopped writing to file!");
    }
}

int VisionTime::nanoDifference(int begin, int end)
{
    // if end-begin<0, then CLOCK_MONOTONIC cycle started from beginning and 1s needs to be added
    if ((end-begin) < 0) {
        qDebug("VisionTime: (debug) end-begin<=0ns\n");
        int returnval = 1000000000+(end-begin);
        qDebug()<<returnval;
        return returnval;
    } else {
        return (end-begin);
    }
}

void VisionTime::CalculateResults(){

    QHash<QString, int> measurementsBuffer;
    QHash<QString, long long> finalResults;
    QHash<QString, int> finalResultOccurrences;

    //std::string line;
    qDebug() << "VisionTime: Starting Calculation" << endl;
    int procMemRecords = procMemory.size();
    int timeMemRecords = timeMemory.size();

    qDebug() << "VisionTime: Process memory records = " << procMemRecords << ", Time memory records = " << timeMemRecords;
    if(procMemRecords == timeMemRecords){
        qDebug() << " - OK!" << endl;
    }else{
        qDebug() << "VisionTime class error @ result calculation phase! Number of process and measurements records does not match.";
    }

    for (int i = 0; i < procMemRecords; i++){

    //}
    //Swhile(std::getline(data,line)){

//        if(line == "process;action;moment"){
//            continue;
//        }
//        std::stringstream  lineStream(line);
        std::string        cell;


        int charPart = 0;
        QString message;
        QString action;
        std::stringstream procRecord(procMemory.at(i));
        int measurement = timeMemory.at(i);

            while(std::getline(procRecord,cell,';'))
            {
                charPart++;

                switch ( charPart ) {
                    case 1:
                        message = cell.c_str();
                        break;
                    case 2:
                        action = cell.c_str();
                        break;
                    default:
                        qDebug() << "VisionTime class error @ result calculation phase! A csv file may be corrupted or data format is incorrect - too much data on one line!";
                        exit(1);
                }
            }


            if(charPart != 2){
                qDebug() << "VisionTime class error @ result calculation phase! A csv file may be corrupted or data format is incorrect - too few data on one line! \n (Debug)Message: " << message << ", action: " << action << ", measurement: " << measurement;
                exit(1);
            }

            if(measurementsBuffer.contains(message)){
                if(action == "e"){
                    int actionStartTime = measurementsBuffer.value(message);
                    int calculation = nanoDifference(actionStartTime, measurement);

                    measurementsBuffer.remove(message);

                    if(finalResults.contains(message)){
                        long long oldValue = finalResults.value(message);
                        int oldCounter = finalResultOccurrences.value(message);
                        if ((oldValue+calculation) < 0){
                            qDebug() << "WTFFFF " << message << " old: " << oldValue << " new: " << calculation;
                        }
                        finalResults.insert(message, (oldValue+calculation));
                        finalResultOccurrences.insert(message, ++oldCounter);
                    }else{
                        // First occurrence
                        finalResults.insert(message, calculation);
                        finalResultOccurrences.insert(message, 1);
                    }
                }else{
                    qDebug() << "VisionTime class error @ result calculation phase! Misuse of markBegin(). Action" << message << " starting twice without ending";
                    exit(1);
                }
            }else{
                if(action == "b"){
                    measurementsBuffer.insert(message, measurement);
                }else{
                    qDebug() << "VisionTime class error @ result calculation phase! Misuse of markbEnd(). Action" << message << " ending twice without new start";
                    exit(1);
                }
            }
    }

    //Measurementsbuffer should be empty at this point
    if(measurementsBuffer.size() != 0 || finalResults.size() != finalResultOccurrences.size()){
        qDebug() << "VisionTime class error @ result calculation phase! Probably misuse of markBegin()/markEnd() functions for some action (number of executions does not match) \n Or program logic mistake :(";
        exit(1);
    }

    beginSaveToFile();
    QHashIterator<QString, long long>  finalResultIterator(finalResults);
    *out << "\n-------------------------------------------"
         << "\nMEASUREMENT OVERVIEW"
         << "\nStarted_measuring_from_frame:;" << startMeasuringFrame
         << "\nMeasured_nr_of_frames:;" << measureNrOfFrames
         << "\n\n-----"
         << "\nRESULTS"
         << "\nProcess;Total_Nanoseconds;Occurrences;Avg_nanoseconds;Avg_ms\n";

    int frameAvgDuration;
    QPair<QString, int> slowestProc;
    QPair<QString, int> fastestProc;
    slowestProc.second = 0;
    fastestProc.second = 1000000000;

    while (finalResultIterator.hasNext()) {
         finalResultIterator.next();

         QString key = finalResultIterator.key();
         long long value = finalResultIterator.value();
         int occurrences = finalResultOccurrences.value(key);

         if(key == "frame"){
            frameAvgDuration = (value/occurrences);
         }else if(value > slowestProc.second){
             slowestProc.first = key;
             slowestProc.second = value/occurrences;
         }

         if(value < fastestProc.second){
            fastestProc.first = key;
            fastestProc.second = value/occurrences;
         }

         int avgNs = (value/occurrences);
         if (avgNs < 0){
             qDebug() << "WTFFFF " << key << " val: " << value << " occ: " << occurrences;
         }

         double avgMs = (avgNs/1000000.0);

         *out << key << ";" << value << ";"
              << occurrences << ";"
              << avgNs << ";";

         int showNrOfDecimals = NULL;
         if(avgMs > 0.009){
             showNrOfDecimals = 2;
         }else{
             showNrOfDecimals = 3;
         }
         *out << QString::number(avgMs, 'f', showNrOfDecimals) << endl;
     }

    *out << "\n-----\nACCORDING_TO_CALCULATIONS"
         << "\nAverage_FPS:;" << (1000000000/frameAvgDuration)
         << "\nSlowest_process:(" << slowestProc.first << ");" << slowestProc.second << ";ns (Avg.)"
         << "\nFastest_process:(" << fastestProc.first << ");" << fastestProc.second << ";ns (Avg.)";

    printMeasurementsData(procMemRecords);
    endSaveToFile();

    qDebug("VisionTime: SUCCESS! Calculation ended. Finishing botmaster work");

    // OPENING FILE WITH RESULTS
    //std::string fileOpenExec("openoffice.org ");
    std::string fileOpenExec("gedit ");

    QByteArray filenameByteArray = file->fileName().toUtf8();
    const char* filenameChars = filenameByteArray.constData();
    //std::ifstream data(filenameChars);

    fileOpenExec.append(filenameChars);

    qDebug() << "VisionTime: Opening Results file. Command: " << fileOpenExec.c_str() << endl;

    system(fileOpenExec.c_str());
    exit(1);
}

void VisionTime::printMeasurementsData(int numberOfRecords){
    *out << "\n\n-----\nRECORDED DATA\n"
         << "process;action;moment\n";

    for(int i = 0; i < numberOfRecords; i++){
        *out << procMemory.at(i).c_str() << ";";
        *out << timeMemory.at(i) << endl;
    }
}
