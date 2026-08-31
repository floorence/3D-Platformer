#ifndef CYCLE_BUTTON_H
#define CYCLE_BUTTON_H

#include "Button.h"
#include "IntGuiElement.h"

class CycleButton: public IntGuiElement {
public:
    CycleButton();

    bool dispatchMouseEvent(float x, float y, MouseEvent event) override;
    int getData() override;
    void setData(int data) override;
    void setOptions(std::vector<std::string> options);

    void setBackgroundColor(glm::vec3 color);
    void draw() override;
private:
    int option = 0;
    std::vector<std::string> options;
    Button button;

    void onBoundsChanged() override;
    float getUnboundWidth(float h) override;
    float getUnboundHeight(float w) override;
};

#endif
