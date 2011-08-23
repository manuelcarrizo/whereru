#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QRect>

class Settings
{
public:
    static QRect geometry();
    static void geometry(const QRect & rect);

    static unsigned int updateInterval();
    static void updateInterval(const unsigned int seconds);

    static bool matchAllWords();
    static void matchAllWords(bool value);

private:
    static QSettings qsettings;
};

#endif // SETTINGS_H
