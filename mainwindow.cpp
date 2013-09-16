#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


#include "pagexmlparser.h"

void f (WId winid){
    void* vhwnd = (void*) winid;
    unsigned long int value = (unsigned long int) vhwnd;

    char dst[32];
    char digits[32];
    unsigned long int i = 0, j = 0, n = 0;

    do {
        n = value % 10;
        digits[i++] = (n < 10 ? (char)n+'0' : (char)n-10+'a');
        value /= 10;

        if (i > 31) {
            break;
        }

    } while (value != 0);

    n = i;
    i--;

    while (i >= 0 && j < 32) {
        dst[j] = digits[i];
        i--;
        j++;
    }
    qDebug () << "f(): " << QString(dst);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _stackedLayout = new QStackedLayout;
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(_stackedLayout);
    setCentralWidget(mainWidget);

    setStyleSheet("QMainWindow { border-image: url(:/backgrounds/bg_gui); }");
    showFullScreen();

    connect (GingaProxy::getInstance(GINGA_PATH, this), SIGNAL(gingaStarted()), this, SLOT(showGingaView()));
    connect (GingaProxy::getInstance(GINGA_PATH), SIGNAL(gingaFinished(int,QProcess::ExitStatus)), this, SLOT(hideGingaView()));

    _gingaView = new QGraphicsView;
    _stackedLayout->addWidget(_gingaView);

    _viewWID = (unsigned long int) _gingaView->winId();


    qDebug () << "effectiveWinId(): " << _gingaView->effectiveWinId();
    qDebug () << "winId(): " << _gingaView->winId();
    qDebug () << "_viewWID: " << _viewWID;
    f(_gingaView->winId());

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

        _stackedLayout->addWidget(currentPage);
        _stackedLayout->setCurrentWidget(currentPage);
    }
}

void MainWindow::changePage(MenuItem *item)
{
    if (item){
        QString path = item->link();

        if (_pages.contains(path))
            _stackedLayout->setCurrentWidget(_pages.value(path));
        else{
            PageXmlParser *pageParser = new PageXmlParser(path);
            if (!pageParser->hasError()){
                Page *newPage = new Page (_viewWID, (Page *)_stackedLayout->currentWidget(), pageParser->title(),
                                          pageParser->description(), pageParser->languague(), pageParser->items());

                connect (newPage, SIGNAL(configurePlay()), this, SLOT(showGingaView()));
                connect (newPage, SIGNAL(menuItemSelected(MenuItem*)), this, SLOT(changePage(MenuItem*)));
                connect (newPage, SIGNAL(parentPageRequested(Page*)), this, SLOT(changePage(Page*)));


                _pages.insert(path, newPage);
                _stackedLayout->addWidget(newPage);
                _stackedLayout->setCurrentWidget(newPage);
            }
        }
    }
}

void MainWindow::showGingaView()
{
    qDebug () << "gingaStarted";

    _lastPage = _stackedLayout->currentWidget();
    _stackedLayout->setCurrentWidget(_gingaView);
}

void MainWindow::hideGingaView()
{
    qDebug () << "gingaFinished";
    if (_lastPage){
        _stackedLayout->setCurrentWidget(_lastPage);
        _lastPage = 0;
    }
}

void MainWindow::changePage(Page *page)
{
    if (page){
        _stackedLayout->setCurrentWidget(page);
    }
}


MainWindow::~MainWindow()
{
}


