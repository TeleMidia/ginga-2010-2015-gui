#ifndef GINGAPROXY_H
#define GINGAPROXY_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <qwindowdefs.h>

class GingaProxy : public QObject
{
    Q_OBJECT
public:
    inline static GingaProxy * getInstance (QString binaryPath = "", QWidget *parent = 0)
    {
        if (!_instance)
            _instance = new GingaProxy (binaryPath, parent);

        return _instance;
    }

    inline QProcess *process () const
    { return _process; }

    inline void setWorkingDirectory (QString workingDirectory)
    { _workingDirectory = workingDirectory; }


    inline void setBinaryPath (QString path)
    { _binaryPath = path; }

    inline QString binaryPath () const
    { return _binaryPath; }

    bool eventFilter(QObject *, QEvent *);

    inline QProcess::ProcessState state () const
    {
        if (_process)
            return _process->state();
        else
            return QProcess::NotRunning;
    }

    inline void killProcess ()
    {
        if (_process && _process->state() == QProcess::Running)
            _process->kill();
    }

    void terminateProcess ();


signals:
    void gingaStarted ();
    void gingaFinished(int, QProcess::ExitStatus);
    
public slots:
    void run (QString, bool parentFlag = false, bool forceKill = true);
    void run (QStringList args, bool forceKill = true);
    void finished (int, QProcess::ExitStatus);
    int sendCommand (QString);

private:
    bool gingaIsRunning () const;

    explicit GingaProxy(QString binaryPath, QWidget *parent = 0);

    static GingaProxy *_instance;

    QProcess *_process;
    QString _binaryPath;
    QString _workingDirectory;
};

#endif // GINGAPROXY_H
