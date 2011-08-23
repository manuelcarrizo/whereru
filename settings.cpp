#include "settings.h"

static const char * ORGANIZATION_NAME = "whereru";
static const char * APPLICATION_NAME = "whereru";

static const char * KEY_GEOMETRY = "geometry";
static const char * KEY_UPDATE_INTERVAL = "update_interval";
static const char * KEY_MATCH_ALL = "match_all";

static const int DEFAULT_UPDATE_INTERVAL = 1800;
static const bool DEFAULT_MATCH_ALL = true;

QSettings Settings::qsettings(ORGANIZATION_NAME, APPLICATION_NAME);

QRect Settings::geometry()
{
    return qsettings.value(KEY_GEOMETRY).toRect();
}

void Settings::geometry(const QRect & rect)
{
    qsettings.setValue(KEY_GEOMETRY, rect);
}


unsigned int Settings::updateInterval()
{
    return qsettings.value(KEY_UPDATE_INTERVAL, DEFAULT_UPDATE_INTERVAL).toInt();
}

void Settings::updateInterval(const unsigned int seconds)
{
    qsettings.setValue(KEY_UPDATE_INTERVAL, seconds);
}


bool Settings::matchAllWords()
{
    return qsettings.value(KEY_MATCH_ALL, DEFAULT_MATCH_ALL).toBool();
}

void Settings::matchAllWords(bool value)
{
    qsettings.setValue(KEY_MATCH_ALL, value);
}
