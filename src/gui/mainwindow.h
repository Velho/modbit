#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts/QChartGlobal>
#include <QtCharts/QScatterSeries>
#include <QChartView>

#include <memory>

#include "parser/modbus.h"

namespace Ui {
class MainWindow;
}

QT_CHARTS_USE_NAMESPACE

namespace modbit {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void OnStartClicked(bool);
    void OnStopClicked(bool);


private:
    QTimer *m_timer;
    std::unique_ptr<Ui::MainWindow> m_window;
    QChartView *m_chartview;
	QChart *m_chart;
    QScatterSeries *m_series;
    modbit::DataParser parser;

    bool m_status;

    void CreateChart();
    void UpdateChart();

    void closeEvent();
};

}

#endif
