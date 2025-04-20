#include <QApplication>
#include <QCommandLineParser>
#include "mainwindow.h"
#include "iniparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("TestButtonApp");
    QCoreApplication::setApplicationVersion("1.0");

    // Парсинг аргументов
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("config", "Path to buttons config file (.ini)");
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty()) {
        qCritical("Error: Config file not specified");
        parser.showHelp(1);
    }

    // Загрузка конфигурации
    QList<TestButtonParams> buttons = IniParser::parseButtonsConfig(args.first());
    if (buttons.isEmpty()) {
        qCritical("No valid buttons configuration loaded");
        return 1;
    }

    // Создание и отображение главного окна
    MainWindow w(buttons);
    w.show();

    return a.exec();
}
