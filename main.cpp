#include "mainwindow.h"
#include "testbutton.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Создаём структуру с параметрами
    TestButtonParams params;
    params.buttonName = "Кнопка 1";
    params.countdownEnabled = true;
    params.countdownInterval = 15;  // 15 секунд

    TestButton widget(params);
    widget.setWindowTitle("Демо TestButton с параметрами");
    widget.resize(500, 100);
    widget.show();

    return a.exec();
}
