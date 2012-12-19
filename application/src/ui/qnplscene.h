#ifndef QNPLSCENE_H
#define QNPLSCENE_H

#include <QGraphicsScene>

class QnplScene : public QGraphicsScene
{
public:
    QnplScene(QObject* parent = 0);

    ~QnplScene();

    void setSize(qreal w, qreal h);
};

#endif // QNPLSCENE_H
