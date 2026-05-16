#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include "Matrix.hpp"
#include "MutableArraySequence.hpp"

class MatrixCalculatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MatrixCalculatorWidget(QWidget *parent = nullptr);

private slots:
    void onMatrixSizeChanged();
    void onAddClicked();
    void onSubtractClicked();
    void onMultiplyClicked();
    void onScalarMultiplyClicked();
    void onTransposeClicked();
    void onDeterminantClicked();
    void onInverseClicked();
    void onRandomFillClicked();

private:
    QSpinBox *rowsA, *colsA;
    QSpinBox *rowsB, *colsB;
    QTableWidget *tableA, *tableB;
    QTableWidget *resultTable;
    QLineEdit *scalarEdit;
    QLabel *detLabel;

    void setupUI();
    void updateTablesSize();
    void fillMatrixFromTable(Matrix<double, MutableArraySequence>& mat, QTableWidget* table);
    void displayMatrix(const Matrix<double, MutableArraySequence>& mat, QTableWidget* table);
    void showError(const QString& msg);
};