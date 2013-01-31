#include <QApplication>
#include <QTranslator>

#include <QDebug>

#include "ui/qnplmainwindow.h"
#include "ui/qnplsettings.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("gingagui");
    application.setApplicationVersion(QString(VERSION));
    application.setOrganizationName("telemidia");
    application.setOrganizationDomain("telemidia.puc-rio.br");

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
