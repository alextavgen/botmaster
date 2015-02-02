/**
 * @file config.h
 * @brief C++ Interface: config
 * 
 * @details Configuration module for Robot Vision System.
 * 
 * Copyright: See COPYING file that comes with this distribution
 * 
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
#ifndef CONFIG_H
#define CONFIG_H
#include <getopt.h> //longopts
#include "simpleini/SimpleIni.h"
#include "objecttype.h"
#include <cv.h>
#include <highgui.h>

/**
 * Configuration model for Robot Vision System
 * 
 * @author Valdur Kaldvee
 * @author Margus Ernits <margus.ernits@itcollege.ee>
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>
*/
class Config{
public:

//Config(int argn, char ** argv);
    ~Config();


private:

    //TODO mis parameetreid võiks veel command linest ette anda? (Margus)
     int brightness;
     int contrast;
     char * bmp_filename;
     const char * video_device;
     const char * serial_device;
     
     const char * robot_name;
     const char * robot_params; 
     Config();
     int readIni();
     int showDebugInfo;
     int sendCmdEnabled;
     int blackPercent;
     int colorPercent;
     int topCut;
     ObjectType blue, yellow, orange, black;


public:
    const char *  getmodemDevice();
    const char * getRobotName();
    const char * getRobotParams();
    void setConfig(int argn, char ** argv);
    static Config& getConfig();
    void setSendCmdEnabled(int);
    void setShowDebugInfo( int);
    void usage(char**);
    int getShowDebugInfo();
    int getSendCmdEnabled();
    int getTopCut();
    void setTopCut(int);    
    int keyS;    

    ObjectType  * getBlue()
    {
      return  & blue;
    }
    ObjectType *  getYellow()
    {
      return  & yellow;
    }
    ObjectType* getOrange()
    {
	return & orange;
    }
    ObjectType  * getBlack()
    {
      return  & black;
    }
    void setShowImage ( int theValue )
    {
      showImage = theValue;
    }
    

    int getShowImage() const
    {
      return showImage;
    }

    void setUseImageFile ( int theValue )
    {
      useImageFile = theValue;
    }
    

    int getUseImageFile() const
    {
      return useImageFile;
    }
    char * getImageFileName()
    {
      return bmp_filename;
    }

protected:
    int showImage;
    int useImageFile;
};

#endif
