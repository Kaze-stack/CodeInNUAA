#include "KA_Engine.h"

KA_Engine::KA_Engine()
{
}

KA_Engine::~KA_Engine()
{
}

int KA_Engine::exec()
{
    _mainWindow.init();
    _mainWindow.createWindow("ver 0.2 RenderCore",
                             100, 100,
                             1280, 720
                             );
    _mainWindow.renderLoop();
    _mainWindow.destroyWindow();
    return 1;
}
