/*
 * 窗口内组件
 * 具有坐标和纹理贴图
 * 抽象类
 */
#ifndef KA_WIDGET_H
#define KA_WIDGET_H
#include "KA_Texture.h"
#include "KA_Shader.h"

class KA_Widget
{
protected:
    short _x, _y;// 在原始窗口下的x，y坐标

    short _w, _h;// 在原始窗口下的宽，高

    short _Mx, _My;// 在全屏幕尺寸下的x，y坐标

    short _Mw, _Mh;// 在全屏幕尺寸下的宽，高

    float _ratio;// 原始窗口变为全屏幕尺寸的比例

    GLuint _shaderID;// 渲染程序句柄

    GLuint _textureID;// 纹理句柄

    bool _isFullScreen;// 当前是否为全屏幕

    KA_Texture *_texUnit;// 上级图片纹理生成单元

public:
    KA_Widget();
    virtual ~KA_Widget();

    // 设置x，y坐标和宽，高
    void setRect(int x, int y, int width, int height);

    // 设置x，y坐标
    void setPos(int x, int y);

    // 设置宽，高
    void setSize(int width, int height);

    // 设置原始窗口至全屏幕尺寸的比例
    void setMaxRatio(float ratio);

    // 设置渲染程序句柄
    void setShader(GLuint shaderID);

    // 设置纹理句柄
    void setTexture(GLuint textureID);

    // 设置上级纹理生成单元
    void setTextureUnit(KA_Texture *unit);

    // 设置当前环境为窗口模式
    void setWindowSize();

    // 设置当前环境为全屏幕模式
    void setFullScreen();

    // 纯虚函数
    // 在循环渲染中显示该组件
    virtual void show() = 0;

    // 纯虚函数
    // 手动销毁该组件
    virtual void destroy() = 0;
};

// inline function
inline void KA_Widget::setShader(GLuint shaderID)
{
    _shaderID = shaderID;
}

inline void KA_Widget::setTexture(GLuint textureID)
{
    _textureID = textureID;
}

inline void KA_Widget::setTextureUnit(KA_Texture *unit)
{
    _texUnit = unit;
}

inline void KA_Widget::setWindowSize()
{
    _isFullScreen = false;
}

inline void KA_Widget::setFullScreen()
{
    _isFullScreen = true;
}

#endif // KA_WIDGET_H
