#include "MatrixCalculatorWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <cstdlib>
#include <ctime>
#include <QLineEdit>

MatrixCalculatorWidget::MatrixCalculatorWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void MatrixCalculatorWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *sizeGroupA = new QGroupBox("Размеры матрицы A");
    QHBoxLayout *sizeLayoutA = new QHBoxLayout;
    rowsA = new QSpinBox;
    rowsA->setRange(1, 6);
    colsA = new QSpinBox;
    colsA->setRange(1, 6);
    sizeLayoutA->addWidget(new QLabel("Строки:"));
    sizeLayoutA->addWidget(rowsA);
    sizeLayoutA->addWidget(new QLabel("Столбцы:"));
    sizeLayoutA->addWidget(colsA);
    sizeGroupA->setLayout(sizeLayoutA);

    QGroupBox *sizeGroupB = new QGroupBox("Размеры матрицы B");
    QHBoxLayout *sizeLayoutB = new QHBoxLayout;
    rowsB = new QSpinBox;
    rowsB->setRange(1, 6);
    colsB = new QSpinBox;
    colsB->setRange(1, 6);
    sizeLayoutB->addWidget(new QLabel("Строки:"));
    sizeLayoutB->addWidget(rowsB);
    sizeLayoutB->addWidget(new QLabel("Столбцы:"));
    sizeLayoutB->addWidget(colsB);
    sizeGroupB->setLayout(sizeLayoutB);

    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget(sizeGroupA);
    sizeLayout->addWidget(sizeGroupB);
    mainLayout->addLayout(sizeLayout);

    QHBoxLayout *tablesLayout = new QHBoxLayout;
    tableA = new QTableWidget;
    tableB = new QTableWidget;
    tablesLayout->addWidget(tableA);
    tablesLayout->addWidget(tableB);
    mainLayout->addLayout(tablesLayout);

    QGridLayout *buttonLayout = new QGridLayout;
    QPushButton *addBtn = new QPushButton("Сложение (A+B)");
    QPushButton *subBtn = new QPushButton("Вычитание (A-B)");
    QPushButton *mulBtn = new QPushButton("Умножение (A*B)");
    scalarMulBtn = new QPushButton("Умножить A на скаляр");
    QPushButton *transposeBtn = new QPushButton("Транспонировать A");
    QPushButton *detBtn = new QPushButton("Определитель A");
    QPushButton *invBtn = new QPushButton("Обратная A");
    QPushButton *randomFillBtn = new QPushButton("Случайно заполнить A и B");

    scalarEdit = new QLineEdit;
    scalarEdit->setPlaceholderText("Скаляр");
    detLabel = new QLabel("Определитель: ");

    buttonLayout->addWidget(addBtn, 0, 0);
    buttonLayout->addWidget(subBtn, 0, 1);
    buttonLayout->addWidget(mulBtn, 0, 2);
    buttonLayout->addWidget(scalarMulBtn, 1, 0);
    buttonLayout->addWidget(transposeBtn, 1, 1);
    buttonLayout->addWidget(detBtn, 1, 2);
    buttonLayout->addWidget(invBtn, 2, 0);
    buttonLayout->addWidget(randomFillBtn, 2, 1);
    buttonLayout->addWidget(scalarEdit, 2, 2);
    buttonLayout->addWidget(detLabel, 3, 0, 1, 3);

    mainLayout->addLayout(buttonLayout);

    QGroupBox *resultGroup = new QGroupBox("Результат");
    QVBoxLayout *resultLayout = new QVBoxLayout;
    resultTable = new QTableWidget;
    resultLayout->addWidget(resultTable);
    resultGroup->setLayout(resultLayout);
    mainLayout->addWidget(resultGroup);

    connect(rowsA, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixCalculatorWidget::onMatrixSizeChanged);
    connect(colsA, &QSpinBox::valueChanged, this, &MatrixCalculatorWidget::onMatrixSizeChanged);
    connect(rowsB, &QSpinBox::valueChanged, this, &MatrixCalculatorWidget::onMatrixSizeChanged);
    connect(colsB, &QSpinBox::valueChanged, this, &MatrixCalculatorWidget::onMatrixSizeChanged);

    connect(addBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onAddClicked);
    connect(subBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onSubtractClicked);
    connect(mulBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onMultiplyClicked);
    connect(scalarMulBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onScalarMultiplyClicked);
    connect(transposeBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onTransposeClicked);
    connect(detBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onDeterminantClicked);
    connect(invBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onInverseClicked);
    connect(randomFillBtn, &QPushButton::clicked, this, &MatrixCalculatorWidget::onRandomFillClicked);

    connect(scalarEdit, &QLineEdit::textChanged, this, &MatrixCalculatorWidget::onScalarTextChanged);

    onMatrixSizeChanged();
    onScalarTextChanged(scalarEdit->text()); // начальное состояние (пусто – кнопка отключена)
}

void MatrixCalculatorWidget::onMatrixSizeChanged()
{
    int ra = rowsA->value(), ca = colsA->value();
    int rb = rowsB->value(), cb = colsB->value();

    tableA->setRowCount(ra);
    tableA->setColumnCount(ca);
    tableB->setRowCount(rb);
    tableB->setColumnCount(cb);

    for (int i = 0; i < ra; ++i)
        for (int j = 0; j < ca; ++j)
            if (tableA->item(i, j) == nullptr)
                tableA->setItem(i, j, new QTableWidgetItem("0"));
    for (int i = 0; i < rb; ++i)
        for (int j = 0; j < cb; ++j)
            if (tableB->item(i, j) == nullptr)
                tableB->setItem(i, j, new QTableWidgetItem("0"));
}

void MatrixCalculatorWidget::fillMatrixFromTable(Matrix<double, MutableArraySequence> &mat, QTableWidget *table)
{
    int rows = table->rowCount();
    int cols = table->columnCount();
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            QTableWidgetItem *item = table->item(i, j);
            if (item)
            {
                bool ok;
                double val = item->text().toDouble(&ok);
                if (!ok)
                    val = 0.0;
                mat.Set(i, j, val);
            }
            else
            {
                mat.Set(i, j, 0.0);
            }
        }
    }
}

void MatrixCalculatorWidget::displayMatrix(const Matrix<double, MutableArraySequence> &mat, QTableWidget *table)
{
    size_t rows = mat.getRows();
    size_t cols = mat.getCols();
    table->setRowCount(static_cast<int>(rows));
    table->setColumnCount(static_cast<int>(cols));
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            double val = mat.Get(i, j);
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(val));
            table->setItem(static_cast<int>(i), static_cast<int>(j), item);
        }
    }
}

void MatrixCalculatorWidget::showError(const QString &msg)
{
    QMessageBox::critical(this, "Ошибка", msg);
}

void MatrixCalculatorWidget::onScalarTextChanged(const QString &text)
{
    if (text.isEmpty())
    {
        scalarMulBtn->setEnabled(false);
        return;
    }
    bool ok;
    text.toDouble(&ok);
    scalarMulBtn->setEnabled(ok);
}

void MatrixCalculatorWidget::onAddClicked()
{
    try
    {
        int ra = rowsA->value(), ca = colsA->value();
        int rb = rowsB->value(), cb = colsB->value();
        if (ra != rb || ca != cb)
        {
            showError("Размеры матриц не совпадают для сложения");
            return;
        }
        Matrix<double, MutableArraySequence> A(ra, ca), B(rb, cb);
        fillMatrixFromTable(A, tableA);
        fillMatrixFromTable(B, tableB);
        Matrix<double, MutableArraySequence> C = A + B;
        displayMatrix(C, resultTable);
        detLabel->setText("Определитель: ");
    }
    catch (const std::exception &e)
    {
        showError(e.what());
    }
}

void MatrixCalculatorWidget::onSubtractClicked()
{
    try
    {
        int ra = rowsA->value(), ca = colsA->value();
        int rb = rowsB->value(), cb = colsB->value();
        if (ra != rb || ca != cb)
        {
            showError("Размеры матриц не совпадают для вычитания");
            return;
        }
        Matrix<double, MutableArraySequence> A(ra, ca), B(rb, cb);
        fillMatrixFromTable(A, tableA);
        fillMatrixFromTable(B, tableB);
        Matrix<double, MutableArraySequence> C = A - B;
        displayMatrix(C, resultTable);
        detLabel->setText("Определитель: ");
    }
    catch (const std::exception &e)
    {
        showError(e.what());
    }
}

void MatrixCalculatorWidget::onMultiplyClicked()
{
    try
    {
        int ra = rowsA->value(), ca = colsA->value();
        int rb = rowsB->value(), cb = colsB->value();
        if (ca != rb)
        {
            showError("Количество столбцов A должно равняться количеству строк B");
            return;
        }
        Matrix<double, MutableArraySequence> A(ra, ca), B(rb, cb);
        fillMatrixFromTable(A, tableA);
        fillMatrixFromTable(B, tableB);
        Matrix<double, MutableArraySequence> C = A * B;
        displayMatrix(C, resultTable);
        detLabel->setText("Определитель: ");
    }
    catch (const std::exception &e)
    {
        showError(e.what());
    }
}

void MatrixCalculatorWidget::onScalarMultiplyClicked()
{
    try
    {
        int ra = rowsA->value(), ca = colsA->value();
        bool ok;
        double scalar = scalarEdit->text().toDouble(&ok);
        if (!ok)
        {
            showError("Введите корректный скаляр");
            return;
        }
        Matrix<double, MutableArraySequence> A(ra, ca);
        fillMatrixFromTable(A, tableA);
        Matrix<double, MutableArraySequence> C = A * scalar;
        displayMatrix(C, resultTable);
        detLabel->setText("Определитель: ");
    }
    catch (const std::exception &e)
    {
        showError(e.what());
    }
}

void MatrixCalculatorWidget::onTransposeClicked()
{
    try
    {
        int ra = rowsA->value(), ca = colsA->value();
        Matrix<double, MutableArraySequence> A(ra, ca);
        fillMatrixFromTable(A, tableA);
        Matrix<double, MutableArraySequence> C = A.transpose();
        displayMatrix(C, resultTable);
        detLabel->setText("Определитель: ");
    }
    catch (const std::exception &e)
    {
        showError(e.what());
    }
}

void MatrixCalculatorWidget::onDeterminantClicked()
{
    try
    {
        int ra = rowsA->value(), ca = colsA->value();
        if (ra != ca)
        {
            showError("Определитель вычисляется только для квадратной матрицы");
            return;
        }
        Matrix<double, MutableArraySequence> A(ra, ca);
        fillMatrixFromTable(A, tableA);
        double det = A.determinant();
        detLabel->setText("Определитель: " + QString::number(det));
        resultTable->clear();
        resultTable->setRowCount(0);
    }
    catch (const std::exception &e)
    {
        showError(e.what());
    }
}

void MatrixCalculatorWidget::onInverseClicked()
{
    try
    {
        int ra = rowsA->value(), ca = colsA->value();
        if (ra != ca)
        {
            showError("Обратная матрица существует только для квадратной матрицы");
            return;
        }
        Matrix<double, MutableArraySequence> A(ra, ca);
        fillMatrixFromTable(A, tableA);
        Matrix<double, MutableArraySequence> C = A.inverse();
        displayMatrix(C, resultTable);
        detLabel->setText("Определитель: ");
    }
    catch (const std::exception &e)
    {
        showError(e.what());
    }
}

void MatrixCalculatorWidget::onRandomFillClicked()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int ra = rowsA->value(), ca = colsA->value();
    int rb = rowsB->value(), cb = colsB->value();
    for (int i = 0; i < ra; ++i)
    {
        for (int j = 0; j < ca; ++j)
        {
            double val = (std::rand() % 20) - 10; // -10..9
            tableA->setItem(i, j, new QTableWidgetItem(QString::number(val)));
        }
    }
    for (int i = 0; i < rb; ++i)
    {
        for (int j = 0; j < cb; ++j)
        {
            double val = (std::rand() % 20) - 10;
            tableB->setItem(i, j, new QTableWidgetItem(QString::number(val)));
        }
    }
}