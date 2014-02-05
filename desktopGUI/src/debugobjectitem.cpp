#include "debugobjectitem.h"

#include <QPainter>
#include <QGraphicsScene>
#include <qmath.h>

#include <QDebug>

#include "util.h"

DebugObjectItem::DebugObjectItem(const QString &object, int seconds,
                                 const int &increment,
                                 QGraphicsScene *sceneParent) :
  QGraphicsRectItem(0, sceneParent), _increment (increment), _object (object)
{
  _diffRectBegin.setWidth(0);
  _realDuration.setWidth(0);
  _diffRectEnd.setWidth(0);

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

  _diffRectEnd.moveLeft(_realDuration.x() + _realDuration.width());

  setToolTip(currentToolTip);
}

void DebugObjectItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *,
                            QWidget *)
{
  QColor color = _isRunning ?  Qt::darkGreen : Qt::darkGray;

  if (_diffRectBegin.width())
  { //Drawing a red rect indicating problem with synchronism on beginning.
    painter->drawRect(0, 0, _diffRectBegin.width(), rect().height());
    painter->fillRect(0, 0, _diffRectBegin.width(), rect().height(), Qt::red);
  }

  //Drawing a rect indicating the media running.
  painter->setPen( color );
  painter->drawRect(_realDuration.x(), 0, _realDuration.width(),
                    rect().height());
  painter->fillRect(_realDuration.x(), 0, _realDuration.width(),
                    rect().height(), color);

  QString time = Util::secondsToString(_seconds);

  painter->setPen( Qt::white);

  QFont font (painter->font());
  font.setBold(true);

  painter->setFont(font);

  QFontMetrics fm (painter->font());
  int textX = (_realDuration.width() - fm.width(time)) / 2;

  //Drawing a text indicating the current media time.
  if (textX + fm.width(time) < _realDuration.width())
    painter->drawText(textX + _realDuration.x(), (height() + fm.height()) / 2,
                      time);

  if (fm.width(time) < _diffRectBegin.width() )
  {//Drawing a text indicating the delay time on beginning.
    time = Util::secondsToString( _diffRectBegin.width() / _increment);

    textX = (_diffRectBegin.width() - fm.width(time)) / 2;

    painter->drawText(textX, (height() + fm.height()) / 2, time);
  }

  if (_diffRectEnd.width())
  { //Drawing a red rect indicating problem with synchronism on media ending.
    painter->drawRect(_diffRectEnd.x(), 0, _diffRectEnd.width(),
                      rect().height());
    painter->fillRect(_diffRectEnd.x(), 0, _diffRectEnd.width(),
                      rect().height(), Qt::red);

    //Drawing a text indicating the delay time on beginning.
    time = Util::secondsToString(_diffRectEnd.width() / _increment);
    textX = (_diffRectEnd.width() - fm.width(time)) / 2;

    painter->drawText(textX + _diffRectEnd.x(), (height() + fm.height()) / 2,
                      time);

  }
}

void DebugObjectItem::incrementWidth(const qreal &width)
{
  int totalWidth;

  _realDuration.setWidth(_realDuration.width() + width);

  _diffRectEnd.moveLeft(_realDuration.x() + width);
  _diffRectEnd.setWidth(0);

  totalWidth = _diffRectBegin.width() + _realDuration.width() +
      _diffRectEnd.width();

  setRect(0, 0, totalWidth, rect().height());
}

void DebugObjectItem::setHeight(const qreal &height)
{
  _diffRectBegin.setHeight(height);
  _realDuration.setHeight(height);
  _diffRectEnd.setHeight(height);
  setRect(0, 0, rect().width(), height);
}

void DebugObjectItem::setSpecStartPos(const int &specStartPos)
{
  _specStartPos = specStartPos;

  qreal diff = _realStartPos - _specStartPos;
  if (diff > 0)
  {
    _diffRectBegin.moveLeft(0);
    _diffRectBegin.setWidth(diff);

    _realDuration.moveLeft(_realDuration.x() + diff);

    setRect(0, 0, rect().width() + diff, rect().height());

    setX(_specStartPos);
  }
}

void DebugObjectItem::setStartPos(const int &pos)
{
  _realStartPos = pos;
  _specStartPos = pos;

  _diffRectBegin.moveLeft(0);
  _realDuration.moveLeft(0);
  _diffRectEnd.moveLeft(0);

  setX(_realStartPos);
}

void DebugObjectItem::setSpecStopPos(const int &specPos)
{
  _specStopPos = specPos;

  qreal diff = _specStopPos - (x() + _realDuration.x() + _realDuration.width());

  if (diff)
  {
    _diffRectEnd.moveLeft(_specStopPos);
    _diffRectEnd.setWidth(qFabs(diff));

    setRect(0, 0, rect().width() + diff, rect().height());
  }
}
