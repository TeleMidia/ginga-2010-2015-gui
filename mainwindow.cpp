#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QMovie>

#include "pagexmlparser.h"
#include "useraccountpage.h"
#include "userpreferences.h"
#include "ippage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _lockCreated = false;
    _usbPage = 0;
    _ipPage = 0;

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

    _usbPathWatcher = new QFileSystemWatcher(this);
    _usbPathWatcher->addPath(USB_XML_PARENT_DIR);

    connect (_usbPathWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(analyzeDir(QString)));

    parsePage(":/pages/main");

    if (!QFile::exists(USB_XML_FILE)){ //create USB_XML_FILE
        system (QString ("gingagui-minipc-walk /mnt " + USB_XML_FILE).toStdString().c_str());
    }

    _loadingPage = new QWidget (this);
    QMovie *movie = new QMovie(":/backgrounds/loading");
    QLabel *loadingLabel = new QLabel(this);
    loadingLabel->setMovie(movie);
    movie->setScaledSize(QSize (SCREEN_WIDTH, SCREEN_HEIGHT));
    movie->start();

    QHBoxLayout *loadingLayout = new QHBoxLayout();
    loadingLayout->addWidget(loadingLabel);
    loadingLayout->setAlignment(loadingLabel, Qt::AlignHCenter | Qt::AlignVCenter);

    _loadingPage->setLayout(loadingLayout);

    _stackedLayout->addWidget(_loadingPage);
}


void MainWindow::parsePage(QString pagePath)
{
    PageXmlParser *pageParser = new PageXmlParser(pagePath);

    if (!pageParser->hasError()){
        Page *currentPage = new Page (0, _gingaPage, pageParser->title(), pageParser->description(),
                                      pageParser->languague(), pageParser->items());

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

        Page *requestedPage = _pages.value(path);
        if (requestedPage && requestedPage != _ipPage){
            _stackedLayout->setCurrentWidget(requestedPage);
            requestedPage->updateValues();
        }
        else{
            Page *newPage = 0;

            PageXmlParser *pageParser = new PageXmlParser(path);
            if (!pageParser->hasError()){
                newPage = new Page ((Page *)_stackedLayout->currentWidget(), _gingaPage, pageParser->title(),
                                          pageParser->description(), pageParser->languague(), pageParser->items());
            }
            else {
                QStringList tokens = path.split("/", QString::SkipEmptyParts);
                QString lastToken = tokens.last();
                if (path.startsWith("exec:")){
                    QProcess *process = new QProcess (this);

                    _lastPage = _stackedLayout->currentWidget();
                    _stackedLayout->setCurrentWidget(_loadingPage);

                    QStringList args = lastToken.split(" ");

                    qDebug () << args;

                    QString program = args[0];
                    args.removeFirst();

                    process->start(program, args);

                    QEventLoop loop (this);
                    connect (process, SIGNAL(finished(int)), &loop, SLOT(quit()));
                    loop.exec();

                    delete process;

                    if (lastToken == "trydhcp"){
                        process = new QProcess(this);

                        process->start("getip");
                        connect (process, SIGNAL(finished(int)), &loop, SLOT(quit()));
                        loop.exec();

                        QString ip = QString (process->readAllStandardOutput()).trimmed();

                        qDebug () << ip;

                        delete process;

                        if (_ipPage)
                            ((IPPage *)_ipPage)->setupIp (ip);
                        else{
                            _ipPage = new IPPage ((Page *)_lastPage, ip);
                            _pages.insert(path, _ipPage);
                            _stackedLayout->addWidget(_ipPage);

                            connect (_ipPage, SIGNAL(parentPageRequested(Page*)), this, SLOT(changePage(Page*)));
                        }

                        _stackedLayout->setCurrentWidget(_ipPage);

                        _lastPage = 0;
                    }
                }
                else{
                    if (tokens.size() > 0){
                        tokens = lastToken.split("#");

                        if (tokens.size() > 1 && tokens.at(0) == "dyncontent.xml"){
                            QString pageRequired = tokens.at(1);
                            if (pageRequired == "usr-acct"){ // User account management
                                newPage = new UserAccountPage ((Page*) _stackedLayout->currentWidget());
                            }
                            else if (pageRequired == "usr-prefs"){
                                newPage = new UserPreferences ((Page *) _stackedLayout->currentWidget());
                            }
                            else if (pageRequired == "mnt-usb"){
                                delete pageParser;
                                pageParser = new PageXmlParser (USB_XML_FILE);

                                newPage = new Page ((Page *)_stackedLayout->currentWidget(), _gingaPage, pageParser->title(),
                                                    pageParser->description(), pageParser->languague(), pageParser->items());
                                _usbPage = newPage;
                            }
                        }
                    }
                }
            }
            if (newPage){
                connect (newPage, SIGNAL(configurePlay()), this, SLOT(showGingaView()));
                connect (newPage, SIGNAL(menuItemSelected(MenuItem*)), this, SLOT(changePage(MenuItem*)));
                connect (newPage, SIGNAL(parentPageRequested(Page*)), this, SLOT(changePage(Page*)));

                _pages.insert(path, newPage);
                _stackedLayout->addWidget(newPage);
                _stackedLayout->setCurrentWidget(newPage);
            }

            delete pageParser;
        }
    }
}

void MainWindow::showGingaView()
{
    qDebug () << "gui::gingaStarted";
    _lastPage = _stackedLayout->currentWidget();
    _stackedLayout->setCurrentWidget(_gingaPage);
    grabKeyboard();
}

void MainWindow::hideGingaView()
{
    qDebug () << "gui::gingaFinished";
    if (_lastPage){
        _stackedLayout->setCurrentWidget(_lastPage);
        _lastPage = 0;
    }
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
        _gingaProxy->killProcess();
    }

    QMainWindow::keyPressEvent(keyEvent);
}

void MainWindow::analyzeDir(QString dir)
{
    QDir directory (dir);
    if (directory.exists("usb.xml.lock")){
        _lockCreated = true;
        connect (_usbPathWatcher, SIGNAL(fileChanged(QString)), this, SLOT(analyzeDir(QString)));
    }
    else if (_lockCreated) {
             _lockCreated = false;
             if (_usbPage){
                 PageXmlParser *parser = new PageXmlParser (USB_XML_FILE);
                 _usbPage->setUpItems(parser->items());
                 delete parser;
             }
    }
}

MainWindow::~MainWindow()
{
}
