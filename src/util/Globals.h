#ifndef GLOBALS_H
#define GLOBALS_H

class FontTexture;
class Shader;

namespace Globals {
    inline FontTexture* Font;
    inline Shader* GuiShader;
    inline Shader* FontShader;
    inline Shader* DefaultShader;
    inline Shader* LightShader;
    inline Shader* FlatShader;
}

#endif