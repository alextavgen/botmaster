#include "maiudebug.h"

MaiuDebug::MaiuDebug()
{
}

void MaiuDebug::alustaLogimist( int debugFile, int debugVideo){

    if( debugFile ) {
        QString logName("Debug/log_");
        logName += QString( getCurrentTime() ) + QString( ".csv" );
        data = new QFile( logName );
        if( data->open( QFile::WriteOnly | QFile::Truncate ) ) {
            out = new QTextStream( data );
            *out << "kaader;"
                    << "aeg;"
                    << "state;"
                    << "pall_x;"
                    << "pall_y;"
                    << "pall_width;"
                    << "pall_distanceH;"
                    << "PALLI_ANDUR;"
                    << "VASAK_EES;"
                    << "PAREM_EES;"
                    << "VASAK_KYLG;"
                    << "PAREM_KYLG;"
                    << "KAUGUS_ANDUR;"
                    << "VASAK_VARAV;"
                    << "PAREM_VARAV;"
                    << "PWM_0;"
                    << "PWM_1;"
                    << "PWM_2" << endl;
        }
    }

    if( debugVideo ) {
        QString vidName("Debug/vid_");
        vidName += QString( getCurrentTime() ) + QString( ".avi" );
        // initsialiseeri font
        cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, 1.0, 0, 1, 8);
        if( !cvVW ){
            //        CV_FOURCC('P','I','M','1')    = MPEG-1 codec
            //        CV_FOURCC('M','J','P','G')    = motion-jpeg codec (does not work well)
            //        CV_FOURCC('M', 'P', '4', '2') = MPEG-4.2 codec
            //        CV_FOURCC('D', 'I', 'V', '3') = MPEG-4.3 codec
            //        CV_FOURCC('D', 'I', 'V', 'X') = MPEG-4 codec
            //        CV_FOURCC('U', '2', '6', '3') = H263 codec
            //        CV_FOURCC('I', '2', '6', '3') = H263I codec
            //        CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec
            cvVW = cvCreateVideoWriter( vidName.toStdString().c_str(), CV_FOURCC( 'D','I','V','X' ), 24, cvSize( 320,240 ) );
        }
    }
}

void MaiuDebug::logi( int debugFile, int debugVideo, IplImage* img, Image::Object* pall, int analog[], int digital[], int pwms[], int state, int kaader, int ms ) {
    if( debugVideo && cvVW ) {
        sprintf(str, "%d", kaader);
        cvPutText(img, str, cvPoint(10, 200), &font, CV_RGB(0,0,0));
        cvPutText(img, QTime(0, ms/60000, (ms/1000)%60, ms%1000).toString("mm:ss").toStdString().c_str(), cvPoint(10, 220), &font, CV_RGB(0,0,0));
        // kirjuta pilt videosse
        cvWriteFrame( cvVW, img );
    }
    QString palliAndmed = "";
    if( pall != NULL ) {
	palliAndmed.append( QString::number( pall->rect.x ) ).append( ";" );
	palliAndmed.append( QString::number( pall->rect.y ) ).append( ";" );
	palliAndmed.append( QString::number( pall->rect.width ) ).append( ";" );
	palliAndmed.append( QString::number( pall->distanceH ) ).append( ";" );
    }
    else {
        palliAndmed.append( "0;0;0;0;0;" );
    }
    // kirjuta logi faili
  /*  if( debugFile && data->isOpen() ) {
    	QString s;
    	switch( state ) {
    		case STOP:
    			s.append( "STOP" );
    			break;
    		case OTSI_PALLI:
    			s.append( "OTSI_PALLI" );
    			break;
    		case POORA_PALLILE:
    			s.append( "POORA_PALLILE" );
    			break;
    		case SOIDA_PALLINI:
    			s.append( "SOIDA_PALLINI" );
    			break;
    		case OTSI_VARAVAT:
    			s.append( "OTSI_VARAVAT" );
    			break;
    		case POORA_VARAVALE:
    			s.append( "POORA_VARAVALE" );
    			break;
    		default:
    			break;
        }*/
        *out << kaader << ";"
                << ms << ";"
                << s << ";"
                << palliAndmed
                << analog[0] << ";"
                << analog[1] << ";"
                << analog[2] << ";"
                << analog[3] << ";"
                << analog[4] << ";"
                << analog[5] << ";"
                << digital[6] << ";"
                << digital[7] << ";"
                << pwms[0] << ";"
                << pwms[1] << ";"
                << pwms[2] << ";"
                << endl;
    }
}

void MaiuDebug::lopetaLogimine( int debugFile, int debugVideo ){
    if( debugVideo && cvVW ){
        cvReleaseVideoWriter( &cvVW );
    }
    if( debugFile && data->isOpen() ){
        data->close();
    }
}
