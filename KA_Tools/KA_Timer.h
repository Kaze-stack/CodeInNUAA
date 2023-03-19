#ifndef KA_TIMER_H
#define KA_TIMER_H

#define GLEW_STATIC
#include <glew.h>

extern "C"{
#include <SDL.h>
}

#include <iostream>
using namespace std;

class KA_Timer
{
private:
    GLuint _start;
    GLuint _now;

    short _fps;
    double _interval;

public:
    KA_Timer();
    ~KA_Timer();

    void setInterval(double interval);
    void setFPS(short fps);

    void start();
    void timing();
};

//inline function
inline void KA_Timer::setInterval(double interval)
{
    _interval = interval;
    _fps = (int)(1000.0 / interval);
}

inline void KA_Timer::setFPS(short fps)
{
    _fps = fps;
    _interval = 1000.0 / (fps * 1.0);
}

inline void KA_Timer::start()
{
    _start = SDL_GetTicks();
}

#endif // KA_TIMER_H
