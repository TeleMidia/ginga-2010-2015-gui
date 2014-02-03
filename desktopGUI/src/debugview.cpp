#include "include/debugview.h"

#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QGraphicsProxyWidget>
#include <QSpinBox>
#include <QBuffer>

#include "util.h"

#include <QDebug>

DebugView::DebugView(QGraphicsView* gingaView, QWidget *parent) :
  QDockWidget("Debug View", parent), _INCREMENT(10), _ITEM_HEIGHT (20),
  _VERTICAL_JUMP(25), _MIN_SNAPSHOT_INTERVAL (5)
{
  _snapshotInterval = _MIN_SNAPSHOT_INTERVAL;
  _gingaView = gingaView;

  QVBoxLayout *mainLayout = new QVBoxLayout;

  _scene = new QGraphicsScene (this);

  _scene->setSceneRect(0, 0, SCENE_WIDTH,
                       SCENE_HEIGHT);

  QGraphicsView * view = new QGraphicsView(_scene);
  view->centerOn(0, 0);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  _labelsWidget = new QWidget;
  _labelsWidget->setMinimumWidth(100);
  _labelsWidget->setSizePolicy(QSizePolicy::Minimum,
                               QSizePolicy::Expanding);

  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->setVerticalSpacing(0);
  gridLayout->setHorizontalSpacing(0);
  _labelsWidget->setLayout(gridLayout);

  QHBoxLayout *timelineLayout = new QHBoxLayout;
  timelineLayout->addWidget(_labelsWidget, 0, Qt::AlignTop);
  timelineLayout->addWidget(view);

  _snapshotsWidget = new QWidget;
  _snapshotsWidget->setLayout(new QHBoxLayout);
  _snapshotsWidget->setStyleSheet("background-color:white;");
  _snapshotsWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QGraphicsScene *snapshotScene = new QGraphicsScene(0, 0,
                                                     _scene->width(), 150,
                                                     this);

  snapshotScene->addWidget(_snapshotsWidget)->setPos(0, 0);

  QGraphicsView *snapshotView = new QGraphicsView(snapshotScene);
  snapshotView->centerOn(0, 0);
  snapshotView->setMinimumHeight(snapshotScene->height());
  snapshotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QLabel *snapshotIntervalLabel = new QLabel ("Snapshot interval:");
  QSpinBox *snapshotSpinBox = new QSpinBox;
  snapshotSpinBox->setMinimum(_MIN_SNAPSHOT_INTERVAL);
  snapshotSpinBox->setValue(_snapshotInterval);

  QVBoxLayout *snapshotIntervalLayout = new QVBoxLayout;
  snapshotIntervalLayout->addWidget(snapshotIntervalLabel, 0, Qt::AlignLeft);
  snapshotIntervalLayout->addWidget(snapshotSpinBox, 0, Qt::AlignLeft);

  QWidget *snapshotChangeWidget = new QWidget;
  snapshotChangeWidget->setLayout(snapshotIntervalLayout);

  mainLayout->addWidget(new QLabel ("Media objects:"));
  mainLayout->addLayout(timelineLayout);
  mainLayout->addWidget(new QLabel ("Snapshots:"));
  mainLayout->addWidget(snapshotView);
  mainLayout->addWidget(snapshotChangeWidget);

  QWidget *debugWidget = new QWidget (this);
  debugWidget->setLayout(mainLayout);

  setWidget(debugWidget);

  _timer = new QTimer (this);
  connect (_timer, SIGNAL(timeout()), this,
           SLOT (updateTimeline()));

  connect (snapshotSpinBox, SIGNAL(valueChanged(int)),
           SLOT(setSnapshotInterval(int)));

  _currentTimeLineItem = _scene->addLine(0, 0, 0, _scene->height());
  _currentTimeLineItem->setZValue(2);

  for (int i = 0; i < SCENE_HEIGHT; i++)
  {
    QColor color = i % 2 ? Qt::lightGray : Qt::white;
    _scene->addRect(0, i * (_VERTICAL_JUMP), _scene->width(),
                    _VERTICAL_JUMP, QPen(color), QBrush (color));
  }

}

void DebugView::startObject(const QVector <QString> &data)
{
  qDebug () << data;

  QString object = data.at(0);
  DebugObjectItem *objectItem = new DebugObjectItem(object, _globalTime,
                                                    _INCREMENT);

  objectItem->setStartPos(_currentX);
  objectItem->setY(_currentY - (_VERTICAL_JUMP - _ITEM_HEIGHT)/2);
  objectItem->setHeight(_ITEM_HEIGHT);

  if (data.size() == 4)
  {
    QString role = data.at(1);
    QString refObject = data.at(2);
    int specTime = data.at(3).toInt();

    DebugObjectItem *refObjectItem = _items.value(refObject, 0);
    if (refObjectItem)
    {
      specTime /= 1000;

      qDebug () << objectItem->x() << " " <<
                   refObjectItem->x() + specTime * _INCREMENT;

      int diffIncrement = specTime * _INCREMENT;

      if (role == Util::G_ON_BEGIN)
        objectItem->setSpecStartPos(refObjectItem->x() + diffIncrement);
      else if (role == Util::G_ON_END)
        objectItem->setSpecStartPos(specTime == -1 ? refObjectItem->x() +
                                                 refObjectItem->width()
                                               :
                                                 refObjectItem->x() +
                                                 refObjectItem->width() +
                                                 specTime * _INCREMENT );
    }
  }

  _scene->addItem(objectItem);

  QGridLayout *layout = (QGridLayout *) _labelsWidget->layout();

  QLabel *itemLabel = new QLabel(object);

  int row = _items.count();

  layout->addWidget(itemLabel, row, 0,
                    Qt::AlignRight | Qt::AlignTop);

  layout->setRowMinimumHeight(row, _VERTICAL_JUMP);

  _items.insertMulti(object, objectItem);
  _currentY += _VERTICAL_JUMP;
}

void DebugView::stopObject(const QVector <QString> & data)
{
  qDebug () << data;


  QString object = data.at(0);

  DebugObjectItem *currentItem = _items.value(object);

  if (data.size() == 4)
  {
    QString role = data.at(1);
    QString refObject = data.at(2);
    int specTime = data.at(3).toInt();

    DebugObjectItem *refObjectItem = _items.value(refObject, 0);
    if (refObjectItem)
    {
      if (specTime != -1)
        specTime /= 1000;

      qDebug () << "stop specTime: " << specTime;

      int posIncrement = specTime * _INCREMENT;

      if (role == Util::G_ON_BEGIN)
        currentItem->setSpecStopPos(refObjectItem->x() + posIncrement);
      else if (role == Util::G_ON_END)
        currentItem->setSpecStopPos(specTime == -1 ? refObjectItem->x() +
                                                 refObjectItem->width()
                                               :
                                                 refObjectItem->x() +
                                                 refObjectItem->width() +
                                                 specTime * _INCREMENT );
    }
  }

  currentItem->stop();
}

void DebugView::updateTimeline()
{
  if (!(_globalTime % _snapshotInterval))
  {
    QPixmap snapshotPixmap = QPixmap::grabWindow(_gingaView->winId());
    if (!snapshotPixmap.isNull())
    {
      QLabel *snapshotLabel = new QLabel;
      snapshotLabel->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);

      QLabel *snapshotTime = new QLabel(Util::secondsToString(_globalTime));

      QVBoxLayout *snapshotLayout = new QVBoxLayout;
      snapshotLayout->addWidget(snapshotLabel);
      snapshotLayout->addWidget(snapshotTime, 0, Qt::AlignCenter);

      QPixmap p = snapshotPixmap.scaledToWidth(100);
      snapshotLabel->setPixmap(p);
      snapshotLabel->setFixedSize(p.size());
      snapshotLabel->setStyleSheet("background-color:white;");

      QByteArray byteArray;
      QBuffer buffer(&byteArray);
      buffer.open(QIODevice::WriteOnly);
      p.scaledToHeight(250).toImage().save(&buffer, "PNG");

      QString htmlImgHover = QString("<img src=\"data:image/png;base64,%1\">").
          arg(QString(buffer.data().toBase64()));
      snapshotLabel->setToolTip(htmlImgHover);

      QWidget *snapshot = new QWidget;
      snapshot->setLayout(snapshotLayout);

      QHBoxLayout *layout = (QHBoxLayout *) _snapshotsWidget->layout();
      layout->addWidget(snapshot, 0, Qt::AlignLeft);

    }
  }

  ++ _globalTime;

  QMapIterator <QString, DebugObjectItem *> iterator (_items);
  while (iterator.hasNext())
  {
    iterator.next();
    DebugObjectItem *item = iterator.value();
    if (item->isRunning())
    {
      item->incrementWidth(_INCREMENT);
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

  _scene->update();

  _currentX = 0;
  _currentY = _VERTICAL_JUMP - _ITEM_HEIGHT;

  _currentTimeLineItem->setPos(0, 0);

  _items.clear();

  clearWidgetLayout(_labelsWidget);
  clearWidgetLayout(_snapshotsWidget);

  _labelsWidget->adjustSize();
  _snapshotsWidget->adjustSize();
}

void DebugView::clearWidgetLayout(QWidget *widget)
{
  QLayoutItem *item = 0;
  while ((item = widget->layout()->takeAt(0)) != 0)
  {
    delete item->widget();
    delete item;
  }
}

void DebugView::stopSession()
{
  _timer->stop();
}

void DebugView::setSnapshotInterval(int value)
{
  if (value >= _MIN_SNAPSHOT_INTERVAL)
    _snapshotInterval = value;
}
