#include "KA_Shader.h"

KA_Shader::KA_Shader()
{
    // 默认为0号空程序
    _programID = 0;
}

KA_Shader::~KA_Shader()
{
}

string KA_Shader::_readShaderSource(string &filePath)
{
    string content;
    string line;
    fstream file;
    file.open(filePath, ios::in);
    if (!file)
    {
        cout << "Shader File Load Fail." << endl;
        exit(1);
    }
    while (!file.eof())
    {
        char buffer;
        file.get(buffer);
        if (!file) break;
        if (buffer == '\n')
        {
            content += line;
            content += '\n';
            line.clear();
            continue;
        }
        else
        {
            line += buffer;
        }
    }
    file.close();

    return content;
}

void KA_Shader::createShader(string vertexShader, string fragmentShader)
{
    string vShaderStr = _readShaderSource(vertexShader);
    string fShaderStr = _readShaderSource(fragmentShader);

    const char *vshaderSource = vShaderStr.c_str();
    const char *fshaderSource = fShaderStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vshaderSource, nullptr);
    glShaderSource(fShader, 1, &fshaderSource, nullptr);

    glCompileShader(vShader);
    glCompileShader(fShader);

    _programID = glCreateProgram();
    if (_programID == 0)
    {
        cout << "Shader Create Fail." << endl;
        exit(1);
    }
    glAttachShader(_programID, vShader);
    glAttachShader(_programID, fShader);
    glLinkProgram(_programID);

    glDetachShader(_programID, fShader);
    glDetachShader(_programID, vShader);
    glDeleteShader(fShader);
    glDeleteShader(vShader);
}

void KA_Shader::use()
{
    glUseProgram(_programID);
    glEnable(GL_TEXTURE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void KA_Shader::disuse()
{
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE);
    glUseProgram(0);
}

void KA_Shader::deleteShader()
{
    if (_programID != 0)
    {
        glDeleteProgram(_programID);
    }
}
