#include "KA_Widget.h"
KA_Widget::KA_Widget()
{
    _textureID = 0;
    _isFullScreen = false;
    _texUnit = nullptr;
}

KA_Widget::~KA_Widget()
{
}

void KA_Widget::setRect(int x, int y, int width, int height)
{
    _x = x;
    _y = y;
    _w = width;
    _h = height;
}

void KA_Widget::setPos(int x, int y)
{
    _x = x;
    _y = y;
}

void KA_Widget::setSize(int width, int height)
{
    _w = width;
    _h = height;
}

void KA_Widget::setMaxRatio(float ratio)
{
    _ratio = ratio;
    _Mx = floor(_x * ratio);
    _My = floor(_y * ratio);
    _Mw = floor(_w * ratio);
    _Mh = floor(_h * ratio);
}
