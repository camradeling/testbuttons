#include "testbutton.h"
#include <QHBoxLayout>

TestButton::TestButton(const TestButtonParams &params, QWidget *parent)
    : QWidget(parent),
    m_countdownEnabled(params.countdownEnabled),
    m_countdownInterval(params.countdownInterval),
    m_currentTime(params.countdownInterval),
    m_coil_number(params.coil_number)
{
    // Инициализация виджетов
    m_nameLabel = new QLabel(params.buttonName, this);
    m_nameLabel->setStyleSheet("font-weight: bold;");

    m_button = new QPushButton("Включить", this);
    m_button->setCheckable(true);
    m_button->setStyleSheet("QPushButton { color: green; }");

    m_countdownLabel = new QLabel(QString::number(m_currentTime) + " сек.", this);
    m_countdownLabel->setVisible(false);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    // Компоновка
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_nameLabel);
    layout->addWidget(m_button);
    layout->addWidget(m_countdownLabel);
    layout->addStretch();

    // Подключение сигналов
    connect(m_button, &QPushButton::clicked, this, &TestButton::onButtonClicked);
    connect(m_timer, &QTimer::timeout, this, &TestButton::updateCountdown);
}

void TestButton::setCountdownVisible(bool visible) {
    m_countdownLabel->setVisible(visible && m_countdownEnabled);
}

void TestButton::setStartTime(int seconds) {
    m_countdownInterval = seconds;
    m_currentTime = seconds;
    m_countdownLabel->setText(QString::number(m_currentTime) + " сек.");
}

void TestButton::onButtonClicked() {
    if (m_button->isChecked()) {
        if (m_countdownEnabled) {
            m_currentTime = m_countdownInterval;
            m_timer->start();
            m_countdownLabel->setVisible(true);
        }
        emit send_set_coil_request_signal(this->m_coil_number, true);
    } else {
        if (m_countdownEnabled) {
            m_timer->stop();
            m_currentTime = m_countdownInterval;
            m_countdownLabel->setText(QString::number(m_currentTime) + " сек.");
            m_countdownLabel->setVisible(false);
        }
        emit send_set_coil_request_signal(this->m_coil_number, false);
    }
}

void TestButton::updateCountdown() {
    m_currentTime--;
    m_countdownLabel->setText(QString::number(m_currentTime) + " сек.");

    if (m_currentTime <= 0) {
        m_timer->stop();
        m_button->setChecked(false);
        m_countdownLabel->setVisible(false);
        emit send_set_coil_request_signal(this->m_coil_number, false);
    }

}

void TestButton::updateButtonState(bool status) {
    if (status) {
        m_button->setText("Выключить");
        m_button->setStyleSheet("QPushButton { color: #FF6B6B; }");
    } else {
        m_button->setText("Включить");
        m_button->setStyleSheet("QPushButton { color: green; }");
    }
}

QString TestButton::get_name()
{
    return m_nameLabel->text();
}

int TestButton::get_coil_number()
{
    return m_coil_number;
}
