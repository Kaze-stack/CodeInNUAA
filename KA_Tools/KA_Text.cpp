#include "KA_Text.h"

KA_Text::KA_Text()
{
    _font = nullptr;

    _textbuffer = new TextData;
    _textbuffer->size = 0;
    _ttf = nullptr;
    _surface = nullptr;
}

KA_Text::~KA_Text()
{
}

void KA_Text::loadWords(const wchar_t *words)
{
    int n = wcslen(words);
    if (_textbuffer == nullptr)
    {   
        _textbuffer = new TextData;
        _textbuffer->data = new Uint16[n + 1];
        _textbuffer->size = n + 1;
    }
    else
    {
        if (_textbuffer->size <= n)
        {
            _textbuffer->data = new Uint16[n + 1];
            _textbuffer->size = n + 1;
        }
    }
    for (int i = 0; i < n; i++)
    {
        _textbuffer->data[i] = words[i];
    }
    _textbuffer->data[n] = u'\0';

    _ttf = TTF_RenderUNICODE_Blended_Wrapped(_font, _textbuffer->data, _color, _linelen);
    if (_ttf == nullptr)
    {
        cout << "TTF Surface Create Fail." << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    _surface = SDL_CreateRGBSurface(0, _ttf->w, _ttf->h, 32, 0, 0, 0, 0);
    SDL_BlitSurface(_ttf, nullptr, _surface, nullptr);
    SDL_FreeSurface(_ttf);

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0,
                 GL_RGBA, _surface->w, _surface->h,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, _surface->pixels);

    SDL_FreeSurface(_surface);
}
