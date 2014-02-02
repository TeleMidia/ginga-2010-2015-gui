#ifndef OBJECTDEBUGITEM_H
#define OBJECTDEBUGITEM_H

#include <QGraphicsRectItem>
#include <QTimer>

class DebugObjectItem : public QObject, public QGraphicsRectItem
{
  Q_OBJECT
public:
  explicit DebugObjectItem(QString object, int seconds,
                           QGraphicsScene *sceneParent = 0);
  inline void setWidth (qreal width)
  { setRect(0, 0, width, rect().height()); }

  inline void setHeight (qreal height)
  { setRect(0, 0, rect().width(), height); }

  inline qreal width () const
  { return rect().width(); }

  inline qreal height () const
  { return rect().height(); }

  inline bool isRunning () const
  { return _isRunning; }

  inline int startTime () const
  { return _startTime; }

  inline int stopTime () const
  { return _stopTime; }

  inline void setSpectPos (int specPos)
  {
    _specPos = specPos;
    setX(_specPos);
  }

  inline void setStartPos (int pos)
  {
    _realStartPos = pos;
    _specPos = pos;
    setX(_realStartPos);
  }

  void stop ();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

private slots:
  inline void tick ()
  { ++ _seconds; }

private:
  QString _object;

  bool _isRunning;

  int _realStartPos;
  int _seconds;
  int _startTime;
  int _stopTime;
  int _specPos;

  QTimer *_timer;
};

#endif // OBJECTDEBUGITEM_H
