/*
 * 窗口内组件: 按钮
 * 具有坐标，纹理贴图和文字贴图
 * 继承自KA_Widget
 */
#ifndef KA_BUTTON_H
#define KA_BUTTON_H
#include "KA_Widget.h"

class KA_Button : public KA_Widget
{
protected:
//    short _x, _y;// 在原始窗口下的x，y坐标

//    short _w, _h;// 在原始窗口下的宽，高

//    short _Mx, _My;// 在全屏幕尺寸下的x，y坐标

//    short _Mw, _Mh;// 在全屏幕尺寸下的宽，高

//    float _ratio;// 原始窗口变为全屏幕尺寸的比例

//    GLuint _shaderID;// 渲染程序句柄

//    GLuint _textureID;// 图片纹理句柄

    GLuint _textID;// 文字纹理句柄

    bool _isHover;// 鼠标指针是否悬浮在组件上

//    bool _isFullScreen;// 当前是否为全屏幕

//    KA_Texture *_texUnit;// 上级图片纹理生成单元

public:
    KA_Button();
    virtual ~KA_Button();

    // 设置文字纹理句柄
    void setText(GLuint textID);

    // 计算鼠标指针是否悬浮在组件上
    void isHover(int x, int y);

    // 在循环渲染中显示该组件
    virtual void show();

    // 手动销毁该组件
    virtual void destroy();
};

#endif // KA_BUTTON_H
