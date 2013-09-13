#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QMap>
#include <QGraphicsView>

#include "page.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changePage (MenuItem *);
    void changePage (Page *);
    void showGingaView ();
    void hideGingaView ();

private:
    void parsePage (QString);

    unsigned long int _viewWID;
    QStackedLayout *_stackedLayout;

    QMap <QString, Page*> _pages;

    QWidget *_lastPage;

    QGraphicsView *_gingaView;
};

#endif // MAINWINDOW_H
