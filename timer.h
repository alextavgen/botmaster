/**
 * @file timer.h
 * @brief C++ Interface: timer
 *
 * @details Measure time between events.
 *
 * Copyright: See COPYING file that comes with this distribution
 *
 */
#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <sys/time.h>

#include <QFile>
#include <QString>
#include <QDateTime>
#include <QTextStream>

class Timer
{
public:
    ~Timer();
    /**
     * @brief Begin saveing the measureing results to file.
     */
    void beginSaveToFile();
    /**
     * @brief End saveing the measureing results to file.
     */
    void endSaveToFile();
    /**
     * @brief Begin or end outputing measureing results to console.
     * @param output - Output the measureing results to console
     */
    void setOutputToConsole(bool);
    /**
     * @brief Mark the begin of the first event.
     */
    void markBegin();
    /**
     * @brief Mark the end of the second event.
     */
    void markEnd();
    /**
     * @brief Returns static Timer class instance. It cannot be accessed directly because Timer is a singleton class.
     */
    static Timer & getTimer();
private:
    Timer();
    bool save_to_file;
    bool output_to_console;
    timespec begin, end, temp;
    QFile* file;
    QTextStream* out;
    /**
     * @brief Get the difference of two counter readings.
     * @param begin - First counter reading
     * @param end - Second counter reading
     * @return temp - The difference of begin and end
     */
    timespec difference(timespec, timespec);
};

#endif // TIMER_H
