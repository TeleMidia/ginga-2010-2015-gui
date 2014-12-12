#ifndef UTIL_H
#define UTIL_H

#include <QString>
extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;

const QString TELEMIDIA_ID = "telemidia";

const QString TELEMIDIA_DOMAIN = "telemidia.puc-rio.br";

const QString APP_NAME = "gingagui";

const QString USER_ACCOUNT_FILE = "gingauser_file"; //Users' file info

const QString USER_PREFERENCES_FILE = "gingaconfig_file"; //Ginga's global variables

const QString USB_XML_FILE = //USB default xml file
#if defined _linux_
    "/var/run/gingagui/usb.xml";
#elif defined __WIN32__
    "C:/tmp/gingagui/usb.xml";
#endif

const QString USB_XML_PARENT_DIR = //USB default xml parent directory
#if defined _linux_
    "/var/run/gingagui/usb.xml";
#elif defined __WIN32__
    "C:/tmp/gingagui";
#endif

const QString USB_WILCARD = "/usr/local/lib/ginga/gui/files/xml/dyncontent.xml#mnt-usb"; //USB Wilcard

const QString GINGA_KEY_PREFIX = "GIEK:";

#endif // UTIL_H
