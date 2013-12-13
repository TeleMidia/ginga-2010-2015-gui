#include "gingaproxy.h"
#include "util.h"

#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QSettings>

GingaProxy* GingaProxy::_instance = 0;

GingaProxy::GingaProxy(QString binaryPath, QWidget *parent) :
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
        {
            return true;
        }
    }

    return false;
}

void GingaProxy::run(QStringList args, bool forceKill)
{
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");\

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

    qDebug() << args;

    _process = new QProcess(this);
    connect (_process, SIGNAL(started()), this, SIGNAL(gingaStarted()));
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
    qDebug () << "gingaproxy::eventtype::" + event->type();

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

        _process->terminate();

        destroyProcess();
    }
}

void GingaProxy::destroyProcess()
{
    if (_process)
    {
        _process->close();
        _process->deleteLater();
        _process = NULL;
    }
}
