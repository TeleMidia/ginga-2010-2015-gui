#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "pagexmlparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _stackedLayout = new QStackedLayout;
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(_stackedLayout);
    setCentralWidget(mainWidget);

    setStyleSheet("QMainWindow { border-image: url(:/backgrounds/bg_gui); }");
    showFullScreen();

    _gingaProxy = GingaProxy::getInstance(GINGA_PATH, this);
    _gingaPage = new GingaPage;

    _stackedLayout->addWidget(_gingaPage);

    connect (_gingaProxy, SIGNAL(gingaStarted()), this, SLOT(showGingaView()));
    connect (_gingaProxy, SIGNAL(gingaFinished(int,QProcess::ExitStatus)), this, SLOT(hideGingaView()));
    connect (this, SIGNAL(keyPressed(QString)), _gingaProxy, SLOT(sendCommand(QString)));

    parsePage(":/pages/main");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::parsePage(QString pagePath)
{
    PageXmlParser *pageParser = new PageXmlParser(pagePath);

    if (!pageParser->hasError()){
        Page *currentPage = new Page (0, pageParser->title(), pageParser->description(), pageParser->languague(), pageParser->items());

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
                Page *newPage = new Page ((Page *)_stackedLayout->currentWidget(), pageParser->title(),
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
    qDebug () << "gui::gingaStarted";
    _stackedLayout->setCurrentWidget(_gingaPage);
    grabKeyboard();
}

void MainWindow::hideGingaView()
{
    qDebug () << "gui::gingaFinished";
    releaseKeyboard();
}

void MainWindow::changePage(Page *page)
{
    if (page){
        _stackedLayout->setCurrentWidget(page);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() - Qt::Key_0 >= 0 && keyEvent->key() - Qt::Key_0 <= 9)
    {
        emit keyPressed("SDLK_" + QString::number(keyEvent->key() - Qt::Key_0));
    }
    else if (keyEvent->key() - Qt::Key_A >= 0 && keyEvent->key() - Qt::Key_A <= 26)
    {
        if (keyEvent->modifiers() == Qt::ShiftModifier)
            emit keyPressed("SDLK_"+QString(('A'+(keyEvent->key() - Qt::Key_A))));
        else
            emit keyPressed("SDLK_"+QString(('a'+(keyEvent->key() - Qt::Key_A))));
    }
    else if (keyEvent->key() == Qt::Key_PageDown)
    {
        emit keyPressed("SDLK_PAGEDOWN");
    }
    else if (keyEvent->key() == Qt::Key_PageUp)
    {
        emit keyPressed("SDLK_PAGEUP");
    }
    else if (keyEvent->key() - Qt::Key_F1 >= 0 && keyEvent->key() - Qt::Key_F1 <= 11)
    {
        emit keyPressed("SDLK_F"+QString::number(keyEvent->key() - Qt::Key_F1 + 1));
    }
    else if (keyEvent->key() == Qt::Key_Down)
    {
        emit keyPressed("SDLK_DOWN");
    }
    else if (keyEvent->key() == Qt::Key_Left)
    {
        emit keyPressed("SDLK_LEFT");
    }
    else if (keyEvent->key() == Qt::Key_Right)
    {
        emit keyPressed("SDLK_RIGHT");
    }
    else if (keyEvent->key() == Qt::Key_Up)
    {
        emit keyPressed("SDLK_UP");
    }
    else if (keyEvent->key() == Qt::Key_Tab)
    {
        emit keyPressed("SDLK_TAB");
    }
    else if (keyEvent->key() == Qt::Key_Space)
    {
        emit keyPressed("SDLK_SPACE");
    }
    else if (keyEvent->key() == Qt::Key_Backspace)
    {
        emit keyPressed("SDLK_BACKSPACE");
    }
    else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
    {
        emit keyPressed("SDLK_RETURN");
    }
    else if (keyEvent->key() == Qt::Key_Escape)
    {
        emit keyPressed("SDLK_QUIT");
    }

    QLabel *l = new QLabel(this);
    l->setText("oisdnwdionfc");
    l->setWindowFlags(Qt::WindowStaysOnTopHint);

    _stackedLayout->addWidget(l);
    _stackedLayout->setCurrentWidget(l);

    QMainWindow::keyPressEvent(keyEvent);
}

MainWindow::~MainWindow()
{
}
