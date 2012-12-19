#include "qnplview.h"

QnplView::QnplView(QWidget* parent)
    : QGraphicsView(parent)
{
    createScene(); setStyleSheet("background: #000 url(\':background/gingagui4\') no-repeat center center;");
}

QnplView::~QnplView()
{

}

void QnplView::createScene()
{
    scene = new QnplScene(this); setScene(scene);
}

void QnplView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    qreal w = event->size().width();
    qreal h = event->size().height();

    scene->setSize(w,h);
}
