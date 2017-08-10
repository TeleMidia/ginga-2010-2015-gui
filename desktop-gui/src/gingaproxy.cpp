#include "gingaproxy.h"

#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QSettings>
#include <QEventLoop>
#include <QTimer>

GingaProxy* GingaProxy::_instance = 0;

GingaProxy::GingaProxy(QString binaryPath, QObject *parent) :
  QObject(parent)
{
  _process = 0;
  setBinaryPath(binaryPath);

  installEventFilter(this);
}

void GingaProxy::run(QString nclFile, bool parentFlag, bool forceKill)
{
  if (gingaIsRunning())
  {
    qDebug () << "Ginga is already running.";

    if (forceKill)
    {
      _process->kill();
      _process->deleteLater();
      _process = 0;
    }
    else
      return;
  }

  QStringList args;
  args << "--ncl" << nclFile;

  if (parentFlag)
  {
    unsigned long long ullWid = (unsigned long long)
                                QApplication::activeWindow()->winId();

    if (ullWid != 0)
    {
      args << "--parent" << ":0.0," + QString::number(ullWid) + ",0,0," +
              QString::number(Util::SCREEN_WIDTH) + "," +
              QString::number(Util::SCREEN_HEIGHT);
    }
  }

  args << "--poll-stdin";

  run (args);
}

bool GingaProxy::gingaIsRunning() const
{
  if (_process)
  {
    QProcess::ProcessState state = _process->state();
    if (state != QProcess::NotRunning)
      return true;

  }
  return false;
}

void GingaProxy::run(QStringList args, bool forceKill)
{
  if (gingaIsRunning())
  {
    qDebug () << "Ginga is already running.";

    if (forceKill)
    {
      _process->kill();
      _process->deleteLater();
      _process = 0;
    }
    else
      return;
  }

  _process = new QProcess(this);
  _process->setReadChannel(QProcess::StandardOutput);
  _process->setReadChannelMode(QProcess::MergedChannels);

  connect(_process, SIGNAL(readyReadStandardOutput()),
          SLOT(catchGingaOutput()));
  connect (_process, SIGNAL(started()), this, SIGNAL(gingaStarted()));
  connect (_process, SIGNAL(error(QProcess::ProcessError)),
           this, SIGNAL(gingaError(QProcess::ProcessError)));
  connect (_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
           SLOT(finished(int,QProcess::ExitStatus)));

  _process->setWorkingDirectory(_workingDirectory);
  _process->start(_binaryPath, args);
}

void GingaProxy::finished(int code, QProcess::ExitStatus status)
{
  if (_process)
  {
    qDebug () << _process->state();
    destroyProcess();
  }

  emit gingaFinished(code, status);
}

bool GingaProxy::eventFilter(QObject *obj, QEvent *event)
{
  qDebug () << "gingaproxy::eventtype::" << event->type();

  return QObject::eventFilter(obj, event);
}

int GingaProxy::sendCommand(QString str)
{
  if (_process)
  {
    qDebug () << "sendCommand:: " << str;
    return _process->write(QString (str + "\n").toStdString().c_str());
  }

  return 0;
}

void GingaProxy::terminateProcess()
{
  if (_process)
  {
    disconnect(_process);

    _process->closeReadChannel(_process->readChannel());
    _process->closeWriteChannel();
    _process->terminate();
    destroyProcess();
    emit gingaFinished();
  }
}

void GingaProxy::destroyProcess()
{
  if (_process)
  {
    _process->deleteLater();
    _process = NULL;
  }
}

QString GingaProxy::version()
{
  QStringList args;
  QString version;
  QEventLoop loop;
  QTimer timer;

  if (_binaryPath != "")
  {

    timer.setInterval(10000);
    timer.setSingleShot(true);

    args << "--version";

    terminateProcess();

    _process = new QProcess(this);
    _process->setReadChannel(QProcess::StandardOutput);
    _process->setReadChannelMode(QProcess::MergedChannels);

    _process->start(_binaryPath, args);
    _process->waitForStarted();

    timer.start();
    connect (&timer, SIGNAL (timeout()), &loop, SLOT(quit()));
    connect (_process, SIGNAL (error()), &loop, SLOT(quit()));
    connect (_process, SIGNAL (timeout()), &loop, SLOT(quit()));
    connect (_process, SIGNAL (readyReadStandardOutput()),
             &loop, SLOT(quit()));

    loop.exec();

    version = _process->readAllStandardOutput();

    killProcess();
  }

  return version;
}

void GingaProxy::catchGingaOutput()
{
  if (_process)
  {
    QString output = _process->readAllStandardOutput();
    emit gingaOutput(output);
  }
}
