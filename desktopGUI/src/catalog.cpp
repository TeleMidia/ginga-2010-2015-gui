#include "include/catalog.h"

#include <QLabel>
#include <QVBoxLayout>

void tree_depth_traversal (QTreeWidgetItem *parent, PBDS_Node *node)
{
  if (node->getType() == PBDS_Node::PRIVATE_BASE)
  {
    PBDS_PrivateBase *base = (PBDS_PrivateBase *) node;
    QList <PBDS_Node *> children = base->getNodes();
    for (int i = 0; i < children.size(); i++)
    {
      PBDS_Node *node = children.at(i);
      QTreeWidgetItem *child = new QTreeWidgetItem(
                                QStringList () << node->getLabel());


      parent->addChild(child);

      tree_depth_traversal(child, node);
    }
  }
}

Catalog::Catalog(QWidget *parent) :
  QDialog(parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;

  QLabel *title = new QLabel ("Catalog");

  _treeWidget = new QTreeWidget;
  _treeWidget->setColumnCount(1);
  _treeWidget->setHeaderLabel("Private Base");

  mainLayout->addWidget(title);
  mainLayout->addWidget(_treeWidget);

  setLayout(mainLayout);
  setWindowTitle(tr("Catalog"));
  setModal(false);
}

void Catalog::open()
{
  _pbds.clear();
  updateCatalog();

  QDialog::show();
}

void Catalog::updateCatalog()
{
  PBDS_PrivateBase *base1 = new PBDS_PrivateBase("XXXX", "Globo");
  PBDS_PrivateBase *base2 = new PBDS_PrivateBase("XXXX.YYYY", "Band");
  PBDS_PrivateBase *base3 = new PBDS_PrivateBase("ZZZZ", "SBT");

  PBDS_Application *app1 = new PBDS_Application ("NCL1", "Copa America");

  _pbds.addNode(base1);
  _pbds.addNode(app1, base1);
  _pbds.addNode(base2, base1);
  _pbds.addNode(base3);

  _treeWidget->clear(); //This removes and deletes all the items in the tree

  QList<QTreeWidgetItem *> items;
  QList <PBDS_Node *> nodes = _pbds.getNodes();
  for (int i = 0; i < nodes.size(); i++)
  {
    QTreeWidgetItem *item = new QTreeWidgetItem(
                              QStringList () << nodes.at(i)->getLabel());
    items.append(item);

    tree_depth_traversal(item, nodes.at(i));
  }

  _treeWidget->insertTopLevelItems(0, items);

  _treeWidget->expandAll();

}
