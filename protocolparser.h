#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H
#define GF 1
#define RA 2
#define RD 3
#define SA 4
#define SD 5
#define GET_DIGITALS RD
#define GET_FRAME 1
#define GET_ANALOGS 2
/** NC - No Command */
#define NC 0
#define START 1

class ProtocolParser
{
public:
    ProtocolParser();
    void parseData(unsigned char*);
    int getCommand();

private:
    unsigned char buf[2048];
    int lastCommand;
    unsigned char *comData;

};

#endif // PROTOCOLPARSER_H
