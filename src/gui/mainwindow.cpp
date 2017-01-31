#include "mainwindow.h"
#include "ui_MainWindow.h"

namespace modbit {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_window(std::unique_ptr<Ui::MainWindow>(new Ui::MainWindow))
{
    m_window->setupUi(this);

    QObject::connect(m_window->pushButton, &QPushButton::clicked,
        this, &MainWindow::on_PushButton_Clicked);
}

MainWindow::~MainWindow()
{}

void MainWindow::on_PushButton_Clicked()
{
    parser.Connect();
}

}
