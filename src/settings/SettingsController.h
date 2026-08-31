#pragma once

#include "util/Observable.h"
#include "settings/SettingsListener.h"

class SettingsController: public Observable<SettingsListener> {
public:
    SettingsController() = default;

    void save(const Settings& settings);
    void load();
private:
    void notifyListeners(const Settings& settings);
};
