#ifndef OBJECTDEBUGITEM_H
#define OBJECTDEBUGITEM_H

#include <QGraphicsRectItem>
#include <QTimer>
#include <QRect>

class DebugObjectItem : public QObject, public QGraphicsRectItem
{
  Q_OBJECT
public:
  explicit DebugObjectItem(const QString &object, int seconds, const int &increment,
                           QGraphicsScene *sceneParent = 0);

  void incrementWidth (const qreal &width);

  void setHeight (const qreal &height);

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

  inline int realDurationRectX () const
  { return _realDuration.x(); }

  inline int realDurationRectWidth () const
  { return _realDuration.width(); }

  void setSpecStopPos (const int &specPos);
  void setSpecStartPos (const int &specPos);
  void setStartPos (const int &pos);

  void stop ();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

private slots:
  inline void tick ()
  { ++ _seconds; }

private:
  const QString _object;
  const int _increment;

  QRect _diffRectBegin;
  QRect _realDuration;
  QRect _diffRectEnd;

  bool _isRunning;

  int _realStartPos;
  int _seconds;
  int _startTime;
  int _stopTime;
  int _specStartPos;
  int _specStopPos;

  QTimer *_timer;
};

#endif // OBJECTDEBUGITEM_H
