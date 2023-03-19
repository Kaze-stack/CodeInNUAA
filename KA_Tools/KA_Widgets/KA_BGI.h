/*
 * 窗口内组件: 背景图片
 * 具有默认坐标和纹理贴图
 * 继承自KA_Widget
 */
#ifndef KA_BGI_H
#define KA_BGI_H
#include "KA_Widget.h"

class KA_BGI : public KA_Widget
{
protected:
//    short _x, _y;// 在原始窗口下的x，y坐标

//    short _w, _h;// 在原始窗口下的宽，高

//    short _Mx, _My;// 在全屏幕尺寸下的x，y坐标

//    short _Mw, _Mh;// 在全屏幕尺寸下的宽，高

//    float _ratio;// 原始窗口变为全屏幕尺寸的比例

//    GLuint _shaderID;// 渲染程序句柄

//    GLuint _textureID;// 纹理句柄

//    bool _isFullScreen;// 当前是否为全屏幕

//    KA_Texture *_texUnit;// 上级图片纹理生成单元

public:
    KA_BGI();
    virtual ~KA_BGI();

    // 在循环渲染中显示该组件
    virtual void show();

    // 手动销毁该组件
    virtual void destroy();
};

#endif // KA_BGI_H
