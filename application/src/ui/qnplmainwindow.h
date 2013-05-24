#ifndef QNPLMAINWINDOW_H
#define QNPLMAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QActionGroup>
#include <QPushButton>
#include <QSpacerItem>
#include <QRegExp>
#include <QDir>
#include <QTranslator>
#include <QFileInfo>
#include <QDesktopServices>

#include <QMacNativeWidget>

#include "qnplview.h"
#include "qnplsettings.h"
#include "qnplpreferencesdialog.h"
#include "qnplaboutdialog.h"
#include "qnplutil.h"
#include "qnplchannelsdialog.h"
#include "qnpliptvtunerdialog.h"
#include "qnplaplicationdialog.h"


class QnplMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QnplMainWindow(QWidget* parent = 0);
    ~QnplMainWindow();

    void load(QString location);

public slots:
    void performOpen();
    void performOpen(QAction* action);
    void performClear();
    void performClose();
    void performQuit();

    void performDevice();

    void performPreferences();
    void performChannels();
    void performIptv();
    void performaplication();


    void performBug();
    void performAbout();

    void performPlay();
    void performStop();

    void performRun();
    void performRunAsPassive();
    void performRunAsActive();

    void performCloseWindow(int);

    void notifyKey(QString key);

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

    QString hwndToString(WId winid);

    QMenu* fileMenu;    
    QMenu* recentMenu;
    QMenu* deviceMenu;
    QMenu* windowMenu;
    QMenu* helpMenu;

    QAction* openAction;
    QAction* tuneAppChannellAction;
    QAction* tuneIPTVChannellAction;
    QAction* tuneBroadChannellAction;
    QAction* quitAction;
    QAction* clearAction;
    QAction* baseAction;
    QAction* passiveAction;
    QAction* activeAction;
    QAction* preferencesAction;
    QAction* bugAction;
    QAction* aboutAction;


    QActionGroup* deviceGroup;

    QToolBar* playToolbar;
    QToolBar* openToolbar;

    QLineEdit* openLine;

    QPushButton* playButton;
    QPushButton* stopButton;
    QPushButton* openButton;
    QPushButton* nextButton;
    QPushButton* previousButton;
    QPushButton* refreshButton;
    QPushButton* channelsButton;



    QString location;

    QProcess* process;

    bool passiveIsRunning;


    QnplView* view;
    QnplSettings* settings;

    QnplPreferencesDialog* preferencesDialog;
    QnplAboutDialog* aboutDialog;
    QnplChannelsDialog * channelDialog;
    QnplIPTVTunerDialog * iptvDialog;
    QnplAplicationDialog * aplication;



};

#endif // QNPLMAINWINDOW_H
