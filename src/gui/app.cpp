#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication qapp(argc, argv);
    modbit::MainWindow mwin;

    mwin.show();
    return qapp.exec(); // Start main event loop.
}
