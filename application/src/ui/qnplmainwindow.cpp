#include "qnplmainwindow.h"

#include <QLayout>
#include <QDir>
#include <QDebug>
#include <QProcessEnvironment>

QnplMainWindow::QnplMainWindow(QnplSettings* settings, QWidget* parent)
    : QMainWindow(parent)
{
    location = "*";
    process = NULL;

    this->settings = settings;

    passiveIsRunning = false;

    createActions();
    createMenus();
    createWidgets();
    createToolbars();
    createConnections();
}

QnplMainWindow::~QnplMainWindow()
{
	performStop();
}

void QnplMainWindow::saveRunMode()
{
  if (runAsBaseAction->isChecked())
  {
    settings->setValue("run_as", "base");
  }
  else if (runAsPassiveAction->isChecked())
  {
    settings->setValue("run_as", "passive");
  }
  else if (runAsActiveAction->isChecked())
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

    runAsBaseAction = new QAction(this);
    runAsBaseAction->setEnabled(true);
    runAsBaseAction->setCheckable(true);
    runAsBaseAction->setText(tr("Run as Base Device"));

    // run as passive action
    runAsPassiveAction = new QAction(this);
    runAsPassiveAction->setEnabled(true);
    runAsPassiveAction->setCheckable(true);
    runAsPassiveAction->setText(tr("Run as Passive Device"));

    // run as active action
    runAsActiveAction = new QAction(this);
    runAsActiveAction->setEnabled(true);
    runAsActiveAction->setCheckable(true);
    runAsActiveAction->setText(tr("Run as Active Device"));

    runGroup = new QActionGroup(this);
    runGroup->setExclusive(true);
    runGroup->addAction(runAsBaseAction);
    runGroup->addAction(runAsPassiveAction);
    runGroup->addAction(runAsActiveAction);

    // clear action
    clearAction = new QAction(this);
    clearAction->setEnabled(true);
    clearAction->setText(tr("Clear Menu"));

    // close action
    closeAction = new QAction(this);
    closeAction->setEnabled(false);
    closeAction->setText(tr("Close"));
    closeAction->setShortcut(QKeySequence("Ctrl+W"));

    // quit action
    quitAction = new QAction(this);
    quitAction->setEnabled(true);
    quitAction->setText(tr("Quit"));

    // play action
    playAction = new QAction(this);
    playAction->setEnabled(true);
    playAction->setText(tr("Play"));
//    playAction->setIcon(QIcon(":icon/play"));

    // stop action
    stopAction = new QAction(this);
    stopAction->setEnabled(false);
    stopAction->setText(tr("Stop"));
//    stopAction->setIcon(QIcon(":icon/stop"));

    // preferences action
    preferencesAction = new QAction(this);
    preferencesAction->setEnabled(true);
    preferencesAction->setText(tr("Preferences..."));

    // bug action
    bugAction = new QAction(this);
    bugAction->setEnabled(false);
    bugAction->setText(tr("Report Bug..."));

    // about action
    aboutAction = new QAction(this);
    aboutAction->setEnabled(true);
    aboutAction->setText(tr("About"));

    if (settings->value("run_as") == "base")
    {
      runAsBaseAction->setChecked(true);
    }
    else if (settings->value("run_as") == "passive")
    {
      runAsPassiveAction->setChecked(true);
    }
    else if (settings->value("run_as") == "active")
    {
      runAsActiveAction->setChecked(true);
    }
}

void  QnplMainWindow::createMenus()
{
    // open menu
    openMenu = new QMenu();
    openMenu->setTitle(tr("Open Recent"));

    openMenu->addSeparator();
    openMenu->addAction(clearAction);

    // file menu
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAction);
    fileMenu->addMenu(openMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(runAsBaseAction);
    fileMenu->addAction(runAsPassiveAction);
    fileMenu->addAction(runAsActiveAction);
//    fileMenu->addAction(closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    // playback menu
//    playMenu = menuBar()->addMenu(tr("Playback"));
//    playMenu->addAction(playAction);
//    playMenu->addAction(stopAction);

    // window menu
    windowMenu = menuBar()->addMenu(tr("Window"));
    windowMenu->addAction(preferencesAction);

    // help menu
    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(bugAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);

    buildRecents();
}

void QnplMainWindow::createToolbars()
{
    // play toolbar
    playToolbar = new QToolBar(tr("Play"));
    playToolbar->setMovable(false);
    playToolbar->setFloatable(false);

//    playToolbar->addAction(playAction);
//    playToolbar->addAction(stopAction);
    playToolbar->addWidget(playButton);
    playToolbar->addWidget(stopButton);

    addToolBar(Qt::BottomToolBarArea, playToolbar);

    // open toolbar
    openToolbar = new QToolBar(tr("Open"));
    openToolbar->setMovable(false);
    openToolbar->setFloatable(false);


    //openToolbar->addWidget(new QLabel("Open:",openToolbar));
    openToolbar->addWidget(openLine);
    openToolbar->addWidget(openButton);

    addToolBar(Qt::BottomToolBarArea, openToolbar);
}

void QnplMainWindow::createWidgets()
{
    playButton = new QPushButton(this);
    playButton->setEnabled(true);
    playButton->setIcon(QIcon(":icon/play"));
//    playButton->setText(tr("Play"));
    playButton->setToolTip(tr("Play"));
//    playButton->setStyleSheet("width: 10px;");

    stopButton = new QPushButton(this);
    stopButton->setEnabled(false);
    stopButton->setIcon(QIcon(":icon/stop"));
//    stopButton->setText(tr("Stop"));
    stopButton->setToolTip(tr("Stop"));
//    stopButton->setStyleSheet("width: 10px;");

    openButton = new QPushButton(this);
    openButton->setEnabled(true);
//    openButton->setText(tr("..."));
    openButton->setIcon(QIcon(":icon/open"));
    openButton->setToolTip(tr("Open a new document"));

    openLine = new QLineEdit(this);
    openLine->setEnabled(true);

    view = new QnplView(this); setCentralWidget(view);

    preferencesDialog = new QnplPreferencesDialog(this);

    aboutDialog = new QnplAboutDialog(this);
}

void  QnplMainWindow::createConnections()
{
    connect(openAction, SIGNAL(triggered()), SLOT(performOpen()));
    connect(closeAction, SIGNAL(triggered()), SLOT(performClose()));
    connect(playAction, SIGNAL(triggered()), SLOT(performRun()));
    connect(stopAction, SIGNAL(triggered()), SLOT(performStop()));
    connect(quitAction, SIGNAL(triggered()), SLOT(performQuit()));
    connect(preferencesAction, SIGNAL(triggered()), SLOT(performPreferences()));
    connect(clearAction, SIGNAL(triggered()),SLOT(performClear()));
    connect(aboutAction, SIGNAL(triggered()),SLOT(performAbout()));

    connect(runAsBaseAction, SIGNAL(clicked()), SLOT(saveRunMode()));
    connect(runAsPassiveAction, SIGNAL(clicked()), SLOT(saveRunMode()));
    connect(runAsActiveAction, SIGNAL(clicked()), SLOT(saveRunMode()));

    connect(openButton, SIGNAL(clicked()), SLOT(performOpen()));
    connect(playButton, SIGNAL(clicked()), SLOT(performRun()));
    connect(stopButton, SIGNAL(clicked()), SLOT(performStop()));

    connect(openMenu,SIGNAL(triggered(QAction*)),SLOT(performRecentOpen(QAction*)));
}

void QnplMainWindow::buildRecents()
{
    openMenu->clear();

    QStringList recents = settings->value("recents").toStringList();

    if (recents.count() > 0){
        foreach(QString r, recents){
            QAction* act = new QAction(this);
            act->setText(r);

            openMenu->addAction(act);

        }
    }

    openMenu->addSeparator();
    openMenu->addAction(clearAction);
}

void QnplMainWindow::performOpen()
{
    if (location != "*"){
        performClose();
    }

    QString f = QFileDialog::getOpenFileName(this,"Open File",
                settings->value("lastdir_opened").toString());

    if (QFile::exists(f)){
        QDir d(f); settings->setValue("lastdir_opened", d.absolutePath());

        performOpen(f);
    }
}

void QnplMainWindow::performOpen(QString file)
{
    QStringList recents = settings->value("recents").toStringList();

    QStringList newRecents;

    newRecents << file << recents;

    newRecents.removeDuplicates();

    while(newRecents.count() > 5){
        newRecents.pop_back();
    }

    settings->setValue("recents", newRecents); buildRecents();

    location = file;

    openLine->setText(location);

    playButton->setEnabled(true);
    playAction->setEnabled(true);

    closeAction->setEnabled(true);

    setWindowTitle("Ginga - "+QFileInfo(location).completeBaseName()+"."+
                   QFileInfo(location).completeSuffix());

    if (settings->value("autoplay").toString() == "true"){
        performPlay();
    }
}

void QnplMainWindow::performClear()
{
    settings->setValue("recents",QStringList());

    openMenu->clear();

    openMenu->addSeparator();
    openMenu->addAction(clearAction);
}

void QnplMainWindow::performClose()
{
    performStop();

    location = "*";

    playButton->setEnabled(false);
    stopButton->setEnabled(false);

    playAction->setEnabled(false);
    stopAction->setEnabled(false);

    closeAction->setEnabled(false);

    openLine->setText("");

    setWindowTitle("Ginga");
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


    qDebug() << "Playing" << location;

    if (location != "*" && location != ""){

        if (location != openLine->text()){
            performOpen(openLine->text());
        }

        QStringList plist;

        QString ps = settings->value("parameters").toString();

        QRegExp rx; rx.setPattern("([-${}\\w\\\\:]+|\\\".*\\\")");

        int pos = 0;
        while ((pos = rx.indexIn(ps, pos)) != -1) {
            plist << rx.cap(1);

            pos += rx.matchedLength();
        }

        QString conf_location = settings->value("gingaconfig_file").toString();

        QString context_dir = QFileInfo(conf_location).absoluteDir().path();

        plist.replaceInStrings("${NCLFILE}", location);
        plist.replaceInStrings("${SCREENSIZE}", settings->value("screensize").toString());

        plist << "--context-dir" << context_dir;

        process = new QProcess(this);
    
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
        process->setProcessEnvironment(env);
        
        connect(process, SIGNAL(finished(int)), SLOT(performCloseWindow(int)), Qt::QueuedConnection);

        QFileInfo finfo(location);
        process->setWorkingDirectory(finfo.absoluteDir().absolutePath());

//        qDebug() << settings->value("location").toString() << plist;

        process->start(settings->value("location").toString(), plist);

        playButton->setEnabled(false);
        stopButton->setEnabled(true);

        playAction->setEnabled(false);
        stopAction->setEnabled(true);

        openButton->setEnabled(false);
        openAction->setEnabled(false);

        openLine->setEnabled(false);

        openMenu->setEnabled(false);

        runAsPassiveAction->setEnabled(false);
        runAsActiveAction->setEnabled(false);
    }
    else
    {
      QMessageBox::information(this,tr("Information"), tr("Please, open NCL document to play."));
    }
}

void QnplMainWindow::performStop()
{
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

    playAction->setEnabled(true);
    stopAction->setEnabled(false);

    openButton->setEnabled(true);
    openAction->setEnabled(true);

    openLine->setEnabled(true);

    openMenu->setEnabled(true);

    runAsPassiveAction->setEnabled(true);
    runAsActiveAction->setEnabled(true);
}

void QnplMainWindow::performPreferences()
{
    preferencesDialog->init(settings);
    preferencesDialog->exec();
}

void QnplMainWindow::performBug()
{
    // TODO
}

void QnplMainWindow::performAbout()
{
    aboutDialog->show();
}

void QnplMainWindow::performRun()
{
  if (runAsBaseAction->isChecked())
  {
    qDebug() << "run as base";
    performPlay();
  }
  else if (runAsPassiveAction->isChecked())
  {
    qDebug() << "run as passive";
    performRunAsPassive();
  }
  else if (runAsActiveAction->isChecked())
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
      plist << "--device-class" << "1";
      plist << "--vmode" << settings->value("screensize").toString();
      plist << "--context-dir" << context_dir;
      plist << "--disable-multicast";

      process = new QProcess(this);

      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
      process->setProcessEnvironment(env);

      connect(process, SIGNAL(finished(int)), SLOT(performCloseWindow(int)), Qt::QueuedConnection);

      process->start(settings->value("location").toString(), plist);

      passiveIsRunning = true;
      settings->setValue("passive_running", true);

      playButton->setEnabled(false);
      stopButton->setEnabled(true);

      playAction->setEnabled(false);
      stopAction->setEnabled(true);

      openButton->setEnabled(false);
      openAction->setEnabled(false);

      openLine->setEnabled(false);

      openMenu->setEnabled(false);

      runAsPassiveAction->setEnabled(false);
      runAsActiveAction->setEnabled(false);
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
    plist << "--device-class" << "2";
    plist << "--device-srv-port" << QString::number(port);
    plist << "--vmode" << settings->value("screensize").toString();
    plist << "--context-dir" << context_dir;

    process = new QProcess(this);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
    process->setProcessEnvironment(env);

    connect(process, SIGNAL(finished(int)), SLOT(performCloseWindow(int)), Qt::QueuedConnection);

    process->start(settings->value("location").toString(), plist);

    playButton->setEnabled(false);
    stopButton->setEnabled(true);

    playAction->setEnabled(false);
    stopAction->setEnabled(true);

    openButton->setEnabled(false);
    openAction->setEnabled(false);

    openLine->setEnabled(false);

    openMenu->setEnabled(false);

    runAsPassiveAction->setEnabled(false);
    runAsActiveAction->setEnabled(false);
}

void QnplMainWindow::performCloseWindow(int)
{
    performStop();
}

void QnplMainWindow::performRecentOpen(QAction* act)
{
    if (act != clearAction)
    {
        performOpen(act->text());
    }
}
