#ifndef QNPLUTIL_H
#define QNPLUTIL_H

#include <QStringList>
#include <QString>

class Util
{   
public:
  static void init ();

  //Util strings
  static QString VERSION;
  static QString CMD_PREFIX;

  //Ginga wilcard strings
  static QString GINGA_KEY_PREFIX;
  static QString GINGA_CLICK_PREFIX;
  static QString GINGA_COMMAND_PREFIX;
  static QString GINGA_QUIT;

  //Preferences labels
  static QString PREFERENCES_ENVIRONMENT;
  static QString PREFERENCES_GINGA;
  static QString PREFERENCES_ADVANCED;

  static QStringList split(QString parameters);

  static int SCREEN_HEIGHT;
  static int SCREEN_WIDTH;
};


#endif // QNPLUTIL_H