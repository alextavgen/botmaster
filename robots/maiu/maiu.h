/**
 * @file maiu.h
 * @brief C++ Interface: maiu
 * 
 * @details Configuration module for Robot Vision System.
 * 
 * Copyright: See COPYING file that comes with this distribution
 * 
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
#ifndef MAIU_H
#define MAIU_H

//TODO Mis asi see on
#define MAX_KIIRUS 100
////TODO Mis asi see on
#define MIN_KIIRUS 50
//TODO Mis asi see on
#define KAUGEL 30
//TODO Mis asi see on
#define POORAMISKIIRUS 80
#define PALLSUUND = 1;

#define PALLI_ANDUR analog[0]
#define EESANDUR_VASAK analog[1]
#define EESANDUR_PAREM analog[2]
#define VASAK_KYLG analog[3]
#define PAREM_KYLG analog[4]
#define KAUGUS_ANDUR analog[5]
#define VARAV_VASAK digital[6]
#define VARAV_PAREM digital[7]

#define VOORU_AEG 90000

#define DEBUG_VIDEO 0
#define DEBUG_FILE 0
#define DEBUG_MODE 1
#define DEBUG_IMAGE 1

#include "abstractrobot.h"
#include "image.h"
#include "view.h"
#include "maiudebug.h"

/**
 * @brief Common class for testing robot functionality across all teams.
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
class Maiu: public AbstractRobot{
public:
  
    Maiu();

    ~Maiu();

    void go();

    MaiuDebug* maiuDebug;

    void omni(float, float, int, float, float);

    inline void viivitus( long t ) {
        usleep( t * 1000 );
    }
  private:
    int state;

    void loo();
    void soidaEdasi();
    void anduriteTest();
    void stop();
    void keera(int speed);
    
    void otsiPalli();
    void vaata_pilti();
    void pallipealeKeeramiseTest2();

    void soida_pallile();
    void loemeKaadrid();
    void naitaPilti();
    void kuvaPilt();
    void otsi_varavat();

    void alustaLogimist();
    void lopetaLogimine();

    void ignore_ball();

    inline bool isBall() {
        requestSensors(0);
        getSensorsResponse(0);
       // std::cout<<"PALLI_ANDUR: " << analog[0] << std::endl;
        if (analog[0] < 600) return false;
        else{
         return true;
        }

    }


    inline bool vasakulEes() {
        if (analog[1] > 200) return true;
        return false;
    }

    inline bool paremalEes() {
        if (analog[2] > 200) return true;
        return false;
    }

    inline bool vasakul() {
        if (analog[3] > 200) return true;
        return false;
    }

    inline bool paremal() {
        if (analog[4] > 200) return true;
        return false;
    }

    inline bool kaugelEes() {
        if (analog[5] > 200) return true;
        return false;
    }

    inline void debug ( QString debug ){
        // debugime ainult, kui debugimine on sisse lylitatud
        if( DEBUG_MODE ){
            std::cout << debug.toStdString() << std::endl;
        }
    }

    int palliKaugus(int y);

    // pwmide vaartused
    int pwms[3];

    // objektid pildid
    Image::Object* pall;
    Image::Object* goal;
    Image::Object* obj;
   
};



#endif
