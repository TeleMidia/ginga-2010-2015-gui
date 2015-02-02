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


class QnplPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    QnplPreferencesDialog(QSettings*, QWidget* parent = 0);

    ~QnplPreferencesDialog();

    void init(QSettings *s);

public slots:
    void showPreferencesItem(QModelIndex index);
    void saveSettings();

    void browseExecutable();
    void browseGingaSettingsFile();

    void customMenuRequested(QPoint);
    void removeVariable ();

private:
    void loadSettings();
    void loadGingaPreferences();
    void saveGingaPreferences();

    QMap <QString, int> _screenSizeMap;

    QWidget* _generalPane;
    QWidget* _runPane;

    Ui::QnplGeneralForm _generalForm;
    Ui::QnplRunForm _runForm;
    Ui::QnplPreferencesForm _preferencesForm;

    QSettings* _settings;
};

#endif // QNPLPREFERENCESDIALOG_H
