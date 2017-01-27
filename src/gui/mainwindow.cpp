#include "mainwindow.h"
#include "ui_MainWindow.h"

namespace modbit {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_window(std::unique_ptr<Ui::MainWindow>(new Ui::MainWindow))
{
    m_window->setupUi(this);
}

MainWindow::~MainWindow()
{}

}
