#ifndef KA_WINDOW_H
#define KA_WINDOW_H

#include "KA_Shader.h"
#include "KA_Texture.h"
#include "KA_Text.h"
#include "KA_Widgets_All.h"

class KA_Window
{
private:
    SDL_Window *_window;
    SDL_GLContext _glcontext;
    SDL_Event _event;
    SDL_Rect _windowRect;

    GLuint _vaoID;
    GLuint _vboID[2];
    float _fullRatio;

    KA_Shader _shader;
    KA_Texture _textureUnit;
    KA_Text _textUnit;

    KA_BGI _testBGI;
    KA_Character _testChar;
    KA_DialogBox _testDiaBox;
    KA_Button _testButton;

protected:
    void _manage();
    void _display();

public:
    KA_Window();
    ~KA_Window();

    void init();

    void createWindow(string title, int x, int y, int width, int height);
    void destroyWindow();

    void renderLoop();
};

#endif // KA_WINDOW_H
