#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include "testbutton.h"
#include "modbus_gateway.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QList<TestButtonParams>& buttons, shared_ptr<ModbusGateway> mgw, QWidget *parent = nullptr);

private:
    void setupUI(const QList<TestButtonParams>& buttons);
    shared_ptr<ModbusGateway> modbusgw;
};

#endif // MAINWINDOW_H
