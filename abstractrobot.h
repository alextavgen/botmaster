/**
 * @file abstractrobot.h
 * @brief C++ Interface: abstractrobot
 * 
 * @details Holds the abstract class AbstractRobot which all the robots inherit.
 * 
 * Copyright: See COPYING file that comes with this distribution
 * 
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
#ifndef ABSTRACTROBOT_H
#define ABSTRACTROBOT_H
//#include "log.h"
#include "camera.h"
#include "config.h"
#include "image.h"
#include "comm.h"
#include "arduinocomm.h"
#include "view.h"



/**
 * @brief This abstract class holds functions that are common to all robots.
 * @author Margus Ernits <margus.ernits@itcollege.ee>
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>
 */

//TODO Aadresside ja kommunikatsiooni vaheline vastavustabel
class AbstractRobot{
public:
    AbstractRobot();

    ~AbstractRobot();
    
	/// @brief Virtual function which can be over-ridden within an inhereting class (individual robots).
    virtual void go()=0;
	
	/// @deprecated This function is deprecated. Use getServo(int servo, int nr) instead.
    int getServo(int servo);
	/**
	 * @brief Reads the value of a given servo from the numbered (@a nr) serial device.
	 * @param servo - Number of a @a servo on a serial device (valid input: 0-9)
	 * @param nr - Serial device number
	 * @return Servo value returned from the serial device ADC
	 */
    int getServo(int servo, int nr);
	
	/// @deprecated This function is deprecated. Use getAnalog(int analog_no, int nr) instead.
    int getAnalog(int analog_no);
	/**
	 * @brief Reads the value of a given analog sensor from the numbered (@a nr) serial device.
	 * @param analog_no - Number of an analog sensor on a serial device
	 * @param nr - Serial device number
	 * @return Analog sensor value returned from the serial device ADC
	 */
    int getAnalog(int analog_no, int nr);
	
	/// @deprecated This function is deprecated. Use getAnalogs(int nr) instead.
    int getAnalogs();

	/**
	 * @brief Reads all sensor values from the numbered (@a nr) serial device into two arrays: analog[8] and digital[8].
	 * @param nr - Serial device number
	 * @return 0 | 1 (0 = SUCCESS, 1 = FAILURE)
	 * @see Equivalent to getSensors(int nr)
	 */
    int getAnalogs(int nr);
	
	/// @deprecated This function is deprecated. Use getSensors(int nr) instead.
    int getSensors();

	/**
	 * @brief Reads all sensor values from the numbered (@a nr) serial device into two arrays: analog[8] and digital[8].
	 * @param nr - Serial device number
	 * @return 0 | 1 (0 = SUCCESS, 1 = FAILURE)
	 * @see Equivalent to getAnalogs(int nr)
	 */
    int getSensors(int nr);

	/// @deprecated This function is deprecated. Use requestSensors(int nr) instead.
    int requestSensors();

	/**
	 * @brief Sends a read request for all sensor values from the numbered (@a nr) serial device
	 * @param nr - Serial device number
	 * @return 0 | 1 (0 = SUCCESS, 1 = FAILURE)
	 * @see Equivalent to getAnalogs(int nr)
	 */
    int requestSensors(int nr);


	/// @deprecated This function is deprecated. Use getSensorsResponse(int nr) instead.
    int getSensorsResponse();

	/**
	 * @brief Writes serial device responce into two arrays: analog[8] and digital[8].
	 * @param nr - Serial device number
	 * @return 0 | 1 (0 = SUCCESS, 1 = FAILURE)
	 * @see Equivalent to getAnalogs(int nr)
	 */
    int getSensorsResponse(int nr);

	/// @deprecated This function is deprecated. Use getAnalogsAvg(int nr) instead.
    int getAnalogsAvg();

	/**
	 * @brief Reads the average of all sensor values from the numbered (@a nr) serial device into two arrays: analog[8] and digital[8].
	 * @param nr - Serial device number
	 * @return 0 | 1 (0 = SUCCESS, 1 = FAILURE)
	 * @todo Does it actually work? Is this being used?
	 */
    int getAnalogsAvg(int nr);
	
	/// @deprecated This function is deprecated. Use setServo(int servo, unsigned char speed, int nr) instead.
    void setServo(int servo, unsigned char speed);
	/**
	 * @brief Sends the new @a speed value of a given @a servo to the numbered (@a nr) serial device.
	 * @param servo - Number of a @a servo on a serial device
	 * @param speed - Servo speed value
	 * @param nr - Serial device number
	 */
    void setServo(int servo, unsigned char speed, int nr);
	
	/// @deprecated This function is deprecated. Use setServoOff(int nr) instead.
    void setServoOff();
	/**
	 * @brief Sends the command to turn off all servos on the numbered (@a nr) serial device.
	 * @param nr - Serial device number
	 */
    void setServoOff(int nr);
	
	/// @deprecated This function is deprecated. Use setServoOn(int nr) instead.
    void setServoOn();
	/**
	 * @brief Sends the command to turn on all servos on the numbered (@a nr) serial device.
	 * @param nr - Serial device number
	 */
    void setServoOn(int nr);
    //DO NOT USE void saveServos();
	
	/// @deprecated This function is deprecated. Use setDcMotor(int motor, int speed, int nr) instead.
    void setDcMotor(int motor, int speed);
	/**
	 * @brief Sends the new @a speed value of a given @a motor to the numbered (@a nr) serial device.
	 * @param motor - Number of the DC motor on the serial device (valid input: 0-4)
	 * @param speed - Motor speed value
	 * @param nr - Serial device number
	 * @see For more detailed information see: http://robot.itcollege.ee/wiki/index.php/ServoBasic_kontroller
	 */
    void setDcMotor(int motor, int speed, int nr);
    void setOmni(int dirDeg, int velocityBody, int velocityAngular);
    void setDigital(unsigned char pin_nr);
    void setDigital(unsigned char pin_nr, int nr);
    void clearDigital(unsigned char pin_nr);
    void clearDigital(unsigned char pin_nr, int nr);
	void sendImage();

    int analog[8]; ///< Holds all the analog sensor values retrieved by Comm::readSerialMulti(char addr, int * analogs, int * digitals, int nr)
    int digital[8]; ///< Holds all the digital sensor values retrieved by Comm::readSerialMulti(char addr, int * analogs, int * digitals, int nr)
    

protected:
  //Camera camera;
  Comm *  com;
  Image * image;
  View * view;
public:
  IplImage* _img;
private:
  //SendThread thread;  // pole vaja
  unsigned char digitalBitMaskValue;
  
};

#endif
