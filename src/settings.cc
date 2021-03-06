#include <QString>
#include <QSettings>
#include "settings.hh"

using std::string;

string Settings::getTokenOrEmpty() {
    QSettings settings;
    QString token = settings.value("token", "").toString();
    return token.toStdString();
}

void Settings::setToken(std::string newToken) {
    QSettings settings;
    settings.setValue("token", QString::fromStdString(newToken));
    settings.sync();
}
