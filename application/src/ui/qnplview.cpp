#include "qnplview.h"

QnplView::QnplView(QWidget* parent)
  : QGraphicsView(parent)
{
  setStyleSheet("background: #000000;");

  scene = new QnplScene(this);
  setScene(scene);
}

QnplView::~QnplView()
{

}

QnplScene* QnplView::getScene()
{
    return scene;
}

void QnplView::keyPressEvent(QKeyEvent *event)
{
  if (event->key() - Qt::Key_0 >= 0 && event->key() - Qt::Key_0 <= 9)
  {
    emit keyPressed("SDLK_"+QString::number(event->key() - Qt::Key_0));
  }
  else if (event->key() - Qt::Key_A >= 0 && event->key() - Qt::Key_A <= 26)
  {
    if (event->modifiers() == Qt::ShiftModifier)
      emit keyPressed("SDLK_"+QString(('A'+(event->key() - Qt::Key_A))));
    else
      emit keyPressed("SDLK_"+QString(('a'+(event->key() - Qt::Key_A))));
  }
  else if (event->key() == Qt::Key_PageDown)
  {
    emit keyPressed("SDLK_PAGEDOWN");
  }
  else if (event->key() == Qt::Key_PageUp)
  {
    emit keyPressed("SDLK_PAGEUP");
  }
  else if (event->key() - Qt::Key_F1 >= 0 && event->key() - Qt::Key_F1 <= 11)
  {
    emit keyPressed("SDLK_F"+QString::number(event->key() - Qt::Key_F1 + 1));
  }
  else if (event->key() == Qt::Key_Down)
  {
    emit keyPressed("SDLK_DOWN");
  }
  else if (event->key() == Qt::Key_Left)
  {
    emit keyPressed("SDLK_LEFT");
  }
  else if (event->key() == Qt::Key_Right)
  {
    emit keyPressed("SDLK_RIGHT");
  }
  else if (event->key() == Qt::Key_Up)
  {
    emit keyPressed("SDLK_UP");
  }
  else if (event->key() == Qt::Key_Tab)
  {
    emit keyPressed("SDLK_TAB");
  }
  else if (event->key() == Qt::Key_Space)
  {
    emit keyPressed("SDLK_SPACE");
  }
  else if (event->key() == Qt::Key_Backspace)
  {
    emit keyPressed("SDLK_BACKSPACE");
  }
  else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    emit keyPressed("SDLK_RETURN");
  }

  QGraphicsView::keyPressEvent(event);
}

void QnplView::resizeEvent(QResizeEvent* event)
{
  QGraphicsView::resizeEvent(event);

  qreal w = event->size().width();
  qreal h = event->size().height();

  scene->setSize(w,h);
}
