#ifndef TEXT_H
#define TEXT_H

#include "gui/Rect.h"
#include "mesh/Mesh.h"

class Text: public Rect {
public:
    glm::vec3 textColor = glm::vec3(1.0f, 1.0f, 1.0f);

    Text();
    Text(std::string text);

    void setText(std::string text);
    void setFontSize(int fontSize);
    void setTextAndSize(std::string text, int fontSize);
    void setCenterText(bool centerText);

    void setPosition(float x, float y);
    void center(float startX, float endX, float startY, float endY);

    void draw(Shader& shader);
private:
    std::string text;
    int fontSize;
    bool centerText = true; // whether or not the actual text will be in the center of the Rect. 
                            // will be drawn in the top left corner of the Rect if not centered

    Mesh mesh;

    void onBoundsChanged() override;
};

#endif