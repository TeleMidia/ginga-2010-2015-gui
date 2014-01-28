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

  QString size = _settings->value(Util::V_SCREENSIZE, "0x0").toString();

  int width = size.section('x',0,0).toInt();
  int height = size.section('x',1,1).toInt();

  if (width == 0 && height ==0)
  {
    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGHT;
  }
  _view->setSceneRect(0, 0, width, height);

  resize(width, height);
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
  _fileMenu->addAction(_tuneBroadcastChannelAction);
  //fileMenu->addAction(tuneIPTVChannellAction);
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
  _helpMenu->addAction(_bugAction);
  _helpMenu->addSeparator();
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

  _developerView = new DeveloperView;
  addDockWidget(Qt::RightDockWidgetArea, _developerView);
  _developerView->setVisible(false);

  _scanProgress = new QProgressDialog ("Scanning channels...", "Abort",
                                       0, 100, this);
  _scanProgress->setWindowFlags(Qt::Dialog | Qt::Desktop);
  _scanProgress->setWindowTitle("Scanning");
  _scanProgress->setWindowIcon(windowIcon());

  _view = new QnplView(this);
  _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _view->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  _gifLabel = new QLabel();
  _movie = new QMovie(":backgrounds/anim-tuning");
  _gifLabel->setMovie(_movie);
  _movie->start();
  _animTuning = _view->getScene()->addWidget(_gifLabel);
  _animTuning->setVisible(false);
  _gifLabel->setVisible(false);

  setCentralWidget(_view);
}

void QnplMainWindow::createDialogs()
{
  _preferencesDialog = new QnplPreferencesDialog(_settings, this);

  _aboutDialog = new QnplAboutDialog(_settings->value(Util::V_LOCATION)
                                     .toString(), this);

  _channelDialog = new QnplChannelsDialog(this);
  connect(_channelDialog, SIGNAL(scanChannelsRequested()), SLOT(scan()));

  _iptvDialog = new QnplIPTVTunerDialog(this);
  _aplication = new QnplAplicationDialog(this);
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
  connect(_bugAction, SIGNAL(triggered()),
          SLOT(performBug()));
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

void QnplMainWindow::load(QString location)
{
  location = location.replace('/',QDir::separator());
  location = location.replace('\\',QDir::separator());

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

  if (QFile::exists(_location))
  {
    _lastChannel.setNull();

    /*QProcessEnvironment enviroment = QProcessEnvironment::systemEnvironment();
    enviroment.insert("LD_LIBRARY_PATH","/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:")*/;

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

      QFileInfo fileInfo (_settings->value(Util::V_CONTEXT_FILE).toString());
      QString contextFilePath = fileInfo.absoluteDir().path();

      if (!contextFilePath.isEmpty())
        parameters << "--context-dir" << contextFilePath;

      QString WID = "";

      foreach (QObject* ob, _view->children())
      {
        QWidget* w = qobject_cast<QWidget*>(ob);

        if (w)
        {
          WID =  hwndToString(w->winId());
        }
      }

      parameters.replaceInStrings(Util::GUI_WID, WID);

      if (_location.endsWith(".ncl"))
        parameters.replaceInStrings(Util::GUI_FILE, _location);

      else if (_location.endsWith(".ts"))
      {
        int index = parameters.indexOf("--ncl");
        if (index != -1)
        {
          parameters.insert(index, "--set-tuner");
          parameters.removeAt(index + 1);
        }

        parameters.replaceInStrings(Util::GUI_FILE, "file:" + _location);

        _tuneApplicationChannelAction->setEnabled(true);
      }

      parameters.replaceInStrings(Util::GUI_SCREENSIZE,
                                  _settings->value(Util::V_SCREENSIZE)
                                  .toString());

      QFileInfo finfo(_location);
      _gingaProxy->setWorkingDirectory(finfo.absoluteDir().absolutePath());

      qDebug() << _settings->value(Util::V_LOCATION).toString() << parameters;

      _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
      _gingaProxy->run(parameters);

      _process = _gingaProxy->process();
      if (!_process) return;

      //_process->setProcessEnvironment(enviroment);

      connect (_process, SIGNAL(started()),
               SLOT(removeCarouselData()));
      setUpProcessConnections(_process);

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
  if (_timer != NULL)
  {
    _timer->stop();

    delete _timer;
    _timer = NULL;
  }

  if (_process != NULL)
  {
    _animTuning->setVisible(false);
    _gingaProxy->terminateProcess();
  }

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
  _iptvDialog->exec();
  qDebug() << _iptvDialog->ip();
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

    QStringList plist;

    plist << Util::split(_settings->value(Util::V_PARAMETERS).toString());
    plist << "--set-tuner" << "sbtvdt:" + channel.frequency;
    plist << "--poll-stdin";

    QFileInfo fileInfo (_settings->value(Util::V_CONTEXT_FILE).toString());
    plist << "--context-dir" << fileInfo.absoluteDir().path();

    if (_settings->value("enablelog").toBool())
      plist << "--enable-log" << "file";


    plist.replaceInStrings("${SCREENSIZE}",
                           _settings->value(Util::V_SCREENSIZE).toString());
    QString WID = "";

    foreach (QObject* ob, _view->children())
    {
      QWidget* w = qobject_cast<QWidget*>(ob);

      if (w)
      {
        WID =  hwndToString(w->winId());
      }
    }

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
    _gingaProxy->setBinaryPath(_settings->value("location").toString());
    _gingaProxy->run(plist);

    _process = _gingaProxy->process();
    setUpProcessConnections(_process);

    connect(_process, SIGNAL(readyReadStandardOutput()),
            SLOT(writeTunerOutput()));
    connect(_process, SIGNAL(readyReadStandardError()),
            SLOT(writeTunerOutput()));

    _isPlayingChannel = false;

    _timer = new QTimer (this);
    connect (_timer, SIGNAL(timeout()),
             SLOT(stopTuning()));
    _timer->start(15000);

    _animTuning->setVisible(true);

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

void QnplMainWindow::writeTunerOutput()
{
  QString p_stdout = _process->readAllStandardOutput();
  QStringList lines = p_stdout.split("\n");

  for (int i = 0; i < lines.count(); i++)
  {
    QString line = lines.at(i).trimmed();
    qDebug () << line;
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
            if (entity == "start" && msg == "?mAV?")
            {//cmd::0::start::?mAV?
              _isPlayingChannel = true;
              _animTuning->setVisible(false);
              _tuneApplicationChannelAction->setEnabled(true);
            }
          }
          else if (status == "1")
          {
            _timer->stop();

            if (msg != "")
              QMessageBox::warning(this, "Warning", msg, QMessageBox::Ok);

            _animTuning->setVisible(false);
            qint64 bytes = _process->write(Util::GINGA_QUIT.toStdString().
                                           c_str());
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

  for (int i = 0; i < lines.count(); i++)
  {
    QString line = lines.at(i).trimmed();
    if (line.startsWith(Util::CMD_PREFIX))
    {
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
              qint64 bytes = _process->write(Util::GINGA_QUIT.toStdString().
                                             c_str());
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
  _settings->sync();
  _preferencesDialog->exec();

  QString ssize = _settings->value(Util::V_SCREENSIZE).toString();

  QString sw = ssize.section('x',0,0);
  QString sh = ssize.section('x',1,1);

  int w = sw.toInt();
  int h = sh.toInt();

  _view->setSceneRect(0,0,w,h);

  resize(w, h);
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
  QString conf_location = _settings->value(Util::V_CONTEXT_FILE).toString();
  QString context_dir = QFileInfo(conf_location).absoluteDir().path();

  QStringList plist;

  plist << "--wid" << viewWID();
  plist << "--device-class" << "1";
  plist << "--vmode" << _settings->value(Util::V_SCREENSIZE).toString();
  plist << "--context-dir" << context_dir;
  plist << "--disable-multicast";
  plist << "--poll-stdin";

  if (_settings->value("enablelog").toBool())
  {
    plist << "--enable-log" << "file";
  }

  _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
  _gingaProxy->run(plist);

  _process = _gingaProxy->process();
  setUpProcessConnections(_process);

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
  QString conf_location = _settings->value(Util::V_CONTEXT_FILE).toString();
  QString context_dir = QFileInfo(conf_location).absoluteDir().path();

  int port =  _settings->value(Util::V_DEVICE_PORT).toInt();
  ++port;

  if (port > 33333)
    _settings->setValue(Util::V_DEVICE_PORT, Util::DEFAULT_PORT);
  else
    _settings->setValue(Util::V_DEVICE_PORT, port);

  QStringList plist;
  plist << "--wid" << viewWID();
  plist << "--device-class" << "2";
  plist << "--device-srv-port" << QString::number(port);
  plist << "--vmode" << _settings->value(Util::V_SCREENSIZE).toString();
  plist << "--context-dir" << context_dir;
  plist << "--poll-stdin";

  if (_settings->value("enablelog").toBool())
    plist << "--enable-log" << "file";


  _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
  _gingaProxy->run(plist);

  _process = _gingaProxy->process();
  setUpProcessConnections(_process);

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

void QnplMainWindow::performCloseWindow(int, QProcess::ExitStatus)
{
  if (_animTuning->isVisible())
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
    load(act->text());

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
  qDebug () << "resizing";

  QString aspectRatio = _settings->value(Util::V_ASPECT_RATIO, "").toString();

  if (aspectRatio == Util::WIDE)
  {
    setFixedHeight(width () * 9 / 16);
    QCoreApplication::processEvents();
    event->accept();
  }
  else if (aspectRatio == Util::STANDARD)
  {
    setFixedHeight(width() * 3 / 4);
    QCoreApplication::processEvents();
    event->accept();
  }
  else
  {
    setMinimumSize(0, 0);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  }


  int w_span = 20;
  int h_span = 100;

  qreal w = width() - w_span ;
  qreal h = height() - h_span;

  _view->setSceneRect (0,0, w, h);

  _animTuning->setPos(0, -h_span);

  _movie->setScaledSize(QSize (w + w_span, h + h_span));
  _gifLabel->setFixedSize (w + w_span, h + h_span);

  _settings->setValue(Util::V_SCREENSIZE,
                      QString::number(width())+ "x" +QString::number(height()));

  if (!event->isAccepted())
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

  QStringList plist;
  plist << "--set-tuner" << "sbtvdt:scan";
  plist << "--wid" << hwndToString(_scanProgress->winId());
  plist << "--poll-stdin";

  if (_settings->value("enablelog").toBool())
    plist << "--enable-log" << "file";

  _gingaProxy->setBinaryPath(_settings->value(Util::V_LOCATION).toString());
  _gingaProxy->run(plist);
  _scanProgress->setValue(1);

  _process = _gingaProxy->process();

  removeCarouselData();

  connect(_process, SIGNAL(readyReadStandardOutput()),
          SLOT(writeScanOutput()));
  connect(_process, SIGNAL(readyReadStandardError()),
          SLOT(writeScanOutput()));

  connect(_gingaProxy, SIGNAL(gingaStarted()),
          _scanProgress, SLOT(exec()));
  connect(_gingaProxy, SIGNAL(gingaFinished(int,QProcess::ExitStatus)),
            this, SLOT(finishScan(int)));
}

void QnplMainWindow::finishScan(int code)
{
  qDebug () << "code::" << code;
  _scanProgress->done(code);
  disconnect(_gingaProxy, SIGNAL(gingaStarted()),
          _scanProgress, SLOT(exec()));
  disconnect(_gingaProxy, SIGNAL(gingaFinished(int, QProcess::ExitStatus)),
          this, SLOT(finishScan(int)));

  qDebug() <<  "finishScan()::finished";
}


void QnplMainWindow::sendKillMessage()
{
  disconnect(_process, SIGNAL(readyReadStandardOutput()),
             this, SLOT(writeScanOutput()));
  disconnect(_process, SIGNAL(readyReadStandardError()),
             this, SLOT(writeScanOutput()));

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
}

void QnplMainWindow::setUpProcessConnections(QProcess *process)
{
  if (process)
  {
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)),
            SLOT(performCloseWindow(int, QProcess::ExitStatus)));
    connect(process, SIGNAL(error(QProcess::ProcessError)),
            SLOT(showErrorDialog(QProcess::ProcessError)));
    connect(process, SIGNAL(started()),
            SLOT(removeCarouselData()));
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
void QnplMainWindow::enableSeekButton()
{
  _seekButton->setEnabled(_seekPlayTime->text() != "::");
}
