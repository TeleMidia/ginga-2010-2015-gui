#ifndef CATALOG_H
#define CATALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <pbds.h>

class Catalog : public QDialog
{
  Q_OBJECT
public:
  explicit Catalog(QWidget *parent = 0);

  ~Catalog() { _pbds.clear(); }

signals:

public slots:
  void open ();

private:
  void updateCatalog ();
  PBDS_Manager _pbds;

  QTreeWidget *_treeWidget;
};

#endif // CATALOG_H
