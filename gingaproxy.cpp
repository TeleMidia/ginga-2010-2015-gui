#include "gingaproxy.h"
#include "util.h"

#include <QDebug>
#include <QApplication>
#include <QWidget>

GingaProxy* GingaProxy::_instance = 0;

GingaProxy::GingaProxy(QString binaryPath, QWidget *parent) :
    QObject(parent)
{
    _process = 0;
    setBinaryPath(binaryPath);

    installEventFilter(this);
}

bool GingaProxy::run(QString nclFile)
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

    QWidget *parentWidget = static_cast<QWidget*> (parent());

    WId wid = parentWidget->winId();

    if (wid != -1){
        _args << "--parent" << ":0.0," + QString::number(wid) + ",0,0," + QString::number(SCREEN_WIDTH) + "," + QString::number(SCREEN_HEIGHT);
    }
    _args << "--poll-stdin";

    qDebug() << _args;

    connect (_process, SIGNAL(started()), this, SIGNAL(gingaStarted()));
    connect (_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));

    _process->start(_binaryPath, _args);

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
