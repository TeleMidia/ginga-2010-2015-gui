#ifndef UTIL_H
#define UTIL_H

#include <QString>
extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;

const QString GINGA_PATH = "C:/Users/Ginga/workspace/ginga/ginga-vs2010-solution/Release_working/ginga.exe"; //Ginga's binary path

const QString USER_ACCOUNT_PATH = "C:/Users/Ginga/workspace/ginga/ginga-vs2010-solution/Release/files/contextmanager/users.ini"; //Users' file info

const QString USER_PREFERENCES_PATH = "C:/Users/Ginga/workspace/ginga/ginga-vs2010-solution/Release/files/contextmanager/contexts.ini"; //Ginga's global variables

const QString USB_XML_FILE = "/var/run/gingagui/usb.xml"; //USB default xml file

const QString USB_XML_PARENT_DIR = "/var/run/gingagui/"; //USB default xml parent directory

const QString USB_WILCARD = "/usr/local/lib/ginga/gui/files/xml/dyncontent.xml#mnt-usb"; //USB Wilcard

const QString GINGA_KEY_PREFIX = "GIEK:";

#endif // UTIL_H
