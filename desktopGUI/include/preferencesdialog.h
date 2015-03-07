#ifndef QNPLPREFERENCESDIALOG_H
#define QNPLPREFERENCESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFile>
#include <QSettings>

#include "ui_qnplenvironmentform.h"
#include "ui_qnplpreferencesform.h"
#include "ui_qnplrunform.h"


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

    Ui::QnplEnviornmentForm _enviornmentForm;
    Ui::QnplRunForm _runForm;
    Ui::QnplPreferencesForm _preferencesForm;

    QSettings* _settings;
};

#endif // QNPLPREFERENCESDIALOG_H
