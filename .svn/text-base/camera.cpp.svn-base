//
// C++ Implementation: camera
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "camera.h"
#include <iostream>
#include "config.h"
#include <QtCore>
#include <visiontime.h>


Camera::Camera()
{
  VisionTime::DO()->markBegin("init_camera_constructor");
  Config  & conf  = Config::getConfig(); //TODO kaamera numbri määramine
  const char* opencv_libraries = 0;
  const char* addon_modules = 0;
  cvGetModuleInfo( 0, &opencv_libraries,&addon_modules );
  qDebug("OpenCV: ");
  qDebug(opencv_libraries);
  qDebug("\n");
  qDebug("Add-on Modules : ");
  qDebug(addon_modules );
  qDebug("\n");
  
#ifdef XI   	
	XI_IMG xi_image;
	xi_image.size = sizeof(XI_IMG);
	xi_image.bp = NULL;
	xi_image.bp_size = 0;
#endif

  pictureFilename=conf.getImageFileName();
  if(conf.getShowImage()==1){
    img=cvLoadImage(pictureFilename);
    if(!img){
      printf("Could not load image file: %s\n",conf.getImageFileName());
      exit(1);
    }
  }else{
#ifdef XI
xiH = NULL;
stat = XI_OK;
	
	// Get number of camera devices
	DWORD dwNumberOfDevices = 0;
	stat = xiGetNumberDevices(&dwNumberOfDevices);
	
	if (!dwNumberOfDevices)
	{
		printf("No camera found\n");
		exit(1);
	}
	// Retrieving a handle to the camera device 
	stat = xiOpenDevice(0, &xiH);
	HandleResult(stat,"xiOpenDevice");
	
	xiSetParamInt(xiH, XI_PRM_BUFFERS_QUEUE_SIZE, 2);
	xiSetParamInt(xiH, XI_PRM_RECENT_FRAME, 1);	

	xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RAW8);
	//xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING, 2);

        xiSetParamInt(xiH, XI_PRM_HEIGHT, 400);
        xiSetParamInt(xiH, XI_PRM_WIDTH, 1280);

        xiSetParamInt(xiH, XI_PRM_OFFSET_X, 0);
        xiSetParamInt(xiH, XI_PRM_OFFSET_Y, 1024/2);

	// Setting "exposure" parameter (10ms=10000us)
	stat = xiSetParamInt(xiH, XI_PRM_EXPOSURE, 16000);
	HandleResult(stat,"xiSetParam (exposure set)");

        // Setting "gain" parameter
        //stat = xiSetParamInt(xiH, XI_PRM_EXPOSURE, 16000);
        HandleResult(xiSetParamInt(xiH, XI_PRM_GAIN, 6),"xiSetParam (set gain)");
	
  // Setting auto white ballance
	//stat = xiSetParamInt(xiH, XI_PRM_AUTO_WB, 1);
	//HandleResult(stat,"xiSetParam (auto white ballance set)");

	//stat = xiSetParamInt(xiH, XI_PRM_GAIN, 15.0);
	//HandleResult(stat,"xiSetParam (gain set)");
	
#else
	//CV_CAP_ANY
  capture = cvCaptureFromCAM( 1 );

  if( !capture ) {
    qDebug("Warning: capture is NULL \nTrying ANY camera device");
  
  capture = cvCaptureFromCAM( 2 );
  } 
 

  if( !capture ) {
    qDebug("Warning: capture is NULL \nTrying ANY camera device");
    capture = cvCaptureFromCAM( CV_CAP_ANY );
  }
 

  if( !capture ) {
    fprintf( stderr, "ERROR: capture is NULL \n" );
    exit(1);
  }
#endif
  }

  //TODO kaamera init read ini failist lugeda
  //TODO käivitada init read
/*
  system("uvcdynctrl  -s 'Exposure, Auto' 1");            // 1 - off, 3 - on
  system("uvcdynctrl  -s 'Exposure (Absolute)' 300");
  system("uvcdynctrl  -s 'Gain' 0");
  system("uvcdynctrl  -s 'White Balance Temperature, Auto' 0");
  system("uvcdynctrl  -s 'White Balance Temperature' 4000");
  system("uvcdynctrl  -s 'Saturation' 64");   // default 32
  system("uvcdynctrl  -s 'Power Line Frequency' 1");      // 0 - off, 1 - 50 Hz, 2 - 60 Hz
  */
#if !(defined XI)
          cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );

          cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
        //  cvSetCaptureProperty(capture,CV_CAP_PROP_FPS,60);

          /*CV_CAP_PROP_POS_MSEC
          film current position in milliseconds or video capture timestamp
          CV_CAP_PROP_POS_FRAMES
          0-based index of the frame to be decoded/captured next
          CV_CAP_PROP_POS_AVI_RATIO
          position in relative units (0 - start of the file, 1 - end of the file)
          CV_CAP_PROP_FRAME_WIDTH
          width of frames in the video stream
          CV_CAP_PROP_FRAME_HEIGHT
          height of frames in the video stream
          CV_CAP_PROP_FPS
          frame rate
          CV_CAP_PROP_FOURCC
          4-character code of codec
          CV_CAP_PROP_FRAME_COUNT
          number of frames in video file
          CV_CAP_PROP_FORMAT
          CV_CAP_PROP_MODE
          CV_CAP_PROP_BRIGHTNESS
          Brightness
          CV_CAP_PROP_CONTRAST
          Contrast
          CV_CAP_PROP_SATURATION
          Saturation
          CV_CAP_PROP_HUE
          Hue
          CV_CAP_PROP_GAIN
          Gain
          CV_CAP_PROP_CONVERT_RGB*/
#endif
VisionTime::DO()->markEnd("init_camera_constructor");
}


Camera::~Camera()
{
}
void Camera::reOpen()
{
    //CV_CAP_ANY
capture = cvCaptureFromCAM( 1 );
qDebug("Avan kaamera uuesti");
if( !capture ) {
qDebug("Warning: capture is NULL \nTrying ANY camera device");

capture = cvCaptureFromCAM( 2 );
}
if( !capture ) {
qDebug("Warning: capture is NULL \nTrying ANY camera device");
capture = cvCaptureFromCAM( CV_CAP_ANY );
}

if( !capture ) {
qDebug("ERROR: capture is NULL \n");
exit(1);
}

cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );

cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
//cvSetCaptureProperty(capture,CV_CAP_PROP_FPS,60);
}


void Camera::close(){
    if(capture)
        cvReleaseCapture(&capture);
    capture=NULL;
}

#ifdef XI
HANDLE Camera::xiH=NULL;
XI_RETURN Camera::stat=0;
#endif


