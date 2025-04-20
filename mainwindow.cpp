#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

MainWindow::MainWindow(const QList<TestButtonParams>& buttons, shared_ptr<ModbusGateway> mgw, QWidget *parent)
    : QMainWindow(parent), modbusgw(mgw)
{
    setupUI(buttons);
    setWindowTitle("Test Buttons Control Panel");
    resize(400, 600);
}

void MainWindow::setupUI(const QList<TestButtonParams>& buttons)
{
    // Основной контейнер с прокруткой
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Центральный виджет для кнопок
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Добавляем TestButton для каждой конфигурации
    for (const TestButtonParams& params : buttons) {
        TestButton *buttonWidget = new TestButton(params, centralWidget);
        mainLayout->addWidget(buttonWidget);
    }

    // Растягиваем последний элемент
    mainLayout->addStretch();

    // Настройка скролла
    scrollArea->setWidget(centralWidget);
    setCentralWidget(scrollArea);
}
