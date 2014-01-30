#ifndef OBJECTDEBUGITEM_H
#define OBJECTDEBUGITEM_H

#include <QGraphicsRectItem>
#include <QTimer>

class DebugObjectItem : public QObject, public QGraphicsRectItem
{
  Q_OBJECT
public:
  explicit DebugObjectItem(int seconds, QGraphicsScene *sceneParent = 0);
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

  void stop ();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

private slots:
  inline void tick ()
  { ++ _seconds; }

private:
  bool _isRunning;
  int _seconds;
  int _startTime;

  QTimer *_timer;
};

#endif // OBJECTDEBUGITEM_H
