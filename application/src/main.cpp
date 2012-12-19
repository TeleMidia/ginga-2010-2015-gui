#include <QApplication>
#include <QTranslator>
#include <QDebug>

#include "ui/qnplmainwindow.h"
#include "ui/qnplsettings.h"


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("Ginga");
    application.setApplicationVersion("1.2.0");
    application.setOrganizationName("TeleMidia Lab");
    application.setOrganizationDomain("telemidia.puc-rio.br");

    QnplSettings* settings = new QnplSettings();

    QTranslator translator;
    translator.load(application.applicationDirPath()+"/translations/"+settings->value("lang").toString()+".qm");

    application.installTranslator(&translator);

    QnplMainWindow window(settings);
    window.setWindowTitle("Ginga");
    window.setWindowIcon(QIcon(":icon/gingagui"));
    window.setFixedWidth(256);
    window.setFixedHeight(172);
    window.show();

    if (argc > 1){
        window.performOpen(QString(argv[1]));
    }

    return application.exec();
}
