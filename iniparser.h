#ifndef INIPARSER_H
#define INIPARSER_H

#include <QString>
#include <QList>
#include <QSettings>
#include <QFile>

#include "testbutton.h"

class IniParser
{
public:
    static QList<TestButtonParams> parseButtonsConfig(const QString& filePath);

private:
    static bool validateParams(TestButtonParams& params);
};

#endif // INIPARSER_H
