#ifndef QNPLVIEW_H
#define QNPLVIEW_H

#include <QGraphicsView>
#include <QDebug>

#include "qnplscene.h"

class QnplView : public QGraphicsView
{
  Q_OBJECT

public:
  QnplView(QWidget* parent = 0);
  ~QnplView();

  QnplScene* getScene();

signals:
  void selected(QString key);

protected:
  void mousePressEvent(QMouseEvent *event);
  void resizeEvent(QResizeEvent* event);
  void keyPressEvent(QKeyEvent *event);

private:
  QnplScene* scene;
};

#endif // QNPLVIEW_H
