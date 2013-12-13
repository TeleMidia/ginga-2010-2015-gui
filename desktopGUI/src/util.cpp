#include "util.h"
#include <QSettings>
#include <QDebug>
#include <QApplication>

QString Util::VERSION = "1.0.7";
QString Util::CMD_PREFIX = "cmd::";

QString Util::GINGA_KEY_PREFIX = "GIEK:";
QString Util::GINGA_CLICK_PREFIX = "GIEC:";
QString Util::GINGA_QUIT = Util::GINGA_KEY_PREFIX + "QUIT\n";
QString Util::GINGA_COMMAND_PREFIX = "GCMD:";

QString Util::PREFERENCES_ENVIRONMENT = "Environment";
QString Util::PREFERENCES_GINGA = "Ginga";
QString Util::PREFERENCES_ADVANCED = "Advanced";


void Util::init()
{
  QSettings settings (QSettings::IniFormat, QSettings::SystemScope,
                      QApplication::organizationName(),
                      QApplication::applicationName());

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
  while ((pos = rx.indexIn(ps, pos)) != -1) {
    plist << rx.cap(1);

    pos += rx.matchedLength();
  }

  return plist;
}


