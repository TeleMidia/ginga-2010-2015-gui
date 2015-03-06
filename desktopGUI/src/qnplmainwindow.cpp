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

#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 300

QnplMainWindow::QnplMainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  _appName = "Ginga GUI";

  setWindowTitle(_appName);
  setWindowIcon(QIcon(":icons/gingagui-128x128"));
  _settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                            "telemidia", "gingagui");

  QString iniPath = QFileInfo(_settings->fileName()).absolutePath();

  QString contextFile = _settings->value(Util::V_CONTEXT_FILE, "").toString();
  if (contextFile.isEmpty())
    contextFile = iniPath + "/ginga/contextFile";

  QFileInfo fileInfo (contextFile);

  if (!fileInfo.exists())
  {
    QFile gingaContextFile (":config/context");
    if (gingaContextFile.open(QIODevice::ReadOnly))
    {
      QString context = QString (gingaContextFile.readAll());

      QDir dir;
      dir.setCurrent(iniPath);
      dir.mkdir("ginga");

      QFile tempGingaContextFile (iniPath + "/ginga/contexts.ini");

      if (tempGingaContextFile.open(QIODevice::WriteOnly))
      {
        tempGingaContextFile.write(context.toStdString().c_str());
        tempGingaContextFile.close();

        _settings->setValue(Util::V_CONTEXT_FILE,
                            tempGingaContextFile.fileName());
        _settings->sync();
      }
      gingaContextFile.close();
    }
  }

  _gingaProxy = GingaProxy::getInstance();
  _isChannel = false;
  _isTuning = false;
  _isPaused = false;
  _location = "";
  _process = NULL;
  _timer = NULL;
  _passiveIsRunning = false;
  _location = "";


  createActions();
  createMenus();
  createRecent();
  createWidgets();
  createDialogs();
  createToolbars();
  createConnections();

  resizeView();
}

QnplMainWindow::~QnplMainWindow()
{
  _gingaProxy->killProcess();
  GingaProxy::deleteInstance();
}

void QnplMainWindow::performDevice()
{
  QString key = "run_as";
  if (_baseAction->isChecked())
  {
    _settings->setValue(key, "base");
  }
  else if (_passiveAction->isChecked())
  {
    _settings->setValue(key, "passive");
  }
  else if (_activeAction->isChecked())
  {
    _settings->setValue(key, "active");
  }
}

void  QnplMainWindow::createActions()
{
  // open action
  _openAction = new QAction(this);
  _openAction->setEnabled(true);
  _openAction->setText(tr("Open..."));
  _openAction->setShortcut(QKeySequence("Ctrl+O"));

  _tuneApplicationChannelAction = new QAction(this);
  _tuneApplicationChannelAction->setEnabled(false);
  _tuneApplicationChannelAction->setText(tr("Tune Application Channel..."));

  _tuneBroadcastChannelAction = new QAction(this);
  _tuneBroadcastChannelAction->setEnabled(true);
  _tuneBroadcastChannelAction->setText(tr("Tune Broadcast Channel..."));

  _tuneIPTVChannellAction = new QAction(this);
  _tuneIPTVChannellAction->setEnabled(true);
  _tuneIPTVChannellAction->setText(tr("Tune IP Channel..."));


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
  _baseAction->setChecked(true);
  _baseAction->setText(tr("Default (Base)"));

  // passive action
  _passiveAction = new QAction(this);
  _passiveAction->setEnabled(true);
  _passiveAction->setCheckable(true);
  _passiveAction->setText(tr("Passive"));
  connect (_passiveAction, SIGNAL(triggered()), SLOT(performRunAsPassive()));

  // active action
  _activeAction = new QAction(this);
  _activeAction->setEnabled(true);
  _activeAction->setCheckable(true);
  _activeAction->setText(tr("Active"));
  connect (_activeAction, SIGNAL(triggered()), SLOT(performRunAsActive()));

  // preferences action
  _preferencesAction = new QAction(this);
  _preferencesAction->setEnabled(true);
  _preferencesAction->setText(tr("Preferences..."));

  // bug action
//  _bugAction = new QAction(this);
//  _bugAction->setEnabled(true);
//  _bugAction->setText(tr("Report Bug..."));

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
  _fileMenu->addAction(_tuneBroadcastChannelAction);
  _fileMenu->addAction(_tuneIPTVChannellAction);
  _fileMenu->addAction(_tuneApplicationChannelAction);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_quitAction);

  // device menu
  _deviceMenu = menuBar()->addMenu(tr("Device"));
  _deviceMenu->addAction(_baseAction);
  _deviceMenu->addAction(_passiveAction);
  _deviceMenu->addAction(_activeAction);

  // tools menu
  _toolsMenu = menuBar()->addMenu(tr("Tools"));
  _toolsMenu->addAction(_preferencesAction);

  // help menu
  _helpMenu = menuBar()->addMenu(tr("Help"));
  //  _helpMenu->addAction(_bugAction);
  //  _helpMenu->addSeparator();
  _helpMenu->addAction(_aboutAction);
}

void QnplMainWindow::createRecent()
{
  _recentMenu->clear();

  foreach(QString file, _settings->value(Util::V_FILES).toStringList())
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
  _playButton->setIcon(QIcon(":icons/play"));
  _playButton->setToolTip(tr("Play"));

  _pauseButton = new QPushButton();
  _pauseButton->setEnabled(false);
  _pauseButton->setIcon(QIcon(":icons/pause"));
  _pauseButton->setToolTip(tr("Pause"));

  _stopButton = new QPushButton();
  _stopButton->setEnabled(false);
  _stopButton->setIcon(QIcon(":icons/stop"));
  _stopButton->setToolTip(tr("Stop"));

#ifdef G_SEEKABLE
  _seekPlayTime = new QLineEdit (this);
  _seekPlayTime->setEnabled(false);
  _seekPlayTime->setValidator(new QRegExpValidator(
                                QRegExp("[ \\d]{3}:[0-5 ][ \\d]:[0-5 ][ \\d]"),
                                this));
  _seekPlayTime->setInputMask("999:99:99");
  _seekPlayTime->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  _seekPlayTime->setMaximumWidth(60);

  _seekButton = new QPushButton("Seek");
  _seekButton->setEnabled(false);

#endif

  _openButton = new QPushButton();
  _openButton->setEnabled(true);
  _openButton->setIcon(QIcon(":icons/open"));
  _openButton->setToolTip(tr("Open a new document"));

  _nextButton = new QPushButton();
  _nextButton->setEnabled(false);
  _nextButton->setIcon(QIcon(":icons/up"));
  _nextButton->setToolTip(tr("Next Channel"));

  _previousButton = new QPushButton();
  _previousButton->setEnabled(false);
  _previousButton->setIcon(QIcon(":icons/down"));
  _previousButton->setToolTip(tr("Previous Channel"));

  _refreshButton = new QPushButton();
  _refreshButton->setIcon(QIcon(":icons/refresh"));
  _refreshButton->setToolTip(tr("Scan"));

  _channelsButton = new QPushButton();
  _channelsButton->setEnabled(true);
  _channelsButton->setIcon(QIcon(":icons/channels"));
  _channelsButton->setToolTip(tr("Channel List"));

  _openLine = new QLineEdit(this);
  _openLine->setEnabled(true);

  _scanProgress =
      new QProgressDialog("Scanning channels...", "Abort", 0, 100, this);
  _scanProgress->setWindowFlags(Qt::Dialog | Qt::Desktop);
  _scanProgress->setWindowTitle("Scanning");
  _scanProgress->setWindowIcon(windowIcon());

  _stackedWidget = new QStackedWidget(this);

  _view = new QnplView(_stackedWidget);
  _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  _animationView = new QWidget(_stackedWidget);
  _animationView->setStyleSheet("background: #000000;");
  _animationView->setContentsMargins(0, 0, 0, 0);
  _animationView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  _gifLabel = new QLabel(_animationView);
  _movie = new QMovie(":backgrounds/anim-tuning");
  _gifLabel->setMovie(_movie);
  _movie->start();
  _gifLabel->setVisible(true);

  _developerView = new DeveloperView(this);
  addDockWidget(Qt::RightDockWidgetArea, _developerView);
  _developerView->setVisible(false);

  _debugView = new DebugView(_view, this);
  addDockWidget(Qt::LeftDockWidgetArea, _debugView);
  _debugView->setVisible(false);

  _stackedWidget->addWidget(_view);
  _stackedWidget->addWidget(_animationView);
  _stackedWidget->setCurrentIndex(0);

  setCentralWidget(_stackedWidget);
}

void QnplMainWindow::createDialogs()
{
  _preferencesDialog = new QnplPreferencesDialog(_settings, this);

  _aboutDialog = new QnplAboutDialog(_settings->value(Util::V_LOCATION)
                                     .toString(), this);

  _channelDialog = new QnplChannelsDialog(this);
  connect(_channelDialog, SIGNAL(scanChannelsRequested()), SLOT(scan()));

  _iptvDialog = new QnplIPTVTunerDialog(this);
}

void QnplMainWindow::createToolbars()
{
  _toolbar = new QToolBar(tr("Play"));
  _toolbar->setMovable(true);
  _toolbar->setFloatable(false);
  _toolbar->addWidget(_playButton);
  _toolbar->addWidget(_pauseButton);
  _toolbar->addWidget(_stopButton);
  _toolbar->addSeparator();

#ifdef G_SEEKABLE
  _toolbar->addWidget(new QLabel ("Skip to time:"));
  _toolbar->addWidget(_seekPlayTime);
  _toolbar->addWidget(_seekButton);
  _toolbar->addSeparator();
#endif

  _toolbar->addWidget(_openLine);
  _toolbar->addWidget(_openButton);
  _toolbar->addSeparator();
  _toolbar->addWidget(new QLabel("CH: "));
  _toolbar->addWidget(_nextButton);
  _toolbar->addWidget(_previousButton);
  _toolbar->addWidget(_refreshButton);
  _toolbar->addWidget(new QLabel("  "));
  _toolbar->addWidget(_channelsButton);

  addToolBar(Qt::BottomToolBarArea, _toolbar);
}

void  QnplMainWindow::createConnections()
{
  connect(_openAction, SIGNAL(triggered()),
          SLOT(performOpen()));
  connect(_recentMenu,SIGNAL(triggered(QAction*)),
          SLOT(performOpen(QAction*)));
  connect(_clearAction, SIGNAL(triggered()),
          SLOT(performClear()));
  connect(_quitAction, SIGNAL(triggered()),
          SLOT(performQuit()));
  connect(_tuneIPTVChannellAction, SIGNAL(triggered()),
          SLOT(performIptv()));
  connect(_tuneApplicationChannelAction, SIGNAL(triggered()),
          SLOT(performAplication()));
  connect(_baseAction, SIGNAL(triggered()),
          SLOT(performDevice()));
  connect(_passiveAction, SIGNAL(triggered()),
          SLOT(performDevice()));
  connect(_activeAction, SIGNAL(triggered()),
          SLOT(performDevice()));
  connect(_preferencesAction, SIGNAL(triggered()),
          SLOT(performPreferences()));
//  connect(_bugAction, SIGNAL(triggered()),
//          SLOT(performBug()));
  connect(_aboutAction, SIGNAL(triggered()),
          SLOT(performAbout()));
  connect(_openButton, SIGNAL(clicked()),
          SLOT(performOpen()));
  connect(_channelsButton, SIGNAL(clicked()),
          SLOT(performChannels()));
  connect(_tuneBroadcastChannelAction, SIGNAL(triggered()),
          SLOT(performChannels()));
  connect (_playButton, SIGNAL(clicked()),
           SLOT(performRun()));
  connect (_pauseButton, SIGNAL(clicked()),
           SLOT(performPause()));
  connect (_stopButton, SIGNAL(clicked()),
           SLOT(performStop()));
  connect (_nextButton, SIGNAL(clicked()),
           SLOT(playNextChannel()));
  connect (_previousButton, SIGNAL(clicked()),
           SLOT(playPreviousChannel()));
  connect (_view, SIGNAL(selected(QString)),
           _gingaProxy, SLOT(sendCommand(QString)));
  connect (_gingaProxy, SIGNAL(gingaStarted()),
           SLOT(startSession()));
  connect(_scanProgress, SIGNAL(canceled()),
          SLOT(sendKillMessage()));

#ifdef G_SEEKABLE
  connect (_seekPlayTime, SIGNAL(returnPressed()),
           SLOT(performSeek()));
  connect (_seekButton, SIGNAL(pressed()),
           SLOT(performSeek()));
  connect (_seekPlayTime, SIGNAL(textChanged(QString)),
           SLOT(enableSeekButton()));
#endif
  connect (_developerView, SIGNAL(sendCommandRequested(QString)),
           _gingaProxy, SLOT(sendCommand(QString)));
  connect(_refreshButton, SIGNAL(clicked()),
          SLOT(scan()));

  connect(_gingaProxy, SIGNAL(gingaFinished(int,QProcess::ExitStatus)),
          SLOT(performCloseWindow()));
  connect(_gingaProxy, SIGNAL(gingaFinished(int,QProcess::ExitStatus)),
          SLOT(performStop()));
  connect(_gingaProxy, SIGNAL(gingaError(QProcess::ProcessError)),
          SLOT(showErrorDialog(QProcess::ProcessError)));
  connect(_gingaProxy, SIGNAL(gingaStarted()),
          SLOT(removeCarouselData()));
  connect (_gingaProxy, SIGNAL(gingaOutput(QString)),
           SLOT(handleGingaOutput(QString)));
  connect(_gingaProxy, SIGNAL(gingaFinished(int, QProcess::ExitStatus)),
          this, SLOT(finishScan(int)));
  connect (_view, SIGNAL (droppedFile(QString)),
           this, SLOT (load(QString)));

  connect (_openLine, SIGNAL (textChanged(QString)),
           this, SLOT (updateLocation (QString)));
}


void QnplMainWindow::performOpen()
{
  if (_location != "*")
    performClose();

  QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                  _settings->value(
                                                    Util::V_LAST_DIR).
                                                  toString(),
                                                  "Files (*.ncl *.ts)");

  if (QFile::exists(fileName))
  {
    QDir dir (fileName);
    _settings->setValue(Util::V_LAST_DIR, dir.absolutePath());
    load(fileName);
  }
}

void QnplMainWindow::updateLocation(const QString &location)
{
  if (_location != location)
    _location = location.trimmed();
}

void QnplMainWindow::resizeView()
{
  _settings->sync();

  // get edit value from setting and set in window
  QString ssize = _settings->value(Util::V_SCREENSIZE).toString();
  QString sw = ssize.section('x',0,0);
  QString sh = ssize.section('x',1,1);

  int w = sw.toInt();
  int h = sh.toInt();

  if (w == 0 && h ==0)
  {
    w = DEFAULT_WIDTH;
    h = DEFAULT_HEIGHT;
  }

  _toolbar->setFixedWidth(w);
  _stackedWidget->setFixedSize(w, h);
  adjustSize();


  _stackedWidget->setMinimumSize(0, 0);
  _stackedWidget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

  _toolbar->setMinimumSize(0, 0);
  _toolbar->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

void QnplMainWindow::load(const QString &location)
{
  QStringList recents = _settings->value(Util::V_FILES).toStringList();

  QStringList newRecents;

  newRecents << location << recents;

  newRecents.removeDuplicates();

  while(newRecents.count() > 5)
    newRecents.pop_back();

  _settings->setValue(Util::V_FILES, newRecents);
  createRecent();

  _location = location;

  _openLine->setText(location);

  _playButton->setEnabled(true);

  setWindowTitle(_appName + " - " + QFileInfo(location).completeBaseName()
                 + "." + QFileInfo(location).completeSuffix());

  if (_settings->value(Util::V_AUTOPLAY).toString() == Util::TRUE_)
    performPlay();
}

void QnplMainWindow::performClear()
{
  _settings->setValue(Util::V_FILES, QStringList());

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

  setWindowTitle(_appName);
}

void QnplMainWindow::performQuit()
{
  QApplication::quit();
}

void QnplMainWindow::performPlay()
{
  _settings->sync();
  _view->setFocus();

  _preferencesAction->setEnabled(false);

  QUrl url (_location);
  if (url.isLocalFile())
    _location = url.toLocalFile();

  if (QFile::exists(_location))
  {
    _lastChannel.setNull();

    QStringList parameters;

    _playButton->setEnabled(false);
    _pauseButton->setEnabled(true);
    _stopButton->setEnabled(true);

#ifdef G_SEEKABLE
    _seekPlayTime->setEnabled(true);
#endif

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
      parameters = Util::split(_settings->value(Util::V_PARAMETERS).toString());
      configureDefaultFlags(parameters);

      if (_location.endsWith(".ncl")){
        parameters.replaceInStrings(Util::GUI_FILE, _location);
      } else if (_location.endsWith(".ts")) {
        int index = parameters.indexOf("--ncl");
        if (index != -1)
        {
          parameters.insert(index, "--set-tuner");
          parameters.removeAt(index + 1);
        }
        parameters.replaceInStrings(Util::GUI_FILE, Util::GINGA_TS_FILE
                                    + _location);
        _tuneApplicationChannelAction->setEnabled(true);
      }

      QFileInfo finfo(_location);
      _gingaProxy->setWorkingDirectory(finfo.absoluteDir().absolutePath());

      qDebug() << _settings->value(Util::V_LOCATION).toString() << parameters;

      _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
      _gingaProxy->run(parameters);

      _view->setFocus();
    }
    // play as passive device
    else if (_passiveAction->isChecked())
      performRunAsPassive();

    // play as active device
    else if (_activeAction->isChecked())
      performRunAsActive();

  }
  else if (!_lastChannel.isNull())
    playChannel(_lastChannel);
  else if(_lastIpChannel != "")
      playIpChannel(_lastIpChannel);
  else
    QMessageBox::information(this, tr ("Information"),
                             tr("Please, open NCL document to play."),
                             QMessageBox::Ok);
}

void QnplMainWindow::performPause()
{
  _isPaused = !_isPaused;

  if (_isPaused)
    _pauseButton->setIcon(QIcon(":icons/resume"));
  else
    _pauseButton->setIcon(QIcon(":icons/pause"));

  _gingaProxy->sendCommand(Util::GINGA_KEY_PREFIX + Util::GINGA_PAUSE_KEY);
}

void QnplMainWindow::performStop()
{
  _view->releaseKeyboard();
  _debugView->stopSession();
  _preferencesAction->setEnabled(true);

  if (_timer != NULL)
  {
    _timer->stop();

    delete _timer;
    _timer = NULL;
  }

  _gingaProxy->stop();

  if (_process != NULL)
    _stackedWidget->setCurrentIndex(0);

  if (_passiveIsRunning)
  {
    _passiveIsRunning = false;
    _settings->setValue(Util::V_PASSIVE, Util::FALSE_);
  }

  _tuneApplicationChannelAction->setEnabled(false);
  _playButton->setEnabled(true);
  _pauseButton->setEnabled(false);
  _pauseButton->setIcon(QIcon(":icons/pause"));
  _stopButton->setEnabled(false);

#ifdef G_SEEKABLE
  _seekPlayTime->setEnabled(false);
  _seekPlayTime->setText("");
  _seekButton->setEnabled(false);
#endif

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

#ifdef __linux
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
#endif
}

void QnplMainWindow::performSeek()
{
  qDebug () << "SEEK";
}

void QnplMainWindow::performAplication()
{
  QString application =
      QFileDialog::getOpenFileName(this, "Application Channel",
                                   _settings->value(Util::V_LAST_DIR).
                                   toString(), "Files (*.ncl *.ts)");
  if (application != "")
  {
    _gingaProxy->sendCommand(Util::GINGA_COMMAND_PREFIX + "start," +
                             application);
    _view->setFocus();
  }

}


void QnplMainWindow::performIptv()
{
    if (_iptvDialog->exec() == QDialog::Accepted)
    {
      QString ipChannel = _iptvDialog->ip();
      if (ipChannel != "")
      {
        ipChannel += ":" + _iptvDialog->port();
        playIpChannel (ipChannel);
        _view->setFocus();
      }
    }
}

void QnplMainWindow::playIpChannel(QString ipChannel){

  performStop();
  _preferencesAction->setEnabled(false);

  _settings->sync();

  QStringList plist;
  plist << Util::split(_settings->value(Util::V_PARAMETERS).toString());

  plist << "--set-tuner" << "ip:" + ipChannel;

  configureDefaultFlags(plist);

  plist.removeAll(Util::GUI_NCL);
  plist.removeAll(Util::GUI_FILE);

  _openLine->setText("IP: " + ipChannel);

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
  _gingaProxy->setBinaryPath(_settings->value("location").toString());
  _gingaProxy->run(plist);

  _process = _gingaProxy->process();
  _isPlayingChannel = false;
  _isTuning = true;

  _timer = new QTimer (this);
  connect (_timer, SIGNAL(timeout()),
           SLOT(stopTuning()));
  _timer->start(15000);

  _stackedWidget->setCurrentIndex(1);
  _lastIpChannel = ipChannel;
  _lastChannel.setNull();
  _location = "";
}

void QnplMainWindow::performChannels()
{
  _channelDialog->loadGingaChannels();
  if (_channelDialog->exec() == QDialog::Accepted)
  {
    Channel selectedChannel = _channelDialog->channel();
    if (!selectedChannel.isNull())
    {
      playChannel (selectedChannel);
      _view->setFocus();
    }
  }
}

void QnplMainWindow::playChannel(Channel channel)
{
  if (channel.frequency != "")
  {
    performStop();
    _preferencesAction->setEnabled(false);

    _settings->sync();

    QStringList plist;
    plist << Util::split(_settings->value(Util::V_PARAMETERS).toString());

    plist << "--set-tuner" << "sbtvdt:" + channel.frequency;

    configureDefaultFlags(plist);

    plist.removeAll(Util::GUI_NCL);
    plist.removeAll(Util::GUI_FILE);

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

    _gingaProxy->setBinaryPath(_settings->value("location").toString());
    _gingaProxy->run(plist);

    _process = _gingaProxy->process();
    _isPlayingChannel = false;
    _isTuning = true;

    _timer = new QTimer (this);
    connect (_timer, SIGNAL(timeout()),
             SLOT(stopTuning()));
    _timer->start(15000);

    _stackedWidget->setCurrentIndex(1);

    _lastChannel = channel;
    _location = "";
  }
}

void QnplMainWindow::stopTuning()
{
  if (!_isPlayingChannel && _process)
  {
    QMessageBox::StandardButton button =
        QMessageBox::warning(this, "Warning",
                             QString("It seems Ginga is  taking so long to"
                                     " tuning this channel. The signal's"
                                     " strength may be weak. Would you like to "
                                     " wait?"),
                             QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No)
      performStop();
  }
}

void QnplMainWindow::writeTunerOutput(QString p_stdout)
{
  QStringList lines = p_stdout.split("\n");

  for (int i = 0; i < lines.count(); i++)
  {
    QString line = lines.at(i).trimmed();
    if (line.startsWith(Util::CMD_PREFIX))
    {
      QStringList tokens = line.split("::");
      if (tokens.count() == 4)
      {
        QString command = tokens.at(0);
        QString status = tokens.at(1);
        QString entity = tokens.at(2);
        QString msg = tokens.at(3);

        if (command == "cmd")
        {
          if (status == "0")
          {
            if (entity == "tuned" && msg == "?mAV?")
            {//cmd::0::tuned::?mAV?
              _isPlayingChannel = true;
              _isTuning = false;
              _stackedWidget->setCurrentIndex(0);
              _tuneApplicationChannelAction->setEnabled(true);
            }
          }
          else if (status == "1")
          {
            if (_timer)
              _timer->stop();

            if (msg != "")
              QMessageBox::warning(this, "Warning", msg, QMessageBox::Ok);

            _stackedWidget->setCurrentIndex(0);
            if (_process)
            {
              qint64 bytes = _process->write(Util::GINGA_QUIT.toStdString().
                                             c_str());
              qDebug () << bytes;
            }
            performStop();
          }
        }
      }
    }
  }
}

void QnplMainWindow::writeScanOutput(QString p_stdout)
{
  QStringList lines = p_stdout.split("\n");

  for (int i = 0; i < lines.count(); i++)
  {
    QString line = lines.at(i).trimmed();
    if (line.startsWith(Util::CMD_PREFIX))
    {
      GingaMessage message = Util::parseMessage(line);

      if (message.command == "cmd")
      {
        if (message.code == "0")
        {
          if (message.messageKey == "tunerscanprogress")
          {
            QString progress = message.data.at(0);
            progress.remove("%");
            bool ok;
            int value = progress.toInt(&ok);
            if (ok)
            {
              _scanProgress->setValue(value == 0 ? _scanProgress->value() :
                                                   value);
              if (value == 100)
                emit scanFinished();
            }
          }
          else if (message.messageKey == "channelfound")
          {
            _scanProgress->setLabelText("Channel found: \'" + message.data.at(0)
                                        + "\'.");
          }
        }
        else if (message.code == "1")
        {
          if (message.messageKey == "tuner")
          {
            QString warningMsg = message.data.at(0);
            if (warningMsg != "")
              QMessageBox::warning(this, "Warning", warningMsg, QMessageBox::Ok);

            _scanProgress->close();
            _gingaProxy->sendCommand(Util::GINGA_QUIT.toStdString().
                                     c_str());
          }
        }
      }

    }
  }
}


void QnplMainWindow::performPreferences()
{
  // Set actual size in settings for be present in settings form
  QString new_size;
  new_size.append(QString::number(_stackedWidget->width()));
  new_size.append("x");
  new_size.append(QString::number(_stackedWidget->height()));
  qDebug() << "new_size=" << new_size << endl;
  _settings->setValue(Util::V_SCREENSIZE,new_size);
  _settings->sync();
  _preferencesDialog->exec();

  resizeView();
}

void QnplMainWindow::performBug()
{
  QDesktopServices::openUrl(
        QUrl("http://www.telemidia.puc-rio.br/~edcaraujo/gingagui/"));
}

void QnplMainWindow::performAbout()
{
  _aboutDialog->show();
}

void QnplMainWindow::performRun()
{
  _settings->sync();
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
  _settings->sync();
  _preferencesAction->setEnabled(false);

  QStringList plist;
  plist << Util::split(_settings->value(Util::V_PARAMETERS).toString());
  plist.removeAll(Util::GUI_NCL);
  plist.removeAll(Util::GUI_FILE);

  plist << "--device-class" << "1";

  configureDefaultFlags(plist);

  _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
  _gingaProxy->run(plist);

  _process = _gingaProxy->process();

  qDebug() << _settings->value(Util::V_LOCATION).toString() << plist;

  _passiveIsRunning = true;
  _settings->setValue(Util::V_PASSIVE, Util::TRUE_);

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
  _settings->sync();
  _preferencesAction->setEnabled(false);

  QStringList plist;
  plist << Util::split(_settings->value(Util::V_PARAMETERS).toString());
  plist.removeAll(Util::GUI_NCL);
  plist.removeAll(Util::GUI_FILE);

  int port =  _settings->value(Util::V_DEVICE_PORT).toInt();
  ++port;

  if (port > 33333)
    _settings->setValue(Util::V_DEVICE_PORT, Util::DEFAULT_PORT);
  else
    _settings->setValue(Util::V_DEVICE_PORT, port);

  plist << "--device-class" << "2";
  plist << "--device-srv-port" << QString::number(port);

  configureDefaultFlags(plist);

  _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
  _gingaProxy->run(plist);

  _process = _gingaProxy->process();

  qDebug() << _settings->value(Util::V_LOCATION).toString() << plist;

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


void QnplMainWindow::configureDefaultFlags(QStringList &plist)
{
  // remove old ---wid use
  int index = plist.indexOf("--wid");
  if (index != -1) {
    if (index + 1 < plist.size()) plist.removeAt(index + 1);

    plist.removeAt(index);
  }

  //configure context file
  QFileInfo fileInfo(_settings->value(Util::V_CONTEXT_FILE).toString());
  QString contextFilePath = fileInfo.absoluteDir().path();
  if (!contextFilePath.isEmpty())
    plist << "--context-dir" << contextFilePath;

  //configure log
  if (_settings->value("enablelog").toBool())
    plist << "--enable-log"
          << "file";

  // configure poll sdin
  plist << "--poll-stdin";

  // configure  if embedded
  if (_settings->value(Util::V_EMBEDDED).toString() == "true") {
#ifdef __linux
    QString ssize = _settings->value(Util::V_SCREENSIZE).toString();
    QString sw = ssize.section('x', 0, 0);
    QString sh = ssize.section('x', 1, 1);
    int w = sw.toInt();
    int h = sh.toInt();
    unsigned long int value = (unsigned long int) _view->focusWidget()->winId();
    plist << "--parent";
    plist << ":0.0," + QString::number(value) + ",0,0," + sw + "," + sh;
#elif defined __WIN32
    QString WID = "";
    foreach (QObject *ob, _view->focusWidget()->children()) {
      QWidget *w = qobject_cast<QWidget *>(ob);
      if (w) {
        unsigned long int value = (unsigned long int) _view->focusWidget()->winId();
        WID = QString::number(value);
      }
    }
    plist << "--wid" << WID;
#endif
  }

  //configure scree nsize
  plist.replaceInStrings(Util::GUI_SCREENSIZE,
                         _settings->value(Util::V_SCREENSIZE).toString());

}

void QnplMainWindow::performCloseWindow()
{
  if (_stackedWidget->currentIndex() == 1)
  {
    _timer->stop();
    QList <QMessageBox *>list = findChildren <QMessageBox *> ();
    foreach (QMessageBox *msgBox, list)
      msgBox->close();


    QMessageBox::warning(this, "Warning",
                         "The signal's strength is too weak to tune this"
                         " channel. Please, check your antenna and try again.",
                         QMessageBox::Ok);

  }
}

void QnplMainWindow::performOpen(QAction* act)
{
  if (act != _clearAction)
    load(act->text());

}

void QnplMainWindow::resizeEvent(QResizeEvent* event)
{
  if (_gingaProxy->gingaIsRunning())
    return;

  QString aspectRatio = _settings->value(Util::V_ASPECT_RATIO, "").toString();

  if (aspectRatio == Util::WIDE)
  {
    setFixedHeight(width () * 9 / 16);
    QCoreApplication::processEvents();
  }
  else if (aspectRatio == Util::STANDARD)
  {
    setFixedHeight(width() * 3 / 4);
    QCoreApplication::processEvents();
  }
  else
  {
    setMinimumSize(0, 0);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  }

  _movie->setScaledSize(_stackedWidget->size());
  _gifLabel->setFixedSize (_stackedWidget->size());

  _settings->setValue(Util::V_SCREENSIZE,
                      QString::number(_stackedWidget->width()) + "x" +
                      QString::number(_stackedWidget->height()));
  event->accept();
  QMainWindow::resizeEvent(event);
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
    playChannel(previousChannel);
}


void QnplMainWindow::scan()
{
  performStop();
  _preferencesAction->setEnabled(false);

  QStringList plist;
  plist << Util::split(_settings->value(Util::V_PARAMETERS).toString());
  plist.removeAll(Util::GUI_NCL);
  plist.removeAll(Util::GUI_FILE);
  plist << "--set-tuner" << "sbtvdt:scan";
  configureDefaultFlags(plist);

  _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
  _gingaProxy->run(plist);
  _scanProgress->setValue(1);

  removeCarouselData();
  _scanProgress->open();
}

void QnplMainWindow::finishScan(int code)
{
  _preferencesAction->setEnabled(true);
  qDebug () << "code::" << code;
  _scanProgress->done(code);
  disconnect(_gingaProxy, SIGNAL(gingaStarted()),
             _scanProgress, SLOT(exec()));
  disconnect(_gingaProxy, SIGNAL(gingaFinished(int, QProcess::ExitStatus)),
             this, SLOT(finishScan(int)));
  disconnect(_gingaProxy, SIGNAL(gingaOutput(QString)),
             this, SLOT(writeTunerOutput(QString)));

  qDebug() <<  "finishScan()::finished";
}


void QnplMainWindow::sendKillMessage()
{
  qDebug () << _gingaProxy->sendCommand(Util::GINGA_QUIT.toStdString().c_str());

  emit scanFinished();
}

void QnplMainWindow::showErrorDialog(QProcess::ProcessError error)
{
  QString errorMessage = "";
  if (error == QProcess::FailedToStart)
    errorMessage = "Error while opening Ginga. Check the binary path.";

  if (errorMessage != "")
    QMessageBox::warning(this, "Warning", errorMessage, QMessageBox::Ok);


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
  if(fileInfo.isDir())
  {
    QDir dir(path);
    QStringList fileList = dir.entryList();

    for(int i = 0; i < fileList.count(); ++i)
    {
      QString entry = fileList.at(i);
      if (entry != "." && entry != "..")
        removePath(path + "/" + entry);
    }

    dir.rmdir(path);
  }
  else
    QFile::remove(path);
}

void QnplMainWindow::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_D && (event->modifiers() & Qt::ControlModifier))
    _developerView->setVisible(!_developerView->isVisible());
  else if (event->key() == Qt::Key_B && (event->modifiers() &
                                         Qt::ControlModifier))
    _debugView->setVisible(!_debugView->isVisible());
}

void QnplMainWindow::handleGingaOutput(QString message)
{
  _developerView->appendConsoleMessage(message);

  QStringList lines = message.split("\n");

  for (int i = 0; i < lines.count(); i++)
  {
    QString line = lines.at(i).trimmed();
    if (line.startsWith(Util::CMD_PREFIX))
    {
      qDebug () << line;
      GingaMessage message = Util::parseMessage(line);

      if (_scanProgress->isVisible())
        writeScanOutput(line);

      if (_isTuning)
        writeTunerOutput(line);

      if (message.command == "cmd" && message.code == "0")
      {
        if (message.messageKey == "startApp")
        {
          _debugView->startSession();
          _debugView->startObject(message.data, false);
        }
        else if (message.messageKey == "start")
          _debugView->startObject(message.data);
        else if (message.messageKey == "stopApp")
          _debugView->stopObject(message.data);
        else if (message.messageKey == "stop")
          _debugView->stopObject(message.data);
      }
    }
  }
}

void QnplMainWindow::startSession()
{
  _developerView->clearSession();
  _debugView->clearSession();
}

void QnplMainWindow::enableSeekButton()
{
  _seekButton->setEnabled(_seekPlayTime->text() != "::");
}
