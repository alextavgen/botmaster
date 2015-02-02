//
// C++ Interface: camera
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CAMERA_H
#define CAMERA_H
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

#ifdef XI
#warning "XIAPI defined"
#include <xiApi.h>
#include <xiExt.h>
#define HandleResult(res,place) if (res!=XI_OK) {printf("Error after %s (%d)",place,res);exit(1);}
#endif

/**
This Class holds camera information

	@author Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>
*/

class Camera{
public:
    Camera();
    ~Camera();
    void reOpen();
    void close();
    
    	// image buffer
    	

    /*!
        \fn Camera::getCapture()
     */
    CvCapture  * getCapture()
    {
       return capture;
    }
    
#ifdef XI
	// Sample for XIMEA API V2.10
	static HANDLE xiH;
	static XI_RETURN stat;
    HANDLE  getXI_Capture(){
    return xiH;
    }
#endif

    IplImage* getImg()
    {
      if(img) cvReleaseImage(&img);
      img=cvLoadImage(pictureFilename);
      if(!img){
        printf("Could not load image file: %s\n",pictureFilename);
        exit(1);
      }
      return img;
    }
    
    IplImage* img; 

public:
  CvCapture  *  capture;
    char * pictureFilename;
  
  
};

#endif
