#include "arduinocomm.h"

/**
 * @file arduinocomm.cpp
 * @brief C++ Implementation: comm
 *
 * @details This file holds the Comm class implementation which creates communication channel to microcontroller,
        polls events from controller, realizes communication protocol.
 *
 * Copyright: See COPYING file that comes with this distribution
 *
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
#include "arduinocomm.h"
#include <iostream>
#include <errno.h>
#include <stdlib.h>

ArduinoComm::ArduinoComm()
{
    Config & temp =Config::getConfig();
    conf = & temp;
    for(int i=0; i<MAX_DEVICES; i++) {
        fds[i]=-1;
    }
}

ArduinoComm::~ArduinoComm()
{
    close(fd);
}

/*!
    \fn Comm::openSerial()
 */
int ArduinoComm::openSerial(const char * modemDevice)
{
    return openSerial(modemDevice, 0);
}

/*!
    \fn Comm::openSerial()
 */
int ArduinoComm::openSerial(const char * modemDevice, const int nr)
{
    char serialDevice[150] = "/dev/ttyUSB";
    serialDevice[11] = nr + '0';

    struct termios options;
    std::cout<<"Serial Open "<<serialDevice<<std::endl;
    /* open the port */
    // if (modemDevice!=NULL){

    //TODO: getmodemDevice peaks ka nr ette saama

    fds[nr] = open(serialDevice, O_RDWR | O_NOCTTY | O_NDELAY);

    /*if(0 && conf->getmodemDevice()!= NULL){
     fds[nr] = open(conf->getmodemDevice(), O_RDWR | O_NOCTTY | O_NDELAY);
   }
  else{
    fds[nr] = open("/dev/usbserial", O_RDWR | O_NOCTTY | O_NDELAY);
  }*/


    if (fds[nr] == -1) {
        //Could not open the port.
        perror("open_port: Unable to open device - ");
    }
    else fcntl(fds[nr], F_SETFL, 0);

    /* get the current options */
    tcgetattr(fds[nr], &options);

    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    /* 8N1 */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    /* set raw input, 1 second timeout */
    options.c_cflag     |= (CLOCAL | CREAD);
    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag     &= ~OPOST;
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 10;

    /* set the options */
    tcsetattr(fds[nr], TCSANOW, &options);

    fd=fds[nr];
    return (fds[nr]);
}

/*!
    \fn ArduinoComm::sendCommand()
    @Author Valdur Kaldvee
 */
int ArduinoComm::sendCommand(char addr, int value)
{
    return sendCommand(addr, value, 0);
}

int ArduinoComm::sendCommand(char addr, int value, int nr)
{
    //   isLocked.wait(&mutex);
    QMutexLocker locker(&mutex);
    //if((conf->getSendCmdEnabled())==0) return 1;

    len = 0;
    buf[0] = addr;
    buf[1] = (char) ( value >> 8 );
    buf[2] = (char) value;
    buf[3] = 13;

    len = write( fds[nr], buf, 4 );
    tcflush( fds[nr], TCIFLUSH );

    if (len < 0) {
        perror("sendCommand write error - ");
        ArduinoComm::reopen("", nr);
        return 1;
    }

    return 0;
}

int ArduinoComm::readSerialMulti(char addr, int * analogs, int * digitals)
{
    return readSerialMulti(addr, analogs, digitals, 0);
}

int ArduinoComm::readSerialMulti(char addr, int * analogs, int * digitals, int nr)
{
    //   isLocked.wait(&mutex);
    QMutexLocker locker(&mutex);
    len = 0;
    buf_len = 0;

    buf[0] = addr;
    buf[1] = 13;
    len = write( fds[nr], buf, 2 );
    tcflush(fds[nr], TCIFLUSH);

    if (len < 0) {
        perror("readSerialMulti write error - ");
        ArduinoComm::reopen("", nr);
        return 1;
    }

    do {
        len = read(fds[nr], &buf[buf_len], 255 - buf_len);
        buf_len += len;
        if (len < 1 || buf_len > 254) {return 1;}
    } while (buf[buf_len-1] != '\n');

    int i, index;

    for ( index = 0, i = 0; index < 8; i+=2, index++ ) {
        int value = ( (unsigned char)buf[i] << 8 ) | (unsigned char)buf[i+1];
        analogs[index] = value;
    }

    unsigned char value = (unsigned char)buf[16];

    for( i = 0; i < 8; i++ ) {
        digitals[i] = ( value >> i ) & 1;
    }

    return 1;
}

int ArduinoComm::requestSerialMulti(char addr)
{
    return requestSerialMulti(addr, 0);
}

int ArduinoComm::requestSerialMulti(char addr, int nr)
{
    //   isLocked.wait(&mutex);
    QMutexLocker locker(&mutex);
    len = 0;

    buf[0] = addr;
    buf[1] = 13;

    len = write( fds[nr], buf, 2 );
    tcflush(fds[nr], TCIFLUSH);

    if (len < 0) {
        perror("readSerialMulti write error - ");
        ArduinoComm::reopen("", nr);
        return 1;
    }
    return 1;
}

int ArduinoComm::serialMultiResponse(int * analogs, int * digitals ) {
    return serialMultiResponse( analogs, digitals, 0 );
}

int ArduinoComm::serialMultiResponse(int * analogs, int * digitals, int nr) {

    int index, i;
    buf_len = 0;

    do {
        len = read(fds[nr], &buf[buf_len], 255 - buf_len);
        buf_len += len;
        if (len < 1 || buf_len > 254) {return 1;}
    } while (buf[buf_len-1] != '\n');

    for ( index = 0, i = 0; index < 8; i+=2, index++ ) {
        int value = ( (unsigned char)buf[i] << 8 ) | (unsigned char)buf[i+1];
        analogs[index] = value;
    }

    unsigned char value = (unsigned char)buf[16];

    for( i = 0; i < 8; i++ ) {
        digitals[i] = ( value >> i ) & 1;
    }

    return 1;
}

/*!
    \fn Comm::readSerial(char)
 */
int ArduinoComm::readSerial(char addr)
{
    return readSerial(addr, 0);
}

int ArduinoComm::readSerial(char addr, int nr)
{
    int len;
    int buf_len = 0;
    char buf[10];
    //   isLocked.wait(&mutex);
    QMutexLocker locker(&mutex);

    sprintf(buf, "\r");
    len = write(fds[nr], buf, strlen(buf));
    tcflush(fds[nr], TCIFLUSH);

    if (len < 0) {
        perror("readSerial write error - ");
        ArduinoComm::reopen("", nr);
        return 1;
    }

    sprintf(buf, "%d\r", addr);
    len = write(fds[nr], buf, strlen(buf));
    tcflush(fds[nr], TCIFLUSH);

    if (len < 0) {
        perror("readSerial write error - ");
        ArduinoComm::reopen("", nr);
        return 1;
    }

    do {
        len = read(fds[nr], &buf[buf_len], 10);
        buf_len += len;
        if (len == 0 || buf_len > 10) return 2;
    } while (buf[buf_len-1] != '\n');

    return atoi(buf);
}


//TODO Siin ka korda seada Ćµige serial device kasutamine.
inline void ArduinoComm::reopen(const char * device)
{
    reopen(device, 0);
    return;
}

inline void ArduinoComm::reopen(const char * device, int nr)
{
    close(fds[nr]);
    ArduinoComm::openSerial(device, nr);

    if(device!=""){
        ArduinoComm::openSerial(device, nr);
        return;
    }
    ArduinoComm::openSerial(device, nr);
    return;
}
