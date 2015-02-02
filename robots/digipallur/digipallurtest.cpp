//
// C++ Implementation: DigiPallur
//
// Description: 
//
//
// Author: Renno Reinurm (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "digipallurtest.h"
#include <QTime>
#include <QDebug>
#include <iostream>

using namespace std;

DigiPallurTest::DigiPallurTest()
{
}


DigiPallurTest::~DigiPallurTest()
{
}
void DigiPallurTest::go()
{
  Config  & conf  = Config::getConfig();
  qDebug("Start testing...\n");
  Image::Object * obj;
  conf.setSendCmdEnabled(1);
  QTime t;

  while(0){
  //    gettimeofday(&a,NULL);
    t.start();
    _img = image->getFrame();
  image->process(1,0,0,0);
  
  if (image->found_objects != NULL) {
    for (int i = 0; i < image->found_objects->total; i++) {

      obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i );
	
	if(obj!=0){
      /*   std::cout<<((obj->type))<<std::endl;
	  std::cout<<((obj->distanceH))<<std::endl;
	  std::cout<<((obj->area))<<std::endl;
	  */
	}
	
    }
  }
  // sendImage();
  view->show(_img);

  //  gettimeofday(&b,NULL);
  t.elapsed();
  //std::cout<<"Töötlus "<<(b.tv_usec-a.tv_usec)/1000<<std::endl;
  //  std::cout<<"Töötlus "<<t<<std::endl;

  qDebug("Töötlus: %d ms", t.elapsed());

  t.restart();
  


  }
  setDigital(0);
  getSensors();
  cout << digital[5] << endl;
  
  //sleep(1);
  //clearDigital(0);
  //getSensors();
  //cout << digital[4] << endl;
  /*setDigital(1);
  sleep(1);
  clearDigital(0);
  */
  

 cout << "TEST"<<endl;
}

