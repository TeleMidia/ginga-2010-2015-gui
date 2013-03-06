#include "qnplutil.h"

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

