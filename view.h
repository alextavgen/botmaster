//
// C++ Interface: view
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VIEW_H
#define VIEW_H
#include "image.h"
#include "config.h"
#include <cv.h>
#include <highgui.h>
// #include <pthread.h>


/**
This class displays image to screen, socket or file

	@author Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>
*/
class View{
public:
    CvFont font;
    static const double hScale=1.0;
    static const double vScale=1.0;
    static const int    lineWidth=1;
    int darker;  // how mach is black darker than averige of image

    static View & getView();
    ~View();
    void show(IplImage *);
    Config * conf;


  private:
    


  
//     Image * image;
    View();

   

    View(View const&);    // Cannot copy singelton
    View& operator=(View const&);  //hide this too

};

#endif
