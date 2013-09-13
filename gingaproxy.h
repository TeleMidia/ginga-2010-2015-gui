#ifndef GINGAPROXY_H
#define GINGAPROXY_H

#include <QObject>
#include <QProcess>
#include <QStringList>

class GingaProxy : public QObject
{
    Q_OBJECT
public:
    inline static GingaProxy * getInstance (QString binaryPath, QObject *parent = 0)
    {
        if (!_instance)
            _instance = new GingaProxy (binaryPath, parent);

        return _instance;
    }

    inline void setBinaryPath (QString path)
    { _binaryPath = path; }

    inline QString binaryPath () const
    { return _binaryPath; }

signals:
    void gingaStarted ();
    void gingaFinished(int, QProcess::ExitStatus);
    
public slots:
    bool run (QString, unsigned long wid = -1);
    void finished (int, QProcess::ExitStatus);

private:
    explicit GingaProxy(QString binaryPath, QObject *parent = 0);

    static GingaProxy *_instance;

    QProcess *_process;
    QString _binaryPath;
    QStringList _args;
};

#endif // GINGAPROXY_H
