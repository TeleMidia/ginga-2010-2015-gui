#include "qnplutil.h"

QString QnplUtil::VERSION = "1.0.7";
QString QnplUtil::CMD_PREFIX = "cmd::gingagui::";

QStringList QnplUtil::split(QString parameters)
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

