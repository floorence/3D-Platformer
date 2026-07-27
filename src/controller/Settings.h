#ifndef SETTINGS_H
#define SETTINGS_H

struct GraphicsSettings {
    int bloomAmount = 1; // blurAmount = bloomAmount * 10
    bool vsync = true;
    bool fullscreen = false;
};
// TODO
struct AudioSettings {
    float masterVolume = 1.0f;
    float musicVolume = 0.8f;
};

struct Settings {
    GraphicsSettings graphics;
    AudioSettings audio;
};

#endif