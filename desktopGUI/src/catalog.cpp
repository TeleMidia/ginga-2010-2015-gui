#include "include/catalog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDebug>

void tree_depth_traversal (QList<QTreeWidgetItem *> &items,
                           CatalogItem *parent, PBDS_Node *node)
{
  QColor active_color(180, 255, 255);
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
  else if (node->getType() == PBDS_Node::APPLICATION)
  {
    CatalogItem *child = NULL;
    child = new CatalogItem(node, QStringList () << node->getLabel() << "");
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
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QHBoxLayout *centerLayout = new QHBoxLayout;
  QVBoxLayout *buttonsLayout = new QVBoxLayout;

  _pbds = PBDS::getInstance();

  QLabel *title = new QLabel ("Catalog");

  _treeWidget = new QTreeWidget;
  _treeWidget->setColumnCount(2);
  _treeWidget->setHeaderLabels(QStringList () << "Private Base" << "Active");
  _treeWidget->setAlternatingRowColors(true);
  _treeWidget->setDragDropMode(QAbstractItemView::InternalMove);
  _treeWidget->setDragEnabled(true);
  _treeWidget->viewport()->setAcceptDrops(true);
  _treeWidget->setDropIndicatorShown(true);
  _treeWidget->viewport()->installEventFilter(this);

  connect (_treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
           this, SLOT(changeIcon(QTreeWidgetItem*)));

  connect (_treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
           this, SLOT(changeIcon(QTreeWidgetItem*)));

  QPushButton *closeButton = new QPushButton ("Close");
  closeButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

  connect (closeButton, SIGNAL(clicked()),
           this, SLOT(close()));

  connect (_treeWidget,
           SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this,
           SLOT (changeButtonsState()));

  _playAppButton = new QPushButton ("Play Application");
  _playAppButton->setEnabled(false);

  _saveAppButton = new QPushButton ("Save Application");
  _saveAppButton->setEnabled(false);

  _removeAppButton = new QPushButton ("Remove Application");
  _removeAppButton->setEnabled(false);

  _importAppButton = new QPushButton ("Import Application");


  connect (_removeAppButton,
           SIGNAL(clicked()), this,
           SLOT (removeCurrentItem()));

  buttonsLayout->addWidget(_playAppButton);
  buttonsLayout->addWidget(_saveAppButton);
  buttonsLayout->addWidget(_removeAppButton);
  buttonsLayout->addWidget(_importAppButton);

  buttonsLayout->setAlignment(Qt::AlignTop);

  centerLayout->addWidget(_treeWidget);
  centerLayout->addLayout(buttonsLayout);

  mainLayout->addWidget(title);
  mainLayout->addLayout(centerLayout);
  mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  setLayout(mainLayout);
  setWindowTitle(tr("Catalog"));
  setModal(false);
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

  _treeWidget->clear(); //This removes and deletes all the items in the tree

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
