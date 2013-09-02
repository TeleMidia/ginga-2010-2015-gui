#include "qnplmainwindow.h"

#include <QLayout>
#include <QDir>
#include <QDebug>
#include <QProcessEnvironment>
#include <QMovie>


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

    scanProgress = new QProgressDialog ("Scanning channels...", "Abort", 0, 100, this);
    scanProgress->setWindowFlags(Qt::Dialog | Qt::Desktop);
    scanProgress->setWindowTitle("Scanning");
    scanProgress->setWindowIcon(windowIcon());

    baseAction->setChecked(true);

    passiveIsRunning = false;

    QString ssize = settings->value("screensize").toString();

    qDebug() << "open" << ssize;

    QString sw = ssize.section('x',0,0);
    QString sh = ssize.section('x',1,1);

    int w = sw.toInt();
    int h = sh.toInt();

    view->setSceneRect(0,0,w,h);

    gifLabel = new QLabel();
    movie = new QMovie("bg-tuning.gif");
    gifLabel->setMovie(movie);

    movie->start();
    animTuning = view->getScene()->addWidget(gifLabel);
    animTuning->setVisible(false);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

    tuneAppChannellAction = new QAction(this);
    tuneAppChannellAction->setEnabled(true);
    tuneAppChannellAction->setText(tr("Tune Application Channel..."));

    tuneBroadChannellAction = new QAction(this);
    tuneBroadChannellAction->setEnabled(true);
    tuneBroadChannellAction->setText(tr("Tune Broadcast Channel..."));

    tuneIPTVChannellAction = new QAction(this);
    tuneIPTVChannellAction->setEnabled(true);
    tuneIPTVChannellAction->setText(tr("Tune IPTV Channel..."));


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
    fileMenu->addAction(tuneBroadChannellAction);
    //fileMenu->addAction(tuneIPTVChannellAction);
//    fileMenu->addAction(tuneAppChannellAction);
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
    playButton = new QPushButton();
    playButton->setEnabled(true);
    playButton->setIcon(QIcon(":icon/play"));
    playButton->setToolTip(tr("Play"));

    stopButton = new QPushButton();
    stopButton->setEnabled(false);
    stopButton->setIcon(QIcon(":icon/stop"));
    stopButton->setToolTip(tr("Stop"));

    openButton = new QPushButton();
    openButton->setEnabled(true);
    openButton->setIcon(QIcon(":icon/open"));
    openButton->setToolTip(tr("Open a new document"));

    nextButton = new QPushButton();
    nextButton->setEnabled(false);
    nextButton->setIcon(QIcon(":icon/up"));
    nextButton->setToolTip(tr("Next Channel"));

    previousButton = new QPushButton();
    previousButton->setEnabled(false);
    previousButton->setIcon(QIcon(":icon/down"));
    previousButton->setToolTip(tr("Previous Channel"));

    refreshButton = new QPushButton();
    refreshButton->setIcon(QIcon(":icon/refresh"));
    refreshButton->setToolTip(tr("Scan"));

    connect(refreshButton, SIGNAL(clicked()), SLOT(scan()));

    channelsButton = new QPushButton();
    channelsButton->setEnabled(true);
    channelsButton->setIcon(QIcon(":icon/channels"));
    channelsButton->setToolTip(tr("Channel List"));

    openLine = new QLineEdit(this);
    openLine->setEnabled(true);

    view = new QnplView(this); setCentralWidget(view);
}

void QnplMainWindow::createDialogs()
{
    preferencesDialog = new QnplPreferencesDialog(this);

    aboutDialog = new QnplAboutDialog(this);

    channelDialog = new QnplChannelsDialog(this);
    connect(channelDialog, SIGNAL(scanChannelsRequested()), SLOT(scan()));

    iptvDialog = new QnplIPTVTunerDialog(this);
    aplication = new QnplAplicationDialog(this);
}

void QnplMainWindow::createToolbars()
{
    // play toolbar
    playToolbar = new QToolBar(tr("Play"));
    playToolbar->setMovable(false);
    playToolbar->setFloatable(false);
    playToolbar->addWidget(playButton);
    playToolbar->addWidget(stopButton);
    playToolbar->addSeparator();

    addToolBar(Qt::BottomToolBarArea, playToolbar);

    // open toolbar
    openToolbar = new QToolBar(tr("Open"));
    openToolbar->setMovable(false);
    openToolbar->setFloatable(false);
    openToolbar->addWidget(openLine);
    openToolbar->addWidget(openButton);
    openToolbar->addSeparator();
    openToolbar->addWidget(new QLabel("CH: "));
    openToolbar->addWidget(nextButton);
    openToolbar->addWidget(previousButton);
    openToolbar->addWidget(refreshButton);
    openToolbar->addWidget(new QLabel("  "));
    openToolbar->addWidget(channelsButton);


    addToolBar(Qt::BottomToolBarArea, openToolbar);
}

void  QnplMainWindow::createConnections()
{
    connect(openAction, SIGNAL(triggered()), SLOT(performOpen()));
    connect(recentMenu,SIGNAL(triggered(QAction*)),SLOT(performOpen(QAction*)));
    connect(clearAction, SIGNAL(triggered()),SLOT(performClear()));
    connect(quitAction, SIGNAL(triggered()), SLOT(performQuit()));
    connect(tuneIPTVChannellAction, SIGNAL(triggered()),SLOT(performIptv()));
    connect(tuneAppChannellAction, SIGNAL(triggered()),SLOT(performAplication()));

    connect(baseAction, SIGNAL(triggered()), SLOT(performDevice()));
    connect(passiveAction, SIGNAL(triggered()), SLOT(performDevice()));
    connect(activeAction, SIGNAL(triggered()), SLOT(performDevice()));

    connect(preferencesAction, SIGNAL(triggered()), SLOT(performPreferences()));

    connect(bugAction, SIGNAL(triggered()),SLOT(performBug()));
    connect(aboutAction, SIGNAL(triggered()),SLOT(performAbout()));

    connect(openButton, SIGNAL(clicked()), SLOT(performOpen()));
    connect(channelsButton, SIGNAL(clicked()), SLOT(performChannels()));

    connect(tuneBroadChannellAction, SIGNAL(triggered()), SLOT(performChannels()));
    connect(playButton, SIGNAL(clicked()), SLOT(performRun()));
    connect(stopButton, SIGNAL(clicked()), SLOT(performStop()));
    connect(nextButton, SIGNAL(clicked()), SLOT(playNextChannel()));
    connect(previousButton, SIGNAL(clicked()), SLOT(playPreviousChannel())  );

    connect(view,SIGNAL(keyPressed(QString)),SLOT(notifyKey(QString)));
}


void QnplMainWindow::performOpen()
{
    qDebug() << view->winId();;

    if (location != "*"){
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
        lastChannel.setNull();

#ifdef Q_OS_LINUX
        QProcessEnvironment enviroment = QProcessEnvironment::systemEnvironment();
        enviroment.insert("LD_LIBRARY_PATH","/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
#endif

        QStringList parameters;

        process = new QProcess(this);

        connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(performCloseWindow(int, QProcess::ExitStatus)), Qt::QueuedConnection);
        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showErrorDialog(QProcess::ProcessError)));

        playButton->setEnabled(false);
        stopButton->setEnabled(true);

        openButton->setEnabled(false);
        openAction->setEnabled(false);

        openLine->setEnabled(false);
        openLine->setText(location);

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

            QString WID = "";

            foreach (QObject* ob, view->children()) {
                QWidget* w = qobject_cast<QWidget*>(ob);

                if (w)
                {
                    WID =  hwndToString(w->winId());
                }
            };

            parameters.replaceInStrings("${WID}", WID);

            if (location.endsWith(".ncl"))
            {
                parameters.insert(parameters.begin(),"--ncl");
                parameters.replaceInStrings("${FILE}", location);
            }
            else if (location.endsWith(".ts"))
            {
                parameters.insert(parameters.begin(),"--set-tuner");
                parameters.replaceInStrings("${FILE}", "fs:"+location);
            }

            parameters.replaceInStrings("${SCREENSIZE}", settings->value("screensize").toString());

#ifdef Q_OS_LINUX
            process->setProcessEnvironment(enviroment);
#endif

            QFileInfo finfo(location);
            process->setWorkingDirectory(finfo.absoluteDir().absolutePath());

            qDebug() << settings->value("location").toString() << parameters;

            process->start(settings->value("location").toString(), parameters);
            view->setFocus();
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
    else if (!lastChannel.isNull()){
        playChannel(lastChannel);
    }
    else
    {
        QMessageBox::information(this,tr("Information"), tr("Please, open NCL document to play."));
    }
}

void QnplMainWindow::performStop()
{
    view->releaseKeyboard();

    if (process != NULL){
        animTuning->setVisible(false);

        disconnect(process);

        process->terminate();
        process->close();

        process->deleteLater();

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

    nextButton->setEnabled(false);
    previousButton->setEnabled(false);

    scanProgress->close();

    view->update();
}

void QnplMainWindow::performAplication()
{
    aplication->exec();
}


void QnplMainWindow::performIptv()
{
    iptvDialog->exec();
    qDebug() << iptvDialog->ler_caixa();
}

void QnplMainWindow::performChannels()
{
    channelDialog->loadGingaChannels();
    if (channelDialog->exec() == QDialog::Accepted){

        Channel selectedChannel = channelDialog->channel();

        playChannel (selectedChannel);
    }
}

void QnplMainWindow::playChannel(Channel channel)
{
    if (channel.frequency != ""){
        performStop();

        process = new QProcess (this);

        connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
        connect(process, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));
        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showErrorDialog(QProcess::ProcessError)));

        QStringList plist;

        plist << "--set-tuner" << "sbtvdt:" + channel.frequency;
        plist << "--vmode" << settings->value("screensize").toString();
        plist << "--wid" << viewWID();
        plist << "--enable-log" << "C:\\log.txt";

        qDebug () << plist;

        openLine->setText(channel.number + " - " + channel.name);

        playButton->setEnabled(false);
        stopButton->setEnabled(true);

        nextButton->setEnabled(true);
        previousButton->setEnabled(true);

        openButton->setEnabled(false);
        openAction->setEnabled(false);

        openLine->setEnabled(false);

        recentMenu->setEnabled(false);

        baseAction->setEnabled(false);
        passiveAction->setEnabled(false);
        activeAction->setEnabled(false);

        process->start(settings->value("location").toString(), plist);

        animTuning->setVisible(true);

        lastChannel = channel;
        location = "";
    }
}

void QnplMainWindow::processOutput()
{
    QString p_stdout = process->readAllStandardOutput();
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
                        if (entity == "start" && msg == "?mAV?"){
                            animTuning->setVisible(false);
                        }
                    }
                    else if (status == "1"){
                        QMessageBox::warning(this, "Warning", msg, QMessageBox::Ok);
                        animTuning->setVisible(false);
                        qint64 bytes = process->write(QnplUtil::QUIT.toStdString().c_str());
                        qDebug () << bytes;
                    }
                }
            }
        }
    }
}

void QnplMainWindow::writeData()
{
    QString p_stdout = process->readAllStandardOutput();
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
                                scanProgress->setValue(value);
                                if (value == 100)
                                    emit scanFinished();
                            }

                        }
                    }
                    else if (status == "1"){
                        if (entity == "tuner"){
                            QMessageBox::warning(this, "Warning", msg, QMessageBox::Ok);
                            scanProgress->close();

                            qint64 bytes = process->write(QnplUtil::QUIT.toStdString().c_str());
                            qDebug () << bytes;
                        }
                    }
                }
            }
        }
    }
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

        plist << "--wid" << viewWID();
        plist << "--device-class" << "1";
        plist << "--vmode" << settings->value("screensize").toString();
        plist << "--context-dir" << context_dir;
        plist << "--disable-multicast";

        if (settings->value("enablelog").toBool()){
            plist << "--enable-log" << "file";
        }

        process = new QProcess(this);
        connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(performCloseWindow(int, QProcess::ExitStatus)), Qt::QueuedConnection);

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
        process->setProcessEnvironment(env);

        connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(performCloseWindow(int, QProcess::ExitStatus)), Qt::QueuedConnection);

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
    plist << "--wid" << viewWID();
    plist << "--device-class" << "2";
    plist << "--device-srv-port" << QString::number(port);
    plist << "--vmode" << settings->value("screensize").toString();
    plist << "--context-dir" << context_dir;

    if (settings->value("enablelog").toBool()){
        plist << "--enable-log" << "file";
    }

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(performCloseWindow(int, QProcess::ExitStatus)), Qt::QueuedConnection);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");
    process->setProcessEnvironment(env);

    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(performCloseWindow(int, QProcess::ExitStatus)), Qt::QueuedConnection);

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

void QnplMainWindow::performCloseWindow(int, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit){
        QMessageBox::critical(this, "Error", "Ginga stop working. Please try again.", QMessageBox::Ok);
    }
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

    view->setSceneRect (0,0,w,h);

    animTuning->setPos(0, -h_span);
    movie->setScaledSize(QSize (w + w_span, h + h_span));
    gifLabel->setFixedSize (w + w_span, h + h_span);
}

void QnplMainWindow::playNextChannel()
{
    Channel nextChannel = channelDialog->nextChannel();
    if(nextChannel.frequency != "")
    {
        playChannel(nextChannel);
    }
}

void QnplMainWindow::playPreviousChannel()
{
    Channel previousChannel = channelDialog->previousChannel();
    if(previousChannel.frequency != "")
    {
        playChannel(previousChannel);
    }
}

void QnplMainWindow::showScanErrorDialog()
{
    if (process)
    {
        QMessageBox::warning(this, "Error", "Channel scanning canceled by user.", QMessageBox::Ok);
        process->kill();
    }
}

void QnplMainWindow::scan()
{
    performStop();

    process = new QProcess (this);

    QStringList plist;
    plist << "--set-tuner" << "sbtvdt:scan";
    plist << "--wid" << viewWID();
    plist << "--poll-stdin";

    if (settings->value("enablelog").toBool()){
        plist << "--enable-log" << "file";
    }

    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(writeData()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(writeData()));

    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showErrorDialog(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), scanProgress, SLOT(done(int)));

    connect(scanProgress, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(scanFinished()));
    connect(scanProgress, SIGNAL(canceled()), this, SLOT(sendKillMessage()));

    scanProgress->setValue(0);
    process->start(settings->value("location").toString(), plist, QProcess::ReadWrite);
    scanProgress->exec();
}


void QnplMainWindow::sendKillMessage()
{
    disconnect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(writeData()));
    disconnect(process, SIGNAL(readyReadStandardError()), this, SLOT(writeData()));

    if (process){
        qint64 bytes = process->write(QnplUtil::QUIT.toStdString().c_str());
        qDebug () << bytes;
    }
    emit scanFinished();
}

void QnplMainWindow::showErrorDialog(QProcess::ProcessError error)
{
    QString errorMessage = "";
    if (error == QProcess::FailedToStart)
        errorMessage = "Error while opening Ginga. Check the binary path.";

    if (errorMessage != "")
        QMessageBox::critical(this, "Error", errorMessage, QMessageBox::Ok);

    performStop();
}
