#ifndef CATALOG_H
#define CATALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <pbds.h>
#include <QPushButton>
#include <QCheckBox>

class Catalog : public QDialog
{
  Q_OBJECT
public:
  explicit Catalog(QWidget *parent = 0);

  ~Catalog() { _pbds->clear(); }

  bool eventFilter(QObject *, QEvent *);

signals:

public slots:
  void open ();
  void removeCurrentItem ();
  void showMoreInformation ();


private slots:
  void changeIcon (QTreeWidgetItem *);
  void changeButtonsState ();

private:
  void updateCatalog ();
  PBDS* _pbds;

  QTreeWidget *_treeWidget;

  QPushButton *_playAppButton;
  QPushButton *_saveAppButton;
  QPushButton *_removeAppButton;
  QPushButton *_importAppButton;
  QCheckBox *_showMoreInformation;
};

class CatalogItem : public QTreeWidgetItem
{
public:
  CatalogItem (PBDS_Node *node, const QStringList strings)
    : QTreeWidgetItem (strings)
  { _node = node; setChildIndicatorPolicy(ShowIndicator);}

  PBDS_Node * getPBDSNode () const {return _node; }

private:
  PBDS_Node *_node; //don't delete me!
};

#endif // CATALOG_H
