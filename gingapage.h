#ifndef GINGAPAGE_H
#define GINGAPAGE_H

#include <QWidget>
#include <QGraphicsView>
#include <QLabel>

#include "infobar.h"

class GingaPage : public QWidget
{
    Q_OBJECT
public:
    explicit GingaPage(QWidget *parent = 0);
    
    inline WId viewWId () const
    { return _view->winId(); }

    void setBarVisible (bool visible);

    inline bool isBarActive () const
    { return _infoBar->isActive(); }

    inline void setInputInfo (QString info)
    { _infoBar->setInputInfo(info); }

signals:
    
public slots:
    
private:
    QGraphicsView *_view;
    InfoBar *_infoBar;
};

#endif // GINGAPAGE_H
