#include "include/catalog.h"

#include <QLabel>
#include <QVBoxLayout>

void tree_depth_traversal (QTreeWidgetItem *parent, PBDS_Node *node)
{
  if (node->getType() == PBDS_Node::PRIVATE_BASE)
  {
    PBDS_PrivateBase *base = (PBDS_PrivateBase *) node;
    QList <PBDS_Node *> children = base->getNodes();
    if (children.size() == 0)
    {
      QTreeWidgetItem *item = new QTreeWidgetItem (QStringList() << "(empty)");
      item->setTextColor(0, Qt::gray);
      parent->addChild(item);
    }
    else
      for (int i = 0; i < children.size(); i++)
      {
        PBDS_Node *node = children.at(i);
        CatalogItem *child = new CatalogItem(node,
                                           QStringList () << node->getLabel());

        if (node->getType() == PBDS_Node::PRIVATE_BASE)
          child->setIcon(0, QIcon(":icons/opened_folder"));
        else if (node->getType() == PBDS_Node::APPLICATION)
          child->setIcon(0, QIcon (":icons/ncl"));

        parent->addChild(child);

        tree_depth_traversal(child, node);
      }
  }
}

Catalog::Catalog(QWidget *parent) :
  QDialog(parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QHBoxLayout *centerLayout = new QHBoxLayout;
  QVBoxLayout *buttonsLayout = new QVBoxLayout;

  QLabel *title = new QLabel ("Catalog");

  _treeWidget = new QTreeWidget;
  _treeWidget->setColumnCount(1);
  _treeWidget->setHeaderLabel("Private Base");
  _treeWidget->setAlternatingRowColors(true);

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

  _saveAppButton = new QPushButton ("Save Application");
  _saveAppButton->setEnabled(false);

  _removeAppButton = new QPushButton ("Remove Application");
  _removeAppButton->setEnabled(false);

  _importAppButton = new QPushButton ("Import Application");

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

void Catalog::changeIcon(QTreeWidgetItem *item)
{
  if (item != NULL && ((CatalogItem *)item)->getPBDSNode()->getType() ==
      PBDS_Node::PRIVATE_BASE)
  {
    item->setIcon(0, item->isExpanded() ? QIcon (":icons/opened_folder") :
                                          QIcon (":icons/closed_folder"));
  }
}

void Catalog::changeButtonsState()
{
  CatalogItem *item = (CatalogItem *) _treeWidget->currentItem();
  if (item != NULL)
  {
    _removeAppButton->setEnabled(
          item->getPBDSNode()->getType() == PBDS_Node::APPLICATION);
  }
}

void Catalog::updateCatalog()
{
  _pbds.update();

  _treeWidget->clear(); //This removes and deletes all the items in the tree

  QList<QTreeWidgetItem *> items;
  QList <PBDS_Node *> nodes = _pbds.getNodes();
  for (int i = 0; i < nodes.size(); i++)
  {
    CatalogItem *item = new CatalogItem( nodes.at(i),
                              QStringList () << nodes.at(i)->getLabel());

    if (nodes.at(i)->getType() == PBDS_Node::PRIVATE_BASE)
      item->setIcon(0, QIcon(":icons/opened_folder"));

    items.append(item);

    tree_depth_traversal(item, nodes.at(i));
  }

  _treeWidget->insertTopLevelItems(0, items);

  _treeWidget->expandAll();
}
