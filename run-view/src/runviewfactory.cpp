#include "runviewfactory.h"

RunViewFactory::RunViewFactory()
{
}

RunViewFactory::~RunViewFactory()
{
  GingaProxy::deleteInstance();
}

void RunViewFactory::releasePluginInstance(IPlugin *plugin)
{
  RunViewPlugin *runView = qobject_cast<RunViewPlugin*>(plugin);

  if (runView)
  {
    delete runView;
  }
}

QList<LanguageType> RunViewFactory::getSupportedLanguages()
{
  QList<LanguageType> lTypes;
  lTypes.append(NCL);
  return lTypes;
}
