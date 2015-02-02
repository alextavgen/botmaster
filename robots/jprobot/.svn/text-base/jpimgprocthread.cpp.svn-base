#include "jpimgprocthread.h"
#include "../../image.h"
#include "../../comm.h"
//#include <cmath>


ImgProcThread::ImgProcThread(Jprobot* v){
	jpr = v;
	obj = NULL;
	Ball = NULL;
	BallPrev = 0;
	_img = jpr->getImg();
	updated = false;
	curFrame = 0;
	BallFrame = 0;
	BallPrevFrame = 0;
	timer.start();

	distYDif = 0;
	distXDif = 0;
	distDif = 0;
	nurkDif = 0;
	speedX = 0;
	speedY = 0;
	speedN = 0;
	distY = 0;
	distX = 0;
	nurkDeg = 0;
	nurkRad = 0;
}


void ImgProcThread::run(){
	//obj = NULL;
	//Ball = NULL;
	//BallPrev = 0;
	//_img = jpr->getImg();
	//while(1){
		if(NAEB_PALLI){
			CopyBall();
		}
		imgProcess();
		if(NAEB_PALLI)BallFrame = curFrame;
		//msleep(40);
		// valitud palli märgistamine
		if((_img != NULL) && NAEB_PALLI){
			cvCircle(_img, cvPoint(Ball->center.x, Ball->center.y), Ball->rect.width*0.7, CV_RGB(255,0,0), 2, CV_AA, 0);
		}
		// pildi näitamine
		/*if(curFrame%2 ==  0){
			
			jpr->getView()->show(_img);
		}*/
		timeDif = timer.elapsed();
		timer.restart();

		calculate();

		curFrame++;
		updated = true;
	//}
}

void ImgProcThread::calculate(){
	if(NAEB_PALLI){			
		distY = laiusToKaugus(getBall()->rect.width);
		distX = laiusToXcm(getBall());

		int n=nurkDeg;
		nurkRad = atan(distX/distY);		
		nurkDeg = nurkRad/0.0174532925;
		if(NAGI_PALLI && ((BallFrame-BallPrevFrame) == 1)){
			distYDif = laiusToKaugus(getBallPrev()->rect.width)-distY;
			distXDif = laiusToXcm(getBallPrev())-distX;
			nurkDif = n-nurkDeg;
			if(timeDif == 0)timeDif = 1;
			speedX = distXDif/(timeDif/1000.0);
			speedY = distYDif/(timeDif/1000.0);
			speedN = nurkDif/(timeDif/1000.0);	
		}else{
			distYDif = 0;
			distXDif = 0;
			nurkDif = 0;
			speedX = 0;
			speedY = 0;
			speedN = 0;
		}					
	}else{
		distY = 0;
		distX = 0;
		nurkDeg = 0;
		nurkRad = 0;			
	}
	dist = sqrt(distY*distY+distX*distX);
	distDif = sqrt(distYDif*distYDif+distXDif*distXDif);
	speed = sqrt(speedX*speedX+speedY*speedY);
}

void ImgProcThread::imgProcess(){
		
	_img = jpr->getImage()->getFrame();
		
		
		jpr->getImage()->process(1,0,0,0);	// TODO only one type of object to find

		//jpr->getSensors();
		
		if (jpr->getImage()->found_objects != NULL) {
			//maxArea.area = 0;
			//maxArea.type = 0;
			Ball = NULL;  			
			for (int i = 0; i < jpr->getImage()->found_objects->total; i++){
	     			obj =  (Image::Object* ) cvGetSeqElem( (jpr->getImage()->found_objects), i );
	      			//if (maxArea.area != 0) maxAreaPrev = maxArea;
				if(isValidBall(obj)){
					if(Ball == NULL) Ball = obj; //CopyObjectValues(Ball, obj); //Ball = obj;
					if((Ball->area) < (obj->area)){				
						//CopyObjectValues(Ball, obj);
						Ball = obj;
					}
				}
			 }
		}
}

bool ImgProcThread::isValidBall(Image::Object *B)
{
	double y=getYwidth(B);
	//std::cout<<"XYZ: "<<(abs(y+y/10.0)+3)<<"/"<<abs(B->rect.width)<<"/"<<(abs(y-y/10.0)-3)<<std::endl;
	//printBall(B);
	
	bool ret = true;
	ret &= (B->rect.y>40);	// ei vaata palle mis on liiga kõrgel pildis
	if(B->rect.y>40)
	ret &= ((abs(y-y/3.0))<abs(B->rect.width));	// liiga väikesed pallid
	ret &= (B->rect.y>0);

	//std::cout<<"Valid: "<<ret<<std::endl;	
	return ret;// && ((abs(y+y/10.0)+3)>abs(B->rect.width)) && ((abs(y-y/10.0)-3)<abs(B->rect.width));
}

void ImgProcThread::CopyBall(){
	/*bool add=false;	
	if(BallStack.isEmpty()){
		add = true;
	}else{
		Pall x = BallStack.top();		
		if(x.frame != curFrame){
			add = true;
		}

	}	
	if(add){	
		Pall copy;//= new Pall();
		copy.id = 0;
		copy.frame = curFrame;
		BallPrev = &(copy.Ball);	
		//if(BallPrev != NULL)delete(BallPrev);
		BallPrev = new Image::Object();
		BallPrev->area = Ball->area;
		BallPrev->distanceH = Ball->distanceH;
		BallPrev->rect = Ball->rect;
		BallPrev->center = Ball->center;
		BallPrev->type = Ball->type;

		BallStack.push(copy);
		BallStack.resize(20);
	}*/
	
	if(BallPrev == NULL){
		//delete(BallPrev);
		BallPrev = new Image::Object();
	
	}	
		BallPrev->area = Ball->area;
		BallPrev->distanceH = Ball->distanceH;
		BallPrev->rect.x = Ball->rect.x;
		BallPrev->rect.y = Ball->rect.y;
		BallPrev->rect.width = Ball->rect.width;
		BallPrev->rect.height = Ball->rect.height;
		BallPrev->center.x = Ball->center.x;
		BallPrev->center.y = Ball->center.y;
		BallPrev->type = Ball->type;
		BallPrevFrame = BallFrame;
	
}

Image::Object* ImgProcThread::getBall(){
	return Ball;
}

Image::Object* ImgProcThread::getBallPrev(){
	return BallPrev;
}

bool ImgProcThread::isUpdated(){
	bool ret = updated;
	updated = false;
	return ret;
}

void ImgProcThread::printBall(Image::Object *B)
{
	/*class Object {
  	public:    int type; //CAN, SOCK, CAN_BEACON,SOCK_BEACON 
  	CvPoint center; // center coordiantes - kaks int tüüpi muutujat x ja y
  	CvRect rect;        // bounding rectangle neli int x, y, width, height.
  	double area;        // pindala
  	int distanceH;      // horizontal distance from image center
  	};*/
	if(B != NULL){
	std::cout<<"----------------------------"<<std::endl;
	std::cout<<"area: "<<B->area<<std::endl;
	std::cout<<"BallFromFrame: "<<curFrame<<std::endl;
	std::cout<<"center.x: "<<B->center.x<<std::endl;
	std::cout<<"center.y: "<<B->center.y<<std::endl;
	std::cout<<"rect.x: "<<B->rect.x<<std::endl;
	std::cout<<"rect.y: "<<B->rect.y<<std::endl;
	std::cout<<"rect.width: "<<B->rect.width<<std::endl;
	std::cout<<"rect.height: "<<B->rect.height<<std::endl;
	
	std::cout<<"distanceH: "<<B->distanceH<<std::endl;
	}
}

/*double ImgProcThread::speedY(){
	if(timeDif == 0)timeDif = 1;
	return distYDif/(timeDif/1000.0);
}

double ImgProcThread::speedX(){
	if(timeDif == 0)timeDif = 1;
	return distXDif/(timeDif/1000.0);
}

double ImgProcThread::speedN(){
	if(timeDif == 0)timeDif = 1;
	return xnurkDif/(timeDif/1000.0);
}*/

double ImgProcThread::calcDist(double deg){
	double dx=sin(deg*0.0174532925)*distX;
	double dy=cos(deg*0.0174532925)*distY;
	return sqrt(dx*dx+dy*dy);
}

