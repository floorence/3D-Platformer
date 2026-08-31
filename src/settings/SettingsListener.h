#ifndef SETTINGS_LISTENER_H
#define SETTINGS_LISTENER_H

#include "Settings.h"

class SettingsListener {
public:
    virtual ~SettingsListener() = default;
    virtual void onSettingsChanged(const Settings& settings) = 0;
};

#endif