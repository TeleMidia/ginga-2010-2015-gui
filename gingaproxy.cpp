#include "gingaproxy.h"
#include "util.h"

#include <QDebug>

GingaProxy* GingaProxy::_instance = 0;

GingaProxy::GingaProxy(QString binaryPath, QObject *parent) :
    QObject(parent)
{
    _process = 0;
    setBinaryPath(binaryPath);
}

bool GingaProxy::run(QString nclFile, unsigned long int wid)
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

    _process = new QProcess(this);

    _args << "--ncl" << nclFile;
    _args << "--vmode" << QString::number(SCREEN_WIDTH) + "x" + QString::number(SCREEN_HEIGHT) /*"fullscreen"*/;

//    if (wid != -1){
//        _args << "--wid" << QString::number(wid);
//    }

    qDebug() << _args;

    connect (_process, SIGNAL(started()), this, SIGNAL(gingaStarted()));
    connect (_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));

    _process->start(_binaryPath, _args);
    _process->waitForFinished(-1);

    return true;
}

void GingaProxy::finished(int code, QProcess::ExitStatus status)
{
    _args.clear();

    if (_process){
        _process->deleteLater();
        _process = 0;
    }

    emit gingaFinished(code, status);
}
