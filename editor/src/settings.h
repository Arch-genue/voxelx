/**
 * @file settings.h
 * @author https://habr.com/ru/articles/149085/
 * @brief App Settings reailization
 * @version 1.0
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

/**
  @brief Синглтон для доступа к конфигурации

  Usage:
  @code
    ...
    ...
    //пердварительная настройка (должен быть где-нибуль в main)
    QApplication::setOrganizationName("Organization name");
    QApplication::setApplicationName("App name");
    ...
    ...
    //установка значений по умолчанию (строка может быть многострочной)
    Settings::setDefaults("SomeKey: value1; SomeSection/SomeKey: value2");

    //или так
    QFile f(":/defaults/config");
    f.open(QIODevice::ReadOnly);
    Settings::setDefaults(f.readAll());
    ...
    ...
    void fun(){
        ...
        QVariant val1 = Settings::get(Settings::SomeKey);
        Settings::set(Settings::SomeKey) = "new val1";
        ...
        QVariant val2 = Settings::get(Settings::SomeKey, Settings::SomeSection);
        Settings::set(Settings::SomeKey, Settings::SomeSection) = "new val2";
        ...
    }
  @endcode
*/
class Settings {
    Q_GADGET
    Q_ENUMS(Section)
    Q_ENUMS(Key)
public:
    enum Section {
        General,
        Layout,
    };

    enum Key {
        LAYWINDOWSIZE,
        HORSPLITSIZE,
        VERSPLITSIZE,
    };

    class ValueRef{
    public:
        ValueRef(Settings &st, const QString &kp) :
            parent(st), keyPath(kp) {}
        ValueRef & operator = (const QVariant &d);
    private:
        Settings &parent;
        const QString keyPath;
    };

    static void loadConfig(const QString &filename);
    static QVariant get(Key, Section /*s*/ = General);

    static void setSections(QStringList sections);
    static void setKeys(QStringList keys);

    static QString getString(Key k, Section s);
    static QStringList getSplitString(Key k, Section s);
    static bool getBool(Key k, Section s);
    static int getInt(Key k, Section s);
    static float getFloat(Key k, Section s);

    static ValueRef set(Key, Section /*s*/ = General);

private:
    static QStringList _sections;
    static QStringList _keys;

    QString keyPath(Section, Key);

    static std::vector<std::vector<QVariant>> _settings;

    static Settings & instance();
    QMetaEnum keys;
    QMetaEnum sections;
    QMap<QString, QVariant> defaults;
    QSettings conf;

    Settings();
    Settings(const Settings &);
    Settings & operator = (const Settings &);
};