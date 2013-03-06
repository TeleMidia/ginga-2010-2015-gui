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

    void performBug();
    void performAbout();

    void performPlay();
    void performStop();

    void performRun();
    void performRunAsPassive();
    void performRunAsActive();

    void performCloseWindow(int);

    void notifyKey(QString key);

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

    QString location;

    QProcess* process;

    bool passiveIsRunning;

    QnplView* view;
    QnplSettings* settings;

    QnplPreferencesDialog* preferencesDialog;
    QnplAboutDialog* aboutDialog;


};

#endif // QNPLMAINWINDOW_H
