#pragma once

#include "settings/Settings.h"

class SettingsListener {
public:
    virtual ~SettingsListener() = default;
    virtual void onSettingsChanged(const Settings& settings) = 0;
};
