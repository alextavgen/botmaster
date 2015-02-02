#include "jprtest.h"
#include <QDebug>

JprTest::JprTest()
{
    kiirus = 50;
}

/** sõidumootorite test, järjest iga mootor edasi ja tagasi sõidus */
void JprTest::motorTest() {
// mootor vasakul ees
                cout<<"mootor0: vasakul, 50 ja -50"<<endl;
                setDcMotor(0, 50);//paneme mootori poorlema
                usleep(1000000);//ootame paar sekundit
                setDcMotor(0, 512);//pidurdame
                usleep(500000);//ootame paar sekundit
                setDcMotor(0, -50);//paneme mootori poorlema
                usleep(1000000);//ootame paar sekundit
                setDcMotor(0, 512);//pidurdame

                // mootor paremal ees
                cout<<"mootor1: paremal, 50 ja -50"<<endl;
                setDcMotor(1, 50);//paneme mootori poorlema
                usleep(1000000);//ootame paar sekundit
                setDcMotor(1, 512);//pidurdame
                usleep(500000);//ootame paar sekundit
                setDcMotor(1, -50);//paneme mootori poorlema
                usleep(1000000);//ootame paar sekundit
                setDcMotor(1, 512);//pidurdame

                // mootor taga
                cout<<"mootor2: taga, 50 ja -50"<<endl;
                setDcMotor(2, 50);//paneme mootori poorlema
                usleep(1000000);//ootame paar sekundit
                setDcMotor(2, 512);//pidurdame
                usleep(500000);//ootame paar sekundit
                setDcMotor(2, -50);//paneme mootori poorlema
                usleep(1000000);//ootame paar sekundit
                setDcMotor(2, 512);//pidurdame
}

void JprTest::solenoidiTest0(){
    // solenoidi test, pallilöömise test
    // kui kaameras on pall, siis lööb juhul, kui pall on löögiteekonna ees ja sealt ära pöörata
    // kui palli näha pole, siis lööb 1 sek peale palli ette andmist rullile
    // või teha kaks eraldi testi?
    cout<<frameCount<<"_"<<isValidKickPath2()<<endl;
    jprControl->omniBreak();
    if(JPR_PALL_EES){
        if(JPR_NAEB_PALLI){
            cout<<pall->rect.y<<endl;
            if(isValidKickPath2()){
                jprControl->solenoid();
                cout<<"kick1"<<endl;
                flag = false;
            }
        }else{
            if(!flag){
                flag = true;
                testTime.restart();
            }
            if(flag && (testTime.elapsed()>1000)){
                jprControl->solenoid();
                cout<<"kick2"<<endl;
                flag = false;
            }
        }
    }
}

void JprTest::rulliTest0(){
    //rulliga palli hoidmise test
    // kiirendab aeglaselt
    // täisakuga 106, tühjema akuga 120, rulli-aku ei mõjutanud tulemust
    imgPallDir+=1;
    jprControl->omniN(0,0,1,imgPallDir/4);
    if(!JPR_PALL_EES){
        jprControl->omniBreak();
        if(flag){
            cout<<imgPallDir/4<<endl;
        }
        imgPallDir = 160;
        flag = false;
    }else flag = true;
}

void JprTest::rulliTest1(){
    //natuke ekstreemsem rulliga palli hoidmise test (not)
    // täisakuga 157 (min), tühjema akuga 194
    imgPallDir+=1;
    jprControl->omniN(1,0,1,imgPallDir);
    if(!JPR_PALL_EES){
        jprControl->omniBreak();
        if(flag){
            cout<<imgPallDir<<endl;
        }
        imgPallDir = 40;
        flag = false;
    }else flag = true;
}

void JprTest::runTest(int i){
    switch(i){
    case 0: testing(); break; //väikesed testid
    case 1: solenoidiTest0(); break;
    case 2: rulliTest0(); break;
    case 3: rulliTest1(); break;
    }
}

/** funktsioon igast väikeste testide tegemiseks */
void JprTest::testing() {
        //kiiruste testimine
        if (0) {
                // nurk(otse 0, k[lg ees 90),kiirus, max kiirus, p66ramise nurk
                for (int i=0;i<255;i+=5) {
                        jprControl->omni(0, 255, 255, 0);
                        usleep(500000);
                        cout<<i<<endl;
                }
        }
        //teine 8 sõit, kiirem
        if (0) {
                jprControl->omni(0, 150, 255, 60);
                usleep(2000000);
                //jprControl->omniBreak();
                //usleep(2000000);
                jprControl->omni(0, 150, 255, -60);
                usleep(2000000);
                //jprControl->omniBreak();
                //usleep(2000000);
        }
        // ümber palli test
        if(0){
                jprControl->setCalcCenterPoint(0,0);
                int kiirus_x, kiirus_w;
                if(JPR_NAEB_PALLI){
                        kiirus_x = 100-pall->rect.y;
                        kiirus_w = pall->distanceH;
                        if(abs(kiirus_w)<10)kiirus_w=0;
                        cout<<kiirus_x<<"_"<<kiirus_w<<endl;
                        //jprControl->omniN(-kiirus_x,0,-kiirus_w/50.0,80);
                        jprControl->omniN(1,0,0.5,100);
                }else{jprControl->omniBreak();};
        }

        //uue omni test. not
        if (0){
                //jprControl->setCalcCenterDegree(0.5,60);
                jprControl->setCalcCenterPoint(3,0);
                cout<<"omniN"<<endl;
                jprControl->omniN(0,0,1,100);
                cout<<jprControl->getWheelSpeed(0)<<"_"<<jprControl->getWheelSpeed(1)<<"_"<<jprControl->getWheelSpeed(2)<<endl;
                //cout<<"D "<<jprControl->centerDistanceW(0)<<"_"<<jprControl->centerDistanceW(1)<<"_"<<jprControl->centerDistanceW(2)<<"_"<<jprControl->centerDistanceW(3)<<endl;
                for(int i=0;i<3;i++){
                        //cout<<"cv"<<jprControl->centerVector(i).x()<<"_"<<jprControl->centerVector(i).y()<<endl;
                        cout<<"WD:"<<jprControl->vectWD[i].x()<<"_"<<jprControl->vectWD[i].y()<<endl;
                        cout<<"WF:"<<jprControl->vectWF[i].x()<<"_"<<jprControl->vectWF[i].y()<<endl;
                }
                usleep(20000);
        }
        //mingi kahtlane test
        if(0) {
                jprControl->omni(0,-100, 255, 0);
                usleep(1000000);
                jprControl->omni(0,0, 255, -100);
                usleep(1000000);
                /*setDcMotor(0, -100);
                setDcMotor(1, 65);
                setDcMotor(2, -87);*/
        }

        //pööramise kiiruse test
        if (0) {
        imgPallDir=90;
                        if (time.elapsed()<4000){
                                setDcMotor(0, imgPallDir);
                                setDcMotor(1, imgPallDir);
                                setDcMotor(2, imgPallDir);
                                        if(JPR_NAEB_PALLI){//kui näeb palli siis jääb seisma
                                                cout<<"pall"<<endl;
                                                jprControl->omniBreak();
                                        }

                        } else { //kui aeg saab otsa ja vahepeal pole palli ikka näinud siis jääb seisma
                                jprControl->omniBreak();

                        }
        }

        //pööramiskiiruse test logimiseks
        // kiirendab max kiiruseni, siis sekundi hoiab kiirust, siis jääb seisma kaheks sekundiks ja alustab otsast peale
        // why: logida pallide ja värava nähtavust eri kiirustel ja kaugustel
        if(0){
            imgPallDir+=1;
            if(!flag && (imgPallDir>=1023)){
                flag=true;
                testTime.restart();
            }

            if(imgPallDir>1023)imgPallDir = 1023;

            if(imgPallDir<1023){
                jprControl->omniN(0,0,1,imgPallDir/4);
            }

            if(!stop && flag && (testTime.elapsed()>1000)){
                testTime.restart();
                stop = true;
            }

            if(stop){
                if(testTime.elapsed()>2000){
                    imgPallDir = 160;
                    stop = false;
                    flag = false;
                }else jprControl->omniBreak();
            }
        }

        //pööramiskiiruse test logimiseks (ver 2) erinevate kaamera setingute juures
        // kiirendab max kiiruseni, siis sekundi hoiab kiirust, siis jääb seisma kaheks sekundiks ja alustab otsast peale
        // why: logida pallide ja värava nähtavust eri kiirustel ja kaugustel
        if(0){ // ei toimi õigesti???
            kiirus+=1;
            int max=1280;//1024*2-1;
            if(!flag && (kiirus>=max)){
                flag=true;
                testTime.restart();
            }

            if(kiirus>max)kiirus = max;

            if(kiirus<max){
                jprControl->omniN(0,0,1,kiirus/8);
            }

            if(!stop && flag && (testTime.elapsed()>1000)){
                testTime.restart();
                stop = true;
            }

            if(stop){
                if(testTime.elapsed()>2000){
                    kiirus = 480;
                    this->imgFrameSkipCount++;
                    stop = false;
                    flag = false;
                }else jprControl->omniBreak();
            }
        }

        //rulliga palli hoidmise test
        // kiirendab aeglaselt
        // täisakuga 106, tühjema akuga 120, rulli-aku ei mõjutanud tulemust
        if(0){
           imgPallDir+=1;
           jprControl->omniN(0,0,1,imgPallDir/4);
           if(!JPR_PALL_EES){
                jprControl->omniBreak();
                if(flag){
                    cout<<imgPallDir/4<<endl;
                }
                imgPallDir = 160;
                flag = false;
           }else flag = true;
        }

        //natuke ekstreemsem rulliga palli hoidmise test (not)
        // täisakuga 157 (min), tühjema akuga 194
        if(0){
            imgPallDir+=1;
            jprControl->omniN(1,0,1,imgPallDir);
            if(!JPR_PALL_EES){
                 jprControl->omniBreak();
                 if(flag){
                     cout<<imgPallDir<<endl;
                 }
                 imgPallDir = 40;
                 flag = false;
            }else flag = true;
        }



        // otsesõidu test
        // pöörab kaugema värava suunale, sõidab max kiirusega väravani,
        // pöörab ümber teise värava poole ja uuesti max kiirusega.
        if(0){
            if(JPR_NAEB_SUVALIST_VARAVAT){
                Image::Object* g = getAnyGoal();
                // kas värav on piisavalt kaugel
                if((g->rect.width)>300){

                }
                // sihi ennast keskele
                if(abs(g->distanceH)>10){
                    jprControl->omniN(0,0,g->distanceH,80);
                }
                // sõida max kiirusega
                jprControl->omniN(1,0,0,255);
            }else{
                // leia värav
                jprControl->omniN(0,0,1,100);
            }
        }

        /** distanceH test*/
        if (0){
                        if (((img->width/2)<(getGoal()->rect.x+getGoal()->rect.width)) && ((img->width/2)>getGoal()->rect.x)){
                                cout<<"distanceH "<<getGoal()->distanceH<<endl;}
        }
        /** pall seinast test*/
        if (0) {
                if(JPR_NAEB_PALLI){
                        if (analog[0]>150 && analog[1]>150){
                        cout<<"sein "<<analog[0]<<" "<<analog[1]<<endl;
                        }else if (analog[0]>150){
                        cout<<"vasakul "<<analog[0]<<" "<<analog[1]<<endl;
                        //näeb palli aga sein on vasakul, vaja palli suhtes ringi sõita paremale
                                jprControl->omni(90, 75, 255, 0);
                        }else if (analog[1]>150){
                        cout<<"paremal "<<analog[0]<<" "<<analog[1]<<endl;
                        jprControl->omni(90, -75, 255, 0);
                        }
                }
        }

        /** palli andurite test */
        if (0) {
                jprControl->omniN(0,0.1,1,65);
                if (!JPR_PALL_EES)
                        cout<<"pall ees"<<imgPallDir++<<endl;
        }

        /** värava suhtes sättimise test */
        if(0) {
            if(JPR_NAEB_VARAVAT){
                if ((img->width/2) > ((getGoal()->rect.x+getGoal()->rect.width)-((getGoal()->rect.x+getGoal()->rect.width)*0.1))) //värava parempoolne äär, pööra vasakule
                {	jprControl->omniN(0,0.1,1,54); /*cout<<"paremalt üle"<<endl;*/
                }
                else if ((img->width/2) < (getGoal()->rect.x+(getGoal()->rect.x*0.1))) //värava vasakpoolne äär, pööra paremale
                {	jprControl->omniN(0,0.1,-1,54); //cout<<"vasakult üle"<<endl;
                }
                else { 	jprControl->omniBreak(); }
                cout<<getGoal()->rect.width<<endl;
                //if(JPR_NAEB_VARAVAT) cout<<"v2rav"<<endl;
            }
        }

        /** värava ja teiste pallide suhtes sättimise test */
        if(0) {
                if(JPR_PALL_EES){
                        if ((img->width/2) > ((getGoal()->rect.x+getGoal()->rect.width)-((getGoal()->rect.x+getGoal()->rect.width)*0.1))) //värava parempoolne äär, pööra vasakule
                        {	jprControl->omniN(0,0.1,1,54); //cout<<"paremalt üle"<<endl;
                        }
                        else if ((img->width/2) < (getGoal()->rect.x+( getGoal()->rect.x*0.1))) //värava vasakpoolne äär, pööra paremale
                        {	jprControl->omniN(0,0.1,-1,54); //cout<<"vasakult üle"<<endl;
                        }else if (!isValidKickPath()) {cout<<"pall ees"<<endl;
                                                jprControl->omni(90, -75, 255, 0);
                                                usleep(250000);
                                        }

                        else {
                                          if(JPR_PALL_EES) {
                                                jprControl->omniBreak();
                                                usleep(250000);
                                                jprControl->solenoid();
                                        } }
                }
        }
        /** palli sihtimise test*/
        if (0) {
                if (JPR_NAEB_PALLI){
                        cout<<"dH: "<<pall->distanceH<<" RectY: "<<pall->rect.y<<" RectW: "<< pall->rect.width<<endl;

                        if (!isValidKickPath() ) {
                                cout<<"pall ees "<<endl;
                        } else {
                                cout<<"safe "<<endl;
                        }
                }

        }

        /** värava põhja test*/
        if (0) {
                if (JPR_PORAND) {
                cout<<"porand"<<endl;
                } else {
                        cout<<"x"<<endl;
                }
        }
        /** värava otsingul järgmise palli suund*/
        if(0) {
                if(JPR_PALL_EES){
                        if((pall->distanceH) < 0){ // 1/-1
                        imgPallDir = -1;
                        } else imgPallDir=1;
                        if ((img->width/2) > (getGoal()->rect.x+getGoal()->rect.width)) //värava parempoolne äär, pööra vasakule
                        {	jprControl->omniN(0,0.1,1,54); //cout<<"paremalt üle"<<endl;
                        }
                        else if ((img->width/2) < getGoal()->rect.x) //värava vasakpoolne äär, pööra paremale
                        {	jprControl->omniN(0,0.1,-1,54); //cout<<"vasakult üle"<<endl;
                        }else if (!isValidKickPath()) {cout<<"pall ees"<<endl;
                                                jprControl->omni(90, -75, 255, 0);
                                                usleep(250000);
                                        }

                        else {
                                          if(JPR_PALL_EES) {
                                                jprControl->omniBreak();
                                                usleep(250000);
                                                jprControl->solenoid();
                                        }
                        }
                        cout<<"pall oli "<<imgPallDir<<endl;
                }
        }
                /** solenoidi test */
        if (0){
                jprControl->omniBreak();
                if (time.elapsed()>10000) {
                        jprControl->solenoid();
                        cout<<"test"<<endl;
                        time.restart();
                        }
        }

        if(0){
			jprControl->omniBreak();
    		if(JPR_PALL_EES){            
				int v=isValidKickPath3();
				//else{
					//jprControl->omniBreak(); usleep(100*1000);	
					if(JPR_NAEB_VARAVAT){
                		if (getGoal()->rect.width<40){ //kui värav on liiga väike, sõida sellele lähemale
						//cout<<"värav auu!"<<endl;
            			jprControl->omni(0, 100, 255, -getGoal()->distanceH/3);
						}       
						else if ((img->width/2) > ((getGoal()->rect.x+getGoal()->rect.width)-((getGoal()->rect.x+getGoal()->rect.width)*0.1))) //värava parempoolne äär, pööra vasakule
                		{	jprControl->omniN(0,0.1,1,54); /*cout<<"paremalt üle"<<endl;*/
                		}
                		else if ((img->width/2) < (getGoal()->rect.x+(getGoal()->rect.x*0.1))) //värava vasakpoolne äär, pööra paremale
                		{	jprControl->omniN(0,0.1,-1,54); //cout<<"vasakult üle"<<endl;
                		}
                		else if (v!=0) {
						cout<<"pall ees"<<endl;
            			jprControl->omni(v*45, 120, 255, 0);//TODO kavalam pallist mööda hiilimine
        				}else{ 	jprControl->solenoid(); }
                	//cout<<getGoal()->rect.width<<endl;
                	//if(JPR_NAEB_VARAVAT) cout<<"v2rav"<<endl;
            		}else{
						jprControl->omni(0,0,255,80);
					}				
					
				
			}
        }
        /** test inertsi jaoks*/
        if (0) {
            if(JPR_PALL_EES) {
                jprControl->omniBreak();
            }
            else if(JPR_NAEB_PALLI){
                char m = 1;
                int kiirus_z;
                int kiirus_x;
                if((pall->distanceH) < 0){
                    m = -1;
                }

                kiirus_x=255;

                if (getYcm(pall) > 45){
                    kiirus_z = (abs(pall->distanceH) / 2.5);
                    //jprControl->omni(0, kiirus_x, 255, -m*kiirus_z);
					//                x   y  nurk       max min  NB!tähtis on muutujate suhe?
                    jprControl->omniN(255,0,-m*kiirus_z,255,55);
                } else {
                    kiirus_z = (abs(pall->distanceH) / 4);
                    //jprControl->omni(0, 70, 255, -m*kiirus_z);
                    jprControl->omniN(70,0,-m*kiirus_z,70,55);
                    cout<<getYcm(pall)<<endl;
                }
            }
        }
		/** test ees oleva infraga palli tuvastamiseks */
		if (0) {
			int p_andur=analog[4];
			if (!(JPR_SEIN_PAREMAL || JPR_SEIN_VASAKUL)){
				if (p_andur>170) {
					cout<<"pall "<<p_andur<<endl;
				}
				else  {
					cout<<p_andur<<endl;
				}
			}
			else {
				cout<<"sein"<<endl;
			}
		}
  if (0){
     curCommandString = QString("avoidObstacles():%1").arg(searchBall).toStdString();
    int kiirus_t=65;
    if (searchBall==ss_searchCircle){
        doCircle(3000,imgPallDir*kiirus_t);//kiiruses hoiab meeles viimati nähtud lähimat palli...
    }
    else if (searchBall==ss_forward){//sõit väljaku keskele
        kiirus_t=150;
        if (time.elapsed()<1000){
          //sõida värava suunas väljaku keskele..kas suvalise värava?
          if(JPR_NAEB_SUVALIST_VARAVAT){   
            //if (getAnyGoal()->rect.width<40){ //kui värav on liiga väike, sõida sellele lähemale
            //cout<<"värav auu!"<<endl;
            jprControl->omni(0, 150, 255, -getAnyGoal()->distanceH/3);
            //}
          }
          else{
            jprControl->omniN(0,0.1,-goalDir,80);
          }
                //jprControl->omni(0, kiirus_t, 255, 0);
        } else { searchBall=ss_searchCircle; 
            time.restart(); }
    }
  }
  if(1){
    jprControl->omni(0,255,255,0);    
    usleep(1000*1000);
    jprControl->omniBreak();
    usleep(10*1000);
    jprControl->omni(0,70,255,0);
    usleep(4000*1000);

  }
}



void JprTest::go() {
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
    jprControl->headLight(0);

    // rull sisse
    jprControl->reel(1);

    //conf.setSendCmdEnabled(0);

    // käsurealt -p lipuga loetud käsk. n. -p "penaltivoor" või -p "varavavaht"
    waitForStart();

    //jprControl->solenoid();
    time.start();
    testTime.start();
    gameTime.start();
    ballTime.start();
    lastFrameTime = 0;
    logFile<<logHeader()<<endl;
    //jprControl->solenoid();
    while(1){
        jprControl->checkSolenoid();
        this->curCommandString = "##nothing";
        frameCount++;

        requestSensors();// annab mikrokontrollerile käsu andurite lugemiseks

        processImage();// pilditöötlus - tagastab leitud palli parameetrid

        getSensorsResponse();// loeb kirjutab anturite väärtused analog ja digital massiivi
        checkStateChanges();

        //getGoal();
        //cout<<"g"<<goalDir<<endl;

        printSensors();
        //jprControl->omniN(0,0,1,54);
        //runTest(0);
        //motorTest();

        jprControl->omni(0, 1, 255, -20);

//        if(pall){
////                    int sp= 0;
////                    sp = pall->distanceH/2.5;
////                    if(sp<0)sp=0;
//                    int sp= 0;
//                    sp = pall->distanceH/2.5;
//                    if(sp>0)sp=0;
//            jprControl->omni(90, 140, 255, -sp);
//        }
        
        //motorTest(); //test mootoritele
        //play();
        //printSensors(); //andurite väärtuste väljatrükk
        logState2();
        if(frameCount%200 == 0)cout<<fps()<<endl;
        //soidaPallile2();
        showGoal(conf.keyS); //0-sinine värav, 1-punane värav
        // näitab pilti ekraanil
        showImage(); // peab olema viimane
        //cout<<conf.keyS<<endl;
        lastFrameTime = gameTime.elapsed();
        if (gameTime.elapsed()>=(90000*3)) {
            cout<<"aeg läbi"<<endl;
            jprControl->omniBreak();
            //exit(0);
            waitForStart();
            gameTime.restart();
        }

    }
}
