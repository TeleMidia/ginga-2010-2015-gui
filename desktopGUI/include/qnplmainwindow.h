#ifndef QNPLMAINWINDOW_H
#define QNPLMAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>
#include <QLineEdit>
#include <QSettings>

#include "gingaproxy.h"
#include "qnplview.h"
#include "qnplpreferencesdialog.h"
#include "qnplaboutdialog.h"
#include "util.h"
#include "qnplchannelsdialog.h"
#include "qnpliptvtunerdialog.h"
#include "qnplaplicationdialog.h"
#include "developerview.h"

class QnplMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QnplMainWindow(QWidget* parent = 0);
    ~QnplMainWindow();

    void load(QString _location);

signals:
    void scanFinished ();

public slots:
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

    void performPlay();
    void performPause();
    void performStop();
    void performSeek();
    void performRun();
    void performRunAsPassive();
    void performRunAsActive();

    void performCloseWindow(int, QProcess::ExitStatus);

    void notifyKey(QString key);

    void writeScanOutput ();

    void scan ();
    void playChannel (Channel channel);
    void showErrorDialog(QProcess::ProcessError);
    void writeTunerOutput ();
    void sendKillMessage ();
    void stopTuning();
    void removeCarouselData();

    void keyPressEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *event);

    void appendDebugMessage(QString);
    void startSession ();

private slots:
    void enableSeekButton ();

private:
    void createMenus();
    void createRecent();
    void createActions();
    void createWidgets();
    void createDialogs();
    void createToolbars();
    void createConnections();
    void removePath (QString);
    void setUpProcessConnections (QProcess *);

    inline QString viewWID () {
        QString WID = "";

        foreach (QObject* ob, _view->children()) {
            QWidget* w = qobject_cast<QWidget*>(ob);

            if (w)
            {
                WID =  hwndToString(w->winId());
            }
        }
        return WID;
    }

    QString hwndToString(WId winid);

    bool _isPlayingChannel;
    bool _isPaused;    

    QString _appName;

    QMenu* _fileMenu;
    QMenu* _recentMenu;
    QMenu* _deviceMenu;
    QMenu* _toolsMenu;
    QMenu* _helpMenu;

    QAction* _openAction;
    QAction* _tuneAppChannellAction;
    QAction* _tuneIPTVChannellAction;
    QAction* _tuneBroadChannellAction;
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
    QToolBar* _openToolbar;

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

    QnplView* _view;
    QSettings* _settings;

    QnplPreferencesDialog* _preferencesDialog;
    QnplAboutDialog* _aboutDialog;
    QnplChannelsDialog * _channelDialog;
    QnplIPTVTunerDialog * _iptvDialog;
    QnplAplicationDialog * _aplication;

    QProgressDialog *_scanProgress;

    Channel _lastChannel;
    QGraphicsProxyWidget *_animTuning;
    QMovie *_movie;
    QLabel *_gifLabel;
    QTimer *_timer;

    GingaProxy *_gingaProxy;
    DeveloperView * _developerView;
};

#endif // QNPLMAINWINDOW_H
