#ifndef DEBUGVIEW_H
#define DEBUGVIEW_H

#include <QDockWidget>
#include <QListWidget>

class DebugView : public QDockWidget
{
public:
  explicit DebugView(QWidget *parent = 0);

  void addObject (QString, int ident = 0);
  void removeObject (QString, int ident = 0);

public slots:
  inline void clear ()
  { _listWidget->clear(); }

private:
  QListWidget * _listWidget;
};

#endif // DEBUGVIEW_H

