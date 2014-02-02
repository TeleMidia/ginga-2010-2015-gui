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
  explicit DebugView(QGraphicsView *gingaView, QWidget *parent = 0);

  void startObject (const QVector <QString> &);
  void stopObject (const QVector <QString> &);
  void startSession ();
  void stopSession ();

public slots:
  void updateTimeline();
  void setSnapshotInterval (int);

private:
  void clearWidgetLayout (QWidget *);

  QWidget *_labelsWidget;
  QWidget *_snapshotsWidget;

  QGraphicsView *_gingaView;

  QGraphicsLineItem *_currentTimeLineItem;
  QGraphicsScene *_scene;
  QMap <QString, DebugObjectItem*> _items;
  QTimer *_timer;

  int _currentX;
  int _currentY;
  int _globalTime;
  int _snapshotInterval;

  const qreal _INCREMENT;
  const qreal _ITEM_HEIGHT;
  const qreal _VERTICAL_JUMP;
  const int _MIN_SNAPSHOT_INTERVAL;
};

#endif // DEBUGVIEW_H

