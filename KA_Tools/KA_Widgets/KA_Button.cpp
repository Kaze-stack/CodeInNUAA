#include "KA_Button.h"

KA_Button::KA_Button()
{
    _textID = 0;
    _isHover = false;
    _texUnit = nullptr;
}

KA_Button::~KA_Button()
{
}

void KA_Button::setText(GLuint textID)
{
    if (_textID != 0)
    {
        glDeleteTextures(1, &_textID);
    }
    _textID = textID;
}

void KA_Button::isHover(int x, int y)
{
    if (_isFullScreen)
    {
        if ((x > _Mx) && (x < _Mx + _Mw) &&
            (y > _My) && (y < _My + _Mh))
        {
            _isHover = true;
        }
        else _isHover = false;
    }
    else
    {
        if ((x > _x) && (x < _x + _w) &&
            (y > _y) && (y < _y + _h))
        {
            _isHover = true;
        }
        else _isHover = false;
    }
}

void KA_Button::show()
{
    if (_isFullScreen)
    {
        glViewport(_Mx, _My, _Mw, _Mh);
    }
    else
    {
        glViewport(_x, _y, _w, _h);
    }

    if (_isHover)
    {
        glUniform1i(glGetUniformLocation(_shaderID, "isHover"), 1);
    }
    else
    {
        glUniform1i(glGetUniformLocation(_shaderID, "isHover"), 0);
    }

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

    glUniform1i(glGetUniformLocation(_shaderID, "isHover"), 0);
}

void KA_Button::destroy()
{
//    glDeleteTextures(1, &_textureID);
    glDeleteTextures(1, &_textID);

//    _texUnit->deleteTexID(_textureID);
    _texUnit = nullptr;

    _textureID = 0;
    _textID = 0;
}
