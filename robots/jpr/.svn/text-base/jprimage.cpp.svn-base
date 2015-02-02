#include "jprimage.h"
#include <QTime>

using namespace std;

JprImage::JprImage() {
    i = 0;

    goalDir = 1;
    imgPallDir = 1;
    imgGoalRedDir = 1;
    imgGoalBlueDir = -1;

    imgValidKickPath = 0;
    imgFrameSkipCount = 0;

    imgFiltPall = 0.3;  // ei vali väravas olevaid palle (kuna on liiga madalal)
    imgMinPallWidth = 6;   // üle kontrollida;
    imgMinGoalWidth = 5;   // tuleks panna palli min suurus pikslites, näiteks 2m peal on pall 8 pikslit

    pall = NULL;
    prevPall = NULL;
    lockPall = NULL;
    goalRed = NULL;
    goalBlue = NULL;
    pallUnFiltered = NULL;
    pallCount = 0;
    prevPallCount = 0;
    lockedPall = false;

    cvVW = NULL;

    imgPallDirIgnore = 0;
}

JprImage::~JprImage() {
    delete pall;
    delete obj;
    if(cvVW){
        cvReleaseVideoWriter(&cvVW);
    }
}
/** tagastab palli objekti*/
Image::Object* JprImage::getPall() {
    return pall;
}
/** kirjutab pildile, mis värav on valitud*/
void JprImage::showGoal(int g){
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX_SMALL, hScale, vScale, 0, lineWidth, 8);
    if((img != NULL)){
        //teksti kirjutamine (pilt, tekst, asukoht(esitähe alumine vasak nurk), font, värv)
        //cvPutText (img,"My comment",cvPoint(20,40), &font, cvScalar(255,255,0));
        if (g==0) {
            // draw a box with blue lines
            cvRectangle(img, cvPoint(280,10), cvPoint(299,30), cvScalar(255,0,0), -1);
        } else if (g==1) {
            // draw a box with red lines
            cvRectangle(img, cvPoint(280,10), cvPoint(299,30), cvScalar(0,0,255), -1);
        }
    }
}

/** kui pall on käes ja tahad lüüa siis kontrollib et teist palli ees ei oleks */
bool JprImage::isValidKickPath() {	
    if (image->found_objects != NULL) {
        for (int i = 0; i < image->found_objects->total; i++){
            obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i );
            if(obj->type == BALL) {
                if(obj->rect.width > 11) {
                    //http://xrjunque.nom.es/precis/Polycalc.aspx
                    //150,100; 105,65; 60,45; 40,30; 30,20;
                    double x = obj->rect.y;

                    int dh = 20*(x -40)*(x -60)*(x -105)*(x -150) / (( -10)*( -30)*( -75)*( -120)) +30*(x -30)*(x -60)*(x -105)*(x -150) / ((10)*( -20)*( -65)*( -110)) +45*(x -30)*(x -40)*(x -105)*(x -150) / ((30)*(20)*( -45)*( -90)) +65*(x -30)*(x -40)*(x -60)*(x -150) / ((75)*(65)*(45)*( -45)) +100*(x -30)*(x -40)*(x -60)*(x -105) / ((120)*(110)*(90)*(45));

                    if(dh > 150) dh = 100;
                    if(dh < 30) dh = 22;

                    if((obj->distanceH < dh) && (obj->distanceH > (-1 * dh))) {
                        return false;
                    }
                }
            } else {
                return true;
            }
        }
        return true;

    } else {
        return true;
    }
}

/** kui pall on käes ja tahad lüüa siis kontrollib et teisi palle ees ei oleks */
bool JprImage::isValidKickPath2() {
    float w = 0;
    if (image->found_objects != NULL) {
        for (int i = 0; i < image->found_objects->total; i++){
            obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i );
            if(obj->type == BALL) {
                w = obj->rect.width;
                // kui kaugus keskjoonest on väiksem kui palli laius, siis ei mahu pall mööda
                // igaksjuhuks 1.2 laiust, kuna ei pruugi otse lüüa
                if(abs(obj->distanceH) < (w*1.2)){
                    return false;
                }
            }
        }
    }
    return true;
}

/** kui pall on käes ja tahad lüüa siis kontrollib et teisi palle ees ei oleks
tagastab suuna kuhu poole oleks mõtekam liikuda, kui pall ees (1 ja -1)
tagastab 0, kui palli ees pole
*/
int JprImage::isValidKickPath3() {
    float w = 0;
    int sum = 0;
    int m = 1;
    bool ret=true;
    if (image->found_objects != NULL) {
        for (int i = 0; i < image->found_objects->total; i++){
            obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i );
            if(obj->type == BALL) {
                w = obj->rect.width;
                m = 1;
                if(obj->distanceH<0)m=-1;
                sum += m*w;
                // kui kaugus keskjoonest on väiksem kui palli laius, siis ei mahu pall mööda
                // igaksjuhuks 1.2 laiust, kuna ei pruugi otse lüüa
                if(abs(obj->distanceH) < (w*1.2)){
                    ret = false;
                }
            }
        }
        if(!ret){
            if(sum>0)return 1; else return -1;
        }
    }
    return 0;
}

/** tuvastatud suvalise värava tagastamine*/
// läheb testides vaja
Image::Object* JprImage::getAnyGoal() {
    if(goalRed)return goalRed;
    if(goalBlue)return goalBlue;
    return NULL;
}

bool JprImage::isValidBall(Image::Object* obj, float k){
//if (obj->area<10) return false;
//return true;
    // k - kordaja, vaikimisi 0.1, ehk 10 protsenti
    // TODO
    // arvutuslikku laiust ja kõrgust võrrelda pildil nähtuga ja selle järgi otsustada kas on pall (üks nendest peab sobima)
    // selleks, et ei võtaks suvalist metallitükki väljakul palliks;
    // TODO: pindala järgi
    double w = obj->rect.width; // liikuv pall on laiem kõrgusega ka vaja teha
    double h = obj->rect.height;
    //if(w<imgMinPallWidth)return false;
    double eksimusW = w*k; // 10 protsenti
    double eksimusH = h*k; // 10 protsenti
    //cout<<w<<"_"<<h<<"_"<<obj->rect.y<<endl;
    if(eksimusW<4)eksimusW = 4; // kaugete pallide jaoks
    if(eksimusH<4)eksimusH = 4;
//    double difW = abs(w-getYwidth(obj));
//    double difH = abs(h-getYheight(obj));
//    if((difW<eksimusW) || (difH<eksimusH))return true; // //võibolla kaugemate pallidega teha väiksema erinevusega või protsendiga
    double difW = (w-getYwidth(obj));
    //cout<<"w_"<<w<<"_"<<getYwidth(obj)<<endl;
    double difH = (h-getYheight(obj));
    if((difW < -eksimusW) && (difH < -eksimusH))return false; // //võibolla kaugemate pallidega teha väiksema erinevusega või protsendiga

    // ülisuur pall
    if((w > (getYwidth(obj)*4)) || (h > (getYheight(obj)*4))){
        return false;
    }
    // värava kontroll
//    Image::Object* g = getAnyGoal();
//    if(g != NULL){
//        // teeme seda kontrolli värava läheduses olles
//        cout<<g->rect.height<<endl;
//        if(g->rect.height > 80){
//            int alumine = g->rect.y - g->rect.height;
//            if(alumine > obj->rect.y) return false;
//        }
//    }

    return true;
}

void JprImage::printBall(Image::Object *B)
{
    if(B != NULL){
        std::cout<<"----------------------------"<<std::endl;
        std::cout<<"area: "<<B->area<<std::endl;
        std::cout<<"BallFromFrame: "<<this->getFrameCount()<<std::endl;
        std::cout<<"center.x: "<<B->center.x<<std::endl;
        std::cout<<"center.y: "<<B->center.y<<std::endl;
        std::cout<<"rect.x: "<<B->rect.x<<std::endl;
        std::cout<<"rect.y: "<<B->rect.y<<std::endl;
        std::cout<<"rect.width: "<<B->rect.width<<std::endl;
        std::cout<<"rect.height: "<<B->rect.height<<std::endl;

        std::cout<<"distanceH: "<<B->distanceH<<std::endl;
    }
}

void JprImage::printBallCalcs(Image::Object *B){
    if(B != NULL){
        std::cout<<"----------------------------"<<std::endl;
        std::cout<<"Ycm: "<<this->getYcm(B)<<std::endl;
        std::cout<<"Xcm: "<<this->laiusToXcm(B)<<std::endl;
        std::cout<<"deg: "<<this->getDegree(B)<<std::endl;
    }
}

/** kaugus kahe palli vahel */
int JprImage::pallDif(Image::Object* p1, Image::Object* p2){
    double y1 = this->getYcm(p1);
    double x1 = this->laiusToXcm(p1);
    double y2 = this->getYcm(p2);
    double x2 = this->laiusToXcm(p2);
    double dy = abs(y1-y2);
    double dx = abs(x1-x2);
    double sqr = dy*dy + dx*dx;
    return sqr;
}

int JprImage::centerDif(Image::Object* p1, Image::Object* p2){
    double dy = abs(p1->center.y - p2->center.y);
    double dx = abs(p1->center.x - p2->center.x);
    double sqr = dy*dy + dx*dx;
    return sqr;
}

int JprImage::centerDif(JprObjekt p1, JprObjekt p2){
    double dy = abs(p1.centerY - p2.centerY);
    double dx = abs(p1.centerX - p2.centerX);
    double sqr = dy*dy + dx*dx;
    return sqr;
}

void JprImage::colorGoal(Image::Object* g){
    if(g && img){
        CvScalar c = CV_RGB(255,0,0);
        if(g->type == GOAL_BLUE)c = CV_RGB(0,0,255);
        CvPoint p1 = g->center;
        CvPoint p2 = g->center;
        p1.x -= 10; p1.y -= 10;
        p2.x += 10; p2.y += 10;
        cvRectangle(img, p1, p2, c, 2 );
    }
}


/** näitab kaamera pilti, joonistab punase ringi valitud pallile */
void JprImage::showImage() {
    i++;
    if(img != NULL){
//        CvRect rect = cvRect(50,30,120,20);

//        CvPoint p1 = cvPoint(rect.x, rect.y);
//        CvPoint p2 = cvPoint(rect.x+rect.width, rect.y+rect.height);
//        CvScalar c = CV_RGB(0,0,0);

//        cvRectangle(img, p1, p2, c, 2 );
//        image->HSLAverage(rect);

        if(JPR_NAEB_PALLI){
            cvCircle(img, cvPoint(pall->center.x, pall->center.y), pall->rect.width * 0.7, CV_RGB(255,0,0), 2, CV_AA, 0);
        }
        if(lockPall != NULL){
            cvCircle(img, cvPoint(lockPall->center.x, lockPall->center.y), lockPall->rect.width * 0.6, CV_RGB(50,100,0), 2, CV_AA, 0);
        }
        Image::Object *g = this->getGoal();
        if(g){
            CvScalar c = CV_RGB(200,200,0);
            CvPoint p1 = g->center;
            CvPoint p2 = g->center;
            p1.x -= 8; p1.y -= 8;
            p2.x += 8; p2.y += 8;
            cvRectangle(img, p1, p2, c, 2 );
        }

        /*if(img != NULL){
        CvPoint left, right;
        left.y = image->fieldBorderCenterY;
        left.x = 0;
        right.y = image->fieldBorderCenterY;
        right.x = img->width - 1;
        cvLine(img, left, right, CV_RGB(0,0,255), 1, CV_AA, 0);
    }*/

        if(imgShowImages){
            if(!(i%3/* üle 3 kaadri saadab pilti */)){
                view->show(img);

            }
        }
        if(SAVE_VIDEO){
            if(cvVW && (getFrameCount()%1 == 0)){
                sprintf(str, "%d", getFrameCount());
                cvPutText(img, str, cvPoint(10, 200), &font, CV_RGB(0,0,0));
                int ms = getGameTime().elapsed();
                cvPutText(img, QTime(0, ms/60000, (ms/1000)%60, ms%1000).toString("mm:ss").toStdString().c_str(), cvPoint(10, 220), &font, CV_RGB(0,0,0));
                cvWriteFrame(cvVW, img); // kirjuta pilt videosse
            }

        }
    }
}

/** pilditöötlus, tuvastatakse lähim pall, väravad (värava suurim ala kuhu saaks lüüa)*/
void JprImage::processImage() {

    for(int i=0;i<1/*imgFrameSkipCount*/;i++){ // alati peab vähemalt 1 kaader olema, kuna ilma selleta tuleb lag, suur lag
        cvGrabFrame(image->capture);// võtame kaadri ilma et seda dekodeeritaks mingile kujule,
        //kuna getFrame hakkab looma igasuguseid massiive pilditöötluse jaoks, manuaalis väidetakse ka, et sünkimiseks hea.
    }

    img = image->getFrame();
    //cvLine(img, cvPoint(img->width, 0), cvPoint(img->width, img->height), CV_RGB(0,255,0), 6,CV_AA, 0);
    //img = image->getFrame();
    image->process(1,0,0,0);

    int minusBlueDistH = 0;
    int plusBlueDistH = 0;
    int minusRedDistH = 0;
    int plusRedDistH = 0;
    int minusPallDistH = 0;
    int plusPallDistH = 0;
    int distH = 0;

    if (image->found_objects != NULL) {
        pall = NULL;
        goalRed = NULL;
        goalBlue = NULL;
        pallUnFiltered = NULL;
        pallCount = 0;
        difRed = 160000;
        difBlue = 160000;
        int difR = 0;
        int difB = 0;
        iRedClose = NULL;
        iBlueClose = NULL;
        // kõigepealt tuvastame väravad
        for (int i = 0; i < image->found_objects->total; i++){
            obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i );
            distH = abs(obj->distanceH);
            if (obj->type==GOAL_RED) {
                if(obj->rect.width >= imgMinGoalWidth){
                    if(goalRed == NULL) goalRed = obj;
                    difR = centerDif(prevRed,JprObjekt(obj));
                    if(difR<difRed){
                        difRed = difR;
                        iRedClose = obj;
                    }
                    if((goalRed->rect.width) < (obj->rect.width)){
                        goalRed = obj;
                    }
                    colorGoal(obj);

                    if(obj->distanceH<0){
                        if(minusRedDistH < distH) minusRedDistH = distH;
                    }else{
                        if(plusRedDistH < distH) plusRedDistH = distH;
                    }
                }
            }
            if (obj->type==GOAL_BLUE) {
                if(obj->rect.width >= imgMinGoalWidth){
                    if(goalBlue == NULL) goalBlue = obj;
                    difB = centerDif(prevBlue,JprObjekt(obj));
                    if(difB<difBlue){
                        difBlue = difB;
                        iBlueClose = obj;
                    }
                    if((goalBlue->rect.width) < (obj->rect.width)){
                        goalBlue = obj;
                    }
                    colorGoal(obj);

                    if(obj->distanceH<0){
                        if(minusBlueDistH < distH) minusBlueDistH = distH;
                    }else{
                        if(plusBlueDistH < distH) plusBlueDistH = distH;
                    }
                }
            }
        }




        if(goalRed){
            if((goalRed != iRedClose) && iRedClose) {
                if(goalRed->rect.width < (iRedClose->rect.width*1.5)){
                    goalRed = iRedClose;
                }
            }
            prevRed.set(goalRed);
        }
        if(goalBlue){
            if((goalBlue != iBlueClose) && iBlueClose){
                if(goalBlue->rect.width < (iBlueClose->rect.width*1.5)){
                    goalBlue = iBlueClose;
                }
            }
            prevBlue.set(goalBlue);
        }

        // pallid tuvastame peale väravat, et saaks palle väravast välja filtreerida
        for (int i = 0; i < image->found_objects->total; i++){
            obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i );
            distH = abs(obj->distanceH);
            if (obj->type==BALL) {
                if(isValidBall(obj,imgFiltPall)){
                    pallCount++;
                    if(pall == NULL) pall = obj;

                    if((pall->area) < (obj->area)){
                        pall = obj;
                        pallUnFiltered = obj;
                    }

                    if(obj->distanceH<0){
                        if(minusPallDistH < distH) minusPallDistH = distH;
                    }else{
                        if(plusPallDistH < distH) plusPallDistH = distH;
                    }
                }else{
                    //                if(pallUnFiltered == NULL) pallUnFiltered = obj;

                    //                if((pallUnFiltered->area) < (obj->area)){
                    //                    pallUnFiltered = obj;
                    //                }
                }



            }
        }

        // objektide suunad pööramiseks
        if(pall != NULL){
            imgPallDir = 1;
            if(minusPallDistH>plusPallDistH)imgPallDir = -1;
            imgPallDir = -imgPallDir;
        }
        if(goalRed != NULL){
            imgGoalRedDir = -1;
            imgGoalBlueDir = 1;
            if(minusRedDistH>plusRedDistH){
                imgGoalRedDir = 1;
                imgGoalBlueDir = -1;
            }
        }
        if(goalBlue != NULL){
            imgGoalRedDir = 1;
            imgGoalBlueDir = -1;
            if(minusBlueDistH>plusBlueDistH){
                imgGoalRedDir = -1;
                imgGoalBlueDir = 1;
            }
        }
    }

    imgValidKickPath = isValidKickPath3();

    if(imgPallDirIgnore != 0)imgPallDir = imgPallDirIgnore;


//    if(pall != NULL){
//        if(prevPall != NULL){
////            // kui palju erineb uus pall vanast
////            if(isSameBall(prevPall, pall)){

////            }else{
////                // uus pall

////            }

//            delete prevPall;
//        }else{
//            // pall on uus
//        }
//        prevPall = copyBall(pall);
//        prevPallCount = pallCount;
//    }

    // suuna aegumine?, millal ei ole enam usaldusväärne?
    // värava oma on alati usaldatav (vähemalt siis kui väravad ei ole kellegi poolt kaetud), palli oma vast mitte


    //cout<<imgGoalRedDir<<"_"<<imgGoalBlueDir<<"_"<<imgPallDir<<"_"<<endl;
    // kuna värav katkeb kahest kohast (Al), siis on kaks väikest väravat külgedel
}
