/*
 * 图片纹理生成单元
 * 负责图片纹理的管理和分发
 * 图片加载库为SDL_image
 */
#ifndef KA_TEXTURE_H
#define KA_TEXTURE_H

#define GLEW_STATIC
#include <glew.h>

extern "C"{
#include <SDL.h>
#include <SDL_image.h>
}

#include <iostream>
#include <fstream>
#include <map>
using namespace std;

class KA_Texture
{
private:
    string _RUID;// 资源文件统一标识

    fstream _enTex;// 加密过的纹理文件

    fstream _deTex;// 解码后的纹理文件

    GLuint _texture;// 当前OpenGL纹理句柄

    GLenum _colorMode;// OpenGL色彩模式
    /*
     * GL_RGB = red, green, blue
     * GL_BGR = blue, green, red
     * GL_RGBA = red, green, blue, alpha
     * GL_BGRA = blue, green, red, alpha
     */

    SDL_Surface *_surface;// 图片像素数据

    map<string, GLuint> _texMap;// 显存中存在的文件RUID和纹理句柄的记录表

    map<string, GLuint>::iterator _mapItr;// 记录表迭代器

protected:
    string _textureDecode(string texFile);

public:
    // 默认0号纹理单元
    KA_Texture();
    ~KA_Texture();

    // 从文件加载当前纹理数据
    void loadTexture(string texFile);

    // 获取当前纹理句柄
    GLuint getTexID();

    // 获取当前纹理RUID
    string getTexRUID();

    // 根据文件RUID删除记录表中的一条记录
    void deleteTexID(string RUID);

    // 根据纹理句柄删除记录表中的一条记录
    void deleteTexID(GLuint texture);

    // 销毁所有的纹理数据
    void destroy();
};

// inline function
inline GLuint KA_Texture::getTexID()
{
    return _texture;
}

inline string KA_Texture::getTexRUID()
{
    return _RUID;
}

//inline void KA_Texture::freeTexture()
//{
//    glDeleteTextures(1, &_texture);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//}

#endif // KA_TEXTURE_H
