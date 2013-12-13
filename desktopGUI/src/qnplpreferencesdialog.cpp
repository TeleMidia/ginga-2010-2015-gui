#include <QMenu>
#include <QAction>
#include <QModelIndexList>
#include <QMessageBox>

#include "qnplpreferencesdialog.h"
#include "util.h"

#include <QDebug>

QnplPreferencesDialog::QnplPreferencesDialog(QWidget* parent)
  : QDialog(parent)
{

  _generalPane = new QWidget();
  _runPane = new QWidget();

  _generalForm.setupUi(new QDialog(this));
  _runForm.setupUi(new QDialog(this));

  _generalPane->setLayout(_generalForm.verticalLayout);
  _runPane->setLayout(_runForm.verticalLayout);

  _preferencesForm.setupUi(this);

  _runForm.table->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(_preferencesForm._treeItems, SIGNAL(clicked(QModelIndex)),
          SLOT(showPreferencesItem(QModelIndex)));

  connect(_preferencesForm._buttonsGroup, SIGNAL(accepted()),
          SLOT(saveSettings()));

  connect(_runForm.pushButton, SIGNAL(clicked()),
          SLOT(browseExecutable()));

  connect(_runForm.pushButton_2, SIGNAL(clicked()),
          SLOT(browseGingaSettingsFile()));

  connect(_runForm.table, SIGNAL(customContextMenuRequested(QPoint)),
          SLOT(customMenuRequested(QPoint)));
}

QnplPreferencesDialog::~QnplPreferencesDialog()
{
  delete _generalPane;
  delete _runPane;
}

void QnplPreferencesDialog::init(QnplSettings* s)
{
  settings = s;

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


  _preferencesForm._layout->addWidget(_generalPane);
  _preferencesForm._layout->addWidget(_runPane);
  _generalPane->show();
  _runPane->hide();

  loadSettings();
}

void QnplPreferencesDialog::loadSettings()
{
  if (settings->value("enablelog").toString() == "true"){
    _generalForm.checkBox_2->setChecked(true);
  }else{
    _generalForm.checkBox_2->setChecked(false);
  }

  if (settings->value("autoplay").toString() == "true"){
    _generalForm.checkBox->setChecked(true);
  }else{
    _generalForm.checkBox->setChecked(false);
  }

  _runForm.lineEdit->setText(settings->value("location").toString());

  if (settings->value("screensize").toString() == "640x480"){
    _generalForm.comboBox_2->setCurrentIndex(1);
  }else if (settings->value("screensize").toString() == "800x600"){
    _generalForm.comboBox_2->setCurrentIndex(2);
  }else if (settings->value("screensize").toString() == "1024x768"){
    _generalForm.comboBox_2->setCurrentIndex(3);
  }else if (settings->value("screensize").toString() == "854x480"){
    _generalForm.comboBox_2->setCurrentIndex(4);
  }else if (settings->value("screensize").toString() == "1280x720"){
    _generalForm.comboBox_2->setCurrentIndex(5);
  }else if (settings->value("screensize").toString() == "1920x1080"){
    _generalForm.comboBox_2->setCurrentIndex(6);
  }else if (settings->value("screensize").toString() == "320x400"){
    _generalForm.comboBox_2->setCurrentIndex(7); //(!)index 6 is an empty option
  }else if (settings->value("screensize").toString() == "400x320"){
    _generalForm.comboBox_2->setCurrentIndex(8);
  }else if (settings->value("screensize").toString() == "320x180"){
    _generalForm.comboBox_2->setCurrentIndex(9);
  }else if (settings->value("screensize").toString() == "320x240"){
    _generalForm.comboBox_2->setCurrentIndex(10);
  }else if (settings->value("screensize").toString() == "240x320"){
    _generalForm.comboBox_2->setCurrentIndex(11);
  }else{
    _generalForm.comboBox_2->setCurrentIndex(0);
  }

  if (settings->value("lang").toString() == "en"){
    _generalForm.comboBox->setCurrentIndex(0);
  }else if (settings->value("lang").toString() == "pt_br"){
    _generalForm.comboBox->setCurrentIndex(1);
  }else if (settings->value("lang").toString() == "en"){
    _generalForm.comboBox->setCurrentIndex(2);
  }

  _runForm.textEdit->setText(settings->value("parameters").toString());

  _runForm.lineEdit_2->setText(settings->value("gingaconfig_file").toString());

  loadGingaPreferences();
}

void QnplPreferencesDialog::saveSettings()
{
  qDebug() << "Saving settings...";

  if (_generalForm.checkBox->isChecked()){
    settings->setValue("autoplay",true);
  }else{
    settings->setValue("autoplay",false);
  }

  if (_generalForm.checkBox_2->isChecked()){
    settings->setValue("enablelog",true);
  }else{
    settings->setValue("enablelog",false);
  }

  settings->setValue("location",_runForm.lineEdit->text());

  switch (_generalForm.comboBox_2->currentIndex()){
    case 1:
      settings->setValue("screensize","640x480");
      break;
    case 2:
      settings->setValue("screensize","800x600");
      break;
    case 3:
      settings->setValue("screensize","1024x768");
      break;
    case 4:
      settings->setValue("screensize","854x480");
      break;
    case 5:
      settings->setValue("screensize","1280x720");
      break;
    case 6:
      settings->setValue("screensize","1920x1080");
      break;
    case 7:
      settings->setValue("screensize","320x400");
      break;
    case 8:
      settings->setValue("screensize","400x320");
      break;
    case 9:
      settings->setValue("screensize","320x180");
      break;
    case 10:
      settings->setValue("screensize","320x240");
      break;
    case 11:
      settings->setValue("screensize","240x320");
      break;

  }

  switch (_generalForm.comboBox->currentIndex()){
    case 0:
      settings->setValue("lang","en");
      break;
    case 1:
      settings->setValue("lang","pt_br");
      break;
    case 2:
      settings->setValue("lang","es");
      break;
  }

  settings->setValue("parameters",_runForm.textEdit->toPlainText());
  settings->setValue("gingaconfig_file", _runForm.lineEdit_2->text());

  saveGingaPreferences();
}

void QnplPreferencesDialog::showPreferencesItem(QModelIndex index)
{
  QString itemSelected = index.data().toString();

  if (itemSelected == Util::PREFERENCES_ENVIRONMENT)
  {
    _generalPane->show();
    _runPane->hide();
    _preferencesForm._titleLabel->setText(tr("Environment"));

  }
  else if (itemSelected == Util::PREFERENCES_GINGA)
  {
    _generalPane->hide();
    _runPane->show();
    _preferencesForm._titleLabel->setText(tr("Ginga"));
  }
  else if (itemSelected == Util::PREFERENCES_ADVANCED)
  {

  }
  _preferencesForm._titleLabel->setText(itemSelected);
}

void QnplPreferencesDialog::loadGingaPreferences()
{
  if (QFile::exists(_runForm.lineEdit_2->text()))
  {

    QFile* file = new QFile(_runForm.lineEdit_2->text());

    if (file->open(QIODevice::ReadOnly))
    {
      QVector<QPair<QString, QString> > params;

      QTextStream* stream = new QTextStream(file);

      QRegExp rx; rx.setPattern("(.*)=(.*)");

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
        if (p.first != "::" && p.first != "||"){
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

void QnplPreferencesDialog::saveGingaPreferences()
{
  if (QFile::exists(_runForm.lineEdit_2->text())){

    QFile* file = new QFile(_runForm.lineEdit_2->text());

    if (file->open(QIODevice::WriteOnly | QIODevice::Text)){
      QTextStream stream(file);
      stream.setCodec("UTF-8");

      stream << "::" << " = " << "0" << endl;
      stream << "||" << " = " << "0" << endl;

      int n = _runForm.table->model()->rowCount();

      QAbstractItemModel* model = _runForm.table->model();

      for (int i=0;i<n;i++){
        QString name = model->data(model->index(i,0)).toString();
        QString value = model->data(model->index(i,1)).toString();

        if (name != ""){
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

void QnplPreferencesDialog::browseExecutable()
{
  QString f = QFileDialog::getOpenFileName(this,tr("Open"));

  if (f != ""){
    _runForm.lineEdit->setText(f);
  }
}

void QnplPreferencesDialog::browseGingaSettingsFile()
{
  QString f = QFileDialog::getOpenFileName(this,tr("Open"));

  if (f != ""){
    _runForm.lineEdit_2->setText(f);

    loadGingaPreferences();
  }
}

void QnplPreferencesDialog::customMenuRequested(QPoint point)
{
  QModelIndex index = _runForm.table->indexAt(point);

  QMenu *menu = new QMenu(this);
  QAction *removeAction = menu->addAction(tr("Remove variable"));

  connect(removeAction, SIGNAL(triggered()), SLOT(removeVariable()));

  menu->popup(_runForm.table->viewport()->mapToGlobal(point));
}

void QnplPreferencesDialog::removeVariable()
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
