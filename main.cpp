#include <QApplication>
#include <QCommandLineParser>
#include "mainwindow.h"
#include "iniparser.h"
#include "modbus_gateway.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("TestButtonApp");
    QCoreApplication::setApplicationVersion("1.0");

    // Настройка парсера аргументов
    QCommandLineParser parser;
    parser.setApplicationDescription("Test Button Application with Modbus Gateway");

    // Добавляем стандартные опции
    parser.addHelpOption();
    parser.addVersionOption();

    // Добавляем пользовательские опции
    parser.addOption({
        {"b", "buttons-config"},
        QCoreApplication::translate("main", "Path to buttons config file (.ini)"),
        "buttons.ini"
    });

    parser.addOption({
        {"c", "channels-config"},
        QCoreApplication::translate("main", "Path to channels config file (.ini)"),
        "channels.ini"
    });

    // Парсим аргументы
    parser.process(a);

    // Получаем пути к конфигурационным файлам
    QString buttonsConfig = parser.value("buttons-config");
    QString channelsConfig = parser.value("channels-config");

    // Валидация обязательного аргумента
    if (buttonsConfig.isEmpty()) {
        qCritical("Error: Buttons config file not specified");
        parser.showHelp(1);
    }

    // Загрузка конфигурации кнопок
    QList<TestButtonParams> buttons = IniParser::parseButtonsConfig(buttonsConfig);
    if (buttons.isEmpty()) {
        qCritical("No valid buttons configuration loaded");
        return 1;
    }

    fprintf(stderr, "channels configuration file = %s\n",channelsConfig.toStdString().c_str());
    bool confok = 1;
    FILE* fpconf=nullptr;
    mxml_node_t* tree = nullptr;
    if ((fpconf = fopen(channelsConfig.toStdString().c_str(), "r")) != NULL)        // открыть файл с конфигурацией в формате XML
    {
        tree = mxmlLoadFile (nullptr, fpconf, MXML_NO_CALLBACK);       // считать конфигурацию
        fclose(fpconf);
        if (tree == nullptr)
        {
            fprintf(stderr, "config file invalid\n");
            confok = 0;
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "cant open config file\n");
        confok = 0;
        return -1;
    }
    fprintf(stderr, "config file valid\n");
    shared_ptr<ModbusGateway> mgw = shared_ptr<ModbusGateway>(new ModbusGateway(tree));
    mgw->init_module();
    // Создание и отображение главного окна
    MainWindow w(buttons, mgw);
    w.show();

    return a.exec();
}
