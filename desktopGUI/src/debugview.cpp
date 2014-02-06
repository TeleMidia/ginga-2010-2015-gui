#include "include/debugview.h"

#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QGraphicsProxyWidget>
#include <QBuffer>

#include "util.h"

#include <QDebug>

DebugView::DebugView(QGraphicsView* gingaView, QWidget *parent) :
  QDockWidget("Debug View", parent), _INCREMENT(10), _ITEM_HEIGHT (20),
  _VERTICAL_JUMP(25), _LABEL_INSTANT("instant")
{  
  _gingaView = gingaView;

  QVBoxLayout *mainLayout = new QVBoxLayout;

  _scene = new QGraphicsScene (this);

  _scene->setSceneRect(0, 0, SCENE_WIDTH,
                       SCENE_HEIGHT);

  QGraphicsView * viewr = new QGraphicsView(_scene);
  viewr->centerOn(0, 0);
  viewr->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  viewr->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

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
  timelineLayout->addWidget(viewr);

  _snapshotsWidget = new QWidget;
  _snapshotsWidget->setLayout(new QHBoxLayout);
  _snapshotsWidget->setStyleSheet("background-color:white;");
  _snapshotsWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  _snapshotsWidget->installEventFilter(this);

  QGraphicsScene *snapshotScene = new QGraphicsScene(0, 0,
                                                     _scene->width(), 150,
                                                     this);

  snapshotScene->addWidget(_snapshotsWidget)->setPos(0, 0);
  snapshotScene->installEventFilter(this);

  QGraphicsView *snapshotView = new QGraphicsView(snapshotScene);
  snapshotView->centerOn(0, 0);
  snapshotView->setFixedHeight(snapshotScene->height());
  snapshotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QPushButton *viewReportButton = new QPushButton ("&View Execution Report");
  viewReportButton->setCheckable(true);
  viewReportButton->setDefault(true);
  viewReportButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QTextEdit *reportTextEdit = new QTextEdit;
  reportTextEdit->setReadOnly(true);

  QVBoxLayout *reportLayout = new QVBoxLayout;
  reportLayout->addWidget(new QLabel ("Execution Report:"));
  reportLayout->addWidget(reportTextEdit);

  _reportWidget = new QWidget;
  _reportWidget->setLayout(reportLayout);
  _reportWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
                               QSizePolicy::MinimumExpanding);
  _reportWidget->hide();

  mainLayout->addWidget(new QLabel ("Media objects:"));
  mainLayout->addLayout(timelineLayout);
  mainLayout->addWidget(new QLabel ("Snapshots:"));
  mainLayout->addWidget(snapshotView);
  mainLayout->addWidget(viewReportButton);
  mainLayout->addWidget(_reportWidget);

  QWidget *debugWidget = new QWidget (this);
  debugWidget->setLayout(mainLayout);

  setWidget(debugWidget);

  _timer = new QTimer (this);
  connect (_timer, SIGNAL(timeout()), this,
           SLOT (updateTimeline()));

  _currentTimeLineItem = _scene->addLine(0, 0, 0, _scene->height());
  _currentTimeLineItem->setZValue(2);

  _selectedSnapshotLineItem = _scene->addLine(0, 0, 0, _scene->height());
  _selectedSnapshotLineItem->setZValue(2);

  _selectedSnapshotRectItem = snapshotScene->addRect(0, 0, 100, 150);
  _selectedSnapshotRectItem->setZValue(2);
  _selectedSnapshotRectItem->hide();

  QPen pen (Qt::blue);
  pen.setWidth(2);
  _selectedSnapshotLineItem->setPen(pen);
  _selectedSnapshotLineItem->hide();

  for (int i = 0; i < SCENE_HEIGHT; i++)
  {
    QColor color = i % 2 ? Qt::lightGray : Qt::white;
    _scene->addRect(0, i * (_VERTICAL_JUMP), _scene->width(),
                    _VERTICAL_JUMP, QPen(color), QBrush (color));
  }

  connect(viewReportButton, SIGNAL(toggled(bool)),
          SLOT(showReportWidget(bool)));
}

void DebugView::startObject(const QVector <QString> &data, const bool &snapshot)
{
  qDebug () << data;

  QString object = data.at(0);
  DebugObjectItem *objectItem = new DebugObjectItem(object, _globalTime,
                                                    _INCREMENT);


  DebugObjectItem *retrievedItem = _items.value(object, 0);

  objectItem->setStartPos(_currentX);
  objectItem->setY( retrievedItem ? retrievedItem->y() :
                                    _currentY - (_VERTICAL_JUMP -
                                                 _ITEM_HEIGHT)/2);
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

  if (!retrievedItem)
  {
    QGridLayout *layout = (QGridLayout *) _labelsWidget->layout();
    QLabel *itemLabel = new QLabel(object);

    int row = _items.count();

    layout->addWidget(itemLabel, row, 0,
                      Qt::AlignRight | Qt::AlignTop);

    layout->setRowMinimumHeight(row, _VERTICAL_JUMP);

    _currentY += _VERTICAL_JUMP;
  }

  _items.insert(object, objectItem);

  qDebug () << "startObject " << snapshot;
  if (snapshot)
    takeSnapshot();
}

void DebugView::stopObject(const QVector <QString> & data, const bool &snapshot)
{
  qDebug () << data;

  QString object = data.at(0);

  DebugObjectItem *currentItem = _items.value(object);
  if (!currentItem) return;

  currentItem->stop();

  if (data.size() == 4)
  {
    QString role = data.at(1);
    QString refObject = data.at(2);
    int specTime = data.at(3).toInt();

    DebugObjectItem *refObjectItem = _items.value(refObject, 0);
    if (refObjectItem)
    {
      int posIncrement = 0;

      if (specTime != -1)
      {
        specTime /= 1000;
        posIncrement = specTime * _INCREMENT;
      }

      int refObjectItemX = refObjectItem->x() +
          refObjectItem->realDurationRectX();

      if (role == Util::G_ON_BEGIN)
        currentItem->setSpecStopPos(refObjectItemX + posIncrement);
      else if (role == Util::G_ON_END)
        currentItem->setSpecStopPos(specTime == -1 ? refObjectItemX +
                                                     refObjectItem->
                                                     realDurationRectWidth()
                                                   :
                                                     refObjectItemX +
                                                     posIncrement);
    }
  }

  if (snapshot)
    takeSnapshot();
}

void DebugView::takeSnapshot()
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
    snapshotPixmap .scaledToHeight(250).toImage().save(&buffer, "PNG");

    QString htmlImgHover = QString("<img src=\"data:image/png;base64,%1\">").
        arg(QString(buffer.data().toBase64()));
    snapshotLabel->setToolTip(htmlImgHover);

    QWidget *snapshot = new QWidget;
    snapshot->setLayout(snapshotLayout);
    snapshot->setProperty(_LABEL_INSTANT, QVariant(_globalTime));

    QHBoxLayout *layout = (QHBoxLayout *) _snapshotsWidget->layout();
    layout->addWidget(snapshot, 0, Qt::AlignLeft);
  }
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
  clearSession();
}

void DebugView::clearSession()
{
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

  _selectedSnapshotLineItem->hide();
  _selectedSnapshotRectItem->hide();

  ((QTextEdit *)(_reportWidget->findChild<QTextEdit *> ()))->clear();
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

  QMapIterator <QString, DebugObjectItem *> iterator (_items);
  while (iterator.hasNext())
  {
    iterator.next();
    DebugObjectItem *item = iterator.value();
    item->stopTimer();
  }

  /*
   * _globalTime diferent than 0 indicates that
   * there was a started session before this call.
   */
  if (_globalTime)
    analyzeExecution();

  _globalTime = 0;
}

bool DebugView::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
    QMouseEvent *mouseEvent = (QMouseEvent *) event;

    QWidget *widget = qobject_cast <QWidget *> (obj);
    if (widget)
    {
      QWidget *selectedSnapshot = widget->childAt(mouseEvent->pos());

      if (selectedSnapshot)
      {
        selectedSnapshot = qobject_cast <QWidget *>
            (selectedSnapshot->parent());
        bool ok;
        int time = selectedSnapshot->property(_LABEL_INSTANT).toInt(&ok);

        if (ok)
        {
          _selectedSnapshotLineItem->show();
          _selectedSnapshotLineItem->setX(time * _INCREMENT);

          _selectedSnapshotRectItem->setPos(selectedSnapshot->x(), 10);
          _selectedSnapshotRectItem->setRect(0, 0, selectedSnapshot->width(),
                                          selectedSnapshot->height());

          _selectedSnapshotRectItem->show();

          QGraphicsView *view = _scene->views().at(0);
          view->centerOn(_selectedSnapshotLineItem->x(), 0);
        }
        else
        {
          _selectedSnapshotLineItem->hide();
          _selectedSnapshotRectItem->hide();
        }
      }
    }
    event->accept();
    return true;
  }
  else if (event->type() == QEvent::GraphicsSceneMousePress)
    _selectedSnapshotLineItem->hide();

  return QDockWidget::eventFilter(obj, event);
}

void DebugView::showReportWidget(const bool &checked)
{
  _reportWidget->setVisible(checked);
}

void DebugView::analyzeExecution()
{
  QTextEdit *reportTextEdit = _reportWidget->findChild <QTextEdit *>();
  if (!reportTextEdit) return;


  QString reportSeparator = "============================================";
  QList <QString> objectsInExecution;
  QList <DebugObjectItem *> objectsBeginDelayed;

  QMapIterator <QString, DebugObjectItem *> iterator (_items);
  while (iterator.hasNext())
  {
    iterator.next();
    QString object = iterator.key();
    DebugObjectItem *item = iterator.value();

    if (item->isRunning())
      objectsInExecution.append(object);

    if (item->diffRectBeginWidth())
      objectsBeginDelayed.append(item);
  }

  QString warningTemplate = "<font color=\"red\">%1</font>";

  QString str = "";
  if (objectsInExecution.count() == 0)
    str += "All objects were stopped before the application end.";
  else
  {
    reportTextEdit->append(warningTemplate.arg("Objects in execution on "
                                               "application end:"));

    for (int i = 0; i < objectsInExecution.size(); i++)
      if (i == 0)
        str += warningTemplate.arg(objectsInExecution.at(i));
      else
        str += warningTemplate.arg(", " + objectsInExecution.at(i));
  }

  reportTextEdit->append(str);
  reportTextEdit->append(reportSeparator);

  str = "";
  if (objectsBeginDelayed.count() == 0)
    reportTextEdit->append("No object started with delay.");
  else
  {
    reportTextEdit->append(warningTemplate.arg("Objects with delay in "
                                               "beginning:"));
    foreach (DebugObjectItem *item, objectsBeginDelayed)
    {
      str = warningTemplate.arg(item->object() +
                                " : " + Util::secondsToString(
                                  item->diffRectBeginWidth()/_INCREMENT));
      reportTextEdit->append(str);
    }
  }
}
