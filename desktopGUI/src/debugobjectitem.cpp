#include "debugobjectitem.h"

#include <QPainter>
#include <QGraphicsScene>

#include <QDebug>

#include "util.h"

DebugObjectItem::DebugObjectItem(int seconds, QGraphicsScene *sceneParent) :
  QGraphicsRectItem(0, sceneParent)
{
  setHeight(10);
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

  QString currentToolTip = toolTip();
  currentToolTip += "\nStop: "
      + Util::secondsToString(_startTime + _seconds);

  setToolTip(currentToolTip);
}

void DebugObjectItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *,
                            QWidget *)
{
  QColor color = _isRunning ?  Qt::darkGreen : Qt::darkGray;
  painter->setPen( color );
  painter->drawRect(0, 0, rect().width(), rect().height());
  painter->fillRect(0, 0, rect().width(), rect().height(), color);

  QString currentTime = Util::secondsToString(_seconds);

  painter->setPen( Qt::white);

  QFont font (painter->font());
  font.setBold(true);

  painter->setFont(font);

  QFontMetrics fm (painter->font());
  int textX = width() / 2 - fm.width(currentTime) / 2;

  if (textX + fm.width(currentTime) < width())
    painter->drawText(textX, height() / 2 + fm.height() / 2, currentTime);

}

