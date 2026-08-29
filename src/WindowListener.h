#pragma once

class WindowListener {
public:
    virtual ~WindowListener() = default;
    virtual void onWindowSizeChanged(int newWidth, int newHeight) = 0;
};