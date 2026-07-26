#ifndef SETTINGS_H
#define SETTINGS_H

struct GraphicsSettings {
    bool bloom = true;
    bool hdr = true;
    bool vsync = true;
    bool fullscreen = false;
};

struct AudioSettings {
    float masterVolume = 1.0f;
    float musicVolume = 0.8f;
};

struct Settings {
    GraphicsSettings graphics;
    AudioSettings audio;
};

#endif