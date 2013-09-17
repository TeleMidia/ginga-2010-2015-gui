#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QMap>
#include <QGraphicsView>
#include <QKeyEvent>

#include "gingapage.h"
#include "page.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void keyPressEvent(QKeyEvent *);
    bool eventFilter(QObject *, QEvent *);

signals:
    void keyPressed (QString);

public slots:
    void changePage (MenuItem *);
    void changePage (Page *);
    void showGingaView ();
    void hideGingaView ();

private:
    void parsePage (QString);

    QStackedLayout *_stackedLayout;

    QMap <QString, Page*> _pages;

    QWidget *_lastPage;

    GingaProxy * _gingaProxy;

    GingaPage *_gingaPage;
};

#endif // MAINWINDOW_H
