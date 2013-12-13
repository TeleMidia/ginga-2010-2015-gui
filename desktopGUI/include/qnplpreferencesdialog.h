#ifndef QNPLPREFERENCESDIALOG_H
#define QNPLPREFERENCESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFile>

#include "ui_qnplenvironmentform.h"
#include "ui_qnplpreferencesform.h"
#include "ui_qnplrunform.h"
#include "ui_advancedpreferences.h"

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

    void customMenuRequested(QPoint);
    void removeVariable ();

private slots:

private:
    void loadSettings();
    void loadGingaPreferences();
    void saveGingaPreferences();

    QWidget* _generalPane;
    QWidget* _runPane;

    Ui::QnplGeneralForm _generalForm;
    Ui::QnplRunForm _runForm;
    Ui::QnplPreferencesForm _preferencesForm;

    QnplSettings* settings;
};

#endif // QNPLPREFERENCESDIALOG_H
