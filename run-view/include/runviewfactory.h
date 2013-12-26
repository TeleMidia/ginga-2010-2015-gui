#ifndef RUNVIEWFACTORY_H
#define RUNVIEWFACTORY_H

#include <core/extensions/IPluginFactory.h>

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

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "run_view.json")
#endif

public:
  RunViewFactory();
  ~RunViewFactory ();

  IPlugin* createPluginInstance() { return new RunViewPlugin (); }

  void releasePluginInstance(IPlugin *);

  QList<LanguageType> getSupportedLanguages();

  QString id() const { return "br.puc-rio.telemidia.RunView"; }

  QIcon icon() const { return QIcon (); }

#if QT_VERSION < 0x050000
  /*!
   * \brief Returns the name of the plug-in.
   *
   * \return QString the name of the plug-in.
   */
  QString name() { return "Run View"; }

  /*!
   * \brief Returns the version of Run View implementation.
   *
   * \return QString the version number as string.
   */
  QString version() { return NCLCOMPOSER_PLUGINS_VERSION; }

  /*!
   * \brief Returns the core version that is compatible with this plugin.
   *
   * \return QString the core version that is compatible with this plugin
   * as a string.
   */
  QString compatVersion() {return "0.1";}

  /*!
   * \brief Returns the vendor of Run View (i.e. Telemidia Lab).
   *
   * \return QString the name of the vendor of Run View.
   */
  QString vendor() {return "Telemidia Lab";}

  /*!
   * \brief Returns the copyright of Run View.
   *
   * \return QString the copyright of Run View.
   */
  QString copyright() {return "Telemidia/PUC-Rio";}

  /*!
   * \brief Returns the license of Run View plugin (i.e. LGPL).
   * \return QString the license of Run View.
   */
  QString license() {return "LGPL";}

  /*!
   * \brief Returns a description of the Run View
   *
   * \return QString the description of Run View.
   */
  QString description() {return "Run View allows user to"
                                " run the application from within composer.";}

  /*!
   * \brief Returns the URL to find more information about the plugin.
   *
   * \return QString the description of Run View.
   */
  QString url() {return "http://composer.telemidia.puc-rio.br/run";}

  /*!
   * \brief Returns the plugin category.
   *
   * \return QString the description of Run View.
   */
  QString category() {return "General";}
#endif

};
#endif // RUNVIEWFACTORY_H
