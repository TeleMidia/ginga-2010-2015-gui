#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFile>
#include <QSettings>

#include "ui_environmentform.h"
#include "ui_preferencesform.h"
#include "ui_runform.h"


class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QSettings*, QWidget* parent = 0);

    ~PreferencesDialog();

    void createForms();

public slots:
    void showPreferencesItem(QModelIndex index);
    void saveSettings();

    void browseExecutable();
    void browseGingaSettingsFile();

    void customMenuRequested(QPoint);
    void removeVariable ();

public:
    int	exec();

private:
    void loadSettings();
    void loadGingaPreferences();
    void saveGingaPreferences();

    QMap <QString, int> _screenSizeMap;

    QWidget* _environmentPane;
    QWidget* _runPane;

    Ui::EnviornmentForm _enviornmentForm;
    Ui::RunForm _runForm;
    Ui::PreferencesForm _preferencesForm;

    QSettings* _settings;
};

#endif // PREFERENCESDIALOG_H
