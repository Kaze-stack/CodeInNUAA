#include "KA_Window.h"

KA_Window::KA_Window()
{
    _window = nullptr;
}

KA_Window::~KA_Window()
{
}

void KA_Window::_manage()
{
    _shader.createShader("../Shader/vertexShader.glsl", "../Shader/fragmentShader.glsl");

    _textUnit.setFont("../Resources/ttftest.ttc", 32);
    _textUnit.setLength(_windowRect.w - 20);

    _testBGI.setSize(_windowRect.w, _windowRect.h);
    _testBGI.setShader(_shader.getShaderID());
    _testBGI.setTextureUnit(&_textureUnit);
    _textureUnit.loadTexture("../Resources/bgi");
    _testBGI.setTexture(_textureUnit.getTexID());

    _testChar.setRect(600, -20, 200, 500);
    _testChar.setShader(_shader.getShaderID());
    _testChar.setTextureUnit(&_textureUnit);
    _textureUnit.loadTexture("../Resources/char");
    _testChar.setTexture(_textureUnit.getTexID());

    _testDiaBox.setSize(_windowRect.w, 236);
    _testDiaBox.setShader(_shader.getShaderID());
    _testDiaBox.setTextureUnit(&_textureUnit);
    _textureUnit.loadTexture("../Resources/diabox");
    _testDiaBox.setTexture(_textureUnit.getTexID());
    _textUnit.loadWords(L"这里是对话框里的一行字。");
    _testDiaBox.setText(_textUnit.getTexID());

    _testButton.setRect(1000, 20, 36, 22);
    _testButton.setShader(_shader.getShaderID());
    _textUnit.setColor(195, 25, 200, 240);
    _textUnit.loadWords(L"跳过");
    _testButton.setText(_textUnit.getTexID());
}

void KA_Window::_display()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);

    _shader.use();

    _testBGI.show();
    _testChar.show();
    _testDiaBox.show();
    _testButton.show();

    _shader.disuse();
}

void KA_Window::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        cout << "SDL Init Fail." << endl;
        exit(1);
    }
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG |
                 IMG_INIT_TIF | IMG_INIT_WEBP) == -1)
    {
        cout << "SDL_image Init Fail." << endl;
        SDL_Quit();
        exit(1);
    }
    if (TTF_Init() == -1)
    {
        cout << "SDL_ttf Init Fail." << endl;
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 36);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void KA_Window::createWindow(string title, int x, int y, int width, int height)
{
    _window = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
    if (_window == nullptr)
    {
        cout << "Window Create Fail." << endl;
        cout << "Error Info :" << SDL_GetError() << endl;
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }
    _windowRect.w = width;
    _windowRect.h = height;

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    _glcontext = SDL_GL_CreateContext(_window);
    SDL_GL_MakeCurrent(_window, _glcontext);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cout << "GLEW Init Fail." << endl;
        SDL_GL_DeleteContext(_glcontext);
        SDL_DestroyWindow(_window);
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }

    float rectangle[18] = {
            -1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            -1.0, -1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, -1.0, 0.0,
            -1.0, -1.0, 0.0
    };
    float texCoord[12] = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0
    };

    glGenVertexArrays(1, &_vaoID);
    glBindVertexArray(_vaoID);

    glGenBuffers(2, _vboID);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

    _manage();
}

void KA_Window::destroyWindow()
{
    _testButton.destroy();
    _testDiaBox.destroy();
    _testChar.destroy();
    _testBGI.destroy();

    _textureUnit.destroy();

    SDL_GL_DeleteContext(_glcontext);
    SDL_DestroyWindow(_window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void KA_Window::renderLoop()
{
    bool quit = false;
    while (!quit)
    {
        _display();
        SDL_GL_SwapWindow(_window);

        while (SDL_PollEvent(&_event))
        {
            if (_event.type == SDL_MOUSEMOTION)
            {
                _testButton.isHover(_event.motion.x, _windowRect.h - _event.motion.y);
            }
            if (_event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }
    }
}
