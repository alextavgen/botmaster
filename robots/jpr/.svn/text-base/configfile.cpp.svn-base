#include "configfile.h"

#include <QFile>
#include <QTextStream>
#include <iostream>

using namespace std;

ConfigFile::ConfigFile(QString fn)
{
    FileName = fn;
    init();
    ReadFile();
}

void ConfigFile::init()
{
    HostIP = "192.168.1.94";
    HostPort = 40404;
    gstSrcPort = 1234;
    StreamCommand = "gst-launch -v videotestsrc ! videoscale ! video/x-raw-yuv,width=320,height=240 ! ffmpegcolorspace ! smokeenc ! udpsink host=192.168.1.94 port=1234";
}

void ConfigFile::ReadFile()
{
    QFile file(FileName);
    cout<<"ConfigFile "<<FileName.toStdString().c_str()<<endl;
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QTextStream qts(&file);
        QString line, str, val;
        char space;
        while ( !qts.atEnd() ) {
            line = qts.readLine();
            //cout<<"FFFFFFFFFFFF "<<line.toStdString().c_str()<<endl;
            QTextStream qts2(line.toStdString().c_str());
            qts2 >> str;
            qts2 >> space;
            //cout<<"FFFFFFFFFFFF "<<str.toStdString().c_str()<<endl;
            val = qts2.readLine();
            //cout<<"FFFFFFFFFFFF "<<str.toStdString().c_str()<<endl;
            if(str == "HostIP"){
                //cout<<"HostIP= "<<val.toStdString().c_str()<<endl;
                HostIP = val;
            } else if(str == "HostPort"){
                //cout<<"HostPort= "<<val.toStdString().c_str()<<endl;
                HostPort = val.toInt();
            } else if(str == "gstSrcPort"){
                //cout<<"gstSrcPort= "<<val.toStdString().c_str()<<endl;
                gstSrcPort = val.toInt();
            } else if(str == "StreamCommand"){
                //cout<<"StreamCommand= "<<val.toStdString().c_str()<<endl;
                StreamCommand = val;
            }
        }
        file.close();
    }
}
