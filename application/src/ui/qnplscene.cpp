#include "qnplscene.h"

QnplScene::QnplScene(QObject* parent)
    : QGraphicsScene(parent)
{

}

QnplScene::~QnplScene()
{

}

void QnplScene::setSize(qreal w, qreal h)
{
    setSceneRect(0,0,w,h);
}
