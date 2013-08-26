#include <QApplication>
#include <QTranslator>
#include <QDebug>

#include "qnplmainwindow.h"
#include "qnplsettings.h"
#include "qnplutil.h"

int main(int argc, char *argv[])
{
    qDebug() << argc << argv;

    QApplication application(argc, argv);
    application.setApplicationName("gingagui");
    application.setApplicationVersion(QnplUtil::VERSION);
    application.setOrganizationName("telemidia");
    application.setOrganizationDomain("telemidia.puc-rio.br");

    QnplSettings settings;
    QString lang = settings.value("lang").toString();

    qDebug() << lang;

    if (lang == "pt_br"){
      QTranslator* translator = new QTranslator();
      translator->load("translations/"+lang+".qm");
      application.installTranslator(translator);
    }else if (lang == "es"){
      QTranslator* translator = new QTranslator();
      translator->load("translations/"+lang+".qm");
      application.installTranslator(translator);
    }

    QnplMainWindow window;
    window.show();

    if (application.arguments().size() > 1)
    {
      QString file = application.arguments().at(1);
      file = file.replace('/',QDir::separator());
      file = file.replace('\\',QDir::separator());

      window.load(file);
    }

    return application.exec();
}
