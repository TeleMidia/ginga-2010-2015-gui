#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include "util.h"

int SCREEN_HEIGHT;
int SCREEN_WIDTH;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please inform the path of the first page.\n");
        return -1;
    }
    QString path = QString::fromAscii(argv[1]);
    QFileInfo fileInfo (path);
    if (!fileInfo.exists())
    {
        printf ("Please inform a valid file.\n");
        return -1;
    }

    QDir::setCurrent(fileInfo.path());

    QApplication a(argc, argv);
    a.setOverrideCursor(Qt::BlankCursor);

    SCREEN_HEIGHT = QApplication::desktop()->height();
    SCREEN_WIDTH = QApplication::desktop()->width();

    MainWindow w(path);
    w.show();
    
    return a.exec();
}
