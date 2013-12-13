#ifndef QNPLMAINWINDOW_H
#define QNPLMAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>

#include "gingaproxy.h"
#include "qnplview.h"
#include "qnplsettings.h"
#include "qnplpreferencesdialog.h"
#include "qnplaboutdialog.h"
#include "util.h"
#include "qnplchannelsdialog.h"
#include "qnpliptvtunerdialog.h"
#include "qnplaplicationdialog.h"

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
    void performStop();

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

    void appendDebugMessage(QString);

    void startSession ();
protected:
    void resizeEvent(QResizeEvent* event);

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

    QToolBar* _playToolbar;
    QToolBar* _openToolbar;

    QLineEdit* _openLine;

    QPushButton* _playButton;
    QPushButton* _stopButton;
    QPushButton* _openButton;
    QPushButton* _nextButton;
    QPushButton* _previousButton;
    QPushButton* _refreshButton;
    QPushButton* _channelsButton;

    QString _location;

    QProcess* _process;

    bool _passiveIsRunning;
    bool _isChannel;

    QnplView* _view;
    QnplSettings* settings;

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
    bool isPlayingChannel;
    QTimer *_timer;

    GingaProxy *_gingaProxy;
};

#endif // QNPLMAINWINDOW_H
