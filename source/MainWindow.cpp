// MainWindow.cpp
#include "MainWindow.hpp"
#include "MatrixCalculatorWidget.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(new MatrixCalculatorWidget, "Матричный калькулятор");
    setCentralWidget(tabWidget);
    resize(900, 600);
}