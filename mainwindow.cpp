#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


#include "pagexmlparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    stackedLayout = new QStackedLayout;
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(stackedLayout);
    setCentralWidget(mainWidget);

    setStyleSheet("QMainWindow { border-image: url(:/backgrounds/bg_gui); }");
    showFullScreen();

    _gingaView = new QGraphicsView;
    stackedLayout->addWidget(_gingaView);

    _viewWID = (unsigned long int) _gingaView->winId();

    parsePage(":/pages/main");
}

void MainWindow::parsePage(QString pagePath)
{
    PageXmlParser *pageParser = new PageXmlParser(pagePath);

    if (!pageParser->hasError()){
        Page *currentPage = new Page (_viewWID, 0, pageParser->title(), pageParser->description(), pageParser->languague(), pageParser->items());

        connect (currentPage, SIGNAL(menuItemSelected(MenuItem*)), this, SLOT(changePage(MenuItem*)));
        connect (currentPage, SIGNAL(configurePlay()), this, SLOT(showGingaView()));

        _pages.insert(pagePath, currentPage);

        stackedLayout->addWidget(currentPage);
        stackedLayout->setCurrentWidget(currentPage);
    }
}

void MainWindow::changePage(MenuItem *item)
{
    if (item){
        QString path = item->link();

        if (_pages.contains(path))
            stackedLayout->setCurrentWidget(_pages.value(path));
        else{
            PageXmlParser *pageParser = new PageXmlParser(path);
            if (!pageParser->hasError()){
                Page *newPage = new Page (_viewWID, (Page *)stackedLayout->currentWidget(), pageParser->title(),
                                          pageParser->description(), pageParser->languague(), pageParser->items());

                connect (newPage, SIGNAL(configurePlay()), this, SLOT(showGingaView()));
                connect (newPage, SIGNAL(menuItemSelected(MenuItem*)), this, SLOT(changePage(MenuItem*)));
                connect (newPage, SIGNAL(parentPageRequested(Page*)), this, SLOT(changePage(Page*)));


                _pages.insert(path, newPage);
                stackedLayout->addWidget(newPage);
                stackedLayout->setCurrentWidget(newPage);
            }
        }
    }
}

void MainWindow::showGingaView()
{
    stackedLayout->setCurrentWidget(_gingaView);
}

void MainWindow::changePage(Page *page)
{
    if (page){
        stackedLayout->setCurrentWidget(page);
    }
}

MainWindow::~MainWindow()
{
}


