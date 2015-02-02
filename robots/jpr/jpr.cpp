#include "jpr.h"
#include <QTime>
#include <QDebug>

// * Method summary * //
// JprControl:
// omni(suund kraadides, kiirus, kiirusMax, sõitesuuna nurk) - onmiwheelide juhtimiseks
// omniBreak - omni break
// solenoid - solenoidi löömine
//
// JprImage:
// pall - Image::Object* leitud palli parameetrid
// pall->distanceH - palli kaugus keskpunktist pixlites
// pall->rect.width - 
// pall->rect.y - 
//
// JprMath:
// getYcm - palli kaugus robotist - cm

using namespace std;

Jpr::Jpr():conf(Config::getConfig()) {
    //jprMath = new JprMath();
    jprStatClass = new JprStatClass();
    jprControl = new JprControl(this);

    /** roboti default state on edasisõit */
    searchBall=ss_forward;
    aimStartTime = 0;

    /* palli viimane suund */
    lastSeen = 0;

    stop = 1;

    fpsTime = 1;
    fpsFrameCount = 1;

    curCommandString = "";
    sleepTime = -1;
    sleeper.start();

    pallEesTime.start();

    pallEesChange = 0; // 0 eelmine olek on teadmatta, 1 ei olnud ees, 2 oli ees;
    pallChange = 0;
    varavChange = 0;
    kiirus_x = 0;

    soidaPallileAeg.start();
}

Jpr::~Jpr() {

    cout<<"destructing jpr"<<endl;
    delete jprControl;
    //delete jprMath;
    delete jprStatClass;
    if(logFile.is_open()){
        if(!qstl.isEmpty()){
            logFile<<qstl.join("\n").toStdString()<<endl;
        }
        logFile.close();
    }

}

void Jpr::finishDebug(){
    if(JPR_DEBUG){
        if(cvVW){
            cvReleaseVideoWriter(&cvVW);
            cvVW = NULL;
        }
        if(logFile.is_open()){
            logFile.close();
        }
    }
}

void Jpr::startDebug(){
    if(JPR_DEBUG){
        QString logName("Debug/log_");
        QString vidName("Debug/vid_");
        logName += QString(curTime().c_str()) + QString(".csv");
        vidName += QString(curTime().c_str()) + QString(".avi");
        if(!logFile.is_open()){
            logFile.open (logName.toStdString().c_str());
        }
        if(!cvVW){
            //        CV_FOURCC('P','I','M','1')    = MPEG-1 codec
            //        CV_FOURCC('M','J','P','G')    = motion-jpeg codec (does not work well)
            //        CV_FOURCC('M', 'P', '4', '2') = MPEG-4.2 codec
            //        CV_FOURCC('D', 'I', 'V', '3') = MPEG-4.3 codec
            //        CV_FOURCC('D', 'I', 'V', 'X') = MPEG-4 codec
            //        CV_FOURCC('U', '2', '6', '3') = H263 codec
            //        CV_FOURCC('I', '2', '6', '3') = H263I codec
            //        CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec
            //cvVW = cvCreateVideoWriter(vidName.toStdString().c_str(), CV_FOURCC('P','I','M','1'), 24, cvSize(320,240));
            cvVW = cvCreateVideoWriter(vidName.toStdString().c_str(), CV_FOURCC('D','I','V','X'), 24, cvSize(320,240));
        }
    }

}

/** funktsioon koha peal antud kiirusel antud aja pööramiseks, pärast on robot edasisõidus */
void Jpr::doCircle (int aeg, int speed){
    curCommandString = QString("doCircle(%1,%2)").arg(aeg).arg(speed).toStdString();
    if (time.elapsed()<aeg){
        //if(!(((jprControl->pwm[0])==speed) && ((jprControl->pwm[1])==speed) && ((jprControl->pwm[2])==speed))){
            for(int i=0;i<3;i++){
                jprControl->pwm[i]=speed;
                setDcMotor(i, speed);
            }
        //}
        /*setDcMotor(0, speed);
		setDcMotor(1, speed);
		setDcMotor(2, speed);*/
    } else {
        searchBall=ss_forward;
        time.restart();
    }
} 

/** tuvastatud värava värvi kindlaks tegemine?*/
Image::Object* Jpr::getGoal() {
    Image::Object* g = NULL;
    Config  & conf  = Config::getConfig();
    if(conf.keyS==0) {
        goalDir = imgGoalBlueDir; g = goalBlue;
    }
    else if (conf.keyS==1){
        goalDir = imgGoalRedDir; g = goalRed;
    }
    /*if(g!=NULL){
        // 7 - vasak, 6 - parem
        if(digital[7] && !digital[6]){
            //goalDir = 1;
            cout<<"v"<<endl;
        }else if(!digital[7] && digital[6]){
            //goalDir = -1;
            cout<<"p"<<endl;
        }
    }*/
    return g;
}

/** sõidab valitud pallile */
void Jpr::soidaPallile() {
    curCommandString = "soidaPallile()";
    if(JPR_NAEB_PALLI){
        //lastSeenBall=time.elapsed();
        time.restart();
        searchBall=ss_searchCircle;
        char m = 1;
        int kiirus_z;

        int s_vasak = analog[0];
        int s_parem = analog[1];

        if((pall->distanceH) < 0){
            m = -1;
        }
        kiirus_z = (abs(pall->distanceH) / 2.5);

        float coef = pall->distanceH/150;

        kiirus_x = pall->rect.y;

        if(kiirus_x > 150) kiirus_x = 150;
        kiirus_x = 150 - kiirus_x + 100;//70

        //cout<<kiirus_x<<" ja "<<kiirus_z<<" ja "<<pall->distanceH<<" ning "<<pall->rect.y<<endl;
        //if((kiirus_z+kiirus_x)>255)kiirus_x-=kiirus_z;
        //cout<<"Kaugus:"<< getYcm(pall) <<endl;
        if ((s_vasak>500) && (s_parem>550)){
          jprControl->solenoid();
        }
        else if(abs(pall->distanceH) > 100){

            if((s_vasak > 220) && (pall->distanceH < 0)){
                jprControl->omni(90, 120, 255, 10);
            } else if ((s_parem > 300) && (pall->distanceH > 0)){
                jprControl->omni(-90, 120, 255, 10);
            } else {
                jprControl->omni(0, kiirus_x, 255, m * (kiirus_z - (30 )));
            }

        }else if(getYcm(pall) <= 7 && abs(pall->distanceH) < 60){
              jprControl->omni(0, 90, 255, 0);
              doNothing(400);

        } else if(getYcm(pall) < 10 ){

            if (kiirus_z<70)
                kiirus_z=70;

            if((s_vasak > 220) && (s_parem < 100)){
                jprControl->omni(90, 120, 255, 35);
            } else if ((s_parem > 300) && (s_vasak < 100)){
                jprControl->omni(-90, 120, 255, 35);
            } else {
                kiirus_x=70;
                jprControl->omni(0, kiirus_x, 255, m * (kiirus_z - (30)));
            }
            /*setDcMotor(0, -1*m * kiirus_z*0.8);
			setDcMotor(1, -1*m * kiirus_z*0.8);
			setDcMotor(2, -1*m * kiirus_z*0.8); */

        } else {
            jprControl->omni(0, kiirus_x, 255, -m*kiirus_z); //((float)abs(kiirus_x))*coef/2
        }

        /* if(getYcm(pall) <= 7 && abs(pall->distanceH) < 60){
                        jprControl->omni(0, 80, 255, 0);
                        //usleep(400000);
                } else if(getYcm(pall) < 10 && abs(pall->distanceH) > 60){
                        if (kiirus_z<70) kiirus_z=70;
                        setDcMotor(0, -1*m * kiirus_z*0.8);
                        setDcMotor(1, -1*m * kiirus_z*0.8);
                        setDcMotor(2, -1*m * kiirus_z*0.8);
                } else {

                        jprControl->omni(0, kiirus_x, 100, -m * kiirus_z);
                }

                if(getYcm(pall) > 20){
                        kiirus_x = 150;
                } else {
                        kiirus_x = 180;
                }

                if (kiirus_z > 130) {
                        kiirus_z = 130;
                }

                if(getYcm(pall) < 14 && abs(pall->distanceH) > 60){
                        setDcMotor(0, m * kiirus_z);
                        setDcMotor(1, m * kiirus_z);
                        setDcMotor(2, m * kiirus_z);
                } else {
                        // nurk(otse 0, k[lg ees 90),kiirus, skaala, p66ramise nurk
                        jprControl->omni(0, kiirus_x, 255, m * (kiirus_z - 30));
                }
*/
    }
}

void Jpr::soidaPallile3() {
 curCommandString = "soidaPallile3()";
  if (JPR_NAEB_PALLI){
        time.restart();
        searchBall=ss_searchCircle;
        
     
        int s_vasak = analog[0];
        int s_parem = analog[1];
        
        int m = -(s_parem > 300) + (s_vasak > 220);
               
  
        double kaugus=getYcm(pall);
/*
        if (kaugus<25) {
          kiirus_x=120;
        } else if(kaugus>40){
          kiirus_x=255;
        }*/

        if((kiirus_x == 255) && (kaugus < 35)){
          jprControl->omniBreak();
          usleep(10*1000);          
          kiirus_x = 120;
        }else if(kaugus<35) {kiirus_x=120;}
        else {kiirus_x=255;}

        if(kiirus_x == 0)kiirus_x = 120;

        double nurk=getDegree(pall);
        
        if ((s_vasak>220) || (s_parem>300)) { 
          //seina ääres
          jprControl->omni(m*90, kiirus_x, 255, 35);
          if ((s_vasak>220) && (s_parem>300)) {
            //nurgas
            jprControl->solenoid();
          }
        } else {
            //keset väljakut
            jprControl->omni(0,kiirus_x,255,-nurk);
        }
  }
}


/** sõidab valitud pallile */
void Jpr::soidaPallile2() {
    curCommandString = "soidaPallile2()";
    if(JPR_NAEB_PALLI){
        stop = 1;
        time.restart();
        searchBall=ss_searchCircle;

        char m = 1;
        int kiirus_z;
        int kiirus_z_slow;
       
        int aku_konst = 0;
        int seina_konst = 0;

        if((pall->distanceH) > 0){
            m = -1;
        }

        lastSeen = m;
        //http://www.mathsisfun.com/straight-line-graph-calculate.html
        // K: 80 - DH: 125
        // K: 40 - DH: 0
        kiirus_z_slow = (abs(pall->distanceH) / 3) + 38.3;
        // K: 45 - DH: 25
        // K: 90 - DH: 125
        kiirus_z = (0.45 * abs(pall->distanceH)) + 33.75;
        //cout<<kiirus_z<<" :: "<< pall->distanceH <<endl;

        if(getYcm(pall) > 15){
            kiirus_x = 205;
        } else {
            kiirus_x = 155;
        }

        if (kiirus_z > 130) {
            kiirus_z = 130;
        }

        if(getYcm(pall) < 10 && abs(pall->distanceH) > 60){
            setDcMotor(0, m * (kiirus_z_slow - 5));
            setDcMotor(1, m * (kiirus_z_slow - 5));
            setDcMotor(2, m * (kiirus_z_slow - 5));
        } else {
            int s_vasak = analog[0];
            int s_parem = analog[1];

            //cout<<"vasak:"<< s_vasak <<endl;
            //cout<<"parem:"<< s_parem <<endl;
            //cout<<"kaugus:"<< getYcm(pall) <<endl;
            //cout<<"h:"<< pall->distanceH <<endl;

            if(abs(pall->distanceH) > 100 && getYcm(pall) < 10){
                if((s_vasak > 220) && (pall->distanceH < 0)){
                    jprControl->omni(90, 120, 255, 10);
                } else if ((s_parem > 300) && (pall->distanceH > 0)){
                    jprControl->omni(-90, 120, 255, 10);
                } else {
                    jprControl->omni(0, kiirus_x, 255, m * (kiirus_z - (30 + aku_konst)));
                }
            } else if(getYcm(pall) < 10) {
                if((s_vasak > 220) && (s_parem < 100)){
                    jprControl->omni(90, 120, 255, 35);
                } else if ((s_parem > 300) && (s_vasak < 100)){
                    jprControl->omni(-90, 120, 255, 35);
                } else {
                    //kiirus_x = 70;
                    jprControl->omni(0, kiirus_x, 255, m * (kiirus_z - (30 + aku_konst)));
                }
            } else if (getYcm(pall) <= 7 && abs(pall->distanceH) < 60){
                doNothing(250);
                //usleep(500000);
            } else {
                jprControl->omni(0, kiirus_x, 255, m * (kiirus_z - (30 + aku_konst)));
            }
        }
    } else if(JPR_PALL_EES) {
        if(stop){
            jprControl->omniBreak();
            usleep(500000);
            stop = false;
        }
        setDcMotor(0, lastSeen * 70);
        setDcMotor(1, lastSeen * 70);
        setDcMotor(2, lastSeen * 70);

    } else {
        stop = 1;
        setDcMotor(0, lastSeen * 70);
        setDcMotor(1, lastSeen * 70);
        setDcMotor(2, lastSeen * 70);
    }
}


/** sensorite väärtuste väljatrükk */
void Jpr::printSensors() {
    for (int y=0;y<8;y++){
        cout<<"analog "<<y<<": "<<analog[y]<<endl;
    }
    for (int y=0;y<8;y++){
        cout<<"digital "<<y<<": "<<digital[y]<<endl;
    }
}

int Jpr::fps(){
    return (1000/frameLength());
    //    int ms = gameTime.elapsed()-fpsTime;
    //    int fc = frameCount-fpsFrameCount;
    //    if(ms>1000){
    //        fpsTime = gameTime.elapsed();
    //        fpsFrameCount = frameCount;
    //    }
    //    return fc/((float)ms/1000.0);
    //    //return frameCount/(gameTime.elapsed()/1000.0);
}

string Jpr::logHeader(){
    return "frame;elapsed;time;fps;skip;curCommand;solTime;0;1;2;3;4;5;6;7;digitals;objCount;goal;type;area;center.x;center.y;rect.width;ball;area;center.x;center.y;rext.x;rect.y;rect.width;distanceH;pwm0;pwm1;pwm2;";
}

void Jpr::logState(){
    // uuendada!
    QString qst = QString("%1;%2;%3;").arg(frameCount).arg(gameTime.elapsed()).arg(gameTime.elapsed()-lastFrameTime);
    qst.append(logSensors());
    qst.append(logGoal());
    qst.append(logBall());
    qst.append(logPWM());
    qst.append("\n");
    qstl<<qst;
    // TODO: stringlsti lisamine ja programmi lõpus faili salvestamine
}

void Jpr::logState2(){
    if(SAVE_LOG){
        if(logFile.is_open()){
            logFile<<frameCount<<";";
            logFile<<gameTime.elapsed()<<";";
            logFile<<gameTime.elapsed()-lastFrameTime<<";";
            logFile<<fps()<<";";
            logFile<<this->imgFrameSkipCount<<";";
            logFile<<this->curCommandString<<";";
            logFile<<this->jprControl->solTime.elapsed()<<";";

            logSensors2();

            logFile<<this->image->found_objects->total<<";";

            logGoal2();
            logBall2();
            logPWM2();

            logFile<<this->gyro.deg()<<";";

            logFile<<endl;


        }
    }
}

QString Jpr::logSensors(){
    QString ret;
    for(int i=0;i<8;i++){
        ret.append(QString("%1;").arg(analog[i]));
    }
    ret.append("_");
    for(int i=0;i<8;i++){
        ret.append(QString("%1").arg(digital[i]));
    }
    ret.append(";");
    return ret;
}

void Jpr::logSensors2(){
    for(int i=0;i<8;i++){
        logFile<<analog[i]<<";";
    }
    logFile<<"_";
    for(int i=0;i<8;i++){
        logFile<<digital[i];
    }
    logFile<<";";
}

QString Jpr::logGoal(){
    QString ret = QString("%1;").arg(JPR_NAEB_VARAVAT);
    if(JPR_NAEB_VARAVAT){
        Image::Object* g= getAnyGoal();
        ret.append(QString("%1;%2;%3;").arg(g->type).arg(g->area).arg(g->center.x).arg(g->center.y).arg(g->rect.width));
    }else{
        ret.append("-;-;-;-;-;");
    }
    return ret;
}

void Jpr::logGoal2(){
    logFile<<JPR_NAEB_VARAVAT<<";";
    if(JPR_NAEB_VARAVAT){
        Image::Object* g= getAnyGoal();
        logFile<<g->type<<";"<<g->area<<";"<<g->center.x<<";"<<g->center.y<<";"<<g->rect.width<<";";
    }else{
        logFile<<"-;-;-;-;-;";
    }
}

QString Jpr::logBall(){
    QString ret = QString("%1;").arg(JPR_NAEB_PALLI);
    if(JPR_NAEB_PALLI){
        Image::Object* g= pall;
        ret.append(QString("%1;%2;%3;%4;%5;").arg(g->area).arg(g->center.x).arg(g->center.y).arg(g->rect.x).arg(g->rect.y).arg(g->rect.width).arg(g->distanceH));
    }else{
        ret.append("-;-;-;-;-;-;-;");
    }
    return ret;
}

void Jpr::logBall2(){
    logFile<<JPR_NAEB_PALLI<<";";
    if(JPR_NAEB_PALLI){
        Image::Object* g= pall;
        logFile<<g->area<<";"<<g->center.x<<";"<<g->center.y<<";"<<g->rect.y<<";"<<g->rect.y<<";"<<g->rect.width<<";"<<g->distanceH<<";";
    }else{
        logFile<<"-;-;-;-;-;-;-;";
    }
}

QString Jpr::logPWM(){
    QString ret;
    for(int i=0; i<3; i++){
        ret.append(QString("%1;").arg(jprControl->pwm[i]));
    }
    return ret;
}

void Jpr::logPWM2(){
    for(int i=0; i<3; i++){
        logFile<<jprControl->pwm[i]<<";";
    }
}


/** väldib seinu, sõidab väljakul ringi */
void Jpr::avoidObstacles(){
    curCommandString = QString("avoidObstacles():%1").arg(searchBall).toStdString();
    int kiirus_t=65;

    if (searchBall==ss_searchCircle){
        doCircle(3000,imgPallDir*kiirus_t);//kiiruses hoiab meeles viimati nähtud lähimat palli...
    }
    else if (searchBall==ss_avoidWall){
        doCircle(1400,kiirus_t);
    }
    else if (searchBall==ss_avoidWallRight){
        doCircle(1000,kiirus_t);
    }
    else if (searchBall==ss_avoidWallLeft){
        doCircle(1000,-kiirus_t);
    }
    else if (searchBall==ss_avoidGoal){
        doCircle(1000,kiirus_t);
    }
    else if (searchBall==ss_forward){
        kiirus_t=150;
        if (time.elapsed()<1000){

            if (JPR_SEIN_EES){
                searchBall=ss_avoidWall;
                time.restart();
            }
            else if(JPR_SEIN_VASAKUL){
                searchBall=ss_avoidWallRight;
                time.restart();
            }
            else if(JPR_SEIN_PAREMAL){
                searchBall=ss_avoidWallLeft;
                time.restart();
            } //sein taga?
            else{
                jprControl->omni(0, kiirus_t, 255, 0);
            }
        } else { searchBall=ss_searchCircle;
            time.restart(); }
    }
}

bool Jpr::waitForBallToLeave(int ms){
    //curCommandString = QString("waitForBallToLeave(%1)").arg(ms).toStdString();
    if((ms < pallEesTime.elapsed()) && (pallEesChange == 1)) return true;
    if(JPR_PALL_EES)return true;
    return false;
}

void Jpr::pallEes() {
    curCommandString = "pallEes()";
    int v_kiirus=54; //kiirus millega pöörata kui juba näeb väravat aga ei ole veel täpselt
    int s_vasak = analog[0];
    int s_parem = analog[1];
    //TODO: seina ja väravaInfra anduritega arvestamine lisada!!!

    
    if (JPR_PALL_EES){
        if ((s_vasak>350) && (s_parem>400)){
          jprControl->omni(0,-150,255,0);
        }
        else if(pallEesChange == 2){        
            ballTime.restart();//esimene kord pall käes, reseti aeg
            jprControl->omni(0,90,255,0);
            doNothing(20);
            //cout<<"gotit"<<endl;
        }else
        if(JPR_NAEB_VARAVAT){
            ballTime.restart();            
            //if((jprControl->pwm[0])>v_kiirus){ //kontroll selleks kui kiiremast pööramisest minnakse üle aeglasemale et kiirusevahetus mõjuks
            if(varavChange == 2){
                jprControl->omniBreak();
                //doNothing(61);
            }else
                // kui värava posti juures
                // UUS ASI
                if(JPR_PAREM_IR > 300){
                jprControl->omni(120, 150, 255, 0);
            }else
                if(JPR_VASAK_IR > 300){
                jprControl->omni(-120, 150, 255, 0);
            }else
            //värava piiridesse satub, jääb korraks seisma ja tulistab kui pall veel alles on
            if (getGoal()->rect.width<40){ //kui värav on liiga väike, sõida sellele lähemale
                //cout<<"värav auu!"<<endl;
                jprControl->omni(0, 200, 255, -getGoal()->distanceH/3);
                // UUS ASI
                if(JPR_PAREM_IR > 150){
                    jprControl->omni(45, 200, 255, 0);
                    doNothing(100);
                }else
                    if(JPR_VASAK_IR > 150){
                    jprControl->omni(-45, 200, 255, 0);
                    doNothing(100);
                }
                aimStartTime = 0;
            }
//            else if ((img->width/2) > ((getGoal()->rect.x+getGoal()->rect.width)-((getGoal()->rect.x+getGoal()->rect.width)*0.1))){ //värava parempoolne äär, pööra vasakule
//                jprControl->omniN(0,0.1,1,v_kiirus);
//                aimStartTime = 0;
//                //cout<<"paremalt üle"<<endl;
//            }
//            else if ((img->width/2) < (getGoal()->rect.x+(getGoal()->rect.x*0.1))){ //värava vasakpoolne äär, pööra paremale
//                jprControl->omniN(0,0.1,-1,v_kiirus);
//                aimStartTime = 0;
//                //cout<<"vasakult üle"<<endl;
//            }
            else if (imgValidKickPath!=0) {
                //cout<<"pall ees"<<endl;
                jprControl->omni(imgValidKickPath*45, 140, 255, 0);//TODO kavalam pallist mööda hiilimine
                // TODO: vb pöörata samal jal ka väravale getGoal()->distanceH ja drifti nurk suuremaks
                aimStartTime = 0;
            }else
            if(((getGoal()->rect.width/2.5) - abs(getGoal()->distanceH))>0) { //sihik on väraval, löö pall
                if(aimStartTime == 0)aimStartTime = gameTime.elapsed();
                if((gameTime.elapsed()-aimStartTime)<100){
                    jprControl->omniBreak();
                    // kontroll, et on keskel olnud vähemalt kaks kaadrit;
                }else{
                    jprControl->solenoid();
                    jprControl->omniBreak();
                    doNothing(50);
                    time.restart();
                    aimStartTime = 0;
                }
            }else{
                jprControl->omniN(0,0.1,-(getGoal()->distanceH),v_kiirus);
                aimStartTime = 0;
            }
        }else{ // ei näe väravat, otsime

            if (ballTime.elapsed()<4000){//mingi aeg otsib väravat pöörates, siis peab nt teise värava poole sõites väljaku keskele poole liikuma
                jprControl->omniN(0,0.1,goalDir,80);//TODO: pöörame kuhu värava infrad näitavad
            }
            else if(ballTime.elapsed()<7500){
                //pööra vastase väravale ja sõida otse edasi..keskpunkti suunas
                if(JPR_NAEB_SUVALIST_VARAVAT){   
                    //if (getAnyGoal()->rect.width<40){ //kui värav on liiga väike, sõida sellele lähemale
                    //cout<<"värav auu!"<<endl;
                    jprControl->omni(0, 150, 255, -getAnyGoal()->distanceH/3);
                    //}
                }
                else{
                    jprControl->omniN(0,0.1,-goalDir,80);
                }
            }else{
                ballTime.restart();
            }
        }
    }
}


/** roboti käitumine,  */
void Jpr::play(){
    if(!JPR_NAEB_PALLI){
        soidaPallileAeg.restart();
        imgPallDirIgnore = 0;
    }

    if(sleeper.elapsed()<sleepTime){
        curCommandString = QString("doNothing=%1_%2").arg(sleeper.elapsed()-sleepTime).arg(sleepTime).toStdString();
    }else if(JPR_PALL_EES){
        pallEes();
    }else if(JPR_NAEB_PALLI){
        aimStartTime = 0;
        imgPallDirIgnore = 0;
        if(soidaPallileAeg.elapsed() < 8000){
            soidaPallile();
        }else{
            jprControl->omni(0,-200,255,80);
            imgPallDirIgnore = 1;
            doNothing(601);
        }
        ballTime.restart();
    }
    else{
        //ei näe palli,  on vaja otsida
        avoidObstacles();
    }    
}

/** penaltikood- kiire edasisõit ja pallile sättimine*/
void Jpr::penalt(){
    curCommandString = "penalt()";
    if (gameTime.elapsed()<1450){
        jprControl->omni(0, 220, 255, 0);
    } else if (gameTime.elapsed()<1900){
        jprControl->omni(0, 80, 255, 0);
    } else{
        if(JPR_NAEB_PALLI){
            soidaPallile();
        } else if (JPR_PALL_EES){
            pallEes();
        }
        else  avoidObstacles();//jprControl->omniBreak();
    }
}

void Jpr::doNothing(int duration){
    sleepTime = duration;
    sleeper.restart();
}

void Jpr::waitForStart(){
    imgShowImages = true;

    finishDebug();
    startDebug();

    conf.setSendCmdEnabled(0);
    do
    {
        requestSensors();
        processImage();// pilditöötlus - tagastab leitud palli parameetrid
        getSensorsResponse();
        gyro.collectCenter(analog[JPR_GYRO_ANALOG]);
        showGoal(conf.keyS); //0-sinine värav, 1-punane värav
        // näitab pilti ekraanil
        showImage(); // peab olema viimane
    }
    while (conf.getSendCmdEnabled()==0);
    gyro.calcCenter();
    finishDebug();
    startDebug();
    cout<<"start"<<endl;
    imgShowImages = false || JPR_SHOW_IMAGE;
}

void Jpr::getStateChange(bool cond, char &val){
    if(cond){
        if((val == 0) || (abs(val) == 1) ){
            val = 2;
        }else if(val == 2)val = -2;
    }else{
        if((val == 0) || (abs(val) == 2) ){
            val = 1;
        }else if(val == 1)val = -1;
    }
}

void Jpr::getStateChange(bool cond, char &val, QTime &t){
    getStateChange(cond, val);
    if(val > 0)t.restart();
}

void Jpr::checkStateChanges(){
    getStateChange(JPR_PALL_EES, pallEesChange, pallEesTime);
    getStateChange(JPR_NAEB_VARAVAT, varavChange);
    getStateChange(JPR_NAEB_PALLI, pallChange);
    //    if(JPR_PALL_EES){
    //        if((pallEesChange == 0) || (abs(pallEesChange) == 1) ){
    //            pallEesChange = 2;
    //            pallEesTime.restart();
    //        }else if(pallEesChange == 2)pallEesChange = -2;
    //    }else{
    //        if((pallEesChange == 0) || (abs(pallEesChange) == 2) ){
    //            pallEesChange = 1;
    //            pallEesTime.restart();
    //        }else if(pallEesChange == 1)pallEesChange = -1;
    //    }

    //    if(JPR_NAEB_VARAVAT){
    //        if((varavChange == 0) || (abs(varavChange) == 1)){
    //            varavChange = 2;
    //        }else if(varavChange == 2)varavChange = -2;
    //    }else{
    //        if((varavChange == 0) || (abs(varavChange) == 2) ){
    //            varavChange = 1;
    //        }else if(varavChange == 1)varavChange = -1;
    //    }

    //    if(JPR_NAEB_PALLI){
    //        if((pallChange == 0) || (abs(pallChange) == 1)){
    //            pallChange = 2;
    //        }else if(pallChange == 2)pallChange = -2;
    //    }else{
    //        if((pallChange == 0) || (abs(pallChange) == 2) ){
    //            pallChange = 1;
    //        }else if(pallChange == 1)pallChange = -1;
    //    }
}

void Jpr::go() {	
    frameCount = 0;
    //	umbes 65-st alates hakkab liikuma, testimiseks
    imgPallDir = 1;

    qDebug("JP GO...\n");	

    conf.setSendCmdEnabled(1);

    /** algväärustame palli andurid enne kui kontrollerist midagi loeme (et alguses kohe laksu ei saaks)*/
    analog[2]=1024;
    analog[3]=1024;
    jprControl->solTime.restart();

    jprControl->omniBreak();

    // valgusti välja
    //setDigital(0);

    // rull sisse
    jprControl->reel(1);


    //conf.setSendCmdEnabled(0);
    time.start();
    gameTime.start();
    ballTime.start();
    camReset.start();

    waitForStart();

    //jprControl->solenoid();
    time.restart();
    gameTime.restart();
    ballTime.restart();
    camReset.restart();
    lastFrameTime = 0;
    if(SAVE_LOG){
        logFile<<logHeader()<<endl;
    }
    
    while(1){
        jprControl->checkSolenoid();
        this->curCommandString = "##nothing";
        frameCount++;

        requestSensors();// annab mikrokontrollerile käsu andurite lugemiseks

        processImage();// pilditöötlus - leiab pallid ja väravad

        getSensorsResponse();// loeb kirjutab anturite väärtused analog ja digital massiivi
        gyro.update(analog[JPR_GYRO_ANALOG]);
        //penalt();
        getGoal();
        checkStateChanges();
        play();

        //soidaPallile2();

        //printSensors(); //andurite väärtuste väljatrükk
        logState2();
        showGoal(conf.keyS); //0-sinine värav, 1-punane värav

        lastFrameTime = gameTime.elapsed();
        // näitab pilti ekraanil
        showImage(); // peab olema viimane

        //peab olema peale pildi näitamist, muidu tuleb error
        if(image->fps < 10){ //image->fps
            if (camReset.elapsed()>2000){
                image->reOpen();
                qDebug("FPS is too slow...reopening Camera!!! (%i)", frameCount);
                camReset.restart();
            }
        }

        if(!JPR_PALL_EES){
            jprControl->headLight((analog[0]>300) && (analog[1]>300));
        }

        /*if (gameTime.elapsed()>=92000) {
            cout<<"aeg läbi"<<endl;
            jprControl->omniBreak();
            //exit(0);
            waitForStart();
            gameTime.restart();
        }*/

    }
}


