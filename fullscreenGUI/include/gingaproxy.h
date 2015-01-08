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
    inline static GingaProxy * getInstance (const QString &binaryPath = "",
                                            QWidget *parent = 0)
    {
        if (!_instance)
            _instance = new GingaProxy (binaryPath, parent);

        return _instance;
    }

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

signals:
    void gingaStarted ();
    void gingaFinished(int, QProcess::ExitStatus);
    
public slots:
    bool run (QString, WId wid);
    void finished (int, QProcess::ExitStatus);
    int sendCommand (QString);

private:
    explicit GingaProxy(QString binaryPath, QWidget *parent = 0);

    static GingaProxy *_instance;

    QProcess *_process;
    QString _binaryPath;
};

#endif // GINGAPROXY_H
