#ifndef QNPLVIEW_H
#define QNPLVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QDebug>

#include "qnplscene.h"

#include <QKeyEvent>

class QnplView : public QGraphicsView
{
  Q_OBJECT
public:
    QnplView(QWidget* parent = 0);

    ~QnplView();

signals:
    void keyPressed(QString key);

protected:
    void resizeEvent(QResizeEvent* event);

    void keyPressEvent(QKeyEvent *event);

private:
    void createScene();

    QnplScene* scene;
};

#endif // QNPLVIEW_H
