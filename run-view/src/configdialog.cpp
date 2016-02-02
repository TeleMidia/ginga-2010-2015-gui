#include "include/configdialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QSettings>
#include <QDesktopServices>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QMenu>
#include <QMessageBox>
#include <QTextStream>
#include <util.h>

ConfigDialog::ConfigDialog(QWidget *parent) :
  QDialog(parent)
{
  _runForm = new Ui::RunForm;
  _runForm->setupUi(this);
  _runForm->contextFileLocation->setEnabled(false);
  _runForm->table->setContextMenuPolicy(Qt::CustomContextMenu);

  QPushButton *saveButton = new QPushButton ("Save");
  QPushButton *cancelButton = new QPushButton ("Cancel");

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(saveButton);
  buttonsLayout->addWidget(cancelButton);

  connect (saveButton, SIGNAL(pressed()),
           SLOT(saveSettings()));
  connect (cancelButton, SIGNAL(pressed()),
           SLOT(reject()));
  connect (_runForm->executableButton, SIGNAL(pressed()),
           SLOT(setGingaLocation()));
  connect (_runForm->contextFileButton, SIGNAL(pressed()),
           SLOT(setContextLocation()));
  connect (_runForm->table, SIGNAL(customContextMenuRequested(QPoint)),
           SLOT(customMenuRequested(QPoint)));

  _runForm->verticalLayout->addLayout(buttonsLayout);

  loadSettings();
}

void ConfigDialog::loadSettings()
{
  QSettings settings (QSettings::IniFormat, QSettings::UserScope,
                      "telemidia", "gingagui");

  QString gingaLocation = settings.value(Util::V_LOCATION, "").toString();
  QString contextLocation = settings.value(Util::V_CONTEXT_FILE, "").toString();
  QString args = settings.value(Util::V_PARAMETERS,
                                Util::defaultParameters()).toString();

  QString aspectRatio = settings.value(Util::V_ASPECT_RATIO, "0").toString();
  if (aspectRatio == "0")
    _runForm->aspectRatioGroupBox->setChecked(false);
  else
  {
    _runForm->aspectRatioGroupBox->setChecked(true);
    if (aspectRatio == Util::WIDE)
      _runForm->wideRadioButton->setChecked(true);
    else
      _runForm->standardRadioButton->setChecked(true);
  }

  QStandardItemModel *model = new QStandardItemModel;
  _runForm->table->setModel(model);

  if (!contextLocation.isEmpty())
  {
    QSettings contextIni (contextLocation, QSettings::IniFormat);

    model->setColumnCount(2);

    model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

    QStringList keys = contextIni.allKeys();

    foreach(QString key, keys)
    {
      if (key == "||" || key == "::") continue;

      QString value = contextIni.value(key, "").toString();

      QStandardItem* keyItem = new QStandardItem(key);
      keyItem->setEditable(true);

      QStandardItem* valueItem = new QStandardItem(value);
      valueItem->setEditable(true);

      QList <QStandardItem *> row;
      row.append(keyItem);
      row.append(valueItem);

      model->appendRow(row);
    }
  }
  model->setRowCount(32);

  /* setting some .ini config defaults */
  if(!settings.contains("local_ginga_passive_args"))
      settings.setValue("local_ginga_passive_args", "--device-class 1");
  if(!settings.contains("local_ginga_active_args"))
      settings.setValue("local_ginga_active_args", "--device-class 2");

  QString passiveargs = settings.value("local_ginga_passive_args").toString();
  QString activeargs = settings.value("local_ginga_active_args").toString();

  _runForm->executableEdit->setText(gingaLocation);
  _runForm->contextFileLocation->setText(contextLocation);
  _runForm->argsEdit->setText(args);
  _runForm->passiveEdit->setText(passiveargs);
  _runForm->activeEdit->setText(activeargs);
}

void ConfigDialog::setGingaLocation()
{
  QString gingaLocation = QFileDialog::getOpenFileName((QWidget *) parent(),
                                                  "Select an executable file");
  if (!gingaLocation.isEmpty())
    _runForm->executableEdit->setText(gingaLocation);
}

void ConfigDialog::setContextLocation()
{
  QString contextLocation = QFileDialog::getOpenFileName((QWidget *) parent(),
                                                    "Select a config file", "",
                                                         "Ini (*.ini)");
  if (!contextLocation.isEmpty())
    _runForm->contextFileLocation->setText(contextLocation);
}

void ConfigDialog::customMenuRequested(QPoint point)
{
  QMenu *menu = new QMenu(this);
  QAction *removeAction = menu->addAction(tr("Remove variable"));

  connect(removeAction, SIGNAL(triggered()), SLOT(removeVariable()));

  menu->popup(_runForm->table->viewport()->mapToGlobal(point));
}

void ConfigDialog::removeVariable()
{
  int answer =
      QMessageBox::question(this, "Remove Variable", "Are you sure you want to "
                            "remove this environment variable?",
                            QMessageBox::Yes, QMessageBox::No);

  if (answer == QMessageBox::No) return;

  QModelIndexList list = _runForm->table->selectionModel()->selection().indexes();

  if (list.size() > 0)
  {
    QModelIndex index = list.at(0);
    _runForm->table->model()->removeRow(index.row());
  }
}

void ConfigDialog::saveSettings()
{
  QSettings settings (QSettings::IniFormat, QSettings::UserScope,
                      "telemidia", "gingagui");

  settings.setValue(Util::V_LOCATION, _runForm->executableEdit->text());
  settings.setValue(Util::V_CONTEXT_FILE, _runForm->contextFileLocation->text());
  settings.setValue(Util::V_PARAMETERS, _runForm->argsEdit->toPlainText());
  settings.setValue("local_ginga_passive_args",
                    _runForm->passiveEdit->toPlainText());
  settings.setValue("local_ginga_active_args",
                    _runForm->activeEdit->toPlainText());

  if (_runForm->aspectRatioGroupBox->isChecked())
    settings.setValue(Util::V_ASPECT_RATIO,
                      _runForm->wideRadioButton->isChecked() ?
                      Util::WIDE : Util::STANDARD);
  else
    settings.setValue(Util::V_ASPECT_RATIO, "0");

  settings.sync();
  saveGingaPreferences();

  accept();
}

void ConfigDialog::saveGingaPreferences()
{
  if (QFile::exists(_runForm->contextFileLocation->text()))
  {
    QFile* file = new QFile(_runForm->contextFileLocation->text());

    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream stream(file);
      stream.setCodec("UTF-8");

      stream << "::" << " = " << "0" << endl;
      stream << "||" << " = " << "0" << endl;

      QAbstractItemModel* model = _runForm->table->model();

      for (int i=0; i < model->rowCount(); i++)
      {
        QString name = model->data(model->index(i,0)).toString();
        QString value = model->data(model->index(i,1)).toString();

        if (name != "")
        {
          stream << name
                 << " = "
                 << value
                 << endl;
        }
      }
    }
    delete file;
  }
}
