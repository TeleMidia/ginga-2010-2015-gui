#include "include/pbds.h"
#include <QDebug>

PBDS_Manager::PBDS_Manager()
{
  _root = new PBDS_PrivateBase ("root", "Private Base Data Structure");
}

void PBDS_Manager::update()
{
  clear();

  PBDS_PrivateBase *base1 = new PBDS_PrivateBase("XXXX", "Globo");
  PBDS_PrivateBase *base2 = new PBDS_PrivateBase("XXXX.YYYY", "Band");
  PBDS_PrivateBase *base3 = new PBDS_PrivateBase("ZZZZ", "SBT");

  PBDS_Application *app1 = new PBDS_Application ("NCL1", "Copa America");

  PBDS_PrivateBase *installed_apps = new PBDS_PrivateBase(
                                       "installed", "Installed Applications");

  PBDS_Application *first_john = new PBDS_Application(
                                   "first_josh", "O primeiro João");

  PBDS_Application *world_cup = new PBDS_Application(
                                   "world_cup", "Copa do Mundo 2010");

  PBDS_PrivateBase *resident_apps = new PBDS_PrivateBase(
                                       "resident", "Resident Applications");

  addNode(base1);
  addNode(app1, base1);
  addNode(base2, base1);
  addNode(base3);
  addNode(installed_apps);
  addNode(resident_apps);

  addNode(first_john, resident_apps);
  addNode(world_cup, resident_apps);
}

bool PBDS_Manager::addNode(PBDS_Node *node, PBDS_PrivateBase *parent)
{
  if (node == NULL)
    return false;

  if (parent == 0)
    parent = _root;

  if (parent->contains(node->getId()))
  {
    qDebug () << "There is already a private base or application with '" +
                 node->getId() + "' id added in base '" + parent->getId() +
                 "'";
    return false;
  }

  parent->addNode(node);
  return true;
}

void PBDS_Manager::clear()
{
  QList <PBDS_Node *> nodes = _root->getNodes();
  foreach (PBDS_Node *node, nodes)
  {
    _root->removeNode(node->getId());
  }
}
