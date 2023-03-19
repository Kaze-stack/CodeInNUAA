#ifndef KA_ENGINE_H
#define KA_ENGINE_H
#include "KA_Window.h"

class KA_Engine
{
private:
    KA_Window _mainWindow;

public:
    KA_Engine();
    ~KA_Engine();

    int exec();
};

#endif // KA_ENGINE_H
