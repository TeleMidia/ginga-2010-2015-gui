#include "util.h"
#include <QSettings>
#include <QDebug>
#include <QApplication>
#include "qnplsettings.h"

QString Util::VERSION = "1.0.7";
QString Util::CMD_PREFIX = "cmd::";

QString Util::GINGA_KEY_PREFIX = "GIEK:";
QString Util::GINGA_CLICK_PREFIX = "GIEC:";
QString Util::GINGA_QUIT = Util::GINGA_KEY_PREFIX + "QUIT\n";
QString Util::GINGA_COMMAND_PREFIX = "GCMD:";
QString Util::GINGA_PAUSE_KEY = "F12";
QString Util::GINGA_TS_FILE = "file:";

QString Util::PREFERENCES_ENVIRONMENT = "Environment";
QString Util::PREFERENCES_GINGA = "Ginga";
QString Util::PREFERENCES_ADVANCED = "Advanced";

QString Util::GUI_NCL = "--ncl";
QString Util::GUI_WID = "${WID}";
QString Util::GUI_FILE = "${FILE}";
QString Util::GUI_SCREENSIZE = "${SCREENSIZE}";

QString Util::V_LOCATION = "location";
QString Util::V_CONTEXT_FILE = "gingaconfig_file";
QString Util::V_PARAMETERS = "parameters";
QString Util::V_ASPECT_RATIO = "aspect_ratio";
QString Util::V_LAST_DIR = "lastdir_opened";
QString Util::V_FILES = "files";
QString Util::V_AUTOPLAY = "autoplay";
QString Util::V_SCREENSIZE = "screensize";
QString Util::V_PASSIVE = "passive_running";
QString Util::V_DEVICE_PORT = "device_port";
QString Util::V_ENABLE_LOG = "enablelog";
QString Util::V_LANG = "lang";
QString Util::V_EMBEDDED = "embedded";

QString Util::G_ON_BEGIN = "onBegin";
QString Util::G_ON_END = "onEnd";

QString Util::TRUE_ = "true";
QString Util::FALSE_ = "false";
QString Util::WIDE = "wide";
QString Util::STANDARD = "standard";
int Util::DEFAULT_PORT = 22222;

void Util::init()
{
  QnplSettings settings;

  VERSION = settings.value("VERSION", "1.0.7").toString();

  CMD_PREFIX = settings.value("GINGA/CMD_PREFIX", "cmd::").toString();

  GINGA_KEY_PREFIX = settings.value("GINGA/KEY_PREFIX", "GIEK:").toString();

  GINGA_CLICK_PREFIX = settings.value("GINGA/CLICK_PREFIX", "GIEC:").toString();

  GINGA_QUIT = settings.value("GINGA/QUIT",
                              GINGA_KEY_PREFIX + "QUIT\n").toString();

  GINGA_COMMAND_PREFIX = settings.value("GINGA/COMMAND_PREFIX",
                                        "GCMD:").toString();

  PREFERENCES_ENVIRONMENT = settings.value("GINGA/ENVIRONMENT_LABEL",
                                           "Environment").toString();

  PREFERENCES_GINGA = settings.value("GINGA/GINGA", "Ginga").toString();

  PREFERENCES_ADVANCED = settings.value("GINGA/ADVANCED",
                                        "Advanced").toString();

  settings.setValue("VERSION", VERSION);
  settings.setValue("GINGA/CMD_PREFIX", CMD_PREFIX);
  settings.setValue("GINGA/KEY_PREFIX", GINGA_KEY_PREFIX);
  settings.setValue("GINGA/CLICK_PREFIX", GINGA_CLICK_PREFIX);
  settings.setValue("GINGA/QUIT", GINGA_QUIT);
  settings.setValue("GINGA/COMMAND_PREFIX", GINGA_COMMAND_PREFIX);
  settings.setValue("GINGA/ENVIRONMENT_LABEL", PREFERENCES_ENVIRONMENT);
  settings.setValue("GINGA/GINGA", PREFERENCES_GINGA);
  settings.setValue("GINGA/ADVANCED", PREFERENCES_ADVANCED);

  settings.sync();


  qDebug () << settings.allKeys();
}

QStringList Util::split(QString parameters)
{

  QStringList plist;

  QString ps = parameters;

  QRegExp rx; rx.setPattern("([-${}\\w\\\\:]+|\\\".*\\\")");

  int pos = 0;
  while ((pos = rx.indexIn(ps, pos)) != -1)
  {
    plist << rx.cap(1);

    pos += rx.matchedLength();
  }

  return plist;
}

QString Util::defaultParameters()
{
  return "--ncl ${FILE} --vmode ${SCREENSIZE} --set-exitonend "
      "--disable-multicast --poll-stdin";
}

GingaMessage Util::parseMessage(QString message)
{
  GingaMessage gingaMessage;

  QStringList tokens = message.split("::");
  if (tokens.count() > 3)
  {
    gingaMessage.command = tokens.at(0);
    gingaMessage.code = tokens.at(1);
    gingaMessage.messageKey = tokens.at(2);

    for (int i = 3; i < tokens.size(); i++)
        gingaMessage.data.push_back(tokens.at(i));
  }

  return gingaMessage;
}

QString Util::secondsToString(int seconds)
{
  QString currentTime = "";

  int min = seconds / 60;
  seconds %= 60;

  if (min > 59)
  {
    int hours = min / 60;
    min %= 60;
    currentTime = QString::number(hours) + ":";
  }

  if (min < 10)
    currentTime += "0" + QString::number(min) + ":";
  else
    currentTime += QString::number(min) + ":";

  if (seconds < 10)
    currentTime += "0" + QString::number(seconds);
  else
    currentTime += QString::number(seconds);

  return currentTime;
}
