//
// C++ Implementation: view
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "view.h"
#include <iostream>
#include "objecttype.h"


View::View()
{
  Config & temp =Config::getConfig();
  conf = & temp;

  cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX_SMALL, hScale, vScale, 0, lineWidth, 8);
  
  // Create a window in which the captured images will be presented
  //cvNamedWindow("Robovision" /*TODO getRobotName from config */, CV_WINDOW_NORMAL);
  cvNamedWindow("Robovision" /*TODO getRobotName from config */, CV_WINDOW_AUTOSIZE);
  cvCreateTrackbar("H min", "Robovision", &((conf->getOrange())->hMin), 255, NULL);
  cvCreateTrackbar("H max", "Robovision", &((conf->getOrange())->hMax), 255, NULL);

  cvCreateTrackbar("S min", "Robovision", &((conf->getOrange())->sMin), 255, NULL);
  cvCreateTrackbar("S max", "Robovision", &((conf->getOrange())->sMax), 255, NULL);

  cvCreateTrackbar("L min", "Robovision", &((conf->getOrange())->lMin), 255, NULL);
  cvCreateTrackbar("L max", "Robovision", &((conf->getOrange())->lMax), 255, NULL);
}


View::~View()
{
}

View & View::getView()
{
  static  View instance;
  return  instance;
}




/*!
    \fn View::show()
 */
void View::show(IplImage * image)
{
  static char  key=0;

  static int frame = 0;
    
  if(1 || frame%10 == 0 /*conf->getShowImage()==1*/){   
	cvShowImage( "Robovision", image);
  }
  frame++;

  key = cvWaitKey(2);
  if(key == 27) {
    exit(0);
//     driveStop();
//     close(fd);
   // break;
  }

//std::cout << " Key pressed:" << key << std::endl;  

  switch(key){
    case 'd':
       if (conf->getShowDebugInfo()) {
         conf->setShowDebugInfo(0);
         qDebug("Debug OFF\n");
       } else {
         conf->setShowDebugInfo(1);
         qDebug("Debug ON\n");
       }
      break;
    case 32: //Space
       conf->setSendCmdEnabled(1);
//        printf("test %d ",conf->getSendCmdEnabled());
       qDebug("Command Send Enabled \n");
      break;
    case '0':
	conf->keyS = 0;
      break;
    case '1':
	conf->keyS = 1;
      break;
    case '2':
	conf->keyS = 2;
      break;
    case '3':
	conf->keyS = 3;
      break;
    case '4':
	conf->keyS = 4;
      break;
    case 'p':
      cvSaveImage("frame.bmp", image);
      break;
  }

}




