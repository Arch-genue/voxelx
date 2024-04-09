#include "settings.h"
#include <QSettings>
#include <QMetaEnum>
#include <QRegExp>
#include <QRegularExpression>
#include <QFile>

QStringList Settings::_sections;
QStringList Settings::_keys;

std::vector<std::vector<QVariant>> Settings::_settings;

Settings::Settings() {
    const QMetaObject &mo = staticMetaObject;
    int idx = mo.indexOfEnumerator("Key");
    keys = mo.enumerator(idx);

    idx = mo.indexOfEnumerator("Section");
    sections = mo.enumerator(idx);
}

QVariant Settings::get(Key k, Section s) {
    Settings &self = instance();
    QString key = self.keyPath(s, k);
    return self.conf.value(key, self.defaults[key]);
}

void Settings::setSections(QStringList sections) {
    _sections = sections;
}
void Settings::setKeys(QStringList keys) {
    _keys = keys;
}

QString Settings::getString(Key k, Section s) {
    return get(k, s).toString();
}
QStringList Settings::getSplitString(Key k, Section s) {
    return get(k, s).toString().split(" ");
}
bool Settings::getBool(Key k, Section s) {
    return get(k, s).toBool();
}
int Settings::getInt(Key k, Section s) {
    return get(k, s).toInt();
}
float Settings::getFloat(Key k, Section s) {
    return get(k, s).toFloat();
}

Settings::ValueRef Settings::set(Key k, Section s){
    Settings &self = instance();
    return ValueRef(self, self.keyPath(s, k));
}

void Settings::loadConfig(const QString &filename) {
    QFile cfgDefaults(filename);
    cfgDefaults.open(QIODevice::ReadOnly);
    QString str = cfgDefaults.readAll();

    _settings.resize(_sections.size());
    for (int i = 0; i < _sections.size(); i++) {
        _settings[i].resize(_keys.size());
    }

    Settings &self = instance();
    //section/key : value
    //section - optional
    QRegExp rxRecord("^\\s*(((\\w+)/)?(\\w+))\\s*:\\s*([^\\s].{0,})\\b\\s*$");

    auto kvs = str.split(QRegularExpression(";\\W*"), Qt::SkipEmptyParts); //key-values
    for(auto kv : kvs){
        if(rxRecord.indexIn(kv) != -1){
            QString section = rxRecord.cap(3);
            QString key = rxRecord.cap(4);
            QString value = rxRecord.cap(5);

            int iKey = self.keys.keyToValue(key.toLocal8Bit().data());
            if(iKey != -1){
                int iSection = self.sections.keyToValue(section.toLocal8Bit().data());
                if(section.isEmpty() || iSection != -1){
                    self.defaults[rxRecord.cap(1)] = value;
                }
            }
        }
    }
}

//Settings::ValueRef-----------------------------------------------------------
Settings::ValueRef & Settings::ValueRef::operator = (const QVariant &data) {
    parent.conf.setValue(keyPath, data);
    return *this;
}


//PRIVATE METHODS--------------------------------------------------------------
QString Settings::keyPath(Section s, Key k) {
    auto szSection = sections.valueToKey(s);
    auto szKey = keys.valueToKey(k);
    return QString(s == General ? "%1" : "%2/%1").arg(szKey).arg(szSection);
}

Settings & Settings::instance() {
    static Settings singleton;
    return singleton;
}