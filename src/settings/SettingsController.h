#ifndef SETTINGS_CONTROLLER_H
#define SETTINGS_CONTROLLER_H

#include "util/Observable.h"
#include "SettingsListener.h"

class SettingsController: public Observable<SettingsListener> {
public:
    SettingsController() = default;

    void save(const Settings& settings);
    void load();
private:
    void notifyListeners(const Settings& settings);
};

#endif