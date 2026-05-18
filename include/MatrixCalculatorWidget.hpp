#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
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
    void onScalarTextChanged(const QString &text);

private:
    QSpinBox *rowsA, *colsA;
    QSpinBox *rowsB, *colsB;
    QTableWidget *tableA, *tableB;
    QTableWidget *resultTable;
    QLineEdit *scalarEdit;
    QPushButton *scalarMulBtn;
    QLabel *detLabel;

    void setupUI();
    void fillMatrixFromTable(Matrix<double, MutableArraySequence>& mat, QTableWidget* table);
    void displayMatrix(const Matrix<double, MutableArraySequence>& mat, QTableWidget* table);
    void showError(const QString& msg);
};