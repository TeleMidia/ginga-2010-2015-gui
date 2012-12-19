#ifndef QNPLVIEW_H
#define QNPLVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

#include "qnplscene.h"

class QnplView : public QGraphicsView
{
public:
    QnplView(QWidget* parent = 0);

    ~QnplView();

protected:
    void resizeEvent(QResizeEvent* event);

private:
    void createScene();

    QnplScene* scene;
};

#endif // QNPLVIEW_H
