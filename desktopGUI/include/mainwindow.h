#ifndef QNPLMAINWINDOW_H
#define QNPLMAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>
#include <QLineEdit>
#include <QSettings>
#include <QStackedWidget>
#include "gingaproxy.h"
#include "gingaview.h"
#include "preferencesdialog.h"
#include "aboutdialog.h"
#include "util.h"
#include "channelsdialog.h"
#include "iptvtunerdialog.h"
#include "developerview.h"
#include "debugview.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

signals:
  void scanFinished ();

public slots:
  void load (const QString &location);
  void updateLocation (const QString &location);

  void resizeView ();

  void performOpen();
  void performOpen(QAction* action);
  void performClear();
  void performClose();
  void performQuit();

  void playNextChannel();
  void playPreviousChannel();

  void performDevice();

  void performPreferences();
  void performChannels();
  void performIptv();
  void performAplication();

  void performBug();
  void performAbout();

  void performPause();
  void performStop();
  void performSeek();
  void performPlay();
  void runAsBase();
  void runAsPassive();
  void runAsActive();

  void performCloseWindow();


  void writeScanOutput (QString);

  void scan ();
  void playChannel (Channel channel);
  void playIpChannel (QString ipChannel);
  void showErrorDialog(QProcess::ProcessError);
  void writeTunerOutput (QString p_stdout);
  void sendKillMessage ();
  void stopTuning();
  void removeCarouselData();

  void keyPressEvent(QKeyEvent *);
  void resizeEvent(QResizeEvent *event);

  void handleGingaOutput(QString);
  void startSession ();

private slots:
  void enableSeekButton ();
  void finishScan (int code);

private:
  void createMenus();
  void createRecent();
  void createActions();
  void createWidgets();
  void createDialogs();
  void createToolbars();
  void createConnections();
  void removePath (QString);
  void configureDefaultFlags(QStringList &plist);

  bool _isTuning;
  bool _isPlayingChannel;
  bool _isPaused;

  QString _appName;

  QMenu* _fileMenu;
  QMenu* _recentMenu;
  QMenu* _deviceMenu;
  QMenu* _toolsMenu;
  QMenu* _helpMenu;

  QAction* _openAction;
  QAction* _tuneApplicationChannelAction;
  QAction* _tuneIPTVChannellAction;
  QAction* _tuneBroadcastChannelAction;
  QAction* _quitAction;
  QAction* _clearAction;
  QAction* _baseAction;
  QAction* _passiveAction;
  QAction* _activeAction;
  QAction* _preferencesAction;
  QAction* _bugAction;
  QAction* _aboutAction;

  QActionGroup* _deviceGroup;

  QToolBar* _toolbar;

  QLineEdit* _openLine;

  QPushButton* _playButton;
  QPushButton* _stopButton;
  QPushButton* _pauseButton;
  QPushButton* _openButton;
  QPushButton* _nextButton;
  QPushButton* _previousButton;
  QPushButton* _refreshButton;
  QPushButton* _channelsButton;
  QPushButton* _seekButton;

  QLineEdit* _seekPlayTime;

  QString _location;

  QProcess* _process;

  bool _passiveIsRunning;
  bool _isChannel;

  GingaView* _view;
  QWidget* _animationView;
  QSettings* _settings;
  QStackedWidget *_stackedWidget;
  PreferencesDialog* _preferencesDialog;
  AboutDialog* _aboutDialog;
  ChannelsDialog * _channelDialog;
  IptvTunerDialog * _iptvDialog;

  QProgressDialog *_scanProgress;

  Channel _lastChannel;
  QString _lastIpChannel;
  QGraphicsProxyWidget *_animTuning;
  QMovie *_movie;
  QLabel *_gifLabel;
  QTimer *_timer;

  GingaProxy *_gingaProxy;
  DeveloperView * _developerView;
  DebugView * _debugView;
};

#endif // QNPLMAINWINDOW_H
