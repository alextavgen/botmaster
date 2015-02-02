#include "protocolparser.h"
#include <iostream>

using namespace std;

ProtocolParser::ProtocolParser()
{

}

void ProtocolParser::parseData(unsigned char *data)
{
    int parsedCommand = NC;
    unsigned char firstChar = 0;
    unsigned char secondChar = 0;

    while(*data++ > 1){cout << *data << endl;}
    while(*data)
    {
         firstChar = *data;
         //cout << *data << endl;
         if (*data++)
         {
            secondChar = *data++;
            break;
         }
    }
   comData = data;

   //cout << "Esimene = "<< firstChar << " Teine = " << secondChar << endl;

   switch(firstChar)
   {
   case 'S':
       switch(secondChar)
       {
       case 'A':
           //Set analog
           lastCommand = 4;
           break;
       case 'D':
           //Set digital
           lastCommand = 5;
           break;
       }
       break;

   case 'R':
       switch(secondChar)
       {
       case 'A':
           //Loe analogi
           lastCommand = GET_ANALOGS;
           break;
       case 'D':
           //Loe digitali
           lastCommand = GET_DIGITALS;
           break;
       case 'F':
           //Saada frame
           lastCommand = GET_FRAME;
           break;
       }
       break;
   }
}

int ProtocolParser::getCommand()
{
    int currentCommand = lastCommand;
    return lastCommand;
}
