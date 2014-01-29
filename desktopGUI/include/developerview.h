#ifndef DEVELOPERVIEW_H
#define DEVELOPERVIEW_H

#include <QDockWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QStringList>

class DeveloperView : public QDockWidget
{
  Q_OBJECT
public:
  explicit DeveloperView(QWidget *parent = 0);

  inline void appendConsoleMessage (QString message)
  { _console->append(message); }

  inline void clear ()
  {
    _history->clear();
    _console->clear();
  }

  bool eventFilter(QObject *, QEvent *);

signals:
  void sendCommandRequested (QString);

private slots:
  void sendButtonPressed();
  void changeSendButtonStatus();

private:
  QLineEdit *_commandLine;
  QPushButton *_sendButton;
  QTextEdit *_history;
  QTextEdit *_console;
  QStringList *_commandsSent;
  int _index;
};

#endif // DEVELOPERVIEW_H
