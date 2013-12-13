#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QDesktopWidget>

#include "qnplmainwindow.h"
#include "qnplsettings.h"
#include "util.h"

int Util::SCREEN_HEIGHT;
int Util::SCREEN_WIDTH;

int main(int argc, char *argv[])
{
    qDebug() << argc << argv;

    QApplication application(argc, argv);
    application.addLibraryPath(QDir::currentPath());
    application.setApplicationName("GingaGui");
    application.setOrganizationName("Telemidia Lab");
    application.setOrganizationDomain("telemidia.puc-rio.br");

    Util::init();

    application.setApplicationVersion(Util::VERSION);

    Util::SCREEN_HEIGHT = QApplication::desktop()->height();
    Util::SCREEN_WIDTH = QApplication::desktop()->width();


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
