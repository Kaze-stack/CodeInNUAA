#include "KA_Character.h"

KA_Character::KA_Character()
{
    _texUnit = nullptr;
}

KA_Character::~KA_Character()
{
}

void KA_Character::show()
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

void KA_Character::destroy()
{
//    glDeleteTextures(1, &_textureID);

//    _texUnit->deleteTexID(_textureID);
    _texUnit = nullptr;

    _textureID = 0;
}
