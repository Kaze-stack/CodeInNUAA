#include "KA_Timer.h"

KA_Timer::KA_Timer()
{
}

KA_Timer::~KA_Timer()
{
}

void KA_Timer::timing()
{
    _now = SDL_GetTicks();
    while (_now - _start < _interval)
    {
        _now = SDL_GetTicks();
    }
}
