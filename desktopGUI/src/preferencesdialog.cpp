#include <QMenu>
#include <QAction>
#include <QModelIndexList>
#include <QMessageBox>

#include "preferencesdialog.h"
#include "include/util.h"

#include <QDebug>

PreferencesDialog::PreferencesDialog(QSettings *settings,
                                             QWidget* parent)
  : QDialog(parent)
{

  _settings = settings;
  createForms();
  loadSettings();
}

PreferencesDialog::~PreferencesDialog()
{
  delete _environmentPane;
  delete _runPane;
}

void PreferencesDialog::createForms()
{
  _environmentPane = new QWidget();
  _runPane = new QWidget();

  _enviornmentForm.setupUi(this);
  _runForm.setupUi(this);

  _environmentPane->setLayout(_enviornmentForm.verticalLayout);
  _runPane->setLayout(_runForm.verticalLayout);

  _preferencesForm.setupUi(this);

  _runForm.table->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(_preferencesForm._treeItems, SIGNAL(clicked(QModelIndex)),
          SLOT(showPreferencesItem(QModelIndex)));

  connect(_preferencesForm._buttonsGroup, SIGNAL(accepted()),
          SLOT(saveSettings()));

  connect(_runForm.executableButton, SIGNAL(clicked()),
          SLOT(browseExecutable()));

  connect(_runForm.contextFileButton, SIGNAL(clicked()),
          SLOT(browseGingaSettingsFile()));

  connect(_runForm.table, SIGNAL(customContextMenuRequested(QPoint)),
          SLOT(customMenuRequested(QPoint)));

  QStandardItemModel *model = new QStandardItemModel(2,1);
  QStandardItem *environmentItem = new QStandardItem(
                                     tr(Util::PREFERENCES_ENVIRONMENT.toStdString().c_str()));

  environmentItem->setEditable(false);
  environmentItem->setIcon(QIcon(":icon/pc"));

  QStandardItem *gingaItem = new QStandardItem(
                               tr(Util::PREFERENCES_GINGA.toStdString().c_str()));

  gingaItem->setEditable(false);
  gingaItem->setIcon(QIcon(":icon/run"));

  QStandardItem *advancedItem = new QStandardItem(
                                  tr(Util::PREFERENCES_ADVANCED.toStdString().c_str()));
  advancedItem->setEditable(false);

  //gingaItem->setChild(0, advancedItem);

  model->setItem(0, environmentItem);
  model->setItem(1, gingaItem);

  _preferencesForm._treeItems->setModel(model);
  _preferencesForm._treeItems->selectionModel()->select(
        _preferencesForm._treeItems->model()->index(0,0),
        QItemSelectionModel::Select);
  _preferencesForm._treeItems->header()->hide();


  _preferencesForm._titleLabel->setText(
        tr(Util::PREFERENCES_ENVIRONMENT.toStdString().c_str()));

  _preferencesForm._titleLabel->setStyleSheet(
        "font-weight: bold; font-size 32px;");


  _preferencesForm._layout->addWidget(_environmentPane);
  _preferencesForm._layout->addWidget(_runPane);
  _environmentPane->show();
  _runPane->hide();

  for (int i = 0; i < _enviornmentForm.screensizeBox->count(); i++)
  {
    QString itemText = _enviornmentForm.screensizeBox->itemText(i);
    _screenSizeMap [itemText.split(" ").at(0)] = i;
  }

}

void PreferencesDialog::loadSettings()
{
  _enviornmentForm.logButton->setChecked(
        _settings->value("enablelog", "true").toString() == "true");

  _enviornmentForm.autoplayButton->setChecked(
        _settings->value(Util::V_AUTOPLAY, "true").toString() == "true");

  _runForm.executableEdit->setText(_settings->value(Util::V_LOCATION).
                                   toString());

  _runForm.contextFileLocation->setText(_settings->value(Util::V_CONTEXT_FILE).
                                        toString());

  int index = _screenSizeMap.value(
                _settings->value(Util::V_SCREENSIZE).toString(), 0);
//  if(index ==  0)
//  {
    QString actual_value;
    if (_settings->value("lang").toString() == "en")
      actual_value =  "actual size: ";
    else if (_settings->value("lang").toString() == "pt_br")
      actual_value =  "tamanho atual: ";
    actual_value.append(_settings->value(Util::V_SCREENSIZE).toString());
    _enviornmentForm.screensizeBox->setItemText(0, actual_value);
//  }
  _enviornmentForm.screensizeBox->setCurrentIndex(index);

  qDebug() << "index=" << index << endl;

  if (_settings->value("lang").toString() == "en")
    _enviornmentForm.languageBox->setCurrentIndex(0);
  else if (_settings->value("lang").toString() == "pt_br")
    _enviornmentForm.languageBox->setCurrentIndex(1);
  else if (_settings->value("lang").toString() == "en")
    _enviornmentForm.languageBox->setCurrentIndex(2);

  _runForm.argsEdit->setText(_settings->value(Util::V_PARAMETERS,
                                              Util::defaultParameters())
                             .toString());

  _runForm.contextFileLocation->setText(_settings->value(Util::V_CONTEXT_FILE)
                                        .toString());

  QString aspectRatio = _settings->value(Util::V_ASPECT_RATIO, "0").toString();
  if ( aspectRatio != "0")
  {
    _runForm.aspectRatioGroupBox->setChecked(true);
    if (aspectRatio == Util::WIDE)
      _runForm.wideRadioButton->setChecked(true);
    else
      _runForm.standardRadioButton->setChecked(true);
  }

  _runForm.embeddedButton->setChecked(
        _settings->value("embedded", "true").toString() == "true");

  loadGingaPreferences();
}

void PreferencesDialog::saveSettings()
{
  qDebug() << "Saving settings...";

  if (_enviornmentForm.autoplayButton->isChecked())
    _settings->setValue(Util::V_AUTOPLAY, true);
  else
    _settings->setValue(Util::V_AUTOPLAY, false);

  if (_enviornmentForm.logButton->isChecked())
    _settings->setValue(Util::V_ENABLE_LOG, true);
  else
    _settings->setValue(Util::V_ENABLE_LOG, false);

  _settings->setValue(
        Util::V_SCREENSIZE,
        _enviornmentForm.screensizeBox->currentText().split(" ").at(0));

  switch (_enviornmentForm.languageBox->currentIndex())
  {
    case 0:
      _settings->setValue(Util::V_LANG,"en");
      break;
    case 1:
      _settings->setValue(Util::V_LANG,"pt_br");
      break;
    case 2:
      _settings->setValue(Util::V_LANG,"es");
      break;
  }

  _settings->setValue(Util::V_LOCATION, _runForm.executableEdit->text());
  _settings->setValue(Util::V_PARAMETERS, _runForm.argsEdit->toPlainText());
  _settings->setValue(Util::V_CONTEXT_FILE,
                      _runForm.contextFileLocation->text());

  QString aspectRatio = "0";

  if (_runForm.aspectRatioGroupBox->isChecked())
  {
    if (_runForm.wideRadioButton->isChecked())
      aspectRatio = Util::WIDE;
    else
      aspectRatio = Util::STANDARD;
  }

  _settings->setValue(Util::V_ASPECT_RATIO, aspectRatio);

  _settings->setValue(Util::V_EMBEDDED, _runForm.embeddedButton->isChecked()?
                        "true" : "false");

  _settings->sync();

  saveGingaPreferences();
}

void PreferencesDialog::showPreferencesItem(QModelIndex index)
{
  QString itemSelected = index.data().toString();

  if (itemSelected == Util::PREFERENCES_ENVIRONMENT)
  {
    _environmentPane->show();
    _runPane->hide();
    _preferencesForm._titleLabel->setText(tr("Environment"));

  }
  else if (itemSelected == Util::PREFERENCES_GINGA)
  {
    _environmentPane->hide();
    _runPane->show();
    _preferencesForm._titleLabel->setText(tr("Ginga"));
  }
  else if (itemSelected == Util::PREFERENCES_ADVANCED)
  {

  }
  _preferencesForm._titleLabel->setText(itemSelected);
}

void PreferencesDialog::loadGingaPreferences()
{
  if (QFile::exists(_runForm.contextFileLocation->text()))
  {

    QFile* file = new QFile(_runForm.contextFileLocation->text());

    if (file->open(QIODevice::ReadOnly))
    {
      QVector<QPair<QString, QString> > params;

      QTextStream* stream = new QTextStream(file);

      QRegExp rx;
      rx.setPattern("(.*)=(.*)");

      while(!stream->atEnd())
      {
        QString line =  stream->readLine().trimmed();

        qDebug() << line;

        if (line.contains(rx))
        {
          rx.indexIn(line);

          params.push_back(QPair<QString, QString>(rx.cap(1).trimmed(),
                                                   rx.cap(2).trimmed()));
        }
      }

      delete stream;

      delete _runForm.table->model();
      _runForm.table->setModel(NULL);

      int ncol = 2;
      int nrow = 32;

      _runForm.table->verticalHeader()->hide();
      _runForm.table->horizontalHeader()->resizeSections(QHeaderView::Stretch);

      QStandardItemModel *model = new QStandardItemModel(nrow, ncol);

      model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
      model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

      int i = 0;

      QPair<QString, QString> p;
      foreach(p, params)
      {
        if (p.first != "::" && p.first != "||")
        {
          QStandardItem* nitem = new QStandardItem(p.first);
          nitem->setEditable(true);

          QStandardItem* vitem = new QStandardItem(p.second);

          model->setItem(i, 0, nitem);
          model->setItem(i, 1, vitem);

          ++i;
        }
      }

      _runForm.table->setModel(model);
    }

    delete file;
  }
}

void PreferencesDialog::saveGingaPreferences()
{
  if (QFile::exists(_runForm.contextFileLocation->text()))
  {

    QFile* file = new QFile(_runForm.contextFileLocation->text());

    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream stream(file);
      stream.setCodec("UTF-8");

      stream << "::" << " = " << "0" << endl;
      stream << "||" << " = " << "0" << endl;

      int n = _runForm.table->model()->rowCount();

      QAbstractItemModel* model = _runForm.table->model();

      for (int i = 0; i < n; i++)
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

void PreferencesDialog::browseExecutable()
{
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open"));

  if (fileName != "")
    _runForm.executableEdit->setText(fileName);
}

void PreferencesDialog::browseGingaSettingsFile()
{
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open"));

  if (fileName != "")
  {
    _runForm.contextFileLocation->setText(fileName);

    loadGingaPreferences();
  }
}

void PreferencesDialog::customMenuRequested(QPoint point)
{

  QMenu *menu = new QMenu(this);
  QAction *removeAction = menu->addAction(tr("Remove variable"));

  connect(removeAction, SIGNAL(triggered()), SLOT(removeVariable()));

  menu->popup(_runForm.table->viewport()->mapToGlobal(point));
}

void PreferencesDialog::removeVariable()
{
  int answer =
      QMessageBox::question(this, "Remove Variable", "Are you sure you want to "
                                                     "remove this environment variable?",
                            QMessageBox::Yes, QMessageBox::No);

  if (answer == QMessageBox::No) return;

  QModelIndexList list = _runForm.table->selectionModel()->selection().indexes();
  qDebug () << list.size();
  if (list.size() > 0)
  {
    QModelIndex index = list.at(0);
    _runForm.table->model()->removeRow(index.row());
  }
}

int PreferencesDialog::exec()
{
  loadSettings();
  return QDialog::exec();
}
