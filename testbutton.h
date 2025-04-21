#ifndef TESTBUTTON_H
#define TESTBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

struct TestButtonParams {
    QString buttonName;        // Название экземпляра
    bool countdownEnabled;     // Включен ли отсчёт
    int countdownInterval;     // Стартовый интервал (сек)
    int coil_number;
};

class TestButton : public QWidget
{
    Q_OBJECT

public:
    explicit TestButton(const TestButtonParams &params, QWidget *parent = nullptr);
    void setCountdownVisible(bool visible);
    void setStartTime(int seconds);
    void updateButtonState(bool status);
    int get_coil_number();
    QString get_name();

signals:
    void send_set_coil_request_signal(int coil_num, bool value);

private slots:
    void onButtonClicked();
    void updateCountdown();

private:


    QLabel *m_nameLabel;
    QPushButton *m_button;
    QLabel *m_countdownLabel;
    QTimer *m_timer;
    bool m_countdownEnabled;
    int m_countdownInterval;
    int m_currentTime;
    int m_coil_number;
};

#endif // TESTBUTTON_H
