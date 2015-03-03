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
  void selected (QString key);
  void droppedFile (QString path);

protected:
  void mousePressEvent(QMouseEvent *event);
  void resizeEvent(QResizeEvent* event);
  void keyPressEvent(QKeyEvent *event);

  void dragEnterEvent(QDragEnterEvent * event);
  void dragMoveEvent (QDragMoveEvent *event);
  void dropEvent (QDropEvent *event);

private:
  QnplScene* scene;
};

#endif // QNPLVIEW_H
