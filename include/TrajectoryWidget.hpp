#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include "TrajectoryCalculator.hpp"

class TrajectoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrajectoryWidget(QWidget *parent = nullptr);
private slots:
    void onCalculateClick();
    void onDiscreteClick();
private:
    QLineEdit *v0Edit;
    QLineEdit *targetMinEdit, *targetMaxEdit;
    QLineEdit *v0ListEdit;
    QPushButton *calcBtn, *discreteBtn;
    QLabel *resultLabel;
    QTableWidget *resultTable;
    ProjectileMotionCalculator calc;
    void setupUI();
    MutableArraySequence<double> parseV0List(const QString& str);
};