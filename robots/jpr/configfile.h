#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QString>

class ConfigFile
{
public:
    ConfigFile(QString fn = "cfg.txt");
    void init();
    void ReadFile();
    QString FileName;

    QString HostIP;
    int HostPort;
    int gstSrcPort;
    QString StreamCommand;
};

#endif // CONFIGFILE_H
