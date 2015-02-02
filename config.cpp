/**
 * @file config.cpp
 * @brief C++ Implementation: config
 * 
 * @details Configuration module for Robot Vision System.
 * 
 * Copyright: See COPYING file that comes with this distribution
 * 
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 * @author Aleksandr Tavgen <aleksandr.tavgen@itcollege.ee (C) 2012>
 */
#include "config.h"
#include <string>
#include <iostream>
#include <QtCore>

Config::Config(/*int argn, char ** argv*/)
{
/** näitab silumisinfot */
showDebugInfo=0;
sendCmdEnabled=0;
brightness=0;
contrast=0;
blackPercent = 50;
colorPercent = 70;
topCut=0;

  //Kasutatakse Image klassis pildi töötlemise juures (.h failis realiseeritud!).
   orange.hMin  = 0;
   orange.hMax  = 35;
   orange.sMin  = 90;
   orange.sMax  = 255;
   orange.lMin  = 50;
   orange.lMax  = 180;

   yellow.hMin    = 47;
   yellow.hMax    = 57;
   yellow.sMin    = 110;
   yellow.sMax    = 255;
   yellow.lMin    = 0;
   yellow.lMax    = 255;

   blue.hMin   = 90;
   blue.hMax   = 105;
   blue.sMin   = 70;
   blue.sMax   = 255;
   blue.lMin   = 0;
   blue.lMax   = 180;

   black.hMin  = 0;
   black.hMax  = 0;
   black.sMin  = 0;
   black.sMax  = 130;
   black.lMin  = 0;
   black.lMax  = 90;

   keyS = 0;

   showImage=0;

}


Config::~Config()
{
}

void Config::usage(char** argv)
{
	printf("Usage: %s -r robot\n\n"
	"Options:\n"
	" -h, --help\n"
    	"\tPrint detailed help screen.\n"
	" -r, --robot\n"
	"\tRobot name. Case sensitive. Required argument.\n"
	" -s, --sdev\n"
    	"\tSerial device name. Case sensitive.\n"
	" -v, --videodev\n"
    	"\tVideo device name. Case sensitive.\n"
	" -f, --file\n"
    	"\tPicture filename. Case sensitive.\n"
	" -c, --contrast\n"
    	"\tContrast value.\n"
	" -p, --params\n"
    	"\tAdditional robot parameters (as string).\n"
	" -b, --brightness\n"
    	"\tBrightness value.\n\n", argv[0]);
}

const char * Config::getmodemDevice()
{
 
   if (serial_device) {
 //	  std::cout<<"testib serial: "<<serial_device<<"\n";  
 	  return serial_device;
    }
  
  return "/dev/ttyUSB0";
}

/*!
    \fn Config::getRobotName()
 */
const char * Config::getRobotName()
{
  if (robot_name) return robot_name;
  return "UNSPECIFIED";
}


const char* Config::getRobotParams()
{
  if (robot_params) return robot_params;
  return "UNSPECIFIED";
}


Config & Config::getConfig()
{
  static Config instance;
  return  instance;
}

/*!
    \fn Config::setConfig(int argn, const char ** argv)
 */
void Config::setConfig(int argn, char ** argv)
{
  struct option longopts[] =
  {
    { "robot",      required_argument, NULL, 'r'}, /* Robot name */
    { "sdev",       required_argument, NULL, 's'}, /* Serial device name */
    { "videodev",   required_argument, NULL, 'v'}, /* video device name */
    { "file",       required_argument, NULL, 'f'}, /* picture filename */
    { "contrast",   required_argument, NULL, 'c'}, /* contrast value */
    { "brightness", required_argument, NULL, 'b'}, /* brightness value */
    { "help",       no_argument,       NULL, 'h'}, /* Displays detailed help */
    { "params",     no_argument,       NULL, 'p'}, /* Parameters passed over to robots */
    { 0, 0, 0, 0 }
  };
  
  if(argn==1)
  {
  usage(argv);
  exit(0);
  }
  
  serial_device=NULL;
  robot_name=NULL;
  robot_params=NULL;
  char c;
  while ((c = getopt_long(argn, argv, "hogs:b:c:v:f:r:p:", longopts, NULL)) != -1)
  {
    switch (c)
    {
      case 'b':
        if (optarg != NULL){
          brightness = atoi(optarg);
        }else{
          printf("Error!!!\n");
          exit(1);
        }
        break;
      case 'c':
        if (optarg != NULL){
          contrast = atoi(optarg);
        }else{
          printf("Error!!!\n");
          exit(1);
        }
        break;
      case 's':
        if (optarg != NULL)
          serial_device = optarg;
        else
        {
          printf("Error!!!\n");
          exit(1);
        }
        break;
      case 'r':
        if (optarg != NULL)
          robot_name = optarg;
        else
        {
          printf("Error!!!\n");
          exit(1);
        }
        break;
      case 'f':
        if (optarg != NULL){
          bmp_filename = optarg;
          showImage=1;
        }
        else
        {
          printf("Error!!!\n");
          exit(1);
        }
        break;
      case 'v':
        if (optarg != NULL)
          video_device = optarg;
        else
        {
          printf("Error!!!\n");
          exit(1);
        }
        break;
      case 'h':
	usage(argv);
	exit(0);
        break;
      case 'p':
        if (optarg != NULL)
          robot_params = optarg;
        else
        {
          printf("Error!!!\n");
          exit(1);
        }
        break;
      case 0:
        break;
      case ':':
        fprintf(stderr, "%s: option `-%c' requires an argument\n",
                argv[0], optopt);
        break;
      case '?':
      default:
        fprintf(stderr, "%s: option `-%c' is invalid:\nExiting...\n",
                argv[0], optopt);
        exit(1);
        break;
    }
  }
  readIni();
}


/**
 * Reads robot configuration from ini file
 * @return int 0 - success
 */
int Config::readIni()
{

  if (robot_name == NULL)
  {
    fprintf(stderr, "Robot name not specified!");
    exit(1);
  }
  std::string confFileName("configs/");
  confFileName.append(robot_name);
  confFileName.append(".ini");

  CSimpleIniA iniConf(true, true, true);
  if (iniConf.LoadFile(confFileName.c_str()) < 0)
  {
    fprintf(stderr, "Failed to load robot configuration file!");
    exit(1);
  }

  const char * niceName 
                = iniConf.GetValue("robot", "robot_name", NULL);

  
  const char * tmp = iniConf.GetValue("robot", "brightness", 0);
  if(tmp && brightness==0) brightness = atoi(tmp);


  tmp = NULL;
  tmp = iniConf.GetValue("robot", "contrast", 0);
  if(tmp && contrast==0) contrast = atoi(tmp);
  //if (!contrast) contrast =255; <-- max value?

  tmp = NULL;
  tmp = iniConf.GetValue("robot", "topCut", 0);
  if(tmp && topCut==0) topCut = atoi(tmp);

  tmp = NULL;
  tmp = iniConf.GetValue("robot", "serial_device", NULL);
  if(tmp && serial_device==0) serial_device = tmp;
  
  tmp = NULL;
  tmp = iniConf.GetValue("robot", "video_device", NULL);
  if(tmp && video_device==0) video_device = tmp;
  
  char message[255];
  sprintf(message, "Robot \"%s\" configuration loaded from \"%s\" ...\n", niceName, confFileName.c_str());
  qDebug()<< message;
  
  // Debugimiseks
  /*
  std::cout<<"Brightness: "<<brightness<<std::endl;
  std::cout<<"Contrast: "<<contrast<<std::endl;
  std::cout<<"Serial: "<<serial_device<<std::endl;
  std::cout<<"Video: "<<video_device<<std::endl<<std::endl;
  */
  
  return 0;
} 

void Config::setShowDebugInfo ( int theValue )
{
  showDebugInfo = theValue;
}
    

int Config::getShowDebugInfo()
{
  return showDebugInfo;
}

void Config::setSendCmdEnabled ( int theValue )
{
  sendCmdEnabled = theValue;
}
    
    
void Config::setTopCut ( int theValue )
{
  topCut = theValue;
}
  

int Config::getSendCmdEnabled()
{
  return sendCmdEnabled;
}

int Config::getTopCut()
{
  return topCut;
}

