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
};

class TestButton : public QWidget
{
    Q_OBJECT

public:
    explicit TestButton(const TestButtonParams &params, QWidget *parent = nullptr);
    void setCountdownVisible(bool visible);
    void setStartTime(int seconds);

private slots:
    void onButtonClicked();
    void updateCountdown();

private:
    void updateButtonState();

    QLabel *m_nameLabel;
    QPushButton *m_button;
    QLabel *m_countdownLabel;
    QTimer *m_timer;
    bool m_countdownEnabled;
    int m_countdownInterval;
    int m_currentTime;
};

#endif // TESTBUTTON_H
