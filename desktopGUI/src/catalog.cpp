#include "include/catalog.h"

#include <QLabel>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>

void pbdsTreeDepthTraversal (QList<QTreeWidgetItem *> &items,
                           CatalogItem *parent, PBDS_Node *node)
{
  QColor active_color(180, 255, 255);

  // handle a PRIVATE_BASE node
  if (node->getType() == PBDS_Node::PRIVATE_BASE)
    {
      CatalogItem *private_base = NULL;
      PBDS_PrivateBase *base = (PBDS_PrivateBase *) node;
      QList <PBDS_Node *> children = base->getNodes();
      QStringList labels = QStringList () << node->getLabel();

      if (node->getType() == PBDS_Node::PRIVATE_BASE)
        labels << (((PBDS_PrivateBase *)node)->isActive() ? "yes" : "no");

      private_base = new CatalogItem(node, labels);
      private_base->setIcon(0, QIcon(":icons/opened_folder"));

      if (base->isActive())
        {
          for (int i = 0; i < private_base->columnCount(); i++)
            private_base->setBackground(i, active_color);
        }

      if (parent != NULL)
        parent->addChild(private_base);
      else
        items.append(private_base);

      if (children.size() == 0)
        {
          CatalogItem *item = new CatalogItem (NULL, QStringList() << "(empty)");
          item->setTextColor(0, Qt::gray);
          if (parent != NULL)
            parent->addChild(item);
          else
            items.append(item);
        }
      else
        {
          for (int i = 0; i < children.size(); i++)
            pbdsTreeDepthTraversal(items, private_base, children.at(i));
        }
    }

  // handle a APPLICATION node
  else if (node->getType() == PBDS_Node::APPLICATION)
    {
      CatalogItem *child = NULL;
      QStringList labels;
      PBDS_Application* app = (PBDS_Application*) node;
      labels << app->getLabel();
      child = new CatalogItem(node, labels);
      child->setIcon(0, QIcon (":icons/ncl"));

      if (parent != NULL)
        {
          PBDS_Node *parent_pbds_node = parent->getPBDSNode();
          if (parent_pbds_node &&
              parent_pbds_node->getType() == PBDS_Node::PRIVATE_BASE &&
              ((PBDS_PrivateBase*)parent_pbds_node)->isActive())
            {
              for (int i = 0; i < child->columnCount(); i++)
                child->setBackground(i, active_color);
            }

          parent->addChild(child);
        }
      else
        items.append(child);
    }
}

Catalog::Catalog(QWidget *parent) :
  QDialog(parent)
{
  mainLayout = new QVBoxLayout;
  _pbds = PBDS::getInstance();
  _pbdsTreeWidget = NULL;
  _presentTreeWidget =NULL;

  // create PBDSTree
//  createPBDSTree();

  // create PRESENTTree
  createPRESENTTree();

  // close button
  QPushButton *closeButton = new QPushButton ("Close");
  closeButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
  connect (closeButton, SIGNAL(clicked()),
           this, SLOT(close()));

  // configure QDialog flags
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  setLayout(mainLayout);
  setWindowTitle(tr("Catalog"));
  setModal(false);
  mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);
}

void Catalog::open()
{
  updateCatalog();
  QDialog::show();
}

bool Catalog::eventFilter(QObject *object, QEvent *event)
{
  if (_pbdsTreeWidget != NULL && object == _pbdsTreeWidget->viewport())
    {
      QEvent::Type type = event->type();
      if (type == QEvent::DragEnter)
        {
          CatalogItem *item = (CatalogItem *) _pbdsTreeWidget->currentItem();

          if (item->parent() != NULL /* I'm a top level item */
              && item->getPBDSNode() != NULL /* I'm an 'empty' label */)
            {
              ((QDragEnterEvent *)event)->acceptProposedAction();
            }
          return true;
        }
      else if (type == QEvent::Drop)
        {
          qDebug () << "drop";
          QTreeWidgetItem *currentItem = _pbdsTreeWidget->itemAt(
                ((QDropEvent*)event)->pos());
          if (currentItem)
            qDebug () << currentItem->text(0);

          //      ((QDropEvent*)event)->acceptProposedAction();
          return true;
        }
    }
  return false;
}

void Catalog::updateCatalog()
{
    _pbds->update();

    if(_pbdsTreeWidget != NULL){

        // This removes and deletes all the items in the tree
        _pbdsTreeWidget->clear();

        QList<QTreeWidgetItem *> items;
        QList <PBDS_Node *> nodes = _pbds->getNodes();
        for (int i = 0; i < nodes.size(); i++)
            pbdsTreeDepthTraversal(items, 0, nodes.at(i));

        _pbdsTreeWidget->insertTopLevelItems(0, items);

        for (int i = 0; i < items.size(); i++)
            _pbdsTreeWidget->resizeColumnToContents(i);

        _pbdsTreeWidget->expandAll();
    }

    if(_presentTreeWidget != NULL){

        // This removes and deletes all the items in the tree
        _presentTreeWidget->clear();

        // fullfill_presentTreeWidget
        QList<QTreeWidgetItem *> items;
        QList <PBDS_Node *> nodes = _pbds->getInstance()->present_apps->getNodes();
        CatalogItem *child = NULL;
        PBDS_Application* app = NULL;
        for (int i = 0; i < nodes.size(); i++){
            app = (PBDS_Application*) nodes.at(i);
            QStringList labels;
            labels << app->getLabel() << app->mainNclUri <<  app->controlCode << app->targetProfile << app->transportType;
            child = new CatalogItem(app, labels);
            child->setIcon(0, QIcon (":icons/ncl"));
            items.append(child);
        }
        _presentTreeWidget->insertTopLevelItems(0, items);
        for (int i = 0; i < items.size(); i++)
            _presentTreeWidget->resizeColumnToContents(i);
        _presentTreeWidget->expandAll();
    }
}

//
// PBDS Tree related methods
//
void Catalog::createPBDSTree()
{
  QLabel *title = new QLabel ("PBDS");
  mainLayout->addWidget(title);
  QHBoxLayout *pbdsLayout = new QHBoxLayout();
  QWidget * buttonsWidget = new QWidget();
  QVBoxLayout *buttonsLayout = new QVBoxLayout();
  buttonsWidget->setLayout(buttonsLayout);

  // create tree collumns
  _pbdsTreeWidget = new QTreeWidget;
  _pbdsTreeWidget->setColumnCount(2);
  _pbdsTreeWidget->setHeaderLabels(QStringList () << "Private Base" << "Active");

  // configure tree flags
  _pbdsTreeWidget->setAlternatingRowColors(true);
  _pbdsTreeWidget->setDragDropMode(QAbstractItemView::InternalMove);
  _pbdsTreeWidget->setDragEnabled(true);
  _pbdsTreeWidget->viewport()->setAcceptDrops(true);
  _pbdsTreeWidget->setDropIndicatorShown(true);
  _pbdsTreeWidget->viewport()->installEventFilter(this);
  _pbdsTreeWidget->setMinimumWidth(200);

  // tree signals
  connect (_pbdsTreeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
           this, SLOT(pbdsChangeIcon(QTreeWidgetItem*)));
  connect (_pbdsTreeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
           this, SLOT(pbdsChangeIcon(QTreeWidgetItem*)));
  connect (_pbdsTreeWidget,
           SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT (pbdsChangeButtonsState()));

  _pbdsPlayAppButton = new QPushButton ("Play Application");
  _pbdsPlayAppButton->setEnabled(false);

  _pbdsSaveAppButton = new QPushButton ("Save Application");
  _pbdsSaveAppButton->setEnabled(false);

  _pbdsRemoveAppButton = new QPushButton ("Remove Application");
  _pbdsRemoveAppButton->setEnabled(false);
  connect (_pbdsRemoveAppButton,
           SIGNAL(clicked()), this,
           SLOT (pbdsRemoveCurrentItem()));

  _pbdsImportAppButton = new QPushButton ("Import Application");

  // add widgets in mainLayout
  buttonsLayout->addWidget(_pbdsPlayAppButton);
  buttonsLayout->addWidget(_pbdsSaveAppButton);
  buttonsLayout->addWidget(_pbdsRemoveAppButton);
  buttonsLayout->addWidget(_pbdsImportAppButton);
  buttonsLayout->setAlignment(Qt::AlignTop);
  buttonsWidget->setMinimumWidth(200);
  pbdsLayout->addWidget(_pbdsTreeWidget);
  pbdsLayout->addWidget(buttonsWidget);
  mainLayout->addLayout(pbdsLayout);

}

void Catalog::pbdsRemoveCurrentItem()
{
  QTreeWidgetItem *currentItem = _pbdsTreeWidget->currentItem();

  for (int i = 0; i < currentItem->columnCount(); i++)
    _pbdsTreeWidget->removeItemWidget(currentItem, i);

  delete currentItem;
}

void Catalog::pbdsChangeIcon(QTreeWidgetItem *item)
{
  if (item != NULL && ((CatalogItem *)item)->getPBDSNode() != NULL
      && ((CatalogItem *)item)->getPBDSNode()->getType() ==
      PBDS_Node::PRIVATE_BASE)
    {
      item->setIcon(0, item->isExpanded() ? QIcon (":icons/opened_folder") :
                                            QIcon (":icons/closed_folder"));
    }
}

void Catalog::pbdsChangeButtonsState()
{
  CatalogItem *item = (CatalogItem *) _pbdsTreeWidget->currentItem();
  if (item != NULL && item->getPBDSNode() != NULL)
    {
      _pbdsRemoveAppButton->setEnabled(
            item->getPBDSNode()->getType() == PBDS_Node::APPLICATION);

      _pbdsPlayAppButton->setEnabled(
            item->getPBDSNode()->getType() == PBDS_Node::APPLICATION);
    }
}

//
// PRESENT Tree related methods
//
void Catalog::createPRESENTTree()
{
  QLabel *title = new QLabel ("Signalized Applications");
  mainLayout->addWidget(title);
  QHBoxLayout *presentLayout = new QHBoxLayout;
  QWidget * buttonsWidget = new QWidget();
  QVBoxLayout *buttonsLayout = new QVBoxLayout();
  buttonsWidget->setLayout(buttonsLayout);

  // create tree collumns
  _presentTreeWidget = new QTreeWidget;
  _presentTreeWidget->setColumnCount(5);
  QStringList labels;
  labels << "Application Id" << "main NCL URI" << "Control Code"<< "Target profile" << "Transport Type";
  _presentTreeWidget->setHeaderLabels(labels);
  _presentTreeWidget->header()->resizeSection(0, 90);
  _presentTreeWidget->header()->resizeSection(1, 400);
  _presentTreeWidget->header()->resizeSection(2 , 90);
  _presentTreeWidget->header()->resizeSection(3 , 90);
  _presentTreeWidget->header()->resizeSection(4 , 90);
  _presentTreeWidget->setMinimumWidth(800);
  _presentTreeWidget->hideColumn(1);
  _presentTreeWidget->hideColumn(2);
  _presentTreeWidget->hideColumn(3);
  _presentTreeWidget->hideColumn(4);

  // configure tree flags
  _presentTreeWidget->setAlternatingRowColors(true);
  _presentTreeWidget->setDragDropMode(QAbstractItemView::InternalMove);
  _presentTreeWidget->setDragEnabled(true);
  _presentTreeWidget->viewport()->setAcceptDrops(true);
  _presentTreeWidget->setDropIndicatorShown(true);
  _presentTreeWidget->viewport()->installEventFilter(this);


  // tree signals

  _presentPlayAppButton = new QPushButton ("Play Application");
  _presentPlayAppButton->setEnabled(false);
  connect (_presentTreeWidget,
           SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT (presentChangeButtonsState()));
  connect (_presentPlayAppButton,
           SIGNAL(clicked()), this,
           SLOT (presentPlayItem()));

  _presentShowMoreCheckBox = new QCheckBox ("Show more information");
  connect (_presentShowMoreCheckBox,
           SIGNAL(clicked()), this,
           SLOT (presentShowMoreInformation()));

  // add widgets in mainLayout
  buttonsLayout->addWidget(_presentPlayAppButton);
  buttonsLayout->addWidget(_presentShowMoreCheckBox);
  buttonsLayout->setAlignment(Qt::AlignTop);
  buttonsWidget->setMinimumWidth(200);
  presentLayout->addWidget(_presentTreeWidget);
  presentLayout->addWidget(buttonsWidget);
  mainLayout->addLayout(presentLayout);
}

void Catalog::presentChangeButtonsState()
{
  CatalogItem *item = (CatalogItem *) _presentTreeWidget->currentItem();
  if (item != NULL && item->getPBDSNode() != NULL)
    {
      _presentPlayAppButton->setEnabled(
            item->getPBDSNode()->getType() == PBDS_Node::APPLICATION);
    }
}

void Catalog::presentPlayItem()
{
    CatalogItem *item = (CatalogItem *) _presentTreeWidget->currentItem();
    PBDS_Application * app = (PBDS_Application *) item->getPBDSNode();
    qDebug() << "currentItem=" << app->mainNclUri;
    emit playApplicationChannelRequested(app->mainNclUri);
    this->accept();
}

void Catalog::presentShowMoreInformation()
{
  _presentTreeWidget->setColumnHidden(1,!_presentShowMoreCheckBox->isChecked());
  _presentTreeWidget->setColumnHidden(2,!_presentShowMoreCheckBox->isChecked());
  _presentTreeWidget->setColumnHidden(3,!_presentShowMoreCheckBox->isChecked());
  _presentTreeWidget->setColumnHidden(4,!_presentShowMoreCheckBox->isChecked());
  _presentTreeWidget->header()->resizeSection(0, 90);
  _presentTreeWidget->header()->resizeSection(1, 400);
  _presentTreeWidget->header()->resizeSection(2 , 90);
  _presentTreeWidget->header()->resizeSection(3 , 90);
  _presentTreeWidget->header()->resizeSection(4 , 90);
}


