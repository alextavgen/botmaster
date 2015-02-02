/**
        @author  Margus Ernits <margus.ernits@itcollege.ee>
 */

//
// C++ Interface: tolmurull
//
// Description: 
//
//
// Authors: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, Katrin Kibbal (C) 2008
// Active developers: Lennart Lüsi
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HAL_H
#define HAL_H

#include "abstractrobot.h"
#include "image.h"
#include "view.h"
#include "haldebug.h"
#include "halmath.h"
#include <iostream>
#include <QFile>
#include <QTime>
#include <math.h>

//TODO Mis asi see on
#define MAX_KIIRUS 250
////TODO Mis asi see on
#define MIN_KIIRUS 150
//TODO Mis asi see on
#define KAUGEL 30
//TODO Mis asi see on
#define POORAMISKIIRUS 90

//TODO Mis asi see on
#define STOP 0
//TODO Mis asi see on
#define OTSI_PALLI 1
//TODO Mis asi see on
#define POORA_PALLILE 2
//TODO Mis asi see on
#define SOIDA_PALLINI 3
//TODO Mis asi see on
#define OTSI_VARAVAT 4
//TODO Mis asi see on
#define POORA_VARAVALE 5

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

/*

HAL dokumentatsioon

Analogs:
Kõik määrangud on tagant ja pealtvaates!
Analog(0) - Palli andud
Analog(1) - Ees vasak andur
Analog(2) - Ees parem andur
Analog(3) - Vasak küljeandur
Analog(4) - Parem küljeandur
Analog(5) - Kaugusandur

Digital(0) - solekas
Digital(6) - vasak majaka andur
Digital(7) - parem majaka andur

Seisundid:
0 - stop
1 - otsi palli
2 - pööra pallile
3 - sõida pallini
4 - otsi värav
5 - pööra väravale (löö) 

*/

class Hal : public AbstractRobot {
public:

    Hal();
    ~Hal();
    void omni(float, float, int, float, float);
    void go();

    // teiste klasside objektid
    HalDebug* halDebug;
    HalMath* halMath;

    // pwmide vaartused
    int pwms[3];

    // objektid pildid
    Image::Object* pall;
    Image::Object* goal;
    Image::Object* obj;

    int state;
    int kaader;
    int i, suund, kiirus, otsing;
    int sein_vasakul();
    int sein_paremal();

    void avoidWalls();
    void stop();
    void keera(int speed);
    void vaata_pilti();
    void otsi_obj();
    void poora_keskele(int kaugusKeskelt);
    void soida_pallini(int kaugusPallist, int palliSuund);
    void loo();
    void soidaEdasi();
    void loemeKaadrid();
    void naitaPilti();
    void kuvaPilt();

    void alustaLogimist();
    void lopetaLogimine();

    inline bool isBall() {
        if (analog[0] < 300) return false;
        return true;
    }

    inline bool vasakulEes() {
        if (analog[1] > 250) return true;
        return false;
    }

    inline bool paremalEes() {
        if (analog[2] > 250) return true;
        return false;
    }

    inline bool vasakul() {
        if (analog[3] > 250) return true;
        return false;
    }

    inline bool paremal() {
        if (analog[4] > 250) return true;
        return false;
    }

    inline bool kaugelEes() {
        if (analog[5] > 400) return true;
        return false;
    }

    // konsooli debugimine
    inline void debug ( QString debug ){
        // debugime ainult, kui debugimine on sisse lylitatud
        if( DEBUG_MODE ){
            std::cout << debug.toStdString() << std::endl;
        }
    }

    inline void viivitus( long t ) {
        usleep( t * 1000 );
    }

};

#endif
