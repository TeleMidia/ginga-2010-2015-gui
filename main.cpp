#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include "util.h"

int SCREEN_HEIGHT;
int SCREEN_WIDTH;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOverrideCursor(Qt::BlankCursor);

    SCREEN_HEIGHT = QApplication::desktop()->height();
    SCREEN_WIDTH = QApplication::desktop()->width();

    MainWindow w;
    w.show();
    
    return a.exec();
}
