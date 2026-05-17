#include "TrajectoryWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QStringList>
#include "exceptions.hpp"

TrajectoryWidget::TrajectoryWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void TrajectoryWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *inputGroup = new QGroupBox("Параметры цели и дискретные скорости");
    QGridLayout *grid = new QGridLayout;

    grid->addWidget(new QLabel("Цель Xmin (м):"), 0, 0);
    targetMinEdit = new QLineEdit;
    targetMinEdit->setPlaceholderText("50");
    grid->addWidget(targetMinEdit, 0, 1);

    grid->addWidget(new QLabel("Цель Xmax (м):"), 1, 0);
    targetMaxEdit = new QLineEdit;
    targetMaxEdit->setPlaceholderText("52");
    grid->addWidget(targetMaxEdit, 1, 1);

    grid->addWidget(new QLabel("Список скоростей (м/с, через запятую):"), 2, 0);
    v0ListEdit = new QLineEdit;
    v0ListEdit->setPlaceholderText("20,25,30,35");
    grid->addWidget(v0ListEdit, 2, 1);

    inputGroup->setLayout(grid);
    mainLayout->addWidget(inputGroup);

    discreteBtn = new QPushButton("Подобрать скорость и угол");
    mainLayout->addWidget(discreteBtn);

    resultLabel = new QLabel("Результат: ");
    mainLayout->addWidget(resultLabel);

    resultTable = new QTableWidget;
    resultTable->setColumnCount(3);
    resultTable->setHorizontalHeaderLabels({"v0 (м/с)", "Угол (град)", "Дальность (м)"});
    mainLayout->addWidget(resultTable);

    connect(discreteBtn, &QPushButton::clicked, this, &TrajectoryWidget::onDiscreteClick);
}

MutableArraySequence<double> TrajectoryWidget::parseV0List(const QString& str) {
    MutableArraySequence<double> result;
    QStringList parts = str.split(',', Qt::SkipEmptyParts);
    for (const QString& part : parts) {
        bool ok;
        double val = part.toDouble(&ok);
        if (!ok) throw InvalidArgument("Неверное значение скорости: " + part.toStdString());
        result.Append(val);
    }
    if (result.GetLength() == 0)
        throw InvalidArgument("Список скоростей пуст");
    return result;
}

void TrajectoryWidget::onDiscreteClick() {
    try {
        double minX = targetMinEdit->text().toDouble();
        double maxX = targetMaxEdit->text().toDouble();
        if (minX > maxX) std::swap(minX, maxX);
        if (minX < 0) throw InvalidArgument("Расстояние не может быть отрицательным");

        MutableArraySequence<double> v0List = parseV0List(v0ListEdit->text());

        double bestV0, bestAngle, bestRange;
        if (calc.findV0AndAngle(v0List, minX, maxX, 1e-6, bestV0, bestAngle, bestRange)) {
            double angleDeg = bestAngle * 180.0 / M_PI;
            resultLabel->setText(QString("Найдено: v0 = %1 м/с, угол = %2°, дальность = %3 м")
                                 .arg(bestV0).arg(angleDeg, 0, 'f', 2).arg(bestRange, 0, 'f', 2));
            resultTable->setRowCount(1);
            resultTable->setItem(0, 0, new QTableWidgetItem(QString::number(bestV0)));
            resultTable->setItem(0, 1, new QTableWidgetItem(QString::number(angleDeg, 'f', 2)));
            resultTable->setItem(0, 2, new QTableWidgetItem(QString::number(bestRange, 'f', 2)));
        } else {
            QMessageBox::warning(this, "Не найдено", "Ни одна из скоростей не позволяет попасть в цель");
            resultLabel->setText("Не найдено");
            resultTable->setRowCount(0);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}