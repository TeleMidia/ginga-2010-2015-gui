#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QDebug>

class GingaView : public QGraphicsView
{
  Q_OBJECT

public:
  GingaView(QWidget* parent = 0);
  ~GingaView();

  QGraphicsScene* getScene();

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
  QGraphicsScene* scene;
};

#endif // VIEW_H
