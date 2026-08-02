#ifndef SETTINGS_CONTROLLER_H
#define SETTINGS_CONTROLLER_H

#include "SettingsListener.h"
#include <vector>

class SettingsController {
public:
    SettingsController() = default;

    void registerListener(SettingsListener* listener);
    void registerListeners(std::vector<SettingsListener*> listeners);

    void save(const Settings& settings);
    void load();
private:
    std::vector<SettingsListener*> listeners;

    void notifyListeners(const Settings& settings);
};

#endif