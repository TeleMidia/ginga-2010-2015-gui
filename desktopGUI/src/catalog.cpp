#include "include/catalog.h"

#include <QLabel>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDebug>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>

void tree_depth_traversal (QList<QTreeWidgetItem *> &items,
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
            tree_depth_traversal(items, private_base, children.at(i));
        }
    }

  // handle a APPLICATION node
  else if (node->getType() == PBDS_Node::APPLICATION)
    {
      CatalogItem *child = NULL;
      QStringList labels;
      PBDS_Application* app = (PBDS_Application*) node;
      labels << app->getLabel() << "" << app->mainNclUri <<  app->controlCode << app->targetProfile << app->transportType;
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

  // Show trees
  createPBDSTree();
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



void Catalog::createPBDSTree()
{
  QLabel *title = new QLabel ("PBDS");
  mainLayout->addWidget(title);
  QHBoxLayout *pbdsLayout = new QHBoxLayout();
  QWidget * buttonsWidget = new QWidget();
  QVBoxLayout *buttonsLayout = new QVBoxLayout();
  buttonsWidget->setLayout(buttonsLayout);

  // create tree collumns
  _treeWidget = new QTreeWidget;
  _treeWidget->setColumnCount(2);
  _treeWidget->setHeaderLabels(QStringList () << "Private Base" << "Active");

  // configure tree flags
  _treeWidget->setAlternatingRowColors(true);
  _treeWidget->setDragDropMode(QAbstractItemView::InternalMove);
  _treeWidget->setDragEnabled(true);
  _treeWidget->viewport()->setAcceptDrops(true);
  _treeWidget->setDropIndicatorShown(true);
  _treeWidget->viewport()->installEventFilter(this);
  _treeWidget->setMinimumWidth(QApplication::desktop()->width() * 15/100);

  // tree signals
  connect (_treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
           this, SLOT(changeIcon(QTreeWidgetItem*)));
  connect (_treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
           this, SLOT(changeIcon(QTreeWidgetItem*)));
  connect (_treeWidget,
           SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT (changeButtonsState()));

  _playAppButton = new QPushButton ("Play Application");
  _playAppButton->setEnabled(false);

  _saveAppButton = new QPushButton ("Save Application");
  _saveAppButton->setEnabled(false);

  _removeAppButton = new QPushButton ("Remove Application");
  _removeAppButton->setEnabled(false);
  connect (_removeAppButton,
           SIGNAL(clicked()), this,
           SLOT (removeCurrentItem()));

  _importAppButton = new QPushButton ("Import Application");

  // add widgets in mainLayout
  buttonsLayout->addWidget(_playAppButton);
  buttonsLayout->addWidget(_saveAppButton);
  buttonsLayout->addWidget(_removeAppButton);
  buttonsLayout->addWidget(_importAppButton);
  buttonsLayout->setAlignment(Qt::AlignTop);
  buttonsWidget->setMinimumWidth(QApplication::desktop()->width() * 10/100);
  pbdsLayout->addWidget(_treeWidget);
  pbdsLayout->addWidget(buttonsWidget);
  mainLayout->addLayout(pbdsLayout);

}

void Catalog::createPRESENTTree()
{
  QLabel *title = new QLabel ("PRESENT Applications");
  mainLayout->addWidget(title);
  QHBoxLayout *presentLayout = new QHBoxLayout;
  QWidget * buttonsWidget = new QWidget();
  QVBoxLayout *buttonsLayout = new QVBoxLayout();
  buttonsWidget->setLayout(buttonsLayout);

  // create tree collumns
  _presentTreeWidget = new QTreeWidget;
  _presentTreeWidget->setColumnCount(5);
  _presentTreeWidget->setHeaderLabels(QStringList () << "Application Id" << "main NCL URI" << "Control Code"<< "Target profile" << "Transport Type");
  _presentTreeWidget->header()->resizeSection(0, 100);
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
  _presentTreeWidget->setMinimumWidth(QApplication::desktop()->width() * 15/100);


  // tree signals
  connect (_presentTreeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
           this, SLOT(changeIcon(QTreeWidgetItem*)));
  connect (_presentTreeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
           this, SLOT(changeIcon(QTreeWidgetItem*)));
  connect (_presentTreeWidget,
           SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT (changeButtonsState()));

  _playAppButton = new QPushButton ("Play Application");
  _playAppButton->setChecked(false);

  _showMoreInformation = new QCheckBox ("Show more information");
  connect (_showMoreInformation,
           SIGNAL(clicked()), this,
           SLOT (showPresentTreeMoreInformation()));

  // add widgets in mainLayout
  buttonsLayout->addWidget(_playAppButton);
  buttonsLayout->addWidget(_showMoreInformation);
  buttonsLayout->setAlignment(Qt::AlignTop);
  buttonsWidget->setMinimumWidth(QApplication::desktop()->width() * 10/100);
  presentLayout->addWidget(_presentTreeWidget);
  presentLayout->addWidget(buttonsWidget);
  mainLayout->addLayout(presentLayout);
}

void Catalog::open()
{
  updateCatalog();

  QDialog::show();
}

void Catalog::removeCurrentItem()
{
  QTreeWidgetItem *currentItem = _treeWidget->currentItem();

  for (int i = 0; i < currentItem->columnCount(); i++)
    _treeWidget->removeItemWidget(currentItem, i);

  delete currentItem;
}

void Catalog::showPresentTreeMoreInformation()
{
  _presentTreeWidget->setColumnHidden(1,!_showMoreInformation->isChecked());
  _presentTreeWidget->setColumnHidden(2,!_showMoreInformation->isChecked());
  _presentTreeWidget->setColumnHidden(3,!_showMoreInformation->isChecked());
  _presentTreeWidget->setColumnHidden(4,!_showMoreInformation->isChecked());
  _presentTreeWidget->header()->resizeSection(0, 100);
  _presentTreeWidget->header()->resizeSection(1, 100);
  _presentTreeWidget->header()->resizeSection(2 , 100);
  _presentTreeWidget->header()->resizeSection(3 , 100);
  _presentTreeWidget->header()->resizeSection(4 , 100);
}


void Catalog::changeIcon(QTreeWidgetItem *item)
{
  if (item != NULL && ((CatalogItem *)item)->getPBDSNode() != NULL
      && ((CatalogItem *)item)->getPBDSNode()->getType() ==
      PBDS_Node::PRIVATE_BASE)
    {
      item->setIcon(0, item->isExpanded() ? QIcon (":icons/opened_folder") :
                                            QIcon (":icons/closed_folder"));
    }
}

void Catalog::changeButtonsState()
{
  CatalogItem *item = (CatalogItem *) _treeWidget->currentItem();
  if (item != NULL && item->getPBDSNode() != NULL)
    {

      _removeAppButton->setEnabled(
            item->getPBDSNode()->getType() == PBDS_Node::APPLICATION);

      _playAppButton->setEnabled(
            item->getPBDSNode()->getType() == PBDS_Node::APPLICATION);
    }
}

void Catalog::updateCatalog()
{
  _pbds->update();

  // This removes and deletes all the items in the tree
  _treeWidget->clear();

  QList<QTreeWidgetItem *> items;
  QList <PBDS_Node *> nodes = _pbds->getNodes();
  for (int i = 0; i < nodes.size(); i++)
    tree_depth_traversal(items, 0, nodes.at(i));

  _treeWidget->insertTopLevelItems(0, items);

  for (int i = 0; i < items.size(); i++)
    _treeWidget->resizeColumnToContents(i);

  _treeWidget->expandAll();
}

bool Catalog::eventFilter(QObject *object, QEvent *event)
{
  if (object == _treeWidget->viewport())
    {
      QEvent::Type type = event->type();
      if (type == QEvent::DragEnter)
        {
          CatalogItem *item = (CatalogItem *) _treeWidget->currentItem();

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
          QTreeWidgetItem *currentItem = _treeWidget->itemAt(
                ((QDropEvent*)event)->pos());
          if (currentItem)
            qDebug () << currentItem->text(0);

          //      ((QDropEvent*)event)->acceptProposedAction();
          return true;
        }
    }
  return false;
}
