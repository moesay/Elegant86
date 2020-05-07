#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    /* w.show(); */
    std::string sss = "hey";
    return a.exec();
}
