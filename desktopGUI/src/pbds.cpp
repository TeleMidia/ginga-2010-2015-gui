#include "include/pbds.h"
#include <QDebug>

PBDS_Manager::PBDS_Manager()
{
  _root = new PBDS_PrivateBase ("root", "Private Base Data Structure");
  PBDS_PrivateBase *installed_apps = new PBDS_PrivateBase(
                                       "installed", "Installed Applications");

  PBDS_PrivateBase *resident_apps = new PBDS_PrivateBase(
                                       "resident", "Resident Applications");

  _root->addNode(installed_apps);
  _root->addNode(resident_apps);
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
