#include "qnplmainwindow.h"

#include <QLayout>
#include <QDir>
#include <QDebug>
#include <QProcessEnvironment>

QnplMainWindow::QnplMainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  setWindowTitle("Ginga GUI");
  setWindowIcon(QIcon(":icon/gingagui-128x128"));
  settings = new QnplSettings();

  createActions();
  createMenus();
  createRecent();
  createWidgets();
  createDialogs();
  createToolbars();
  createConnections();

  location = "";
  process = NULL;


  if (settings->value("run_as") == "base")
  {
    baseAction->setChecked(true);
  }
  else if (settings->value("run_as") == "passive")
  {
    passiveAction->setChecked(true);
  }
  else if (settings->value("run_as") == "active")
  {
    activeAction->setChecked(true);
  }

  passiveIsRunning = false;

  QString ssize = settings->value("screensize").toString();

  qDebug() << "open" << ssize;

  QString sw = ssize.section('x',0,0);
  QString sh = ssize.section('x',1,1);

  int w = sw.toInt();
  int h = sh.toInt();

  view->setSceneRect(0,0,w,h);

  resize(w+20, h+100);
}

QnplMainWindow::~QnplMainWindow()
{
  performStop();
}

void QnplMainWindow::performDevice()
{
  if (baseAction->isChecked())
  {
    settings->setValue("run_as", "base");
  }
  else if (passiveAction->isChecked())
  {
    settings->setValue("run_as", "passive");
  }
  else if (activeAction->isChecked())
  {
    settings->setValue("run_as", "active");
  }
}

void  QnplMainWindow::createActions()
{
  // open action
  openAction = new QAction(this);
  openAction->setEnabled(true);
  openAction->setText(tr("Open..."));
  openAction->setShortcut(QKeySequence("Ctrl+O"));

  // quit action
  quitAction = new QAction(this);
  quitAction->setEnabled(true);
  quitAction->setText(tr("Quit"));

  // clear action
  clearAction = new QAction(this);
  clearAction->setEnabled(true);
  clearAction->setText(tr("Clear Menu"));

  // base action
  baseAction = new QAction(this);
  baseAction->setEnabled(true);
  baseAction->setCheckable(true);
  baseAction->setText(tr("Default (Base)"));

  // passive action
  passiveAction = new QAction(this);
  passiveAction->setEnabled(true);
  passiveAction->setCheckable(true);
  passiveAction->setText(tr("Passive"));

  // active action
  activeAction = new QAction(this);
  activeAction->setEnabled(true);
  activeAction->setCheckable(true);
  activeAction->setText(tr("Active"));

  // preferences action
  preferencesAction = new QAction(this);
  preferencesAction->setEnabled(true);
  preferencesAction->setText(tr("Preferences..."));

  // bug action
  bugAction = new QAction(this);
  bugAction->setEnabled(true);
  bugAction->setText(tr("Report Bug..."));

  // about action
  aboutAction = new QAction(this);
  aboutAction->setEnabled(true);
  aboutAction->setText(tr("About"));

  // device group
  deviceGroup = new QActionGroup(this);
  deviceGroup->setExclusive(true);
  deviceGroup->addAction(baseAction);
  deviceGroup->addAction(passiveAction);
  deviceGroup->addAction(activeAction);
}

void  QnplMainWindow::createMenus()
{
  // recent menu
  recentMenu = new QMenu();
  recentMenu->setTitle(tr("Open Recent"));
  recentMenu->addSeparator();
  recentMenu->addAction(clearAction);

  // file menu
  fileMenu = menuBar()->addMenu(tr("File"));
  fileMenu->addAction(openAction);
  fileMenu->addMenu(recentMenu);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);

  // device menu
  deviceMenu = menuBar()->addMenu(tr("Device"));
  deviceMenu->addAction(baseAction);
  deviceMenu->addAction(passiveAction);
  deviceMenu->addAction(activeAction);

  // window menu
  windowMenu = menuBar()->addMenu(tr("Window"));
  windowMenu->addAction(preferencesAction);

  // help menu
  helpMenu = menuBar()->addMenu(tr("Help"));
  helpMenu->addAction(bugAction);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutAction);
}

void QnplMainWindow::createRecent()
{
  recentMenu->clear();

  foreach(QString file, settings->value("files").toStringList())
  {
    QAction* action = new QAction(this);
    action->setText(file);

    recentMenu->addAction(file);
  }

  recentMenu->addSeparator();
  recentMenu->addAction(clearAction);
}

void QnplMainWindow::createWidgets()
{
  playButton = new QPushButton(this);
  playButton->setEnabled(true);
  playButton->setIcon(QIcon(":icon/play"));
  playButton->setToolTip(tr("Play"));

  stopButton = new QPushButton(this);
  stopButton->setEnabled(false);
  stopButton->setIcon(QIcon(":icon/stop"));
  stopButton->setToolTip(tr("Stop"));

  openButton = new QPushButton(this);
  openButton->setEnabled(true);
  openButton->setIcon(QIcon(":icon/open"));
  openButton->setToolTip(tr("Open a new document"));

  openLine = new QLineEdit(this);
  openLine->setEnabled(true);

  view = new QnplView(this); setCentralWidget(view);
}

void QnplMainWindow::createDialogs()
{
  preferencesDialog = new QnplPreferencesDialog(this);

  aboutDialog = new QnplAboutDialog(this);
}

void QnplMainWindow::createToolbars()
{
  // play toolbar
  playToolbar = new QToolBar(tr("Play"));
  playToolbar->setMovable(false);
  playToolbar->setFloatable(false);
  playToolbar->addWidget(playButton);
  playToolbar->addWidget(stopButton);

  addToolBar(Qt::BottomToolBarArea, playToolbar);

  // open toolbar
  openToolbar = new QToolBar(tr("Open"));
  openToolbar->setMovable(false);
  openToolbar->setFloatable(false);
  openToolbar->addWidget(openLine);
  openToolbar->addWidget(openButton);

  addToolBar(Qt::BottomToolBarArea, openToolbar);
}

void  QnplMainWindow::createConnections()
{
  connect(openAction, SIGNAL(triggered()), SLOT(performOpen()));
  connect(recentMenu,SIGNAL(triggered(QAction*)),SLOT(performOpen(QAction*)));
  connect(clearAction, SIGNAL(triggered()),SLOT(performClear()));
  connect(quitAction, SIGNAL(triggered()), SLOT(performQuit()));

  connect(baseAction, SIGNAL(triggered()), SLOT(performDevice()));
  connect(passiveAction, SIGNAL(triggered()), SLOT(performDevice()));
  connect(activeAction, SIGNAL(triggered()), SLOT(performDevice()));

  connect(preferencesAction, SIGNAL(triggered()), SLOT(performPreferences()));

  connect(bugAction, SIGNAL(triggered()),SLOT(performBug()));
  connect(aboutAction, SIGNAL(triggered()),SLOT(performAbout()));

  connect(openButton, SIGNAL(clicked()), SLOT(performOpen()));
  connect(playButton, SIGNAL(clicked()), SLOT(performRun()));
  connect(stopButton, SIGNAL(clicked()), SLOT(performStop()));

  connect(view,SIGNAL(keyPressed(QString)),SLOT(notifyKey(QString)));
}


void QnplMainWindow::performOpen()
{
  qDebug() << view->winId();;

  if (location != "*"){
    performClose();
  }

  QString f = QFileDialog::getOpenFileName(this,"Open File",
                                           settings->value("lastdir_opened").toString(),"NCL (*.ncl)");

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

  this->location = location;

  openLine->setText(location);

  playButton->setEnabled(true);

  setWindowTitle("Ginga GUI - "+QFileInfo(location).completeBaseName()+"."+
                 QFileInfo(location).completeSuffix());

  if (settings->value("autoplay").toString() == "true"){
    performPlay();
  }
}

void QnplMainWindow::performClear()
{
  settings->setValue("files",QStringList());

  recentMenu->clear();

  recentMenu->addSeparator();
  recentMenu->addAction(clearAction);
}

void QnplMainWindow::performClose()
{
  performStop();

  location = "*";

  playButton->setEnabled(false);
  stopButton->setEnabled(false);

  openLine->setText("");

  setWindowTitle("Ginga GUI");
}

void QnplMainWindow::performQuit()
{
  performStop();
  performClose();

  if (process != NULL){
    process->close();

    disconnect(process);

    delete(process);

    process = NULL;
  }


  exit(0);
}

void QnplMainWindow::performPlay()
{
  if (QFile::exists(location))
  {

#ifdef Q_OS_LINUX
    QProcessEnvironment enviroment = QProcessEnvironment::systemEnvironment();
    enviroment.insert("LD_LIBRARY_PATH","/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
#endif

    QStringList parameters;

    process = new QProcess(this);

    connect(process, SIGNAL(finished(int)), SLOT(performCloseWindow(int)), Qt::QueuedConnection);


    playButton->setEnabled(false);
    stopButton->setEnabled(true);

    openButton->setEnabled(false);
    openAction->setEnabled(false);

    openLine->setEnabled(false);

    recentMenu->setEnabled(false);

    baseAction->setEnabled(false);
    passiveAction->setEnabled(false);
    activeAction->setEnabled(false);

    // playing as base device
    if (baseAction->isChecked())
    {
      parameters = QnplUtil::split(settings->value("parameters").toString());

      parameters << "--context-dir" << settings->value("gingaconfig_file").toString();

      if (settings->value("enablelog").toBool()){
        parameters << "--enable-log" << "file";
      }

      parameters.replaceInStrings("${WID}", hwndToString(view->winId()));
      parameters.replaceInStrings("${NCLFILE}", location);
      parameters.replaceInStrings("${SCREENSIZE}", settings->value("screensize").toString());


#ifdef Q_OS_LINUX
      process->setProcessEnvironment(enviroment);
#endif

      QFileInfo finfo(location);
      process->setWorkingDirectory(finfo.absoluteDir().absolutePath());

      qDebug() << settings->value("location").toString() << parameters;

      // executing
      process->start(settings->value("location").toString(), parameters);
    }
    // play as passive device
    else if (passiveAction->isChecked())
    {

      performRunAsPassive();
    }
    // play as active device
    else if (activeAction->isChecked())
    {

      performRunAsActive();
    }
  }
  else
  {
    QMessageBox::information(this,tr("Information"), tr("Please, open NCL document to play."));
  }
}

void QnplMainWindow:: performStop()
{
  view->releaseKeyboard();

  if (process != NULL){
    process->close();

    disconnect(process);

    delete(process);

    process = NULL;
  }

  if (passiveIsRunning){
    passiveIsRunning = false;
    settings->setValue("passive_running", false);
  }

  playButton->setEnabled(true);
  stopButton->setEnabled(false);

  openButton->setEnabled(true);
  openAction->setEnabled(true);

  openLine->setEnabled(true);

  recentMenu->setEnabled(true);

  baseAction->setEnabled(true);
  passiveAction->setEnabled(true);
  activeAction->setEnabled(true);
}

void QnplMainWindow::performPreferences()
{
  preferencesDialog->init(settings);
  preferencesDialog->exec();

  QString ssize = settings->value("screensize").toString();

  QString sw = ssize.section('x',0,0);
  QString sh = ssize.section('x',1,1);

  int w = sw.toInt();
  int h = sh.toInt();

  view->setSceneRect(0,0,w,h);

  resize(w+20, h+100);
}

void QnplMainWindow::performBug()
{
  QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/~edcaraujo/gingagui/"));
}

void QnplMainWindow::performAbout()
{
  aboutDialog->show();
}

void QnplMainWindow::performRun()
{
  if (baseAction->isChecked())
  {
    qDebug() << "run as base";
    performPlay();
  }
  else if (passiveAction->isChecked())
  {
    qDebug() << "run as passive";
    performRunAsPassive();
  }
  else if (activeAction->isChecked())
  {
    qDebug() << "run as active";
    performRunAsActive();
  }
}

void QnplMainWindow::performRunAsPassive()
{
  if (!settings->value("passive_running").toBool())
  {
    QString conf_location = settings->value("gingaconfig_file").toString();
    QString context_dir = QFileInfo(conf_location).absoluteDir().path();

    QStringList plist;
    plist << "--wid" << hwndToString(view->winId());
    plist << "--device-class" << "1";
    plist << "--vmode" << settings->value("screensize").toString();
    plist << "--context-dir" << context_dir;
    plist << "--disable-multicast";

    if (settings->value("enablelog").toBool()){
      plist << "--enable-log" << "file";
    }

    process = new QProcess(this);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
    process->setProcessEnvironment(env);

    connect(process, SIGNAL(finished(int)), SLOT(performCloseWindow(int)), Qt::QueuedConnection);

    qDebug() << settings->value("location").toString() << plist;

    process->start(settings->value("location").toString(), plist);

    passiveIsRunning = true;
    settings->setValue("passive_running", true);

    playButton->setEnabled(false);
    stopButton->setEnabled(true);

    openButton->setEnabled(false);
    openAction->setEnabled(false);

    openLine->setEnabled(false);

    recentMenu->setEnabled(false);

    baseAction->setEnabled(false);
    passiveAction->setEnabled(false);
    activeAction->setEnabled(false);
  }
  else
  {
    QMessageBox::information(this,tr("Information"), tr("Only one passive device client is allowed per host."));
  }
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
  plist << "--wid" << hwndToString(view->winId());
  plist << "--device-class" << "2";
  plist << "--device-srv-port" << QString::number(port);
  plist << "--vmode" << settings->value("screensize").toString();
  plist << "--context-dir" << context_dir;

  if (settings->value("enablelog").toBool()){
    plist << "--enable-log" << "file";
  }

  process = new QProcess(this);

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
  process->setProcessEnvironment(env);

  connect(process, SIGNAL(finished(int)), SLOT(performCloseWindow(int)), Qt::QueuedConnection);

  qDebug() << settings->value("location").toString() << plist;

  process->start(settings->value("location").toString(), plist);

  playButton->setEnabled(false);
  stopButton->setEnabled(true);

  openButton->setEnabled(false);
  openAction->setEnabled(false);

  openLine->setEnabled(false);

  recentMenu->setEnabled(false);

  baseAction->setEnabled(false);
  passiveAction->setEnabled(false);
  activeAction->setEnabled(false);
}

void QnplMainWindow::performCloseWindow(int)
{
  performStop();
}

void QnplMainWindow::notifyKey(QString key)
{
  if (process != NULL)
  {
    qDebug() << "Writing:" << key;

    process->write(QString(key+"\n").toStdString().c_str());
  }
}

void QnplMainWindow::performOpen(QAction* act)
{
  if (act != clearAction)
  {
    load(act->text());
  }
}

QString QnplMainWindow::hwndToString(WId winid)
{
#ifdef Q_WS_WIN
  void* vhwnd = winid;
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
}

void QnplMainWindow::resizeEvent(QResizeEvent* event)
{
  QMainWindow::resizeEvent(event);

  qDebug() << "resizing...";

  qreal w = event->size().width()-20;
  qreal h = event->size().height()-100;

  settings->setValue("screensize",QString::number(w)+"x"+QString::number(h));

  view->setSceneRect(0,0,w,h);
}


