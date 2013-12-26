#include "runviewfactory.h"

RunViewFactory::RunViewFactory()
{
}

RunViewFactory::~RunViewFactory()
{
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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(RunView, RunViewFactory)
#endif
