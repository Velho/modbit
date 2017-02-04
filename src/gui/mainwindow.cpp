#include "mainwindow.h"
#include "ui_MainWindow.h"

#include <QTimer>

namespace modbit {

    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        m_window(std::unique_ptr<Ui::MainWindow>(new Ui::MainWindow)),
        m_status { false }
{
    m_window->setupUi(this);

    QObject::connect(
        m_window->actionStart_Trending, &QAction::triggered,
        this, &MainWindow::OnStartClicked);

    QObject::connect(
        m_window->actionStop_Trending, &QAction::triggered,
        this, &MainWindow::OnStopClicked);

    CreateChart();

    m_chartview = new QChartView(m_chart);
    this->setCentralWidget(m_chartview);

    this->m_window->actionStart_Trending->setEnabled(true);
    this->m_window->actionStop_Trending->setEnabled(false);

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, this, &MainWindow::UpdateChart);
    m_timer->start(5000); // 5 seconds
}

MainWindow::~MainWindow()
{
    delete m_timer;
    delete m_chart;
    delete m_chartview; // Deleted by m_chart ??
}

void MainWindow::OnStartClicked(bool triggered)
{
    if(m_status == false) {
        m_status = true;
        parser.Connect();
        this->m_window->actionStart_Trending->setEnabled(false);
        this->m_window->actionStop_Trending->setEnabled(true);
    }
}

void MainWindow::OnStopClicked(bool triggered)
{
    if (m_status == true) {
        m_status = false;
        parser.Disconnect();
        this->m_window->actionStart_Trending->setEnabled(true);
        this->m_window->actionStop_Trending->setEnabled(false);
    }
}

/*
 * DataTable is QPair of QPointF & QString.
*/

static int random_data { 0 };

void MainWindow::CreateChart()
{
    m_chart = new QChart;
    m_series = new QScatterSeries;

    m_chart->setTitle("ModbusRTU");
    m_series->setName("X;Regs/Y;Values");

    m_series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_series->setMarkerSize(10);
    
    // Append test data.
    m_series->append(0, 0);

    m_chart->addSeries(m_series); // Add to the chart for rendering.

    m_chart->createDefaultAxes();
    m_chart->axisX()->setRange(0, 100); // 100 regs.
    m_chart->axisY()->setRange(0, UINT16_MAX);

    m_chart->setDropShadowEnabled(false);
}

void MainWindow::UpdateChart()
{
    auto modbus = parser.GetData();
    
    //Q_ASSERT(modbus == nullptr);

    if (modbus == nullptr)
        return;

    if (modbus->GetData().size() == 0)
        return;

    for (auto x = 0; x < modbus->GetData().size(); x++)
        m_series->append(modbus->GetData()[x]->reg, modbus->GetData()[x]->val);

    //m_series->append(modbus->GetData()[x]->val, modbus->GetData()[x]->reg);
    // m_series->append(modbus->GetData()[x]->reg, modbus->GetData()[x]->val);

    /*
    if(parser.GetData()->GetData().size() != 0)
        *m_series << QPointF(parser.GetData()->GetData()[parser.GetIdx()]->reg, parser.GetData()->GetData()[parser.GetIdx()]->val);
    */
}

void MainWindow::closeEvent()
{

}

}
