#ifndef QNPLVIEW_H
#define QNPLVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QDebug>

#include "qnplscene.h"

#include <QKeyEvent>

class QnplView : public QGraphicsView
{
public:
    QnplView(QWidget* parent = 0);

    ~QnplView();

protected:
    void resizeEvent(QResizeEvent* event);

//    void keyPressEvent(QKeyEvent *event);

    bool winEvent(MSG *message, long *result);

private:
    void createScene();

    QnplScene* scene;
};

#endif // QNPLVIEW_H
