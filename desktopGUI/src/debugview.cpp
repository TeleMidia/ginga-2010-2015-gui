#include "include/debugview.h"

#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QDebug>

DebugView::DebugView(QWidget *parent) :
  QDockWidget("Debug View", parent), _INCREMENT(3), _ITEM_HEIGHT (20), _JUMP(25)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;

  _scene = new QGraphicsScene (this);

  _scene->setSceneRect(0, 0, SCENE_WIDTH,
                       SCENE_HEIGHT);

  QGraphicsView *view = new QGraphicsView(_scene);
  view->centerOn(0, 0);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  _labelsWidget = new QWidget;
  _labelsWidget->setMinimumWidth(100);
  //_labelsWidget->setLayout(new QVBoxLayout);
  _labelsWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                               QSizePolicy::Maximum);
  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->setVerticalSpacing(0);
  gridLayout->setHorizontalSpacing(0);
  _labelsWidget->setLayout(gridLayout);


  QHBoxLayout *timelineLayout = new QHBoxLayout;
  timelineLayout->addWidget(_labelsWidget, 0, Qt::AlignTop);
  timelineLayout->addWidget(view);

  mainLayout->addWidget(new QLabel ("Media objects:"));
  mainLayout->addLayout(timelineLayout);

  QWidget *debugWidget = new QWidget (this);
  debugWidget->setLayout(mainLayout);

  setWidget(debugWidget);

  _timer = new QTimer (this);
  connect (_timer, SIGNAL(timeout()), this,
           SLOT (updateTimeline()));

  _currentTimeLineItem = _scene->addLine(0, 0, 0, _scene->height());
  _currentTimeLineItem->setZValue(2);

  for (int i = 0; i < SCENE_HEIGHT; i++)
  {
    QColor color = i % 2 ? Qt::lightGray : Qt::white;
    _scene->addRect(0, i * (_JUMP), _scene->width(),
                    _JUMP, QPen(color), QBrush (color));
  }

}

void DebugView::startObject(QString object)
{
  DebugObjectItem *objectItem = new DebugObjectItem(_globalTime);
  objectItem->setPos(_currentX, _currentY - (_JUMP - _ITEM_HEIGHT)/2);

  objectItem->setHeight(_ITEM_HEIGHT);

  _scene->addItem(objectItem);

  QGridLayout *layout = (QGridLayout *) _labelsWidget->layout();

  QLabel *itemLabel = new QLabel(object);
  //itemLabel->setFixedHeight(_JUMP);

  int row = _items.count();

  layout->addWidget(itemLabel, row, 0,
                    Qt::AlignRight | Qt::AlignTop);

  layout->setRowMinimumHeight(row, _JUMP);

  _items.insertMulti(object, objectItem);
  _currentY += _JUMP;
}

void DebugView::stopObject(QString object)
{
  DebugObjectItem *currentItem = _items.value(object);
  currentItem->stop();
}

void DebugView::updateTimeline()
{
  ++ _globalTime;

  QMapIterator <QString, DebugObjectItem *> iterator (_items);
  while (iterator.hasNext())
  {
    iterator.next();
    DebugObjectItem *item = iterator.value();
    if (item->isRunning())
    {
      item->setWidth(item->width() + _INCREMENT);
    }

  }
  _currentX += _INCREMENT;
  _currentTimeLineItem->setX(_currentX);

  _scene->update();
}

void DebugView::startSession()
{
  _timer->start(1000);
  _globalTime = 0;

  QList <QGraphicsItem *> items = _scene->items();
  foreach (QGraphicsItem *item, items)
  {
    DebugObjectItem *debugObjectItem = dynamic_cast<DebugObjectItem *> (item);
    if (debugObjectItem)
    {
      _scene->removeItem(item);
      delete item;
    }
  }

  _currentX = 0;
  _currentY = _JUMP - _ITEM_HEIGHT;

  _currentTimeLineItem->setPos(0, 0);

  _items.clear();

  QLayoutItem *item = 0;
  while ((item = _labelsWidget->layout()->takeAt(0)) != 0)
  {
    delete item->widget();
    delete item;
  }

}

void DebugView::stopSession()
{
  _timer->stop();
}
