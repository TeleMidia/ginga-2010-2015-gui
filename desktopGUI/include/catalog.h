#ifndef CATALOG_H
#define CATALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <pbds.h>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>

class Catalog : public QDialog
{
  Q_OBJECT
public:
  explicit Catalog(QWidget *parent = 0);
  ~Catalog() { _pbds->clear(); }
  bool eventFilter(QObject *, QEvent *);
  void updateCatalog ();

signals:

public slots:
  void open ();
  void pbdsRemoveCurrentItem();
  void presentShowMoreInformation();
  void presentPlayItem();


private slots:
  void pbdsChangeIcon (QTreeWidgetItem *);
  void pbdsChangeButtonsState ();
  void presentChangeButtonsState();

private:
  void createPBDSTree();
  void createPRESENTTree();

  PBDS* _pbds;

  QVBoxLayout *mainLayout;
  QTreeWidget *_pbdsTreeWidget;
  QPushButton *_pbdsPlayAppButton;
  QPushButton *_pbdsSaveAppButton;
  QPushButton *_pbdsRemoveAppButton;
  QPushButton *_pbdsImportAppButton;
  QTreeWidget *_presentTreeWidget;
  QCheckBox   *_presentShowMoreCheckBox;
  QPushButton *_presentPlayAppButton;
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
