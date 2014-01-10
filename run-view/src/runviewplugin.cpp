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

int Util::SCREEN_HEIGHT;
int Util::SCREEN_WIDTH;

RunViewPlugin::RunViewPlugin()
{
  _gingaView = new QnplView;
  _runWidget = new QWidget;
  _runWidget->setFocusPolicy(Qt::ClickFocus);

  _projectControl = ProjectControl::getInstance();
  _gingaProxy = GingaProxy::getInstance("");

  Util::SCREEN_HEIGHT = QApplication::desktop()->height();
  Util::SCREEN_WIDTH = QApplication::desktop()->width();

  _playButton = new QPushButton();
  _playButton->setIcon(QIcon (":/icons/play"));
  _playButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

  _stopButton = new QPushButton();
  _stopButton->setIcon(QIcon (":/icons/stop"));
  _stopButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
  _stopButton->setEnabled(false);

  QPushButton *configButton = new QPushButton;
  configButton->setIcon(QIcon(":/icons/config"));
  configButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

  _lineEdit = new QLineEdit();
  _lineEdit->setEnabled(false);

  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->addWidget(_playButton);
  bottomLayout->addWidget(_stopButton);
  bottomLayout->addWidget(_lineEdit);
  bottomLayout->addWidget(configButton);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(_gingaView);
  layout->addLayout(bottomLayout);

  _runWidget->setLayout(layout);
  _runWidget->setFocusProxy(_gingaView);

  connect (_playButton, SIGNAL(pressed()),
           SLOT(playApplication()));
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
          file.write(project->getChildren().at(0)->toString(0,false).toLatin1());

        file.close();
      }
    }
  }

  QSettings settings (QSettings::IniFormat, QSettings::UserScope,
                      "telemidia", "gingagui");

  QString gingaLocation = settings.value("location", "").toString();
  QString contextLocation = settings.value("gingaconfig_file", "").toString();
  QString args = settings.value("parameters", "${FILE} --wid ${WID} "
                                "--vmode ${SCREENSIZE} --set-exitonend "
                                "--disable-multicast --poll-stdin").toString();

  QStringList argsList = args.split(" ");

  QString vmode = QString::number(_gingaView->width()) + "x" +
      QString::number(_gingaView->height());

  argsList.replaceInStrings("${FILE}", nclFilePath);
  argsList.replaceInStrings("${WID}", QString::number((unsigned long long)
                                                      _gingaView->winId()));
  argsList.replaceInStrings("${SCREENSIZE}", vmode);

  argsList << "--context-dir" << contextLocation;

  qDebug () << gingaLocation;
  qDebug () << nclFilePath;
  qDebug () << argsList;
  _gingaProxy->setBinaryPath(gingaLocation);
  _gingaProxy->run(argsList);
  _gingaView->setFocus();
}

void RunViewPlugin::execConfigDialog()
{
  ConfigDialog *configDialog = new ConfigDialog(_runWidget);
  configDialog->exec();

  delete configDialog;
}
