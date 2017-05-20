#include "include/runviewplugin.h"

#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QInputDialog>

int Util::SCREEN_HEIGHT;
int Util::SCREEN_WIDTH;

RunViewPlugin::RunViewPlugin()
{
  _settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                            "telemidia", "gingagui", this);
  _gingaView = new GingaView;
  _gingaView->installEventFilter(this);

  _runWidget = new QWidget;
  _runWidget->setFocusPolicy(Qt::ClickFocus);
  _runWidget->setVisible(false);

  _projectControl = ProjectControl::getInstance();
  _gingaProxy = GingaProxy::getInstance();

  Util::SCREEN_HEIGHT = QApplication::desktop()->height();
  Util::SCREEN_WIDTH = QApplication::desktop()->width();

  QAction *action_playApplication = new QAction(tr("Play NCL document"), _runWidget);
  action_playApplication->setShortcut(tr("Ctrl+R"));

  QAction *action_runPassiveDevice = new QAction(tr("Run passive device"), _runWidget);
  QAction *action_runActiveDevice = new QAction(tr("Run active device"), _runWidget);

  connect (action_playApplication, SIGNAL(triggered()),
           this, SLOT(playApplication()));

  connect (action_runPassiveDevice,SIGNAL(triggered()),
           this, SLOT(runPassiveDevice()));

  connect (action_runActiveDevice,SIGNAL(triggered()),
           this, SLOT(runActiveDevice()));

  //_playButton = new QPushButton();
  _playButton = new QToolButton(0); //create a run_NCL button (It used to be created in design mode)

  menu_Multidevice = new QMenu(0); // assign a dropdown menu to the button
  menu_Multidevice->addAction(action_runPassiveDevice);
  menu_Multidevice->addAction(action_runActiveDevice);

  _playButton->setMenu(menu_Multidevice);
  _playButton->setDefaultAction(action_playApplication);
  _playButton->setPopupMode(QToolButton::MenuButtonPopup);

  action_playApplication->setIcon(QIcon (":/icons/play"));
  //_playButton->setIcon(QIcon (":/icons/play"));
  // _playButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  _stopButton = new QPushButton();
  _stopButton->setIcon(QIcon (":/icons/stop"));
  // _stopButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  _stopButton->setEnabled(false);

  QPushButton *configButton = new QPushButton;
  configButton->setIcon(QIcon(":/icons/config"));
  // configButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  _lineEdit = new QLineEdit();
  _lineEdit->setEnabled(false);

  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->addWidget(_playButton);
  bottomLayout->addWidget(_stopButton);
  bottomLayout->addWidget(_lineEdit);
  bottomLayout->addWidget(configButton);

  QVBoxLayout *layout = new QVBoxLayout;
  if ( !_settings->contains(Util::V_EMBEDDED))
    _settings->setValue(Util::V_EMBEDDED, "true");

  if (_settings->value(Util::V_EMBEDDED).toString() == "true")
	layout->addWidget(_gingaView);
  else
    _gingaView->setVisible(false);
    
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  layout->addLayout(bottomLayout);

  _runWidget->setLayout(layout);
  _runWidget->setFocusProxy(_gingaView);

  /*connect (_playButton, SIGNAL(pressed()),
           SLOT(playApplication())); */
  connect (_stopButton, SIGNAL(pressed()),
           _gingaProxy, SLOT(stop()));
  connect (configButton, SIGNAL(pressed()),
           SLOT(execConfigDialog()));
  connect (_gingaProxy, SIGNAL(gingaStarted()),
           SLOT(updateGUI()));
  connect (_gingaProxy, SIGNAL(gingaFinished(int,QProcess::ExitStatus)),
           SLOT(updateGUI()));
  connect (_gingaView, SIGNAL(selected(QString)),
           _gingaProxy, SLOT(sendCommand(QString)));

  QString aspectRatio = _settings->value(Util::V_ASPECT_RATIO, "").toString();
  if (aspectRatio == Util::WIDE)
    _gingaView->resize(_gingaView->width(),_gingaView->width() * 9 / 16);
  else if (aspectRatio == Util::STANDARD)
    _gingaView->resize(_gingaView->width(),_gingaView->width() * 3 / 4);
}

void RunViewPlugin::updateGUI()
{
  if (_gingaProxy->state() == QProcess::Running)
  {
    _playButton->setEnabled(false);
    _stopButton->setEnabled(true);
  }
  else
  {
    _playButton->setEnabled(true);
    _stopButton->setEnabled(false);
  }
}

void RunViewPlugin::playApplication()
{
  QString location = _currentProject->getLocation();
  QString nclFilePath = location.mid (0, location.lastIndexOf(".")) + ".ncl";

  if (_currentProject->isDirty())
  {
    int answer = QMessageBox::question(0, "Save project", "Would you like to "
                                       "save the project before run?",
                                       QMessageBox::Yes, QMessageBox::No);
    if (answer == QMessageBox::Yes)
    {
      _projectControl->saveProject(_currentProject->getLocation());
      QFile file(nclFilePath);
      if(file.open(QFile::WriteOnly | QIODevice::Truncate))
      {
        if(project->getChildren().size())
          file.write(project->getChildren().at(0)->toString(0, false).toLatin1());

        file.close();
      }
    }
  }

  QString gingaLocation = _settings->value(Util::V_LOCATION, "").toString();
  QString args = _settings->value(Util::V_PARAMETERS,
                                  Util::defaultParameters()).toString();

  QStringList argsList = args.split(" ");

  argsList.replaceInStrings(Util::GUI_FILE, nclFilePath);
  if(_settings->value(Util::V_EMBEDDED).toString() == "true")
  {
    QString vmode = QString::number(_gingaView->width()) +
            "x" +
            QString::number(_gingaView->height());

#ifdef __linux
    _gingaProxy->run(args);
    int sw = _gingaView->width();
    int sh = _gingaView->height();
    unsigned long int value = (unsigned long int) _gingaView->winId();
    argsList << "--parent";
    argsList << ":0.0," + QString::number(value) + ",0,0," +
    QString::number(sw) + "," + QString::number(sh);
#elif defined __WIN32
    QString WID = "";
    foreach (QObject *ob, _gingaView->focusWidget()->children())
    {
      QWidget *w = qobject_cast<QWidget *>(ob);
      if (w)
      {
        unsigned long int value = (unsigned long int) w->winId();
        WID = QString::number(value);
      }
    }
    argsList << "--wid" << WID;
#endif

    argsList.replaceInStrings(Util::GUI_SCREENSIZE, vmode);
  }

  if(!_settings->value(Util::V_CONTEXT_FILE).toString().isEmpty())
  {
    QFileInfo fileInfo(_settings->value(Util::V_CONTEXT_FILE).toString());
    QString contextFilePath = fileInfo.absoluteDir().path();
    if (!contextFilePath.isEmpty())
        argsList << "--context-dir" << contextFilePath;
  }

  qCDebug(CPR_PLUGIN_RUN) << gingaLocation;
  qCDebug(CPR_PLUGIN_RUN) << nclFilePath;
  qCDebug(CPR_PLUGIN_RUN) << argsList;

  _gingaProxy->setBinaryPath(gingaLocation);
  _gingaProxy->run(argsList);

  if(_settings->value(Util::V_EMBEDDED).toString() == "true")
  {
    _gingaView->show();
    _gingaView->setFocus();
  }
}

void RunViewPlugin::runPassiveDevice()//see ComposerMainWindow::runPassiveDevice
{
  int i;
  bool ok;
  //GlobalSettings settings;
  //_settings->beginGroup("runginga");
  QString command = _settings->value(Util::V_LOCATION, "").toString();
  QString args = _settings->value("local_ginga_passive_args").toString();
  //_settings->endGroup();

  /* PARAMETERS */
  //\todo Other parameters
  QStringList args_list = Utilities::splitParams(args);
  // args_list.replaceInStrings("${nclpath}", nclpath);

  int value = QInputDialog::getInt(
        _runWidget,
        tr("Multidevices"),//title
        tr("Passive"), //label
        1, //start
        1, //min
        5, //max
        1, //step
        &ok );

  if( ok )
  {
    qCDebug(CPR_PLUGIN_RUN) << command << args_list;
    for(i = 0; i < value; i++)
      QProcess::startDetached(command, args_list);
  }
}

void RunViewPlugin::runActiveDevice()
{
  int i;
  bool ok;
  //GlobalSettings settings;
  //_settings->beginGroup("runginga");
  QString command = _settings->value(Util::V_LOCATION, "").toString();
  QString args = _settings->value("local_ginga_active_args").toString();
  //_settings->endGroup();

  /* PARAMETERS */
  //\todo Other parameters
  QStringList args_list = Utilities::splitParams(args);
  // args_list.replaceInStrings("${nclpath}", nclpath);

  int value = QInputDialog::getInt(
        _runWidget,
        tr("Multidevices"),
        tr("Active"),
        1,
        1,
        5,
        1,
        &ok );

  if( ok )
  {
    qCDebug(CPR_PLUGIN_RUN)<< command << args_list;
    for(i = 0; i < value; i++)
      QProcess::startDetached(command, args_list);
  }
}

void RunViewPlugin::init ()
{
  QString location;
  int index;
  
  _currentProject = IPlugin::getProject();
  location = _currentProject->getLocation ();
  index = location.lastIndexOf (".");


  location = location.replace (index, location.length() - index, ".ncl");

  _lineEdit->setText (location);

  Util::init();
}

void RunViewPlugin::execConfigDialog()
{
  ConfigDialog *configDialog = new ConfigDialog(_runWidget);
  configDialog->exec();

  delete configDialog;
}
