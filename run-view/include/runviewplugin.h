#ifndef RUNVIEWPLUGIN_H
#define RUNVIEWPLUGIN_H

#include <extensions/IPlugin.h>
#include <modules/ProjectControl.h>

#include "../include/gingaproxy.h"
#include "../include/gingaview.h"
#include "include/configdialog.h"

#include <QPushButton>
#include <QToolButton>
#include <QMenu>
#include <QAction>

using namespace composer::extension;

/*!
 * \brief Run View allows user to
 *        run the application from within composer."
 */
class RunViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  explicit RunViewPlugin();
  inline ~RunViewPlugin(){ delete _runWidget; }
  inline QWidget* getWidget() { return _runWidget;}

public slots:
  void init ();
  void playApplication ();
  void runPassiveDevice();
  void runActiveDevice();
  void execConfigDialog();
  void updateGUI();

  inline bool eventFilter(QObject *obj, QEvent *event)
  {
    if (event->type() == QEvent::Resize)
    {
      QString aspectRatio = _settings->value(Util::V_ASPECT_RATIO,
                                             "").toString();
      if (aspectRatio == Util::WIDE)
      {
        _gingaView->setFixedHeight(_gingaView->width() * 9 / 16);
        return true;
      }
      else if (aspectRatio == Util::STANDARD)
      {
        _gingaView->setFixedHeight(_gingaView->width() * 3 / 4);
        return true;
      }
      else
      {
        _gingaView->setMinimumSize(0, 0);
        _gingaView->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
      }
    }
    return IPlugin::eventFilter(obj, event);
  }


private:
  ProjectControl * _projectControl;
  Project *_currentProject;

  QWidget *_runWidget;
  GingaView * _gingaView;
  GingaProxy * _gingaProxy;

  QToolButton *_playButton;
  QMenu *menu_Multidevice;
  QPushButton *_stopButton;
  QLineEdit *_lineEdit;
  QSettings *_settings;
};

#endif // RUNVIEWPLUGIN_H
