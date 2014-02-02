#include "debugobjectitem.h"

#include <QPainter>
#include <QGraphicsScene>

#include <QDebug>

#include "util.h"

DebugObjectItem::DebugObjectItem(QString object, int seconds, QGraphicsScene *sceneParent) :
  QGraphicsRectItem(0, sceneParent)
{
  _object = object;

  _isRunning = true;
  _seconds = 0;

  _timer = new QTimer (this);
  connect (_timer, SIGNAL(timeout()),
           SLOT(tick()));
  _timer->start(1000);

  _startTime = seconds;

  setToolTip("Start: " + Util::secondsToString(_startTime));
}

void DebugObjectItem::stop ()
{
  _isRunning = false;
  _timer->stop();

  _stopTime = _startTime + _seconds;

  QString currentToolTip = toolTip();
  currentToolTip += "\nStop: "
      + Util::secondsToString(_stopTime);

  setToolTip(currentToolTip);
}

void DebugObjectItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *,
                            QWidget *)
{
  QColor color = _isRunning ?  Qt::darkGreen : Qt::darkGray;

  int diff = _realStartPos - _specPos;

  if (diff)
  {
    qDebug () << "diff " << _object << ": " << diff;
    painter->setPen(Qt::red);
    painter->drawRect(0, 0, diff, rect().height());
    painter->fillRect(0, 0, diff, rect().height(), Qt::red);
  }


  painter->setPen( color );
  painter->drawRect(diff, 0, rect().width() - diff, rect().height());
  painter->fillRect(diff, 0, rect().width() - diff, rect().height(), color);

  QString currentTime = Util::secondsToString(_seconds);

  painter->setPen( Qt::white);

  QFont font (painter->font());
  font.setBold(true);

  painter->setFont(font);

  QFontMetrics fm (painter->font());
  int textX = (width() - diff) / 2 - fm.width(currentTime) / 2;

  if (textX + fm.width(currentTime) < width())
    painter->drawText(textX + diff, height() / 2 + fm.height() / 2,
                      currentTime);

}
