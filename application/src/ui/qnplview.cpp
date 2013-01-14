#include "qnplview.h"

QnplView::QnplView(QWidget* parent)
    : QGraphicsView(parent)
{
    createScene();
    setStyleSheet("background: #000");
//    setStyleSheet("background: #000 url(\':icon/gingagui-64x64\') no-repeat center center;");
}

QnplView::~QnplView()
{

}

void QnplView::createScene()
{
    scene = new QnplScene(this); setScene(scene);
}

//void QnplView::keyPressEvent(QKeyEvent *event)
//{
//  qDebug() << "Key!!!";

//  QGraphicsView::keyPressEvent(event);
//}


bool QnplView::winEvent(MSG *message, long *result)
{
  *result = 0;
  return true;
}

void QnplView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    qreal w = event->size().width();
    qreal h = event->size().height();

    scene->setSize(w,h);
}
