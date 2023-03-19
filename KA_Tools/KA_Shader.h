/*
 * OpenGL渲染器程序
 * 分为顶点着色器(VertexShader)和片段着色器(FragmentShader)
 */
#ifndef KA_SHADER_H
#define KA_SHADER_H

#define GLEW_STATIC
#include <glew.h>

#include <iostream>
#include <fstream>
using namespace std;

class KA_Shader
{
private:
    GLuint _programID;// OpenGL渲染程序句柄

protected:
    // 从文件读取Shader代码
    string _readShaderSource(string &filePath);

public:
    KA_Shader();
    ~KA_Shader();

    // 创建渲染程序
    void createShader(string vertexShader, string fragmentShader);

    // 获取渲染程序句柄
    GLuint getShaderID();

    // 设置纹理单元
    void setTextureUnit();

    // 传递int变量
    void setUniformi(const char *uniform,int value);

    // 传递float变量
    void setUniformf(const char *uniform,float value);

    // 传递double变量
    void setUniformd(const char *uniform,double value);

    // 使用渲染程序
    void use();

    // 停用渲染程序
    void disuse();

    // 删除渲染程序
    void deleteShader();
};

// inline function
inline GLuint KA_Shader::getShaderID()
{
    return _programID;
}

inline void KA_Shader::setTextureUnit()
{
    glUniform1i(glGetUniformLocation(_programID, "tex"), 0);
}

inline void KA_Shader::setUniformi(const char *uniform, int value)
{
    glUniform1i(glGetUniformLocation(_programID, uniform), value);
}

inline void KA_Shader::setUniformf(const char *uniform, float value)
{
    glUniform1f(glGetUniformLocation(_programID, uniform), value);
}

inline void KA_Shader::setUniformd(const char *uniform, double value)
{
    glUniform1d(glGetUniformLocation(_programID, uniform), value);
}

#endif // KA_SHADER_H
