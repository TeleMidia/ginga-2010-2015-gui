#ifndef PBDS_H
#define PBDS_H

#include <QMap>
#include <QString>
#include <QList>

class PBDS_Node
{
public:
  enum PBDS_NodeType {PRIVATE_BASE, APPLICATION};

  PBDS_Node (const QString &id, const QString &label, PBDS_NodeType type)
  {
    _id = id;
    _label = label;
    _type = type;
  }
  virtual ~PBDS_Node() {}

  QString getId () const { return _id; }
  QString getLabel () const { return _label; }
  PBDS_NodeType getType () const { return _type; }

protected:
  QString _id;
  QString _label;
  PBDS_NodeType _type;
};

class PBDS_PrivateBase : public PBDS_Node
{
public:
  PBDS_PrivateBase (const QString &id, const QString &label)
    : PBDS_Node(id, label, PRIVATE_BASE) {}

  ~PBDS_PrivateBase()
  {
    foreach (QString id, _nodes.keys())
    {
      PBDS_Node *node = _nodes.value(id);
      delete node;
    }
  }

  void addNode (PBDS_Node *node)
  {
    if (node != NULL)
      _nodes[node->getId()] = node;
  }

  void removeNode (QString id)
  {
    PBDS_Node *node = _nodes.take(id);
    if (node != NULL)
      delete node;
  }

  QList <PBDS_Node *> getNodes () const
  {
    QList <PBDS_Node *> list;
    foreach (QString id, _nodes.keys())
        list.append(_nodes.value(id));

    return list;
  }

  bool contains (QString id) const { return _nodes.contains(id); }

protected:
  QMap <QString, PBDS_Node *> _nodes;
};

class PBDS_Application : public PBDS_Node
{
public:
  PBDS_Application (const QString &id, const QString &label)
    : PBDS_Node(id, label, APPLICATION) {}

  ~PBDS_Application () {}

  void addURI (QString const &uri) { _uris.append(uri); }
  QList <QString> getURIs () const { return _uris; }

private:
  bool _isPersistent;
  QList <QString> _uris;
};

class PBDS_Manager
{
public:
  PBDS_Manager();

  void clear ();
  bool addNode(PBDS_Node *node, PBDS_PrivateBase *parent = 0);
  QList <PBDS_Node *> getNodes () const { return _root->getNodes(); }

private:
  PBDS_PrivateBase *_root;

};

#endif // PBDS_H
