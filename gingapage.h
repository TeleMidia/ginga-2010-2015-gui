#ifndef GINGAPAGE_H
#define GINGAPAGE_H

#include <QWidget>
#include <QGraphicsView>

#include "infobar.h"

class GingaPage : public QWidget
{
    Q_OBJECT
public:
    explicit GingaPage(QWidget *parent = 0);
    
    inline WId viewWId () const
    { return _view->winId(); }

//    inline void setBarVisible (bool visible)
//    { _infoBar->setVisible(visible); }

//    inline bool isBarVisible () const
//    { return _infoBar->isVisible(); }

//    inline void setInputInfo (QString info)
//    { _infoBar->setInputInfo(info); }

signals:
    
public slots:
    
private:
    QGraphicsView *_view;
//    InfoBar *_infoBar;
};

#endif // GINGAPAGE_H
