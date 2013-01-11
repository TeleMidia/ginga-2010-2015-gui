#include "qnplsettings.h"

#include <QApplication>
#include <QDir>

QnplSettings::QnplSettings()
    : QSettings("TeleMidia Lab", "Ginga")
{
    if (value("autoplay").toString() ==  ""){
        setValue("autoplay", true);
    }

    if (value("screensize").toString() == ""){
        setValue("screensize", "854x480");
    }

    if (value("location").toString() == ""){
        setValue("location", QApplication::applicationDirPath()+"/"+"ginga.exe");
    }

    if (value("device_port").toString() == ""){
        setValue("device_port", 22222);
    }

    if (value("passive_running").toString() == ""){
        setValue("passive_running", false);
    }

    if (value("run_as").toString() == "")
    {
      setValue("run_as", "base");
    }

//    if (value("parameters").toString() == ""){
        setValue("parameters", "--ncl ${NCLFILE} --wid ${WID} --vmode ${SCREENSIZE} --disable-fkeys --set-exitonend --disable-multicast");
//    }

    // fixing parameters for this version (1.0.0)


    if (value("lang").toString() == ""){
        setValue("lang", "en");
    }


    if (value("gingaconfig_file").toString() == ""){
        setValue("gingaconfig_file", "C:\\ProgramData\\Ginga\\contexts.ini");
    }

//    if (value("gingaconfig_file").toString() == ""){
//        setValue("gingaconfig_file", QDir::homePath()+"/AppData/Roaming/telemidia/ginga/contexts.ini");
//    }

}

QnplSettings::~QnplSettings()
{

}
