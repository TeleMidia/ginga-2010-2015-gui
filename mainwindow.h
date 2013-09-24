#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QMap>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QFileSystemWatcher>

#include "gingapage.h"
#include "page.h"
#include "infobar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    inline WId gingaViewWId () const
    { return _gingaPage->viewWId(); }

    ~MainWindow();

public:
    void keyPressEvent(QKeyEvent *);

signals:
    void keyPressed (QString);

public slots:
    void changePage (MenuItem *);
    void changePage (Page *);
    void showGingaView ();
    void hideGingaView ();
    void analyzeDir (QString);

private:
    void parsePage (QString);

    QStackedLayout *_stackedLayout;

    QMap <QString, Page*> _pages;

    QWidget *_lastPage;

    Page * _usbPage;

    Page * _ipPage;

    GingaProxy * _gingaProxy;

    GingaPage *_gingaPage;

    QWidget *_loadingPage;

    QFileSystemWatcher *_usbPathWatcher;

    bool _lockCreated;
};

#endif // MAINWINDOW_H
