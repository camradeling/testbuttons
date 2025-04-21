#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "testbutton.h"
#include "modbus_gateway.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QList<TestButtonParams>& buttons,
                        std::shared_ptr<ModbusGateway> mgw,
                        QWidget *parent = nullptr);
signals:
    void status_callback(MessageBuffer *packet);

public slots:
    void status_callback_slot(MessageBuffer *packet);
    void send_status_poll();
    void send_set_coil_request(int coil_num, bool value);
private slots:
    void updateConnectionStatus(bool isConnected);  // Слот для обновления статуса
    void onTimerTimeout();         // Слот для обработки таймера


private:
    void setupUI(const QList<TestButtonParams>& buttons);
    vector<TestButton *> allButtons;
    std::shared_ptr<ModbusGateway> modbusgw;
    QLabel *statusLabel;           // Метка статуса Online/Offline
    QTimer *checkTimer;            // Таймер для периодической проверки
    uint16_t coil_state = 0;
};

#endif // MAINWINDOW_H
