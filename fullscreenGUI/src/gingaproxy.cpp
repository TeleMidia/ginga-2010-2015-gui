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

bool GingaProxy::run(QString nclFile, WId wid)
{
    if (_process)
    {
        QProcess::ProcessState state = _process->state();

        if (state == QProcess::Running){
            qDebug () << "Ginga is already running.";
            return false;
        }
        if (state == QProcess::NotRunning)
            delete _process;
    }
    QStringList args;

    _process = new QProcess(this);

    if (nclFile.startsWith("ip:"))
    {
      args << "--set-tuner" << nclFile;
    }
    else if (nclFile.endsWith(".ts"))
    {
      args << "--set-tuner" << QString ("file:" + nclFile);
    }
    else
      args << "--ncl" << nclFile;

    unsigned long long ullWid = (unsigned long long) QApplication::activeWindow()->winId();
            //wid;

    if (ullWid != 0)
    {
#if __linux__
        /* --parent :display,winId,x,y,w,h */
        args << "--parent" << ":0.0," + QString::number(ullWid) + ",0,0,"
             + QString::number(QApplication::activeWindow()->width()) + "," +
             QString::number(QApplication::activeWindow()->height());
#else
        args << "--wid" << QString::number(ullWid);
#endif
    }

    args << "--poll-stdin";

    qDebug() << _binaryPath;
    qDebug() << args;

    connect (_process, SIGNAL(started()), this, SIGNAL(gingaStarted()));
    connect (_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));

    _process->start(_binaryPath, args);

    return true;
}

void GingaProxy::finished(int code, QProcess::ExitStatus status)
{
    if (_process){

        qDebug () << _process->state();

        _process->deleteLater();
        _process = 0;
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
    if (_process){
        qDebug () << "sendCommand:: " << str;

        return _process->write(QString (str + "\n").toStdString().c_str());
    }

    return 0;
}
