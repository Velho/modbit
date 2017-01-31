#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "parser/modbus.h"

namespace Ui {
class MainWindow;
}

namespace modbit {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_PushButton_Clicked();

private:
    std::unique_ptr<Ui::MainWindow> m_window;
    modbit::DataParser parser;
};

}

#endif
