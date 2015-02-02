//
// C++ Implementation: hal
//
// Description: r
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, Katrin Kibbal (C) 2008
//
// Copyright: See COPYING file that comes with this distribution    timer1.start();
//
//


#include "hal.h"

int soiduKiirus = 255;
int pooramisKiirus = 0;

//int pooramisSuund = 1;
bool keeramisSuund = 1;
// 0 otse ja 180 tagurpidid
int soiduSuund = 0;

int palliSuund = 1;
int loomisState = 1; // 0 pööramise pealt, 1 kui ei taha pöörata

//erinevad errorid
int suurError = 160; //80,160
int vaikeError = 110; //55,110

// Näitab kui palju tohib keskteljest mööda minna
int error = 0;

//Mida see timer teeb? WTF nimi timerile!
QTime timer0;
//Mida see timer teeb? WTF nimi timerile!
QTime timer1;

bool otsiStart = 1;
bool otsiStartE = 0;

int selection = 0;

bool pallEes = 0;

int valitud_varav = GOAL_RED;

int varavaSuund = 1;

bool muudaSuunda = true;

Hal::Hal() {
    halDebug = new HalDebug();
    halMath = new HalMath();
    
    // anname pwmidele algvaartused
    for (int i = 0; i < 3; i++) {
        pwms[i] = 666; //WTF!! kas see on pidur?
    }
}

Hal::~Hal() {

}

/*
        Kolmerattalise roboti liikumise meetod
        dirDeg 			- liikumis nurk (nurk vastupäeva)
        velocityBody 	- kiirus
        velocityMax		- maksimaalne kiirus (ntx 255)
        velocityAngular - pöörlemiskiirus
        battery 		- patarei (1)
*/
void Hal::omni(float dirDeg, float velocityBody, int velocityMax, float velocityAngular, float battery) {

    halMath->omni( pwms, dirDeg, velocityBody, velocityMax, velocityAngular, battery);

    for (int i = 0; i < 3; i++) {
        setDcMotor(i, pwms[i]);
    }
}

// Vaatame kaamera pilti ja otsime sealt objektid: kõige suurema palli ja värava
void Hal::vaata_pilti(){
    _img = image->getFrame();
    image->process(0, 0, 0, 0); // TODO only one type of object to find (177)

    //Otsime palli ja värava
    if (image->found_objects != NULL) {
        pall = NULL;
        goal = NULL;
        for (int i = 0; i < image->found_objects->total; i++) {
            obj = (Image::Object*) cvGetSeqElem((image->found_objects), i);		        
            if (obj->rect.y > 1 && obj->area < 1)  // liiga väikseid obj/palli ei vaata
            {
            }
            else {
                if (obj->type == BALL) {
                    if(abs(obj->distanceH) < 25 /* TODO Seda on vaja tuunida */)
                    {

                        pallEes = true;
                    }
                    else{
                        pallEes = false;
                    }
                    if (pall == NULL)
                    {
                        pall = obj;
                    }
                    //if ((pall->rect.y) < ((obj->rect.y) * 1.1f) && (pall->rect.y) > ((obj->rect.y) * 0.9f)) {
                    if ((pall->rect.y*1.1) /* TODO seda korrutaks 1.1f ga või hoopis areat! */ < (obj->rect.y))
                    {
                        pall = obj;
                    }
                }
            }
            //TODO värava valik realiseerida
            if (obj->type == valitud_varav) {
                if (goal == NULL) goal = obj;
                if ((goal->area) < (obj->area)) goal = obj;
            }
        }
    }
}

//Mis asi see on?
//bool soidaMujale = false;

// Palli ja värava otsimine, selleks sõidab mingitmoodi ringi
void Hal::otsi_obj(){
    
    switch(state) {
    case OTSI_PALLI:

        if (timer0.elapsed() < 5000)  
        {
            keera(palliSuund * POORAMISKIIRUS);
        }
        else
        {

            if (EESANDUR_VASAK > 150 && EESANDUR_PAREM > 150)   // Siis oleme nurgas, tagurdame

            {
                debug( "tagurdan nurgast välja" );
                omni(180, 250, 255,0, 1);
                viivitus(200);
                keera(180);
                viivitus(700);
            }
            else if (EESANDUR_VASAK > 160)			// Vasakul sein ees, sõidame paremale
            {
                debug( "paremale - katki" );
                keera(180);
                viivitus(500);
            }
            else if (EESANDUR_PAREM > 160)			// Paremal sein ees, sõidame vasakule
            {
                debug( "vasakule - katki" );
                keera(-180);
                viivitus(500);
            }
            else if (KAUGUS_ANDUR > 350)
            {
                debug( "olen varavas" );
                keera(180);
                viivitus(500);
            }
            else {
                debug( "olen soitmas" );
                omni(0, 250, 255, 0, 1);
            }

            if(timer0.elapsed() > 7000)
                timer0.start();

            

        }
	break;

    case OTSI_VARAVAT:
        requestSensors(); //TODO SEE OSA ON KAHTLANE!
        getSensorsResponse(); //TODO SEE OSA ON KAHTLANE!

        //Vasakul
        if (digital[6] == 1 && digital[7] == 0 && !keeramisSuund) {
            keeramisSuund = 1;
            //Paremal
        } else if(digital[6] == 0 && digital[7] == 1 && keeramisSuund) {
            keeramisSuund = 0;
        }

        //Vasakule
        if(keeramisSuund) {
            if (goal == NULL){
                keera (-POORAMISKIIRUS * 1.2);
            } else {
                keera (-POORAMISKIIRUS * 0.3f);//tuleb realiseerida teises kohas
            }//Paremale
        } else {
            if (goal == NULL){
                keera (POORAMISKIIRUS * 1.2);
            } else {
                keera (POORAMISKIIRUS * 0.3f);//tuleb realiseerida teises kohas
            }
        }

        break;
    }
}

// Pöörame palli poole nii, et kiirus väheneb sõltuvalt kaugusest keskjoonest
void Hal::poora_keskele(int kaugusKeskelt) {
    /*
        int x = Y / kaugusKeskelt;
        if(x > 1) x = 1;
        float nurk = asin(x);
        int keeramisKiirus = soiduKiirus * nurk / 150;
        */
    //int kiirus = kaugusKeskelt / 1.75f;
    // selle valemi järgi kiirus muutub vahemikus 52 - 80
    //int kiirus = 0.2f * abs(kaugusKeskelt) + 52;
    int kiirus = 0.3f * abs(kaugusKeskelt) + 40;

    // Kui peaks juhtuma nii, siis ei lasta kiirust kunagi alla/üle lubatu
    //if (kiirus < MIN_KIIRUS) kiirus = MIN_KIIRUS;
    if (kiirus > MAX_KIIRUS) kiirus = MAX_KIIRUS;

    // TODO: Vastavalt palli kaugusele
    //if (palliKaugus(palliY) > KAUGEL) error = suurError;
    //if (palliKaugus(palliY) < KAUGEL) error = vaikeError;
    if (kaugusKeskelt >= error) {
        omni(0, 0, 255, -kiirus, 1);
    } else if(kaugusKeskelt <= -error) {
        omni(0, 0, 255, kiirus, 1);
    } else {
        //omni(0, 0, 255, 0, 1);
        stop();
    }

}

// Läheneme pallile, kiirus väheneb sõltuvalt palli verikaalsest asendist ()
void Hal::soida_pallini(int kaugusPallist, int pooramisSuund)
{
    soiduKiirus = 255;
    int minKiirus = 200;
    int nurkKiirus = 0;
    /*if (error == suurError){
                nurkKiirus = (-0.1f * soiduKiirus + 50) * pooramisSuund;
        }
        else {
                nurkKiirus = 0;
        }*/
    // Kui kaugus suurem, kui ... paneme sajaga
    if (kaugusPallist > KAUGEL)
    {
        nurkKiirus = (-0.1f * soiduKiirus + 50) * pooramisSuund;
        //std::cout << "nurkkiirus = " << nurkKiirus << std::endl;
        omni(0,soiduKiirus,255,nurkKiirus,1);
    }
    // pallile lähedal, sõidame natukene edasi ja jääme seisma
    else if (kaugusPallist <= 0)
    {
    	QString s;
    	s.append( "Soidan pallile sisse kaugus[" ).append( QString::number( kaugusPallist ) ).append( "]" );
        debug( s );
        soidaEdasi();
    } else {
        //float y = (float)pall->rect.y;
        //soiduKiirus = 20 + (1 / (y / 32)) * soiduKiirus;
        //if(soiduKiirus < minKiirus) { soiduKiirus = minKiirus; }
        soiduKiirus = kaugusPallist * 4.4f + minKiirus;
        nurkKiirus = (-0.1f * soiduKiirus + 40) * pooramisSuund;
        if(soiduKiirus < minKiirus)	soiduKiirus = minKiirus;
        omni(0,soiduKiirus,255,nurkKiirus,1);
    }

    // Vaatame, kas sein on ees
    // Siis oleme nurgas, tagurdame
    if (vasakulEes() && paremalEes())
    {
        omni(180, 250, 255,0, 1);
        viivitus(200);
        keera(150);
        viivitus(700);
        stop();
       // loemeKaadrid();
    }
    // Vasakul sein ees, sõidame paremale
    if (vasakulEes())
    {
        omni(270,120,255,30,1);
        viivitus(300);
        stop();
       // loemeKaadrid();
    }
    // Paremal sein ees, sõidame vasakule
    if (paremalEes())
    {
        omni(90,120,255,-30,1);
        viivitus(300);
        stop();
        //loemeKaadrid();
    }
    if (kaugelEes())
    {
        omni(180, 150, 255,30, 1);
        viivitus(500);
        keera(100);
        viivitus(500);
        stop();
      //  loemeKaadrid();
    }

}

// Lööme palli
void Hal::loo() {

    //viivitus(300);
    setDigital(0);
    viivitus(300);
    clearDigital(0);
    loemeKaadrid();
    debug( "--------------LÕIN PALLI ------------!" );
}

// Sõidame natukene edasi
void Hal::soidaEdasi(){
    omni(0,110,255,0,1);
    viivitus(600);
    stop();
}



void Hal::go() {


    
    Config & conf = Config::getConfig();

    debug( "Hal ootab starti..." );
    conf.setSendCmdEnabled(0);
    timer1.start();

    state = STOP;
    kaader = 0;

    //Soit
    while (0) {
	/*
        Kolmerattalise roboti liikumise meetod
        dirDeg 			- liikumis nurk (nurk vastupäeva)
        velocityBody 	- kiirus
        velocityMax		- maksimaalne kiirus (ntx 255)
        velocityAngular - pöörlemiskiirus
        battery 		- patarei (1)
*/ 
        conf.setSendCmdEnabled(1);
        omni(0,110,255,0,1);
        viivitus(2000);
        stop();
        omni(180,110,255,0,1);
        viivitus(2000);
        stop();
	
    }
    //Loomine test
    while (0) {

        conf.setSendCmdEnabled(1);
        vaata_pilti();
        getAnalogs();
        if(isBall()) {loo();}
        else
            debug( "Game Over!" );
	viivitus(1000);
    }

    while (1) {

        requestSensors(); 

        vaata_pilti();
    	
        getSensorsResponse(); 
        //getAnalogs();
        /*
        if( timer1.elapsed() > VOORU_AEG ) {
         stop();
         state = STOP;
         halDebug->lopetaLogimine( DEBUG_FILE, DEBUG_VIDEO );
            debug( "Game Over!" );
            conf.setSendCmdEnabled(0);
            break;
        }
*/

                //if (state == OTSI_PALLI && pall == NULL) vaata_pilti();
                //if (state == OTSI_VARAVAT && goal == NULL) vaata_pilti();


                switch(state){
            
            // Robot seisab ja ei tee midagi
            case STOP:
                selection = conf.keyS;
                if(selection == 0){
                    debug( "valiti punane varav" );
                    valitud_varav = GOAL_RED;
                }else if(selection == 1){
                    debug( "valiti sinine varav" );
                    valitud_varav = GOAL_BLUE;
                }
                //std::cout << "selection: " << selection << std::endl;
                if(conf.getSendCmdEnabled())
                {
                    conf.setSendCmdEnabled(1);
                    debug( "STOP: alustame kaklemist" );
                    halDebug->alustaLogimist( DEBUG_FILE, DEBUG_VIDEO );
                    timer1.start();
                    //std::cout << "läksin otsima" << std::endl;
                    if(isBall() && state != POORA_VARAVALE) {
                        state = OTSI_VARAVAT;
                    }
                    else
                    {
                        state = OTSI_PALLI;
                    }
                }
                //stop();
                break;

                // Pöörame ringi ja otsime palli
            case OTSI_PALLI:
                if(pall == NULL) {

                    debug( "OTSIN_PALLI: otsin palli" );
                    otsiStart = 1;
                    if (otsiStart == 1 && otsiStartE != 1) {
                        timer0.start();
                        otsiStartE = otsiStart;
                    }

                    otsi_obj();

                } else {
                    debug( "OTSIN_PALLI: leidsin palli" );
                    otsiStart = 0;
                    otsiStartE = otsiStart;
                    state = POORA_PALLILE;
            	}
                break;

                // Kui leidsime palli siis pöörame roboti täpselt palli suunas
            case POORA_PALLILE:

                // Kaotasime palli, lähme tagasi otsimise juurde
                if(pall == NULL) {
                    debug( "kaotasin palli" );
                    state = OTSI_PALLI;
                }
                if(pall != NULL){
                    if (halMath->palliKaugus(pall->rect.y) > KAUGEL) error = suurError;
                    if (halMath->palliKaugus(pall->rect.y) < KAUGEL) error = vaikeError;
                    // pall keskel, hakkame palli poole sõitma
                    if(abs(pall->distanceH) <= error) {
                        debug( "POORAN_PALLIELE: Pall keskel!" );
                        state = SOIDA_PALLINI;
                        stop();
                    }
                    else {
                        debug( "POORAN_PALLIELE: Pooran pallile" );
                        // pöörame palli suunas
                        poora_keskele(pall->distanceH);
                        debug( "POORAN_PALLIELE: Kaugus keskelt: "/* + pall->distanceH*/ );
                    }
                }
                break;

                //Sõidame palli suunas
            case SOIDA_PALLINI:

                if (pall != NULL) {
                    if (halMath->palliKaugus(pall->rect.y) > KAUGEL) error = suurError;
                    if (halMath->palliKaugus(pall->rect.y) < KAUGEL) error = vaikeError;
                    //Saime palli kätte, lähme väravat otsima
                    if (isBall()) {
                        stop();
                        //seina äärest eemale
                        if (analog[1] > 200 && analog[2] > 200){
                            omni(180, 150, 255, 0, 1);
                            viivitus(400);//TODO testida
                            stop();
                        }
                        // vasakul
                        else if(analog[1] > 200) {
                            omni(270, 150, 255, 0,1);
                            viivitus(400);
                            stop();
                        }
                        // paremal
                        else if(analog[2] > 200) {
                            omni(90, 150, 255, 0,1);
                            viivitus(400);
                            stop();
                        }
                        state = OTSI_VARAVAT;
                        //Pall on keskelt liiga kaugele läinud
                    } else if(pall->distanceH >= error) {
                        state = POORA_PALLILE;
                        //stop();
                    }
                    else if(pall->distanceH <= -error) {
                        state = POORA_PALLILE;
                        //stop();
                    }

                    else {
                        debug( "SOIDAN_PALLINI: Soidan palli poole" );
                        if (pall->distanceH > 0) {
                            palliSuund = -1;
                        }
                        else {
                            palliSuund = 1;
                        }
                        //std::cout << "palliSuund: " <<  pall->rect.y << std::endl;
                        soida_pallini(halMath->palliKaugus(pall->rect.y), palliSuund);

                    }
                }
                // Kaotasime palli silmist, lähme tagasi palli otsimise juurde
                else if(pall == NULL)
                {
                    state = OTSI_PALLI;
                }
                break;
                // Nüüd on pall käes, aeg väravat otsida
            case OTSI_VARAVAT:
                // Kaotasime palli, otsime uuesti
                if (isBall() == false)
                {
                    state = OTSI_PALLI;
                }
                // Värav leitud, pöörame keskele
                else if (goal != NULL)
                {
                    otsiStart = 0;
                    otsiStartE = otsiStart;
                    state = POORA_VARAVALE;

                }
                else if (goal == NULL)
                {
                    otsiStart = 1;
                    if (otsiStart == 1 && otsiStartE != 1) {
                        timer0.start();
                        otsiStartE = otsiStart;
                    }
                    debug( "OTSIN_VARAVAT: Otsin v2ravat!" );

                    // Pöörame ringi ja otsime väravat
                    otsi_obj();
                }

                break;

                // Pöörame roboti löömiseks värava keskkoha suunas
            case POORA_VARAVALE:
                debug( "POORAN:VARAVALE: V2rav n2ha,keeran!" );



                //Kaotasime palli, lähme uuesti palli otsima
                if (isBall() == false)
                {
                    state = OTSI_PALLI;
                }
                // Kaotasime värava silmist, otsime uuesti
                else if (goal == NULL)
                {
                    state = OTSI_VARAVAT;
                    // kui lähme väravat otsima/pöörama siis loomisState = 0
                    loomisState = 0;
                }
                if(goal != NULL){
                    //error = goal->rect.width * 1;
                    //POORAMISKIIRUS=POORAMISKIIRUS * 0.2;
		    keera (POORAMISKIIRUS * 0.3f);
                    if( loomisState == 0 )
                    {
                        error = goal->rect.width * 1.0; //110 // Kui pöörde pealt
                    }
                    else
                    {
                        // error = goal->rect.width * 0.5f;
                        error = goal->rect.width * 0.8f; //50
                        state = OTSI_VARAVAT;
                    }

                    // TODO: kauguse kontroll, mida kaugemal seda täpsemalt sihime
                    if (abs(goal->distanceH) < error)
                    {
                        //system("mplayer pumm.ogg& >> /dev/null");
                        //stop();
                        //vaata_pilti();
                        if (!pallEes){
                            // pall on olemas ja suunatud värava keskele, siis lööme väravale
                            debug( "POORAN:VARAVALE: loome palli!" );
                            loo();
                            loemeKaadrid();
                            state = OTSI_PALLI;
                        }
                        else
                        {
                            //tuleks teha detailsemalt, vaadata kas seina või teist robotit ei ole ees
                            // ja sellest sõltuvalt otsustatakse kuidas edasi käitutakse.
                            omni(90, 150, 255, 0 , 1);
                            viivitus(300);
                            stop();
                            viivitus(50);
                            debug( "POORAN:VARAVALE: teine pall ees!" );
                            //loo();
                           // loemeKaadrid();

                        }
                        loemeKaadrid();
                        // Kui pall läinud, siis kohe uut otsima
                        /*if (isBall() == false)*/

                    }
                    else
                    {
                        soiduKiirus = 0;
                        // kui lähme värava(t/le) otsima/pöörama siis loomisState = 0
                        loomisState = 0;
                        poora_keskele( goal->distanceH );
                    }
                }
                break;
        }
        naitaPilti();
    }
}


void Hal::naitaPilti() {
    if(valitud_varav == GOAL_RED)
        cvCircle(_img, cvPoint(5, 5), 5, CV_RGB(255,0,0), 5, CV_AA, 0);
    else if(valitud_varav == GOAL_BLUE)
        cvCircle(_img, cvPoint(5, 5), 5, CV_RGB(0,0,255), 5, CV_AA, 0);

    // save the image if debugging is enabled
    if( state != STOP ){
    	kaader++;
    	//std::cout << "pall: " << analog[0] << std::endl;
        halDebug->logi( DEBUG_FILE, DEBUG_VIDEO, _img, pall, analog, digital, pwms, state, kaader, timer1.elapsed() );
    }
    
    if( kaader % 3 == 0 ){
        kuvaPilt();
    }
}

void Hal::kuvaPilt() {
    if( DEBUG_IMAGE ) {
        view->show( _img );
    }
}

void Hal::loemeKaadrid(){
    for(int i = 0; i < 5 /*TODO siin võib lugeda 2 kaadrit*/; i++){
        _img = image->getFrame();
    }
}

void Hal::avoidWalls() {
    if (paremalEes() && vasakulEes()) {
        std::cout << "Paremal sein ees!" << std::endl;
    }
    if (vasakulEes() && !paremalEes()) {
        
    }
    if (paremalEes() && !vasakulEes()) {
        std::cout << "Paremal sein ees!" << std::endl;
    }
    
}

// Pöörame, kõiki mootoreid ühes suunas
void Hal::keera(int speed) {
    
    
    omni(0, 0, 255, speed, 1);
}

// Kõik mootorid seisma
void Hal::stop() {
    for (int i = 0; i < 3; i++) {
        setDcMotor(i, 512);
    }
    soiduKiirus = 0;
}


