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

  void startObject (const QVector <QString> &, const bool &snapshot = true);
  void stopObject (const QVector <QString> &, const bool &snapshot = true);
  void startSession ();
  void stopSession ();

  bool eventFilter(QObject *, QEvent *);

public slots:
  void updateTimeline();
  void clearSession ();
  void takeSnapshot ();
  void showReportWidget (const bool &);
  void analyzeExecution ();

private:  
  void clearWidgetLayout (QWidget *);

  QWidget *_labelsWidget;
  QWidget *_snapshotsWidget;
  QWidget *_reportWidget;

  QGraphicsView *_gingaView;

  QGraphicsLineItem *_currentTimeLineItem;
  QGraphicsLineItem *_selectedSnapshotLineItem;
  QGraphicsRectItem *_selectedSnapshotRectItem;

  QGraphicsScene *_scene;
  QMap <QString, DebugObjectItem*> _items;
  QTimer *_timer;

  int _currentX;
  int _currentY;
  int _globalTime;

  const char * _LABEL_INSTANT;
  const qreal _INCREMENT;
  const qreal _ITEM_HEIGHT;
  const qreal _VERTICAL_JUMP;
};

#endif // DEBUGVIEW_H
