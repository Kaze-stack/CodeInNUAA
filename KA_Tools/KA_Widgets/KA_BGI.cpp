#include "KA_BGI.h"

KA_BGI::KA_BGI()
{
    _x = 0;
    _y = 0;
    _texUnit = nullptr;
}

KA_BGI::~KA_BGI()
{
}

void KA_BGI::show()
{
    if (_isFullScreen)
    {
        glViewport(_Mx, _My, _Mw, _Mh);
    }
    else
    {
        glViewport(_x, _y, _w, _h);
    }
    glUniform1i(glGetUniformLocation(_shaderID, "tex"), 0);
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void KA_BGI::destroy()
{
//    glDeleteTextures(1, &_textureID);

//    _texUnit->deleteTexID(_textureID);
    _texUnit = nullptr;

    _textureID = 0;
}
