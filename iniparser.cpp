#include "iniparser.h"
#include <QDebug>

QList<TestButtonParams> IniParser::parseButtonsConfig(const QString& filePath)
{
    QList<TestButtonParams> buttons;

    // Проверка существования файла
    if (!QFile::exists(filePath)) {
        qCritical() << "Config file not found:" << filePath;
        return buttons;
    }

    QSettings settings(filePath, QSettings::IniFormat);

    // Проверка на ошибки чтения
    if (settings.status() != QSettings::NoError) {
        qCritical() << "Error reading config file:" << settings.status();
        return buttons;
    }

    QStringList groups = settings.childGroups();

    for (const QString& group : groups) {
        settings.beginGroup(group);

        TestButtonParams params;
        params.buttonName = settings.value("Name", "").toString().trimmed();
        params.countdownEnabled = settings.value("CountdownEnabled", false).toBool();
        params.countdownInterval = settings.value("CountdownInterval", 10).toInt();
        params.coil_number = settings.value("CoilNumber", 10).toInt();
        fprintf(stderr, "adding %s, coil number %d\n", params.buttonName.toStdString().c_str(), params.coil_number);
        if (validateParams(params)) {
            buttons.append(params);
        }

        settings.endGroup();
    }

    return buttons;
}

bool IniParser::validateParams(TestButtonParams& params)
{
    // Проверка имени
    if (params.buttonName.isEmpty()) {
        qWarning() << "Button name cannot be empty";
        return false;
    }

    // Проверка интервала
    if (params.countdownEnabled && params.countdownInterval <= 0) {
        qWarning() << "Invalid interval for button" << params.buttonName
                   << "using default value (10). Provided:" << params.countdownInterval;
        params.countdownInterval = 10;
    }

    return true;
}
