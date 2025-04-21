#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QStatusBar>

MainWindow::MainWindow(const QList<TestButtonParams>& buttons,
                       std::shared_ptr<ModbusGateway> mgw,
                       QWidget *parent)
    : QMainWindow(parent), modbusgw(mgw)
{
    setupUI(buttons);
    setWindowTitle("Test Buttons Control Panel");
    resize(400, 600);

    // Инициализация таймера
    checkTimer = new QTimer(this);
    checkTimer->setInterval(3000); // 3 секунд
    connect(checkTimer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    checkTimer->start();
}

void MainWindow::setupUI(const QList<TestButtonParams>& buttons)
{
    // Создаем виджет для статусной строки
    QWidget *statusWidget = new QWidget();
    QHBoxLayout *statusLayout = new QHBoxLayout(statusWidget);

    // Метка статуса соединения
    statusLabel = new QLabel("Offline", statusWidget);
    statusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();

    // Добавляем статусную строку в нижнюю часть окна
    statusBar()->addPermanentWidget(statusWidget, 1);

    // Основной контейнер с прокруткой
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Центральный виджет для кнопок
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Создаем временный список для сортировки
    QList<TestButton*> tempButtons;

    // Сначала создаем все кнопки
    for (const TestButtonParams& params : buttons) {
        TestButton *buttonWidget = new TestButton(params, centralWidget);
        tempButtons.append(buttonWidget);
        connect(buttonWidget, &TestButton::send_set_coil_request_signal,
                this, &MainWindow::send_set_coil_request);
    }

    // Сортируем кнопки по m_coil_number
    std::sort(tempButtons.begin(), tempButtons.end(),
              [](TestButton* a, TestButton* b) {
                  return a->get_coil_number() < b->get_coil_number();
              });

    // Добавляем отсортированные кнопки в layout
    for (TestButton* button : tempButtons) {
        mainLayout->addWidget(button);
        allButtons.push_back(button); // сохраняем в основном списке
    }

    // Растягиваем последний элемент
    mainLayout->addStretch();

    // Настройка скролла
    scrollArea->setWidget(centralWidget);
    setCentralWidget(scrollArea);
    shared_ptr<BasicChannel> schan = modbusgw->CHPL->channel_by_alias("localUI");
    dynamic_cast<VirtualChannel*>(schan.get())->callback = std::bind(
        &MainWindow::status_callback,
        this,
        std::placeholders::_1
        );
    connect(this, &MainWindow::status_callback, this, &MainWindow::status_callback_slot,Qt::QueuedConnection);
}

void MainWindow::updateConnectionStatus(bool isConnected)
{
    if(isConnected) {
        statusLabel->setText("Online");
        statusLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    } else {
        statusLabel->setText("Offline");
        statusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    }
}

void MainWindow::onTimerTimeout()
{
    send_status_poll();
}

void MainWindow::send_status_poll()
{
    // periodic poll for coil status
    // hardcode for now
    ModbusPDU req = { .SlaveAddress = MB_BROADCAST_ADDR, .FunctionCode = 1, .reg = 0, .cnt = 16 };
    timespec_t now;
    if (clock_gettime(CLOCK_REALTIME, &now)) {
        // TODO: error handling
    }
    req.timestamp = now;
    shared_ptr<BasicChannel> schan = modbusgw->CHPL->channel_by_alias("localUI");
    std::unique_ptr<MessageBuffer> packet(new MessageBuffer(schan->clientfd, ModbusPacketConstructor::serialize_request(req, MODBUS_TCP_PDU_TYPE), CHAN_DATA_PACKET));
    schan->send_message_buffer(&schan->inQueue, std::move(packet), true);
}

void MainWindow::send_set_coil_request(int coil_num, bool value)
{
    // set coil
    // hardcode for now
    ModbusPDU req = { .SlaveAddress = MB_BROADCAST_ADDR, .FunctionCode = 5, .reg = (uint16_t)coil_num, .cnt = 1 };
    req.values.push_back(value);
    timespec_t now;
    if (clock_gettime(CLOCK_REALTIME, &now)) {
        // TODO: error handling
    }
    req.timestamp = now;
    shared_ptr<BasicChannel> schan = modbusgw->CHPL->channel_by_alias("localUI");
    std::unique_ptr<MessageBuffer> packet(new MessageBuffer(schan->clientfd, ModbusPacketConstructor::serialize_request(req, MODBUS_TCP_PDU_TYPE), CHAN_DATA_PACKET));
    schan->send_message_buffer(&schan->inQueue, std::move(packet), true);
}

void MainWindow::status_callback_slot(MessageBuffer *packet)
{
    fprintf(stderr, "packet received\n");
    for (int i = 0; i < packet->Length(); i++)
        fprintf(stderr, "%02X ", packet->Data()[i]);
    fprintf(stderr, "\n");
    if (packet->Length() > 6 && packet->Data()[7] == 0x05) {
        fprintf(stderr, "coil set confirm\n");
        updateConnectionStatus(true);
        send_status_poll();
    }
    else if (packet->Length() >= 11 && packet->Data()[7] == 0x01) {
        coil_state = *(uint16_t*)&packet->Data()[9];
        fprintf(stderr, "coil state %04X\n",coil_state);
        for (auto button : allButtons) {
            fprintf(stderr, "%d) shift value %d\n", button->get_coil_number(), (coil_state & (1 << button->get_coil_number())));
            button->updateButtonState((coil_state & (1 << button->get_coil_number())) ? true : false);
        }
        updateConnectionStatus(true);
    }
    else {
        fprintf(stderr, "packet invalid");
        updateConnectionStatus(false);
    }
}
