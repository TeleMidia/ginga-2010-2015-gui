#ifndef RUNVIEWPLUGIN_H
#define RUNVIEWPLUGIN_H

#include <core/extensions/IPlugin.h>
#include <core/modules/ProjectControl.h>

#include "../include/gingaproxy.h"
#include "../include/qnplview.h"
#include "include/configdialog.h"

#include <QPushButton>

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

  inline QWidget* getWidget() { return _runWidget;}

public slots:
  inline void init ()
  {
    _currentProject = IPlugin::getProject();
    _lineEdit->setText(_currentProject->getLocation());
  }

  void playApplication ();
  void execConfigDialog ();
  void updateGUI ();

private:
  ProjectControl * _projectControl;
  Project * _currentProject;

  QWidget * _runWidget;
  QnplView * _gingaView;
  GingaProxy * _gingaProxy;

  QPushButton *_playButton;
  QPushButton *_stopButton;
  QLineEdit *_lineEdit;
};

#endif // RUNVIEWPLUGIN_H
