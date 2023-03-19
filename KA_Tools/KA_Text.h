/*
 * 文字纹理生成单元
 * 负责文字纹理句柄的管理和分发
 * 文字编码格式为UNICODE
 * 字体库格式为TTF
 */
#ifndef KA_TEXT_H
#define KA_TEXT_H

#define GLEW_STATIC
#include <glew.h>

extern "C"{
#include <SDL.h>
#include <SDL_ttf.h>
}

#include <iostream>
#include <cwchar>
using namespace std;

struct TextData
{
    Uint16 *data;// Unicode数组指针
    int size;// 数组长度
};

class KA_Text
{
private:
    GLuint _texture;// OpenGL纹理句柄

    int _size;// 字体大小

    SDL_Color _color;// 字体颜色

    TTF_Font *_font;// 字体文件

    int _linelen;// 单行文字长度

    TextData *_textbuffer;// 文字数据

    SDL_Surface *_ttf;// 未规格化文字像素数据

    SDL_Surface *_surface;// 规格化文字图像数据

public:
    // 默认0号纹理单元
    KA_Text();
    ~KA_Text();

    // 设置字体大小
    void setSize(int size);

    // 设置字体颜色
    void setColor(SDL_Color color);

    // 设置字体颜色及透明度
    void setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);

    // 设置字体
    void setFont(string fontFile);

    // 设置字体及大小
    void setFont(string fontFile, int size);

    // 设置单行文字长度
    void setLength(int lineLength);

    // 加载文字
    void loadWords(const wchar_t *words);

    // 获取纹理句柄
    GLuint getTexID();
};

//inline function
inline void KA_Text::setSize(int size)
{
    _size = size;
}

inline void KA_Text::setColor(SDL_Color color)
{
    _color = color;
}

inline void KA_Text::setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
    _color = {red, green, blue, alpha};
}

inline void KA_Text::setFont(string fontFile)
{
    _font = TTF_OpenFont(fontFile.c_str(), _size);
    if (_font == nullptr)
    {
        cout << "Font Load Fail." << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
}

inline void KA_Text::setFont(string fontFile, int size)
{
    _font = TTF_OpenFont(fontFile.c_str(), size);
    if (_font == nullptr)
    {
        cout << "Font Load Fail." << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
}

inline void KA_Text::setLength(int lineLength)
{
    _linelen = lineLength;
}

inline GLuint KA_Text::getTexID()
{
    return _texture;
}

#endif // KA_TEXT_H
