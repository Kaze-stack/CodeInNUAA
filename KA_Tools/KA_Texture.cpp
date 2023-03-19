#include "KA_Texture.h"

KA_Texture::KA_Texture()
{
    _surface = nullptr;
    _RUID.resize(8);
}

KA_Texture::~KA_Texture()
{
}

string KA_Texture::_textureDecode(string texFile)
{
    char buffer;
    string dtexFile;
    _enTex.open(texFile, ios::in);
    if (!_enTex)
    {
        cout << "Image File Load Fail." << endl;
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }

    if (_RUID.size() != 0)
    {
        _RUID.clear();
    }
    for (int i = 0; i < 8; i++)
    {
        _enTex.get(buffer);
        _RUID += buffer;
    }

    _mapItr = _texMap.find(_RUID);
    if (_mapItr != _texMap.end()) // 该纹理已存在记录表中
    {
        _texture = _texMap[_RUID];
        return dtexFile;
    }
    else // 该纹理不在记录表中
    {
        dtexFile = texFile + _RUID;
        _deTex.open(dtexFile, ios::out);
        if (!_deTex)
        {
            cout << "Image File Create Fail." << endl;
            IMG_Quit();
            SDL_Quit();
            exit(1);
        }
        while (!_enTex.eof())
        {
            _enTex.get(buffer);
            if (!_enTex) break;
            _deTex << buffer;
        }
        _enTex.close();
        _deTex.close();
        return dtexFile;
    }
}

void KA_Texture::loadTexture(string texFile)
{
    texFile = _textureDecode(texFile);
    if (texFile.size() == 0)
    {
        return ;
    }
    _surface = IMG_Load(texFile.c_str());
    if (_surface == nullptr)
    {
        cout << "Image Load Fail." << endl;
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }
    GLint interMode = _surface->format->BytesPerPixel;
    if (interMode == 3)
    {
        if (_surface->format->Rmask == 0x0000ff)
        {
            _colorMode = GL_RGB;
        }
        else
        {
            _colorMode = GL_BGR;
        }
    }
    else if (interMode == 4)
    {
        if (_surface->format->Rmask == 0x0000ff)
        {
            _colorMode = GL_RGBA;
        }
        else
        {
            _colorMode = GL_BGRA;
        }
    }
    else
    {
        cout << "Wrong Color Channel." << endl;
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0,
                 GL_RGBA, _surface->w, _surface->h,
                 0, _colorMode, GL_UNSIGNED_BYTE, _surface->pixels);

    SDL_FreeSurface(_surface);
    _texMap.insert(pair<string, GLuint>(_RUID, _texture));
    remove(texFile.c_str());
}

void KA_Texture::deleteTexID(string RUID)
{
    if (_texMap.erase(RUID) == 0)
    {
        cout << "RUID: " << RUID << ' ';
        cout << "Texture Missing!" << endl;
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }
}

void KA_Texture::deleteTexID(GLuint texture)
{
    _mapItr = _texMap.begin();
    for (; _mapItr != _texMap.end(); _mapItr++)
    {
        if (_mapItr->second == texture)
        {
            _texMap.erase(_mapItr);
            return ;
        }
    }
    cout << "Handle: " << texture << ' ';
    cout << "Texture Missing!" << endl;
    IMG_Quit();
    SDL_Quit();
    exit(1);
}

void KA_Texture::destroy()
{
    _mapItr = _texMap.begin();
    for (; _mapItr != _texMap.end(); _mapItr++)
    {
        glDeleteTextures(1, &_mapItr->second);
    }
    _texMap.clear();
}
