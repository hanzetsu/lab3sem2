#include "TrajectoryWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QStringList>
#include "exceptions.hpp"
#include "Constants.hpp"

TrajectoryWidget::TrajectoryWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    connect(targetMinEdit, &QLineEdit::textChanged, this, &TrajectoryWidget::checkInputs);
    connect(targetMaxEdit, &QLineEdit::textChanged, this, &TrajectoryWidget::checkInputs);
    connect(v0ListEdit, &QLineEdit::textChanged, this, &TrajectoryWidget::checkInputs);
    checkInputs();
}

void TrajectoryWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGroupBox *inputGroup = new QGroupBox("Параметры цели и дискретные скорости");
    QGridLayout *grid = new QGridLayout;

    targetMinEdit = new QLineEdit;
    targetMinEdit->setPlaceholderText("52");
    targetMaxEdit = new QLineEdit;
    targetMaxEdit->setPlaceholderText("67");
    v0ListEdit = new QLineEdit;
    v0ListEdit->setPlaceholderText("20,25,30,35");
    discreteBtn = new QPushButton("Подобрать скорость и угол");
    resultLabel = new QLabel("Результат: ");
    resultTable = new QTableWidget;
    resultTable->setColumnCount(3);
    resultTable->setHorizontalHeaderLabels({"v0 (м/с)", "Угол (град)", "Дальность (м)"});

    grid->addWidget(new QLabel("Цель Xmin (м):"), 0, 0);
    grid->addWidget(targetMinEdit, 0, 1);
    grid->addWidget(new QLabel("Цель Xmax (м):"), 1, 0);
    grid->addWidget(targetMaxEdit, 1, 1);
    grid->addWidget(new QLabel("Список скоростей (через запятую):"), 2, 0);
    grid->addWidget(v0ListEdit, 2, 1);
    inputGroup->setLayout(grid);
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(discreteBtn);
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(resultTable);

    canvas = new TrajectoryCanvas(this);
    canvas->setMinimumHeight(300);
    mainLayout->addWidget(canvas, 1);

    connect(discreteBtn, &QPushButton::clicked, this, &TrajectoryWidget::onDiscreteClick);
}

void TrajectoryWidget::checkInputs() {
    bool ok = true;
    if (targetMinEdit->text().isEmpty() || targetMaxEdit->text().isEmpty()) {
        ok = false;
    } else {
        double minX = targetMinEdit->text().toDouble();
        double maxX = targetMaxEdit->text().toDouble();
        if (minX <= 0 || maxX <= 0) ok = false;
    }
    if (v0ListEdit->text().isEmpty()) {
        ok = false;
    } else {
        QStringList parts = v0ListEdit->text().split(',', Qt::SkipEmptyParts);
        if (parts.isEmpty()) {
            ok = false;
        } else {
            for (const QString& p : parts) {
                bool conv;
                p.toDouble(&conv);
                if (!conv) { ok = false; break; }
            }
        }
    }
    discreteBtn->setEnabled(ok);
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
    if (result.GetLength() == 0) throw InvalidArgument("Список скоростей пуст");
    return result;
}

void TrajectoryWidget::onDiscreteClick() {
    if (!discreteBtn->isEnabled()) return;
    try {
        double minX = targetMinEdit->text().toDouble();
        double maxX = targetMaxEdit->text().toDouble();
        if (minX > maxX) std::swap(minX, maxX);
        if (minX < 0) throw InvalidArgument("Расстояние не может быть отрицательным");

        MutableArraySequence<double> v0List = parseV0List(v0ListEdit->text());

        double bestV0, bestAngle, bestRange;
        if (calc.findV0AndAngle(v0List, minX, maxX, 1e-6, bestV0, bestAngle, bestRange)) {
            double angleDeg = bestAngle * 180.0 / Constants::PI;
            resultLabel->setText(QString("Найдено: v0 = %1 м/с, угол = %2°, дальность = %3 м")
                                 .arg(bestV0).arg(angleDeg, 0, 'f', 2).arg(bestRange, 0, 'f', 2));
            resultTable->setRowCount(1);
            resultTable->setItem(0, 0, new QTableWidgetItem(QString::number(bestV0)));
            resultTable->setItem(0, 1, new QTableWidgetItem(QString::number(angleDeg, 'f', 2)));
            resultTable->setItem(0, 2, new QTableWidgetItem(QString::number(bestRange, 'f', 2)));
            canvas->setTrajectory(bestV0, bestAngle, minX, maxX);
            canvas->startAnimation();
        } else {
            QMessageBox::warning(this, "Не найдено", "Ни одна из скоростей не позволяет попасть в цель");
            resultLabel->setText("Не найдено");
            resultTable->setRowCount(0);
            canvas->setTrajectory(0, 0, 0, 0);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}