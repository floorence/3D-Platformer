#pragma once

#include <vector>

template <typename T>
class Observable {
public:
    virtual ~Observable() = default;

    void registerListener(T* listener) {
        listeners.push_back(listener);
    }

    void registerListeners(std::vector<T*> listeners) {
        this->listeners.insert(this->listeners.end(), listeners.begin(), listeners.end());
    }
protected:
    std::vector<T*> listeners;
};