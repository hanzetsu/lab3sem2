#include "MainWindow.hpp"
#include "MatrixCalculatorWidget.hpp"
#include "TrajectoryWidget.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(new MatrixCalculatorWidget, "Матричный калькулятор");
    tabWidget->addTab(new TrajectoryWidget, "Расчёт траектории");
    setCentralWidget(tabWidget);
    resize(900, 600);
}