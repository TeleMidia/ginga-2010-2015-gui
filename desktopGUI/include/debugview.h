#ifndef DEBUGVIEW_H
#define DEBUGVIEW_H

#include <QDockWidget>
#include <QGraphicsScene>
#include <QMap>
#include <QTimer>

#include "debugobjectitem.h"

#define SCENE_WIDTH 100000
#define SCENE_HEIGHT 5000

class DebugView : public QDockWidget
{
  Q_OBJECT
public:
  explicit DebugView(QWidget *parent = 0);

  void startObject (QString);
  void stopObject (QString);
  void startSession ();
  void stopSession ();

public slots:
  void updateTimeline();

private:
  QWidget *_labelsWidget;

  QGraphicsLineItem *_currentTimeLineItem;
  QGraphicsScene *_scene;
  QMap <QString, DebugObjectItem*> _items;
  QTimer *_timer;

  int _currentX;
  int _currentY;
  int _globalTime;

  const qreal _INCREMENT;
  const qreal _ITEM_HEIGHT;
  const qreal _JUMP;
};

#endif // DEBUGVIEW_H

