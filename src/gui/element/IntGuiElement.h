#pragma once

#include "gui/element/GuiElement.h"

/**
 * Gui element whose data can be represented by or transformed to an integer
 */
class IntGuiElement: public GuiElement {
public:
    virtual ~IntGuiElement() = default;

    virtual void setData(int data) = 0;
    virtual int getData() = 0;
};
