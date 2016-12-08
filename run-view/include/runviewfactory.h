#ifndef RUNVIEWFACTORY_H
#define RUNVIEWFACTORY_H

#include <extensions/IPluginFactory.h>
#include "runviewplugin.h"

using namespace composer::extension;

/*!
 * \brief Handles the creation and deletion of RunViewPlugin objects.
 */
class RunViewFactory : public QObject,
    public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "run_view.json")

public:
  RunViewFactory();
  ~RunViewFactory ();

  IPlugin* createPluginInstance() { return new RunViewPlugin (); }

  void releasePluginInstance(IPlugin *);

  QList<LanguageType> getSupportedLanguages();

  QString id() const { return "br.puc-rio.telemidia.RunView"; }

  QIcon icon() const { return QIcon (); }
};
#endif // RUNVIEWFACTORY_H
