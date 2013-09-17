#ifndef GINGAPAGE_H
#define GINGAPAGE_H

#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>

class GingaPage : public QWidget
{
    Q_OBJECT
public:
    explicit GingaPage(QWidget *parent = 0);
    
    inline WId viewWId () const
    { return _view->winId(); }

signals:
    
public slots:
    
private:
    QGraphicsView *_view;
    QPushButton *_test;
};

#endif // GINGAPAGE_H
