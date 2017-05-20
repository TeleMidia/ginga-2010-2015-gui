#include "../include/gingaview.h"
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QUrl>
#include <QMimeData>

#include "../include/util.h"

GingaView::GingaView(QWidget* parent)
  : QGraphicsView(parent)
{
  setStyleSheet("background: #000000;");

  scene = new QGraphicsScene(this);
  setScene(scene);

  setAcceptDrops(true);
}

GingaView::~GingaView()
{
}

QGraphicsScene* GingaView::getScene()
{
  return scene;
}

void GingaView::keyPressEvent(QKeyEvent *event)
{
  if (!(event->modifiers() & Qt::ControlModifier || event->modifiers() &
        Qt::AltModifier))
  {

    if (event->key() - Qt::Key_0 >= 0 && event->key() - Qt::Key_0 <= 9)
    {
      emit selected(Util::GINGA_KEY_PREFIX + QString::number(event->key() -
                                                                Qt::Key_0));
    }
    else if (event->key() - Qt::Key_A >= 0 && event->key() - Qt::Key_A <= 26)
    {
      if (event->modifiers() == Qt::ShiftModifier)
        emit selected(Util::GINGA_KEY_PREFIX + QString(('A'+(event->key()-
                                                                  Qt::Key_A))));
      else
        emit selected(Util::GINGA_KEY_PREFIX + QString(('a'+(event->key()-
                                                                  Qt::Key_A))));
    }
    else if (event->key() == Qt::Key_PageDown)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "PAGEDOWN");
    }
    else if (event->key() == Qt::Key_PageUp)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "PAGEUP");
    }
    else if (event->key() - Qt::Key_F1 >= 0 && event->key() - Qt::Key_F1 <= 11)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "F"+QString::number(event->key() -
                                                               Qt::Key_F1 + 1));
    }
    else if (event->key() == Qt::Key_Down)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "DOWN");
    }
    else if (event->key() == Qt::Key_Left)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "LEFT");
    }
    else if (event->key() == Qt::Key_Right)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "RIGHT");
    }
    else if (event->key() == Qt::Key_Up)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "UP");
    }
    else if (event->key() == Qt::Key_Tab)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "TAB");
    }
    else if (event->key() == Qt::Key_Space)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "SPACE");
    }
    else if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Back)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "BACKSPACE");
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
      emit selected(Util::GINGA_KEY_PREFIX + "RETURN");
    }
    else if (event->key() == Qt::Key_Escape)
    {
      emit selected(Util::GINGA_QUIT);
    }
  }
  QGraphicsView::keyPressEvent(event);
}

void GingaView::mousePressEvent(QMouseEvent *event)
{
  emit selected(Util::GINGA_CLICK_PREFIX + QString::number(event->x()) + "," +
                QString::number(event->y()));

  QGraphicsView::mousePressEvent(event);
}

void GingaView::dragEnterEvent(QDragEnterEvent *event)
{
  bool isAcceptable = false;
  if (event->mimeData()->hasUrls() && event->mimeData()->urls().size() == 1)
  {
    QUrl url = event->mimeData()->urls().at(0);
    if (url.toString().endsWith(".ncl"))
      isAcceptable = true;
  }

  event->setAccepted(isAcceptable);
}

void GingaView::dragMoveEvent(QDragMoveEvent *)
{
  /*
   * Do nothing. We just need to avoid this widget to
   * call the method QGraphicsView::dragMoveEvent(),
   * once that method blocks our dropEvent function.
   */
}


void GingaView::dropEvent(QDropEvent *event)
{
  foreach (const QUrl &url, event->mimeData()->urls())
  {
    const QString fileName = url.toLocalFile();
    emit droppedFile(fileName);
  }
}

void GingaView::resizeEvent(QResizeEvent* event)
{
  qreal w = event->size().width();
  qreal h = event->size().height();

  scene->setSceneRect(0,0,w,h);

  QGraphicsView::resizeEvent(event);
}
