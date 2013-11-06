
#include "qnplmainwindow.h"

#include <QLayout>
#include <QDir>
#include <QDebug>
#include <QProcessEnvironment>
#include <QMovie>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>

QnplMainWindow::QnplMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Ginga GUI");
    setWindowIcon(QIcon(":icon/gingagui-128x128"));
    settings = new QnplSettings();

    _gingaProxy = GingaProxy::getInstance();
    _isChannel = false;

    createActions();
    createMenus();
    createRecent();
    createWidgets();
    createDialogs();
    createToolbars();
    createConnections();

    _location = "";
    _process = NULL;
    _timer = NULL;

    _scanProgress = new QProgressDialog ("Scanning channels...", "Abort", 0, 100, this);
    _scanProgress->setWindowFlags(Qt::Dialog | Qt::Desktop);
    _scanProgress->setWindowTitle("Scanning");
    _scanProgress->setWindowIcon(windowIcon());

    _baseAction->setChecked(true);

    _passiveIsRunning = false;

    QString ssize = settings->value("screensize").toString();

    qDebug() << "open" << ssize;

    QString sw = ssize.section('x',0,0);
    QString sh = ssize.section('x',1,1);

    int w = sw.toInt();
    int h = sh.toInt();

    _view->setSceneRect(0,0,w,h);

    _gifLabel = new QLabel();
    _movie = new QMovie(":background/anim-tuning");

    if (!_movie->isValid()){
        qDebug () << "Cannot find tunning gif";
    }

    _gifLabel->setMovie(_movie);

    _movie->start();
    _animTuning = _view->getScene()->addWidget(_gifLabel);
    _animTuning->setVisible(false);
    _gifLabel->setVisible(false);

    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    resize(w+20, h+100);

    _developerView = new DeveloperView;
    addDockWidget(Qt::RightDockWidgetArea, _developerView);

    _developerView->setVisible(false);
    connect (_developerView, SIGNAL(sendCommandRequested(QString)), _gingaProxy, SLOT(sendCommand(QString)));
}

QnplMainWindow::~QnplMainWindow()
{
    performStop();
}

void QnplMainWindow::performDevice()
{
    if (_baseAction->isChecked())
    {
        settings->setValue("run_as", "base");
    }
    else if (_passiveAction->isChecked())
    {
        settings->setValue("run_as", "passive");
    }
    else if (_activeAction->isChecked())
    {
        settings->setValue("run_as", "active");
    }
}

void  QnplMainWindow::createActions()
{
    // open action
    _openAction = new QAction(this);
    _openAction->setEnabled(true);
    _openAction->setText(tr("Open..."));
    _openAction->setShortcut(QKeySequence("Ctrl+O"));

    _tuneAppChannellAction = new QAction(this);
    _tuneAppChannellAction->setEnabled(false);
    _tuneAppChannellAction->setText(tr("Tune Application Channel..."));

    _tuneBroadChannellAction = new QAction(this);
    _tuneBroadChannellAction->setEnabled(true);
    _tuneBroadChannellAction->setText(tr("Tune Broadcast Channel..."));

    _tuneIPTVChannellAction = new QAction(this);
    _tuneIPTVChannellAction->setEnabled(true);
    _tuneIPTVChannellAction->setText(tr("Tune IPTV Channel..."));


    // quit action
    _quitAction = new QAction(this);
    _quitAction->setEnabled(true);
    _quitAction->setText(tr("Quit"));

    // clear action
    _clearAction = new QAction(this);
    _clearAction->setEnabled(true);
    _clearAction->setText(tr("Clear Menu"));

    // base action
    _baseAction = new QAction(this);
    _baseAction->setEnabled(true);
    _baseAction->setCheckable(true);
    _baseAction->setText(tr("Default (Base)"));

    // passive action
    _passiveAction = new QAction(this);
    _passiveAction->setEnabled(true);
    _passiveAction->setCheckable(true);
    _passiveAction->setText(tr("Passive"));
    connect (_passiveAction, SIGNAL(triggered()), this, SLOT(performRunAsPassive()));

    // active action
    _activeAction = new QAction(this);
    _activeAction->setEnabled(true);
    _activeAction->setCheckable(true);
    _activeAction->setText(tr("Active"));
    connect (_activeAction, SIGNAL(triggered()), this, SLOT(performRunAsActive()));

    // preferences action
    _preferencesAction = new QAction(this);
    _preferencesAction->setEnabled(true);
    _preferencesAction->setText(tr("Preferences..."));

    // bug action
    _bugAction = new QAction(this);
    _bugAction->setEnabled(true);
    _bugAction->setText(tr("Report Bug..."));

    // about action
    _aboutAction = new QAction(this);
    _aboutAction->setEnabled(true);
    _aboutAction->setText(tr("About"));

    // device group
    _deviceGroup = new QActionGroup(this);
    _deviceGroup->setExclusive(true);
    _deviceGroup->addAction(_baseAction);
    _deviceGroup->addAction(_passiveAction);
    _deviceGroup->addAction(_activeAction);
}

void  QnplMainWindow::createMenus()
{
    // recent menu
    _recentMenu = new QMenu();
    _recentMenu->setTitle(tr("Open Recent"));
    _recentMenu->addSeparator();
    _recentMenu->addAction(_clearAction);

    // file menu
    _fileMenu = menuBar()->addMenu(tr("File"));
    _fileMenu->addAction(_openAction);
    _fileMenu->addMenu(_recentMenu);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_tuneBroadChannellAction);
    //fileMenu->addAction(tuneIPTVChannellAction);
    _fileMenu->addAction(_tuneAppChannellAction);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_quitAction);

    // device menu
    _deviceMenu = menuBar()->addMenu(tr("Device"));
    _deviceMenu->addAction(_baseAction);
    _deviceMenu->addAction(_passiveAction);
    _deviceMenu->addAction(_activeAction);

    // window menu
    _windowMenu = menuBar()->addMenu(tr("Window"));
    _windowMenu->addAction(_preferencesAction);

    // help menu
    _helpMenu = menuBar()->addMenu(tr("Help"));
    _helpMenu->addAction(_bugAction);
    _helpMenu->addSeparator();
    _helpMenu->addAction(_aboutAction);
}

void QnplMainWindow::createRecent()
{
    _recentMenu->clear();

    foreach(QString file, settings->value("files").toStringList())
    {
        QAction* action = new QAction(this);
        action->setText(file);

        _recentMenu->addAction(file);
    }

    _recentMenu->addSeparator();
    _recentMenu->addAction(_clearAction);
}

void QnplMainWindow::createWidgets()
{
    _playButton = new QPushButton();
    _playButton->setEnabled(true);
    _playButton->setIcon(QIcon(":icon/play"));
    _playButton->setToolTip(tr("Play"));

    _stopButton = new QPushButton();
    _stopButton->setEnabled(false);
    _stopButton->setIcon(QIcon(":icon/stop"));
    _stopButton->setToolTip(tr("Stop"));

    _openButton = new QPushButton();
    _openButton->setEnabled(true);
    _openButton->setIcon(QIcon(":icon/open"));
    _openButton->setToolTip(tr("Open a new document"));

    _nextButton = new QPushButton();
    _nextButton->setEnabled(false);
    _nextButton->setIcon(QIcon(":icon/up"));
    _nextButton->setToolTip(tr("Next Channel"));

    _previousButton = new QPushButton();
    _previousButton->setEnabled(false);
    _previousButton->setIcon(QIcon(":icon/down"));
    _previousButton->setToolTip(tr("Previous Channel"));

    _refreshButton = new QPushButton();
    _refreshButton->setIcon(QIcon(":icon/refresh"));
    _refreshButton->setToolTip(tr("Scan"));

    connect(_refreshButton, SIGNAL(clicked()), SLOT(scan()));

    _channelsButton = new QPushButton();
    _channelsButton->setEnabled(true);
    _channelsButton->setIcon(QIcon(":icon/channels"));
    _channelsButton->setToolTip(tr("Channel List"));

    _openLine = new QLineEdit(this);
    _openLine->setEnabled(true);

    _view = new QnplView(this);
    setCentralWidget(_view);
}

void QnplMainWindow::createDialogs()
{
    _preferencesDialog = new QnplPreferencesDialog(this);

    _aboutDialog = new QnplAboutDialog(this);

    _channelDialog = new QnplChannelsDialog(this);
    connect(_channelDialog, SIGNAL(scanChannelsRequested()), SLOT(scan()));

    _iptvDialog = new QnplIPTVTunerDialog(this);
    _aplication = new QnplAplicationDialog(this);
}

void QnplMainWindow::createToolbars()
{
    // play toolbar
    _playToolbar = new QToolBar(tr("Play"));
    _playToolbar->setMovable(false);
    _playToolbar->setFloatable(false);
    _playToolbar->addWidget(_playButton);
    _playToolbar->addWidget(_stopButton);
    _playToolbar->addSeparator();

    addToolBar(Qt::BottomToolBarArea, _playToolbar);

    // open toolbar
    _openToolbar = new QToolBar(tr("Open"));
    _openToolbar->setMovable(false);
    _openToolbar->setFloatable(false);
    _openToolbar->addWidget(_openLine);
    _openToolbar->addWidget(_openButton);
    _openToolbar->addSeparator();
    _openToolbar->addWidget(new QLabel("CH: "));
    _openToolbar->addWidget(_nextButton);
    _openToolbar->addWidget(_previousButton);
    _openToolbar->addWidget(_refreshButton);
    _openToolbar->addWidget(new QLabel("  "));
    _openToolbar->addWidget(_channelsButton);

    addToolBar(Qt::BottomToolBarArea, _openToolbar);
}

void  QnplMainWindow::createConnections()
{
    connect(_openAction, SIGNAL(triggered()), SLOT(performOpen()));
    connect(_recentMenu,SIGNAL(triggered(QAction*)),SLOT(performOpen(QAction*)));
    connect(_clearAction, SIGNAL(triggered()),SLOT(performClear()));
    connect(_quitAction, SIGNAL(triggered()), SLOT(performQuit()));
    connect(_tuneIPTVChannellAction, SIGNAL(triggered()),SLOT(performIptv()));
    connect(_tuneAppChannellAction, SIGNAL(triggered()),SLOT(performAplication()));

    connect(_baseAction, SIGNAL(triggered()), SLOT(performDevice()));
    connect(_passiveAction, SIGNAL(triggered()), SLOT(performDevice()));
    connect(_activeAction, SIGNAL(triggered()), SLOT(performDevice()));

    connect(_preferencesAction, SIGNAL(triggered()), SLOT(performPreferences()));

    connect(_bugAction, SIGNAL(triggered()),SLOT(performBug()));
    connect(_aboutAction, SIGNAL(triggered()),SLOT(performAbout()));

    connect(_openButton, SIGNAL(clicked()), SLOT(performOpen()));
    connect(_channelsButton, SIGNAL(clicked()), SLOT(performChannels()));

    connect(_tuneBroadChannellAction, SIGNAL(triggered()), SLOT(performChannels()));
    connect(_playButton, SIGNAL(clicked()), SLOT(performRun()));
    connect(_stopButton, SIGNAL(clicked()), SLOT(performStop()));
    connect(_nextButton, SIGNAL(clicked()), SLOT(playNextChannel()));
    connect(_previousButton, SIGNAL(clicked()), SLOT(playPreviousChannel())  );

    connect(_view, SIGNAL(selected(QString)), _gingaProxy, SLOT(sendCommand(QString)));

    connect(_gingaProxy, SIGNAL(gingaStarted()), this, SLOT(startSession()));
//    connect(view,SIGNAL(selected(QString)),SLOT(notifyKey(QString)));
}


void QnplMainWindow::performOpen()
{
    qDebug() << _view->winId();;

    if (_location != "*"){
        performClose();
    }

    QString f = QFileDialog::getOpenFileName(this,"Open File",
                                             settings->value("lastdir_opened").toString(),"Files (*.ncl *.ts)");

    if (QFile::exists(f)){
        QDir d(f); settings->setValue("lastdir_opened", d.absolutePath());

        load(f);
    }
}

void QnplMainWindow::load(QString location)
{
    location = location.replace('/',QDir::separator());
    location = location.replace('\\',QDir::separator());

    QStringList recents = settings->value("files").toStringList();

    QStringList newRecents;

    newRecents << location << recents;

    newRecents.removeDuplicates();

    while(newRecents.count() > 5){
        newRecents.pop_back();
    }

    settings->setValue("files", newRecents); createRecent();

    this->_location = location;

    _openLine->setText(location);

    _playButton->setEnabled(true);

    setWindowTitle("Ginga GUI - "+QFileInfo(location).completeBaseName()+"."+
                   QFileInfo(location).completeSuffix());

    if (settings->value("autoplay").toString() == "true"){
        performPlay();
    }
}

void QnplMainWindow::performClear()
{
    settings->setValue("files",QStringList());

    _recentMenu->clear();

    _recentMenu->addSeparator();
    _recentMenu->addAction(_clearAction);
}

void QnplMainWindow::performClose()
{
    performStop();

    _location = "*";

    _playButton->setEnabled(false);
    _stopButton->setEnabled(false);

    _openLine->setText("");

    setWindowTitle("Ginga GUI");
}

void QnplMainWindow::performQuit()
{
    performStop();
    performClose();

    if (_process != NULL){
        _process->close();

        disconnect(_process);

        delete(_process);

        _process = NULL;
    }

    exit(0);
}

void QnplMainWindow::performPlay()
{
    if (QFile::exists(_location))
    {
        _lastChannel.setNull();

        QProcessEnvironment enviroment = QProcessEnvironment::systemEnvironment();
        enviroment.insert("LD_LIBRARY_PATH","/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");

        QStringList parameters;

        _playButton->setEnabled(false);
        _stopButton->setEnabled(true);

        _openButton->setEnabled(false);
        _openAction->setEnabled(false);

        _openLine->setEnabled(false);
        _openLine->setText(_location);

        _recentMenu->setEnabled(false);

        _baseAction->setEnabled(false);
        _passiveAction->setEnabled(false);
        _activeAction->setEnabled(false);

        // playing as base device
        if (_baseAction->isChecked())
        {
            parameters = QnplUtil::split(settings->value("parameters").toString());

            QFileInfo fileInfo (settings->value("gingaconfig_file").toString());
            parameters << "--context-dir" << fileInfo.absoluteDir().path();

            if (settings->value("enablelog").toBool()){
                parameters << "--enable-log" << "file";
            }

            QString WID = "";

            foreach (QObject* ob, _view->children()) {
                QWidget* w = qobject_cast<QWidget*>(ob);

                if (w)
                {
                    WID =  hwndToString(w->winId());
                }
            };

            parameters.replaceInStrings("${WID}", WID);

            if (_location.endsWith(".ncl"))
            {
                parameters.insert(parameters.begin(),"--ncl");
                parameters.replaceInStrings("${FILE}", _location);
            }
            else if (_location.endsWith(".ts"))
            {
                parameters.insert(parameters.begin(),"--set-tuner");
                parameters.replaceInStrings("${FILE}", "file:"+_location);

                _tuneAppChannellAction->setEnabled(true);
            }

            parameters.replaceInStrings("${SCREENSIZE}", settings->value("screensize").toString());

            QFileInfo finfo(_location);
            _gingaProxy->setWorkingDirectory(finfo.absoluteDir().absolutePath());

            qDebug() << settings->value("location").toString() << parameters;

            _gingaProxy->setBinaryPath(settings->value("location").toString());
            _gingaProxy->run(parameters);

            _process = _gingaProxy->process();
            if (!_process) return;

            _process->setProcessEnvironment(enviroment);

            connect (_process, SIGNAL(started()), this, SLOT(removeCarouselData()));
            setUpProcessConnections(_process);

            _view->setFocus();
        }
        // play as passive device
        else if (_passiveAction->isChecked())
        {

            performRunAsPassive();
        }
        // play as active device
        else if (_activeAction->isChecked())
        {

            performRunAsActive();
        }
    }
    else if (!_lastChannel.isNull()){
        playChannel(_lastChannel);
    }
    else
    {
        QMessageBox::information(this, tr ("Information"), tr("Please, open NCL document to play."), QMessageBox::Ok);
    }
}

void QnplMainWindow::performStop()
{
    _view->releaseKeyboard();
    if (_timer != NULL){
        _timer->stop();

        delete _timer;
        _timer = NULL;
    }

    if (_process != NULL){
        _animTuning->setVisible(false);

//        disconnect(process);

//        process->terminate();
//        process->close();

//        process->deleteLater();

//        process = NULL;

        _gingaProxy->terminateProcess();
    }

    if (_passiveIsRunning){
        _passiveIsRunning = false;
        settings->setValue("passive_running", false);
    }

    _tuneAppChannellAction->setEnabled(false);
    _playButton->setEnabled(true);
    _stopButton->setEnabled(false);

    _openButton->setEnabled(true);
    _openAction->setEnabled(true);

    _openLine->setEnabled(true);

    _recentMenu->setEnabled(true);

    _baseAction->setEnabled(true);
    _passiveAction->setEnabled(true);
    _activeAction->setEnabled(true);

    _nextButton->setEnabled(false);
    _previousButton->setEnabled(false);

    _scanProgress->close();

    _view->update();

    removeCarouselData();
}

void QnplMainWindow::performAplication()
{
    QString application = QFileDialog::getOpenFileName(this, "Application Channel", settings->value("lastdir_opened").toString(), "Files (*.ncl *.ts)");
    if (application != ""){
        _gingaProxy->sendCommand(QnplUtil::GINGA_COMMAND_PREFIX + "start," + application);
    }
}


void QnplMainWindow::performIptv()
{
    _iptvDialog->exec();
    qDebug() << _iptvDialog->ler_caixa();
}

void QnplMainWindow::performChannels()
{
    _channelDialog->loadGingaChannels();
    if (_channelDialog->exec() == QDialog::Accepted){

        Channel selectedChannel = _channelDialog->channel();
        if (!selectedChannel.isNull())
            playChannel (selectedChannel);
    }
}

void QnplMainWindow::playChannel(Channel channel)
{
    if (channel.frequency != ""){
        performStop();

        QStringList plist;

        plist << QnplUtil::split(settings->value("parameters").toString());
        plist << "--set-tuner" << "sbtvdt:" + channel.frequency;
        plist << "--poll-stdin";

        QFileInfo fileInfo (settings->value("gingaconfig_file").toString());
        plist << "--context-dir" << fileInfo.absoluteDir().path();

        if (settings->value("enablelog").toBool()){
            plist << "--enable-log" << "file";
        }

        plist.replaceInStrings("${SCREENSIZE}", settings->value("screensize").toString());
        QString WID = "";

        foreach (QObject* ob, _view->children()) {
            QWidget* w = qobject_cast<QWidget*>(ob);

            if (w)
            {
                WID =  hwndToString(w->winId());
            }
        };

        plist.removeAll("${FILE}");
        plist.replaceInStrings("${WID}", WID);

        _openLine->setText(channel.number + " - " + channel.name);

        _playButton->setEnabled(false);
        _stopButton->setEnabled(true);

        _nextButton->setEnabled(true);
        _previousButton->setEnabled(true);

        _openButton->setEnabled(false);
        _openAction->setEnabled(false);

        _openLine->setEnabled(false);

        _recentMenu->setEnabled(false);

        _baseAction->setEnabled(false);
        _passiveAction->setEnabled(false);
        _activeAction->setEnabled(false);

//        process->start(settings->value("location").toString(), plist);
        _gingaProxy->setBinaryPath(settings->value("location").toString());
        _gingaProxy->run(plist);

        _process = _gingaProxy->process();
        setUpProcessConnections(_process);

        connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(writeTunerOutput()));
        connect(_process, SIGNAL(readyReadStandardError()), this, SLOT(writeTunerOutput()));

        isPlayingChannel = false;

        _timer = new QTimer (this);
        connect (_timer, SIGNAL(timeout()), this, SLOT(stopTuning()));
        _timer->start(15000);

        _animTuning->setVisible(true);

        _lastChannel = channel;
        _location = "";
    }
}

void QnplMainWindow::stopTuning()
{
    if (!isPlayingChannel && _process){

        QMessageBox::StandardButton button = QMessageBox::warning(this, "Warning", QString ("It seems Ginga is taking so long ") +
                             QString ("to tuning this channel. The signal's strength may be weak.") +
                             QString (" Would you like to wait?"), QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::No){
            performStop();
        }
    }
}

void QnplMainWindow::writeTunerOutput()
{
    QString p_stdout = _process->readAllStandardOutput();
    QStringList lines = p_stdout.split("\n");

    for (int i = 0; i < lines.count(); i++){
        QString line = lines.at(i).trimmed();
        qDebug () << line;
        if (line.startsWith(QnplUtil::CMD_PREFIX)){
            QStringList tokens = line.split("::");
            if (tokens.count() == 4){
                QString command = tokens.at(0);
                QString status = tokens.at(1);
                QString entity = tokens.at(2);
                QString msg = tokens.at(3);

                if (command == "cmd"){
                    if (status == "0"){
                        if (entity == "start" && msg == "?mAV?"){//cmd::0::start::?mAV?
                            isPlayingChannel = true;
                            _animTuning->setVisible(false);
                            _tuneAppChannellAction->setEnabled(true);
                        }
                    }
                    else if (status == "1"){
                        _timer->stop();

                        if (msg != "")
                            QMessageBox::warning(this, "Warning", msg, QMessageBox::Ok);

                        _animTuning->setVisible(false);
                        qint64 bytes = _process->write(QnplUtil::QUIT.toStdString().c_str());
                        qDebug () << bytes;
                        performStop();
                    }
                }
            }
        }
        appendDebugMessage(line);
    }
}

void QnplMainWindow::writeScanOutput()
{
    QString p_stdout = _process->readAllStandardOutput();
    qDebug() << p_stdout;

    QStringList lines = p_stdout.split("\n");

    for (int i = 0; i < lines.count(); i++){
        QString line = lines.at(i).trimmed();
        if (line.startsWith(QnplUtil::CMD_PREFIX)){

            QStringList tokens = line.split("::");
            if (tokens.count() == 4){
                QString command = tokens.at(0);
                QString status = tokens.at(1);
                QString entity = tokens.at(2);
                QString msg = tokens.at(3);

                if (command == "cmd"){
                    if (status == "0"){
                        if (entity == "tunerscanprogress"){
                            msg.remove("%");
                            bool ok;
                            int value = msg.toInt(&ok);
                            if (ok){
                                _scanProgress->setValue(value);
                                if (value == 100)
                                    emit scanFinished();
                            }
                        }
                        else if (entity == "channelfound"){
                            _scanProgress->setLabelText("Channel found: \'" + msg + "\'.");
                        }
                    }
                    else if (status == "1"){
                        if (entity == "tuner"){
                            if (msg != "")
                                QMessageBox::warning(this, "Warning", msg, QMessageBox::Ok);

                            _scanProgress->close();
                            qint64 bytes = _process->write(QnplUtil::QUIT.toStdString().c_str());
                            qDebug () << bytes;
                        }
                    }
                }
            }
        }
        appendDebugMessage(line);
    }
}


void QnplMainWindow::performPreferences()
{
    _preferencesDialog->init(settings);
    _preferencesDialog->exec();

    QString ssize = settings->value("screensize").toString();

    QString sw = ssize.section('x',0,0);
    QString sh = ssize.section('x',1,1);

    int w = sw.toInt();
    int h = sh.toInt();

    _view->setSceneRect(0,0,w,h);

    resize(w+20, h+100);
}

void QnplMainWindow::performBug()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/~edcaraujo/gingagui/"));
}

void QnplMainWindow::performAbout()
{
    _aboutDialog->show();
}

void QnplMainWindow::performRun()
{
    if (_baseAction->isChecked())
    {
        qDebug() << "run as base";
        performPlay();
    }
    else if (_passiveAction->isChecked())
    {
        qDebug() << "run as passive";
        performRunAsPassive();
    }
    else if (_activeAction->isChecked())
    {
        qDebug() << "run as active";
        performRunAsActive();
    }
}

void QnplMainWindow::performRunAsPassive()
{

    QString conf_location = settings->value("gingaconfig_file").toString();
    QString context_dir = QFileInfo(conf_location).absoluteDir().path();

    QStringList plist;

    plist << "--wid" << viewWID();
    plist << "--device-class" << "1";
    plist << "--vmode" << settings->value("screensize").toString();
    plist << "--context-dir" << context_dir;
    plist << "--disable-multicast";
    plist << "--poll-stdin";

    if (settings->value("enablelog").toBool()){
        plist << "--enable-log" << "file";
    }
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");

    _gingaProxy->setBinaryPath(settings->value("location").toString());
    _gingaProxy->run(plist, env);

    _process = _gingaProxy->process();
    setUpProcessConnections(_process);

    _process->setProcessEnvironment(env);

    qDebug() << settings->value("location").toString() << plist;

    _passiveIsRunning = true;
    settings->setValue("passive_running", true);

    _playButton->setEnabled(false);
    _stopButton->setEnabled(true);

    _openButton->setEnabled(false);
    _openAction->setEnabled(false);

    _openLine->setEnabled(false);

    _recentMenu->setEnabled(false);

    _baseAction->setEnabled(false);
    _passiveAction->setEnabled(false);
    _activeAction->setEnabled(false);

    _view->setFocus();
}

void QnplMainWindow::performRunAsActive()
{
    QString conf_location = settings->value("gingaconfig_file").toString();
    QString context_dir = QFileInfo(conf_location).absoluteDir().path();

    int port =  settings->value("device_port").toInt();
    port++;

    if (port > 33333){
        settings->setValue("device_port",22222);
    }else{
        settings->setValue("device_port",port);
    }

    QStringList plist;
    plist << "--wid" << viewWID();
    plist << "--device-class" << "2";
    plist << "--device-srv-port" << QString::number(port);
    plist << "--vmode" << settings->value("screensize").toString();
    plist << "--context-dir" << context_dir;
    plist << "--poll-stdin";

    if (settings->value("enablelog").toBool()){
        plist << "--enable-log" << "file";
    }

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");

    _gingaProxy->setBinaryPath(settings->value("location").toString());
    _gingaProxy->run(plist, env);

    _process = _gingaProxy->process();
    setUpProcessConnections(_process);

    qDebug() << settings->value("location").toString() << plist;

    _playButton->setEnabled(false);
    _stopButton->setEnabled(true);

    _openButton->setEnabled(false);
    _openAction->setEnabled(false);

    _openLine->setEnabled(false);

    _recentMenu->setEnabled(false);

    _baseAction->setEnabled(false);
    _passiveAction->setEnabled(false);
    _activeAction->setEnabled(false);

    _view->setFocus();
}

void QnplMainWindow::performCloseWindow(int, QProcess::ExitStatus)
{
    if (_animTuning->isVisible()){
        _timer->stop();
        QList <QMessageBox *>list = findChildren <QMessageBox *> ();
        foreach (QMessageBox *msgBox, list){
            msgBox->close();
        }

        QMessageBox::warning(this, "Warning", QString ("The signal's strength is too weak to tune this channel. ") +
                             QString ("Please, check your antenna and try again."), QMessageBox::Ok);

    }
    performStop();
}

void QnplMainWindow::notifyKey(QString key)
{
    if (_process != NULL)
    {
        qDebug() << "Writing:" << key;
//        process->write(QString(key+"\n").toStdString().c_str());
    }
}

void QnplMainWindow::performOpen(QAction* act)
{
    if (act != _clearAction)
    {
        load(act->text());
    }
}

QString QnplMainWindow::hwndToString(WId winid)
{
    unsigned long int value = (unsigned long int) winid;
    return QString::number(value);

    /*
#ifdef Q_OS_WIN
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

  return QString(dst);
#endif

#ifdef Q_WS_X11
  unsigned long int value = (unsigned long int) winid;
  return QString::number(value);
#endif
    */
}

void QnplMainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    qDebug() << "resizing...";

    int w_span = 20;
    int h_span = 100;

    qreal w = event->size().width() - w_span ;
    qreal h = event->size().height() - h_span;

    settings->setValue("screensize", QString::number(w) + "x" + QString::number(h));

    _view->setSceneRect (0,0,w,h);

    _animTuning->setPos(0, -h_span);

    _movie->setScaledSize(QSize (w + w_span, h + h_span));
    _gifLabel->setFixedSize (w + w_span, h + h_span);
}

void QnplMainWindow::playNextChannel()
{
    Channel nextChannel = _channelDialog->nextChannel();
    if(nextChannel.frequency != "")
    {
        playChannel(nextChannel);
    }
}

void QnplMainWindow::playPreviousChannel()
{
    Channel previousChannel = _channelDialog->previousChannel();
    if(previousChannel.frequency != "")
    {
        playChannel(previousChannel);
    }
}


void QnplMainWindow::scan()
{
    performStop();

    QStringList plist;
    plist << "--set-tuner" << "sbtvdt:scan";
    plist << "--wid" << hwndToString(_scanProgress->winId());
    plist << "--poll-stdin";

    if (settings->value("enablelog").toBool()){
        plist << "--enable-log" << "file";
    }

    _process = new QProcess(this);
    _scanProgress->setValue(0);
    _gingaProxy->setBinaryPath(settings->value("location").toString());
    _gingaProxy->run(plist);
//    process->start(settings->value("location").toString(), plist, QProcess::ReadWrite);

    _process = _gingaProxy->process();
    setUpProcessConnections(_process);

    connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(writeScanOutput()));
    connect(_process, SIGNAL(readyReadStandardError()), this, SLOT(writeScanOutput()));

    connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(scanFinished()));
    connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), _scanProgress, SLOT(done(int)));
    connect(_gingaProxy, SIGNAL(gingaStarted()), _scanProgress, SLOT(exec()));
    connect(_scanProgress, SIGNAL(canceled()), this, SLOT(sendKillMessage()));
}


void QnplMainWindow::sendKillMessage()
{
    disconnect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(writeScanOutput()));
    disconnect(_process, SIGNAL(readyReadStandardError()), this, SLOT(writeScanOutput()));

    qDebug () << _gingaProxy->sendCommand(QnplUtil::QUIT.toStdString().c_str());

//    if (process){
//        qint64 bytes = process->write(QnplUtil::QUIT.toStdString().c_str());
//        qDebug () << bytes;
//    }
    emit scanFinished();
}

void QnplMainWindow::showErrorDialog(QProcess::ProcessError error)
{
    QString errorMessage = "";
    if (error == QProcess::FailedToStart){
        errorMessage = "Error while opening Ginga. Check the binary path.";
    }

    if (errorMessage != ""){
        QMessageBox::warning(this, "Warning", errorMessage, QMessageBox::Ok);
    }

    performStop();
}

void QnplMainWindow::removeCarouselData()
{
    QString carouselPath = QDir::tempPath() + "/ginga/carousel";
    if (QFile::exists(carouselPath))
        removePath(carouselPath);
}

void QnplMainWindow::removePath(QString path)
{
    QFileInfo fileInfo(path);
    if(fileInfo.isDir()){
        QDir dir(path);
        QStringList fileList = dir.entryList();
        for(int i = 0; i < fileList.count(); ++i){
            QString entry = fileList.at(i);
            if (entry != "." && entry != "..")
                removePath(path + "/" + entry);
        }
        dir.rmdir(path);
    }
    else{
        QFile::remove(path);
    }
}

void QnplMainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_D && (event->modifiers() & Qt::ControlModifier))
    {
        _developerView->setVisible(!_developerView->isVisible());
    }
}

void QnplMainWindow::setUpProcessConnections(QProcess *process)
{
    if (process){
        connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(performCloseWindow(int, QProcess::ExitStatus)));
        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showErrorDialog(QProcess::ProcessError)));
        connect(process, SIGNAL(started()), this, SLOT(removeCarouselData()));
    }
}

void QnplMainWindow::appendDebugMessage(QString message)
{
    if (! _process) return;

    _developerView->appendConsoleMessage(message);
}

void QnplMainWindow::startSession()
{
    _developerView->clear();
}
