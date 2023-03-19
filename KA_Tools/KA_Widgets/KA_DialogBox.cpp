#include "KA_DialogBox.h"

KA_DialogBox::KA_DialogBox()
{
    _textID = 0;
    _x = 0;
    _y = 0;
    _texUnit = nullptr;
}

KA_DialogBox::~KA_DialogBox()
{
}

void KA_DialogBox::setText(GLuint textID)
{
    if (_textID != 0)
    {
        glDeleteTextures(1, &_textID);
    }
    _textID = textID;
}

void KA_DialogBox::show()
{
    if (_isFullScreen)
    {
        glViewport(_Mx, _My, _Mw, _Mh);
        if (_textureID != 0)
        {
            glUniform1i(glGetUniformLocation(_shaderID, "tex"), 0);
            glBindTexture(GL_TEXTURE_2D, _textureID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        if (_textID != 0)
        {
            glUniform1i(glGetUniformLocation(_shaderID, "tex"), 0);
            glBindTexture(GL_TEXTURE_2D, _textID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    else
    {
        glViewport(_x, _y, _w, _h);
        if (_textureID != 0)
        {
            glUniform1i(glGetUniformLocation(_shaderID, "tex"), 0);
            glBindTexture(GL_TEXTURE_2D, _textureID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glViewport(_x + 24, _h - 52, 217, 32);
        if (_textID != 0)
        {
            glUniform1i(glGetUniformLocation(_shaderID, "tex"), 0);
            glBindTexture(GL_TEXTURE_2D, _textID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}

void KA_DialogBox::destroy()
{
//    glDeleteTextures(1, &_textureID);
    glDeleteTextures(1, &_textID);

//    _texUnit->deleteTexID(_textureID);
    _texUnit = nullptr;

    _textureID = 0;
    _textID = 0;
}
