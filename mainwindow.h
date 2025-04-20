#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include "testbutton.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QList<TestButtonParams>& buttons, QWidget *parent = nullptr);

private:
    void setupUI(const QList<TestButtonParams>& buttons);
};

#endif // MAINWINDOW_H
