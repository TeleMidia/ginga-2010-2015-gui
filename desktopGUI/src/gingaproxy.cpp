#include "gingaproxy.h"

#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QSettings>

#define DIV 1024

size_t getPeakRSS(Q_PID);

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) ||
defined(__gnu_linux__)
#include <stdio.h>

#endif

#else
#error "Cannot define getPeakRSS( ) or getCurrentRSS( ) for an unknown OS."
#endif


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
    {
      return true;
    }
  }

  return false;
}

void GingaProxy::run(QStringList args, bool forceKill)
{
//  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//  env.insert("LD_LIBRARY_PATH", "/usr/local/lib/lua/5.1/socket:/usr/local/lib/ginga:/usr/local/lib/ginga/adapters:/usr/local/lib/ginga/cm:/usr/local/lib/ginga/mb:/usr/local/lib/ginga/mb/dec:/usr/local/lib/ginga/converters:/usr/local/lib/ginga/dp:/usr/local/lib/ginga/ic:/usr/local/lib/ginga/iocontents:/usr/local/lib/ginga/players:/usr/local/lib:");

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
  connect(_process, SIGNAL(readyReadStandardOutput()),
          SLOT(catchGingaOutput()));
  connect(_process, SIGNAL(readyReadStandardError()),
          SLOT(catchGingaOutput()));

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

void GingaProxy::catchGingaOutput()
{
  if (_process)
  {
    QString output = _process->readAllStandardOutput();
    if (output.isEmpty())
      output = _process->readAllStandardError();

    emit gingaOutput(output);
  }
}

/**
 * Returns the peak (maximum so far) resident set size (physical
 * memory use) measured in bytes, or zero if the value cannot be
 * determined on this OS.
 */
size_t getPeakRSS(Q_PID pid)
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( pid->hProcess, &info, sizeof(info) );
    return (size_t)info.WorkingSetSize;

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
    /* AIX and Solaris ------------------------------------------ */
    struct psinfo psinfo;
    int fd = -1;
    if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
        return (size_t)0L;      /* Can't open? */
    if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    {
        close( fd );
        return (size_t)0L;      /* Can't read? */
    }
    close( fd );
    return (size_t)(psinfo.pr_rssize * 1024L);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
#if defined(__APPLE__) && defined(__MACH__)
    return (size_t)rusage.ru_maxrss;
#else
    return (size_t)(rusage.ru_maxrss * 1024L);
#endif

#else
    /* Unknown OS ----------------------------------------------- */
    return (size_t)0L;          /* Unsupported. */
#endif
}
