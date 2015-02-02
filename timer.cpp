#include "timer.h"

Timer::Timer()
{
    save_to_file = false;
    output_to_console = true;
}

Timer::~Timer()
{
    endSaveToFile();
}

Timer & Timer::getTimer()
{
    static Timer instance;
    return instance;
}

void Timer::beginSaveToFile() {
    save_to_file = true;
    if( !file || !file->isOpen() ) {
        QString logName("Debug/log_");
        logName += QString( QDateTime::currentDateTime().toString( QString("dd:MMM:yyyy_hh:mm:ss") ) ) + QString( ".csv" );
        file = new QFile( logName );
        if( file->open( QFile::WriteOnly | QFile::Truncate ) ) {
            out = new QTextStream( file );
            *out << "begin;end;difference" << endl;
        }
    }
}

void Timer::endSaveToFile() {
    save_to_file = false;
    if( file->isOpen() ){
        out->flush();
        file->close();
    }
}

void Timer::setOutputToConsole(bool output) {
    output_to_console = output;
}

void Timer::markBegin() {
    clock_gettime(CLOCK_MONOTONIC, &begin);
}

void Timer::markEnd() {
    clock_gettime(CLOCK_MONOTONIC, &end);
    temp = difference(begin, end);
    if(  output_to_console ) {
        std::cout << "B [" << begin.tv_sec << ":" << begin.tv_nsec
            << "] E [" << end.tv_sec << ":" << end.tv_nsec
            << "] D [" << difference(begin,end).tv_sec << ":" << difference(begin,end).tv_nsec << "]"
            << std::endl;
    }
    if( save_to_file ) {
        *out << begin.tv_sec << ":" << begin.tv_nsec << ";"
            << end.tv_sec << ":" << end.tv_nsec << ";"
            << difference(begin,end).tv_sec << ":" << difference(begin,end).tv_nsec << ";"
            << endl;
    }
}

timespec Timer::difference(timespec begin, timespec end)
{
        timespec temp;
        if ((end.tv_nsec-begin.tv_nsec)<0) {
                temp.tv_sec = end.tv_sec-begin.tv_sec-1;
                temp.tv_nsec = 1000000000+end.tv_nsec-begin.tv_nsec;
        } else {
                temp.tv_sec = end.tv_sec-begin.tv_sec;
                temp.tv_nsec = end.tv_nsec-begin.tv_nsec;
        }
        return temp;
}
