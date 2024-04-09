#include "vsettings.h"

VSettings::VSettings(const QString &filename) {
    _filename = filename;
}

VSettings::~VSettings() {}

void VSettings::setSections(QStringList sections) {
    _sections = sections;
}

void VSettings::setKeys(QStringList keys) {
    _keys = keys;
}

#include <QRegularExpression>

void VSettings::save() {
    QString input = "splint Layout/HorSplitSize: 200 800;";
    QRegularExpression re("(\\w+)\\s(\\w+)/(\\w+):\\s(\\d+\\s\\d+);");

    QRegularExpressionMatch match = re.match(input);
    if (match.hasMatch()) {
        QString type = match.captured(1);
        QString section = match.captured(2);
        QString key = match.captured(3);
        QString values = match.captured(4);
        QStringList valuesList = values.split(" ");
        int value1 = valuesList[0].toInt();
        int value2 = valuesList[1].toInt();

        qDebug() << "Type:" << type;
        qDebug() << "Section:" << section;
        qDebug() << "Key:" << key;
        qDebug() << "Values:" << values;
        qDebug() << "Value 1:" << value1;
        qDebug() << "Value 2:" << value2;
    } else {
        qDebug() << "String does not match the pattern.";
    }
}
