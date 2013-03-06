#ifndef QNPLPREFERENCESDIALOG_H
#define QNPLPREFERENCESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFile>

#include "ui_qnplenvironmentform.h"
#include "ui_qnplpreferencesform.h"
#include "ui_qnplrunform.h"

#include "qnplsettings.h"

class QnplPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    QnplPreferencesDialog(QWidget* parent = 0);

    ~QnplPreferencesDialog();

    void init(QnplSettings* s);

public slots:
    void showPreferencesItem(QModelIndex index);
    void saveSettings();

    void browseExecutable();
    void browseGingaSettingsFile();

private:
    void loadSettings();
    void loadGingaPreferences();
    void saveGingaPreferences();

    QWidget* wGeneral;
    QWidget* wRun;

    Ui::QnplGeneralForm formGeneral;
    Ui::QnplRunForm formRun;
    Ui::QnplPreferencesForm formPreferences;

    QnplSettings* settings;
};

#endif // QNPLPREFERENCESDIALOG_H
