#include "include/pbds.h"
#include <QDebug>

PBDS* PBDS::_instance = NULL;

PBDS* PBDS::getInstance(){
  if ( _instance == NULL)
    _instance = new PBDS();
  return _instance;
}


PBDS::PBDS()
{
  root = new PBDS_PrivateBase ("root", "Private Base Data Structure");

  // create default bases
  present_apps = new PBDS_PrivateBase(
                                       "present", "PRESENT Applications");
  installed_apps = new PBDS_PrivateBase(
                                       "installed", "Installed Applications");
  resident_apps = new PBDS_PrivateBase(
                                       "resident", "Resident Applications");

  addNode(installed_apps);
  addNode(resident_apps);

  fillPBDSWithExamples();
}

void PBDS::fillPBDSWithExamples()
{
  PBDS_PrivateBase *base1 = new PBDS_PrivateBase("XXXX", "Globo");
  PBDS_PrivateBase *base2 = new PBDS_PrivateBase("XXXX.YYYY", "service1");
  PBDS_PrivateBase *base3 = new PBDS_PrivateBase("ZZZZ", "SBT");

  PBDS_Application *app1 = new PBDS_Application ("NCL1", "Copa America");

  PBDS_Application *first_john = new PBDS_Application(
                                   "first_josh", "O primeiro João");
  PBDS_Application *world_cup = new PBDS_Application(
                                   "world_cup", "Copa do Mundo 2010");

  addNode(base1);
  addNode(app1, base1);
  addNode(base2, base1);
  addNode(base3);
  base1->setActive(true);

  addNode(first_john, resident_apps);
  addNode(world_cup, resident_apps);
}

void PBDS::update()
{
}

bool PBDS::addNode(PBDS_Node *node, PBDS_PrivateBase *parent)
{
  if (node == NULL)
    return false;

  if (parent == 0)
    parent = root;

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

void PBDS::clear()
{
  QList <PBDS_Node *> nodes = root->getNodes();
  foreach (PBDS_Node *node, nodes)
  {
    root->removeNode(node->getId());
  }
}
