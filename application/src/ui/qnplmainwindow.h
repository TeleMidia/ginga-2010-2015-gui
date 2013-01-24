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

#include "qnplview.h"
#include "qnplsettings.h"
#include "qnplpreferencesdialog.h"
#include "qnplaboutdialog.h"

class QnplMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QnplMainWindow(QnplSettings* settings, QWidget* parent = 0);
    virtual ~QnplMainWindow();

public slots:
    void performOpen();
    void performOpen(QString f);
    void performRecentOpen(QAction* act);
    void performClear();
    void performClose();
    void performQuit();

    void performRun();
    void performRunAsPassive();
    void performRunAsActive();

    void saveRunMode();

    void performPlay();
    void performStop();

    void performPreferences();

    void performBug();
    void performAbout();

    void performCloseWindow(int);

    void notifyKey(QString key);

private:
    void load();

    void createMenus();
    void createActions();
    void createWidgets();
    void createToolbars();
    void createConnections();

#ifdef Q_OS_MAC
    QString hwndToString(NSView winid);
#else
    QString hwndToString(WId winid);
#endif

    void buildRecents();

    QMenu* fileMenu;
    QMenu* openMenu;
    QMenu* playMenu;
    QMenu* windowMenu;
    QMenu* helpMenu;

    QToolBar* playToolbar;
    QToolBar* openToolbar;

    QAction* openAction;
    QAction* runAsBaseAction;
    QAction* runAsPassiveAction;
    QAction* runAsActiveAction;
    QAction* clearAction;
    QAction* closeAction;
    QAction* quitAction;

    QAction* playAction;
    QAction* stopAction;

    QAction* preferencesAction;

    QAction* bugAction;
    QAction* aboutAction;

    QLineEdit* openLine;

    QPushButton* playButton;
    QPushButton* stopButton;
    QPushButton* openButton;

    QString location;

    QProcess* process;

    bool passiveIsRunning;

    QnplView* view;
    QnplSettings* settings;

    QnplPreferencesDialog* preferencesDialog;
    QnplAboutDialog* aboutDialog;

    QActionGroup* runGroup;
};

#endif // QNPLMAINWINDOW_H
