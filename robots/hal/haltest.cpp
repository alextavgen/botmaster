#include "haltest.h"
int i = 0;
HalTest::HalTest()
{
    halMath = new HalMath();
}

void HalTest::go() {
    Config & conf = Config::getConfig();
    conf.setSendCmdEnabled(1);

    while(1) {

        switch(conf.keyS) {
        case 1: vaata_pilti();
            break;
        case 2: loomiseTest();
            break;
        case 3: anduriteTest();
            break;
        case 4: anduriteTest2();
            break;
        case 5: mootoriteTest();
            break;
        case 6: palliAndmeteTest();
            break;
        case 7: pallipealeKeeramiseTest();
            break;
        case 8: pallipealekeeramiseTest2();
            break;
        case 9: varavaAndmeteTest();
            break;
        case 0: pealeSoitmiseAndmeteTest();
            break;
        default:
            omni(0, 0, 0, 0, 1);
            break;
        }
    }
}

void HalTest::omni(float dirDeg, float velocityBody, int velocityMax, float velocityAngular, float battery) {

    halMath->omni( pwms, dirDeg, velocityBody, velocityMax, velocityAngular, battery);

    for (int i = 0; i < 3; i++) {
        setDcMotor(i, pwms[i]);
    }
}

void HalTest::stop() {
    for (int i = 0; i < 3; i++) {
        setDcMotor(i, 512);
    }
}

void HalTest::loo() {
    setDigital(0);
    viivitus(300);
    clearDigital(0);
}


void HalTest::vaata_pilti(){
    _img = image->getFrame();
    image->process(1, 0, 0, 0); // TODO only one type of object to find
    bool pallEes = 0;
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
                    if ((pall->rect.y) < (obj->rect.y))
                    {
                        pall = obj;
                    }
                }
            }
            //TODO värava valik realiseerida
            if (obj->type == GOAL_RED) {
                if (goal == NULL) goal = obj;
                if ((goal->area) < (obj->area)) goal = obj;
            }
        }
    }
}

void HalTest::mootoriteTest() {
    /*for (int i = 0; i < 3; i++) {
        setDcMotor(i, 100);
        viivitus(2000);
        setDcMotor(i, 0);
    }*/

    omni(0,100,255,0,1);
    viivitus(5000);
    omni(90,100,255,0,1);
    viivitus(5000);
    omni(270,100,255,0,1);
    viivitus(5000);
    stop();
}

void HalTest::anduriteTest() {
    // digital
    requestSensors();
    getSensorsResponse();
    std::cout<<"PALLI_ANDUR: " << analog[0] << std::endl;
	std::cout<<"EESANDUR_VASAK: " << analog[1] << std::endl;
	std::cout<<"EESANDUR_PAREM: " << analog[2] << std::endl;
	std::cout<<"VASAK_KYLG: " << analog[3] << std::endl;
	std::cout<<"PAREM_KYLG: " << analog[4] << std::endl;
	std::cout<<"KAUGUS_ANDUR: " << analog[5] << std::endl;
	std::cout<<"VARAV_VASAK: " << digital[6] << std::endl;
	std::cout<<"VARAV_PAREM: " << digital[7] << std::endl;
    viivitus(100);
}

void HalTest::anduriteTest2() {
    getAnalogs();
    if ( isBall() ) {
        std::cout << "PALL" << std::endl;
    }
    std::cout << "Eesandur vasak " << analog[1] << std::endl;
    std::cout << "Eesandur parem " << analog[2] << std::endl;

    if ( vasakulEes() ) {
        std::cout << "Vasakul sein ees!" << std::endl;
    }
    if ( paremalEes() ) {
        std::cout << "Paremal sein ees!" << std::endl;
    }
    if ( paremal() ) {
        std::cout << "Paremal sein!" << std::endl;
    }
    if ( vasakul() ) {
        std::cout << "Vasakul sein!" << std::endl;
    }
    if ( kaugelEes() ) {
        std::cout << "Sein ees!" << std::endl;
    }
}

void HalTest::loomiseTest() {
    // TESTID
    //int i = 0;
    //while(1){
    	//vaata_pilti();
    	i++;
    	std::cout << "ootame" << std::endl;
    	if( i % 50 == 0 ) {
        	std::cout << "Lööme" << std::endl;
        	loo();
    	}
    	//view->show( _img );
    //}
}

void HalTest::pallipealeKeeramiseTest() {
    int soiduKiirus = 255;
    //Keera palli peale ja stop!
    vaata_pilti();
    if( pall != 0 )
    {
        int kaugusKeskelt = pall->distanceH;
        int kiirus = kaugusKeskelt / 4.0f;

        //Ajutine
        if( soiduKiirus == 0 ) {
            kiirus = kaugusKeskelt / 1.5f;
        }

        if (kiirus < 50) kiirus = 50;
        if (kiirus > 80) kiirus = 80;
        if (kaugusKeskelt >= 30) {
            omni(0, soiduKiirus, 255, -kiirus, 1);
        } else if(kaugusKeskelt <= -30) {
            omni(0, soiduKiirus, 255, kiirus, 1);
        } else {
            omni(0, soiduKiirus, 255, 0, 1);
        }
        std:: cout << "Keskelt: " << kaugusKeskelt << std::endl;
        std::cout << "Y = " << pall->rect.y << std::endl;
        
        view->show( _img );
    }
}

void HalTest::pallipealekeeramiseTest2() {
    // Keeramis test 2
    vaata_pilti();
    if( pall != 0 )
    {
        int kaugusKeskelt = pall->distanceH;
        // selle valemi järgi kiirus muutub vahemikus 52 - 80
        int kiirus = 0.3f * abs(kaugusKeskelt) + 42;

        // Kui peaks juhtuma nii, siis ei lasta kiirust kunagi alla/üle lubatu
        //if (kiirus < MIN_KIIRUS) kiirus = MIN_KIIRUS;
        if (kiirus > MAX_KIIRUS) kiirus = MAX_KIIRUS;

        if (kaugusKeskelt >= 30) {
            omni(0, 0, 255, -kiirus, 1);
        } else if(kaugusKeskelt <= -30) {
            omni(0, 0, 255, kiirus, 1);
        } else {
            omni(0, 0, 255, 0, 1);
        }
        std:: cout << "Keskelt: " << kaugusKeskelt << std::endl;
        std::cout << "Y = " << pall->rect.y << std::endl;
    }
    view->show( _img );
}

void HalTest::palliAndmeteTest() {
    vaata_pilti();
    if( pall != 0 )
    {
        std::cout << "area = " << pall->area << std::endl;
        std::cout << "Y = " << pall->rect.y << std::endl;
        std::cout << "kaugus keskelt = " << pall->distanceH << std::endl;
        if ( isBall() ) {
            std::cout << "PALL EES "  << std::endl;
        }
        float y = (float)pall->rect.y;
        std::cout << "Kiirus = " << 20 + (1 / (y / 32)) * 150 << std::endl;
    }
    view->show( _img );
}

void HalTest::pealeSoitmiseAndmeteTest() {
    vaata_pilti();
    if( pall != NULL ) {
        std:: cout << "Keskelt: " << pall->distanceH << std::endl;
        int m = halMath->palliKaugus( pall->rect.y );
        std::cout << "kaugus = " << m << std::endl;
        std::cout << "kauguskeskelt = " << abs( pall->distanceH ) << std::endl;
        std::cout << "pooramiskiirus = " << 0.3f * abs( pall->distanceH ) + 42 << std::endl;
        std::cout << "Y = " << pall->rect.y << std::endl;
        std::cout << "pall area = " << pall->area << std::endl;
    }
    view->show( _img );
}

void HalTest::varavaAndmeteTest() {
    vaata_pilti();
    if( goal != NULL ) {
        std::cout << "värava laius(x)= " << goal->rect.width << std::endl;
    }
    view->show( _img );
}
