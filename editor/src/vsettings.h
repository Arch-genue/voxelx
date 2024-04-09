/**
 * @file vsettings.h
 * @author Vlad Kartsaev
 * @brief 
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <QVariant>
#include <QSettings>
#include <QMetaEnum>
#include <map>
#include <QStringList>

class VSettings {
private:
    QString _filename;
    QStringList _sections;
    QStringList _keys;
public:
    VSettings(const QString &filename);
    ~VSettings();

    void setSections(QStringList sections);
    void setKeys(QStringList keys);

    void save();
};