#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.show();
    w.setWindowTitle("Image Labeling Application");
    return a.exec();
}
