
/**
 * @file maiu.cpp
 * @brief C++ Implementation: maiu
 * 
 * @details Configuration module for Robot Vision System.
 * 
 * Copyright: See COPYING file that comes with this distribution
 * 
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
#include "maiu.h"
#include <sys/time.h>
#include <QTime>
#include <QDebug>
#include <iostream>
#include <math.h>
#include "server.h"

int soitKiirus = 80;
//255 enne

int poordeKiirus = 0;

//int pooramisSuund = 1;
bool suundKeeramine = 1;
// 0 otse ja 180 tagurpidid
int soitSuund = 0;

int pallSuund = 1;
int stateLoomine = 1; // 0 pööramise pealt, 1 kui ei taha pöörata

//erinevad errorid
int suurViga = 160; //80,160
int vaikeViga = 110; //55,110*/

// Näitab kui palju tohib keskteljest mööda minna
int viga = 0;

bool startOtsi = 1;
bool startOtsiE = 0;

int valik = 0;

bool pallOnEes = 0;

int valitudVarav = GOAL_BLUE;

bool muudaSuund = true;
bool ignoreeripalle = false;

float ballDistanceX = 0.0;
float ballDistanceY = 0.0;
float ballAngleDeg = 0.0;
float b = 0.0;
int turnCoef = 0;

//Mida see stopper teeb? WTF nimi stopperile!
QTime stopper0;
//Mida see stopper teeb? WTF nimi stopperile!
QTime stopper1;

QTime ignoreball;

using namespace std;

Maiu::Maiu(){

 //maiuDebug = new MaiuDebug();
}


Maiu::~Maiu()
{
}

void Maiu::stop() {
    for (int i = 0; i < 3; i++) {
        setDcMotor(i, 512);
    }
     soitKiirus = 0;
    
}

void Maiu::omni(float dirDeg, float velocityBody, int velocityMax, float velocityAngular, float battery) {
        float velocityX, velocityY;
        float velocityWheel[3];
        float max = 0.0;
        float scaleCoef;
        float dirRad;
        int pwm[3];
        int pwmSum = 0;
        int direction;
        int i;

        dirRad = dirDeg * 0.0174532925;	// degrees to radians

        // Correcting speed according battery
        direction = 1;
        if (velocityBody < 0) direction = -1;
        velocityBody = sqrt((velocityBody*velocityBody) * battery) * direction;

        direction = 1;
        if (velocityAngular < 0) direction = -1;
        velocityAngular = sqrt((velocityAngular*velocityAngular) * battery) * direction;


        velocityX = velocityBody * cos(dirRad);
        velocityY = velocityBody * sin(dirRad);

        //Vw[0] = round(cos(w1) * Vx + sin(w1) * Vy + L * Va);
        //Vw[1] = round(cos(w2) * Vx + sin(w2) * Vy + L * Va);
        //Vw[2] = round(cos(w3) * Vx + sin(w3) * Vy + L * Va);

        // 150, 30, 270 (60, 300, 180)
        velocityWheel[0] = -0.866025404 * velocityX + 0.5 * velocityY + velocityAngular;
        velocityWheel[1] = 0.866025404 * velocityX + 0.5 * velocityY + velocityAngular;
        velocityWheel[2] = -velocityY + velocityAngular;

        // Voltage is not linearly proposional to power
        max = 0.0;
        for (i = 0; i < 3; i++) {
            if (abs(velocityWheel[i]) > max) max = abs(velocityWheel[i]);
        }

        if (max > 0) {
            for (i = 0; i < 3; i++) {
                direction = 1;
                if (velocityWheel[i] < 0) direction = -1;
                velocityWheel[i] = sqrt((max * max) * (abs(velocityWheel[i]) / max)) * direction;
            }
        }

        // Scaling
        max = 0.0;
        for (i = 0; i < 3; i++) {
            if (abs(velocityWheel[i]) > velocityMax) max = abs(velocityWheel[i]);
        }

        if (max > velocityMax) {
            scaleCoef = velocityMax / max;
            for (i = 0; i < 3; i++) {
                velocityWheel[i] = velocityWheel[i] * scaleCoef;
            }
        }

        for (i = 0; i < 3; i++) {
            pwm[i] = round(velocityWheel[i]);
            pwmSum = pwmSum + pwm[i];
        }

        for (i = 0; i < 3; i++) {
            // If not all wheels are stopped break wheels with 0 speed
            if (pwm[i] == 0 && pwmSum != 0) {
                setDcMotor(i, 512);
            } else {
                setDcMotor(i, pwm[i]);
            }
        }
    }

void Maiu::naitaPilti() {
    if(valitudVarav == GOAL_YELLOW)
        cvCircle(_img, cvPoint(5, 5), 5, CV_RGB(255,255,0), 5, CV_AA, 0);
    else if(valitudVarav == GOAL_BLUE)
        cvCircle(_img, cvPoint(5, 5), 5, CV_RGB(0,0,255), 5, CV_AA, 0);

   /* // save the image if debugging is enabled
    if( state != STOP ){
        kaader++;
        //std::cout << "pall: " << analog[0] << std::endl;
        maiuDebug->logi( DEBUG_FILE, DEBUG_VIDEO, _img, pall, analog, digital, pwms, state, kaader, stopper1.elapsed() );
    }

    if( kaader % 3 == 0 ){
        kuvaPilt();
    }*/
}

void Maiu::kuvaPilt() {
    if( DEBUG_IMAGE ) {
        view->show( _img );
    }
}

void Maiu::loemeKaadrid(){
    for(int i = 0; i < 5 /*TODO siin võib lugeda 2 kaadrit*/; i++){
        _img = image->getFrame();
    }
}

// Pöörame, kõiki mootoreid ühes suunas
void Maiu::keera(int speed) {

    omni(0, 0, 255, speed, 1);
}

void Maiu::loo() {

    //viivitus(300);
    setDigital(0);
    viivitus(250);
    clearDigital(0);
    //loemeKaadrid();
    qDebug( "--------------SHOT FIRED! ------------!" );
}

int Maiu::palliKaugus(int y)
{
        int kaugus = (int) (308362.484895514 * pow(y, -2.16049138430261));
        //int kaugus = (int) (342.781116297389 * pow(y, -0.448966183073739));
    //int kaugus = (int)(209.460992688557 * pow(y,-0.980263301970066) * 10 - 10);
    //int kaugus = (int) (3524.1113825362 * pow(y, -1.2495304685));
    //int kaugus = (int) (145.454390312423 * pow(0.976310673281194, y));
    return kaugus;
}

void Maiu::vaata_pilti(){
    _img = image->getFrame();
    image->process(1, 0, 0, 0); // TODO only one type of object to find
    //bool pallOnEes = 0;
    //Otsime palli ja värava
    if (image->found_objects != NULL) {
        pall = NULL;
        goal = NULL;
        for (int i = 0; i < image->found_objects->total; i++) {
            obj = (Image::Object*) cvGetSeqElem((image->found_objects), i);
            if (obj->rect.y > 50 && obj->area < 300)  // liiga väikseid obj/palli ei vaata
            {

            }
            else {
                if (obj->type == BALL) {
                    if(abs(obj->distanceH) < 20)
                    {
                        pallOnEes = true;
                    }
                    else{
                        pallOnEes = false;
                    }
                    if (pall == NULL)
                    {
                        pall = obj;
                    }
                    //if ((pall->rect.y) < ((obj->rect.y) * 1.1f) && (pall->rect.y) > ((obj->rect.y) * 0.9f)) {
                    if ((pall->rect.y) < (obj->rect.y))
                    {
                        pall = obj;
                    }
                }
            }
            //TODO värava valik realiseerida
            if (obj->type == valitudVarav) {
                if (goal == NULL) goal = obj;
                if ((goal->area) < (obj->area)) goal = obj;
            }
        }
    }
}

// Palli ja värava otsimine, selleks sõidab mingitmoodi ringi
void Maiu::otsiPalli(){

    omni(0,0,255,40,1);
    }

void Maiu::anduriteTest() {
    // digital
    requestSensors();
    getSensorsResponse();
    std::cout<<"PALLI_ANDUR: " << analog[0] << std::endl;
        //std::cout<<"EESANDUR_VASAK: " << analog[1] << std::endl;
        //std::cout<<"EESANDUR_PAREM: " << analog[2] << std::endl;
        //std::cout<<"VASAK_KYLG: " << analog[3] << std::endl;
        //std::cout<<"PAREM_KYLG: " << analog[4] << std::endl;
        //std::cout<<"KAUGUS_ANDUR: " << analog[5] << std::endl;
        //std::cout<<"VARAV_VASAK: " << digital[6] << std::endl;
        //std::cout<<"VARAV_PAREM: " << digital[7] << std::endl;
    viivitus(100);
}

void Maiu::soidaEdasi(){
    omni(0,100,255,0,1); //100 enne
    viivitus(700);

}

//LENNARTI KOOD HAKKAB SIIT
void Maiu::ignore_ball()  {
ignoreball.start();
ignoreeripalle = true;
qDebug( "IGNORE BALL" );
}

void Maiu::soida_pallile() {
    int kaugusKeskelt;
    int veasuurus;
    int kiirus = 0;
        //palli kaugus ekraani vertikaalsest keskpunktis: pall->distanceH
    if(pall->rect.y!=0||pall->rect.y<300){
     soitKiirus = 255 - pall->rect.y + 10;  //tuunime kordajat
     }
     if(soitKiirus > 180) soitKiirus = 180;
     //if(soitKiirus < 60) soitKiirus = 60;
     if(pall->distanceH!=0||pall->distanceH<640){
     kaugusKeskelt = pall->distanceH;
     }

if (pall->area != 0 && pall->center.y != 0) {
     //ballDistanceY = 1131458219.26 * pow((float) pall->center.y, -2.99);
     ballDistanceY = 10524.11 * pow((float) pall->center.y, -1.34);
     std::cout<<"ballDistanceY: " << ballDistanceY << std::endl;

     // b = tan(30 deg) * ballDistance
     b = 0.577350269 * ballDistanceY;

     ballDistanceX = (pall->distanceH / 320.0) * b;
     std::cout<<"ballDistanceX: " << ballDistanceX << std::endl;

     ballAngleDeg = atan(ballDistanceX / ballDistanceY) * 180 / M_PI;
}
     //sprintf(str, "Palli nurk: %.1f", ballAngleDeg);
     //cvPutText(_img, str, cvPoint(ball.rect.x, ball.rect.y - 55), &(image->font), CV_RGB(0, 0, 0));

     //sprintf(str, "Palli kaugus(cm): %.0f", ballDistanceY);
     //cvPutText(_img, str, cvPoint(ball.rect.x, ball.rect.y - 15), &(image->font), CV_RGB(0, 0, 0));

     turnCoef = sqrt(35.0 - ballDistanceY);
     if (turnCoef < 0) turnCoef = 0;

     kiirus = abs(sqrt(pow(45.0, 2.0) * (fabs(ballAngleDeg) / 30.0)) + turnCoef);
     std::cout<<"Pooramiskiirus: " << kiirus << std::endl;


     //int kiirus = 0.2f * abs(kaugusKeskelt) + 10; //+47 32*3.2  Valem vaja välja arvutada! Nurk alpha.
     //if (kiirus > 70) kiirus = 70; //120
   //  if (kiirus < 70) kiirus = 70;

     if (kaugelEes()){
      qDebug( "Andur tuvastab seina?!" );

     }

     if (kaugusKeskelt >= 15) {
         omni(0, soitKiirus, 255, kiirus, 1);
     } else if(kaugusKeskelt <= -15) {
         omni(0, soitKiirus, 255, -kiirus, 1);
     } else if (pall->rect.y >= 140){
         qDebug( "OTSI_PALLI: soidaEdasi funct " );
         //std::cout<<"Soidan edasi: " << pall->rect.y << std::endl;
         soidaEdasi();
     }
     else {
         omni(0, soitKiirus, 255, 0, 1);

     }

}

void Maiu::otsi_varavat() {

    //Majakaandurid:
    /*if (digital[6] == 1 && digital[7] == 0 && !suundKeeramine) {
        suundKeeramine = 1;
    } else if(digital[6] == 0 && digital[7] == 1 && suundKeeramine) {
        suundKeeramine = 0;
    }
    if(suundKeeramine) {
        if (goal == NULL){
            keera(-110);
            qDebug( "OTSI_VARAVAT: otsime varavat " );
        } else {
            keera(-65);
            qDebug( "OTSI_VARAVAT: leidsime varava poorame aeglaselt " );
        }
    } else {
        if (goal == NULL){
            keera(110);
            qDebug( "OTSI_VARAVAT: otsime varavat " );
        } else {
            keera(65);
            qDebug( "OTSI_VARAVAT: leidsime varava poorame aeglaselt " );
        }
    }*/
    if(goal == NULL){
        keera(80);
    }
    if (goal != NULL){
        keera(40);

        int goalLeft;
        int goalRight;
        int goalCenter;
        int center;
        int offset;

        offset = goal->rect.width / 20;

        goalLeft = goal->rect.x + offset; // std::cout<<"GoalLeft: " << goalLeft << " " << goal->rect.x << " " << offset << std::endl;
        goalRight = goal->rect.x + goal->rect.width - offset;  //std::cout<<"goalRight " << goalRight << std::endl;
        goalCenter = goalLeft + (goal->rect.width / 2); // std::cout<<"goalCenter: " << goalCenter << std::endl;
        center = _img->width / 2; // std::cout<<"Center " << center << std::endl;

        if (center > goalLeft && center < goalCenter){
            //if(!pallOnEes) {
               qDebug( "OTSI_VARAVAT: sihik paigas, l66me!! " );
               loo();

               viivitus(200);
               //}
        }
        if (center < goalRight && center > goalCenter){
            //if(!pallOnEes) {
                qDebug( "OTSI_VARAVAT: sihik paigas, l66me!! " );
                loo();
                viivitus(200);

            //}
        }
        /*else{
            qDebug( "OTSI_VARAVAT: teine pall ees!" );
            omni(90,100,255,0,1);
            viivitus(300);
            stop();
            state=OTSI_VARAVAT;
        }*/

        //viga = goal->rect.width * 0.8f;

        /*if (abs(goal->distanceH) < viga){
            qDebug( "OTSI_VARAVAT: sihik paigas, l66me!! " );
            if(!pallOnEes){
            loo();
            }else{
                qDebug( "OTSI_VARAVAT: teine pall ees!" );
                omni(90,100,255,0,1);
                viivitus(600);
                state=OTSI_VARAVAT;
            }
        }*/
    }
}
//LENNARTI KOOD LÕPPEB SIIN

void Maiu::go()
{

 Config  & conf  = Config::getConfig();

 qDebug("Maiu is go...\n");
 //stopper1.start();

 state = STOP;
 //kaader = 0;

 while(0){

   conf.setSendCmdEnabled(1);
   omni(0,0,255,0,1);
   viivitus(2000);

   /*
   Kolmerattalise roboti liikumise meetod
   dirDeg 			- liikumis nurk (nurk päripäeva: 90 paremale, 270 vasakule)
   velocityBody 	- kiirus
   velocityMax		- maksimaalne kiirus (ntx 255)
   velocityAngular - pöörlemiskiirus
   battery 		- patarei (1)
*/

 }
 while (1) {

    //requestSensors();
    vaata_pilti();
    naitaPilti();
    kuvaPilt();
    //getSensorsResponse();





    if(ignoreball.elapsed()>600){
    ignoreeripalle=false;
    }



     switch(state){
     case STOP:
         valik = conf.keyS;
         if(valik == 1){
             qDebug( "valiti kollane varav" );
             valitudVarav = GOAL_YELLOW;
         }else if(valik == 2){
             qDebug( "valiti sinine varav" );
             valitudVarav = GOAL_BLUE;
         }else if(valik == 3){
             anduriteTest();
         }

         //kuvaPilt();

         if(conf.getSendCmdEnabled())
         {
             conf.setSendCmdEnabled(1);
             qDebug( "STOP: alustame kaklemist" );
             stopper1.start(); //stopper1???????????????????????????????
             if(isBall() && state != OTSI_VARAVAT) {
                 state = OTSI_VARAVAT;
             }
             else
             {
                 state = OTSI_PALLI;
             }
         }
         break;

     case OTSI_PALLI:

         if(isBall()){
             state = OTSI_VARAVAT;
         }
         else if( pall != 0) {
             state = SOIDA_PALLINI;
         }else{
         qDebug( "OTSI_PALLI: soidame ringi ja otsime palle " );
             //sõida ringi väljakul
             omni(0,0,255,40,1); //360

         }

     break;

     case SOIDA_PALLINI:

         if( pall == 0) {
             state = OTSI_PALLI;
         }else if(isBall()){
             state = OTSI_VARAVAT;
         }
         else{
             //std::cout<<"rect.y: " << pall->rect.y << std::endl;
             //std::cout<<"distanceH: " << pall->distanceH << std::endl;
           // qDebug( "SOIDA_PALLINI: soidame pallile " );

             soida_pallile();
         }

     break;

     case OTSI_VARAVAT:
         if(!isBall()){
            state = OTSI_PALLI;
         }
         else {
             qDebug("Pall olemas!");
             otsi_varavat();
         }

     break;

     }

     }

}
