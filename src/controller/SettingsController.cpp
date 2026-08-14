#include "SettingsController.h"
#include "json/json.hpp"
#include <fstream>
using json = nlohmann::json;

void SettingsController::registerListener(SettingsListener* listener) {
    listeners.push_back(listener);
}

void SettingsController::registerListeners(std::vector<SettingsListener*> listeners) {
    this->listeners.insert(this->listeners.end(), listeners.begin(), listeners.end());
}

void SettingsController::save(const Settings& settings) {
    json j;

    for (auto& category: settings.children) {
        for (auto& setting: category->getChildren()) {
            j[category->name][setting->name] = setting->value;
        }
    }

    notifyListeners(settings);
    std::ofstream("save/settings.json") << j.dump(4);
}

void SettingsController::load() {
    std::ifstream file("save/settings.json");

    if (file) {
        json j;
        file >> j;
        Settings settings;

        for (auto& category: settings.children) {
            for (auto& setting: category->getChildren()) {
                setting->value = j[category->name][setting->name];
            }
        }
       
        notifyListeners(settings);
    }
}

void SettingsController::notifyListeners(const Settings& settings) {
    for (auto& listener: listeners) {
        listener->onSettingsChanged(settings);
    }
}
