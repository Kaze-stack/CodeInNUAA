#ifndef LEX_ANALYSIS_H
#define LEX_ANALYSIS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cwchar>

#include "const.h"

class LexAnalysis{
private:
    int _row;// 行号
    int _col;// 列号

    wchar_t _wch;// 宽位字符缓冲
    char _ch;// 有效字符
    bool _lex;// 当前循环是否解析
    bool _eof;// 是否到达文件尾
    bool _over;// 词法分析是否结束

    std::string _word;// 当前单词
    std::string _type;// 单词类型

    std::string _fileName;// 被解析文件名称
    std::wfstream _inFile;// 被解析文件

    std::string _ofileName;// 输出文件名称
    std::fstream _outLog;// 输出文件

    // 判断是否为字母
    static bool isCharacter(char a)
    {
        if (a >= 'a' && a <= 'z')
        {
            return true;
        }
        else if (a >= 'A' && a <= 'Z')
        {
            return true;
        }
        else return false;
    }

    // 判断是否为数字
    static bool isNumber(char a)
    {
        if (a >= '0' && a <= '9')
        {
            return true;
        }
        else return false;
    }

    // 判断是否为系统保留字
    bool isSysKey() const;

    // 判断是否为标识符
    bool isID() const;

    // 判断是否为整型
    bool isInteger() const;

    // 分析当前单词类型
    void typeAnalysis();

    // 词法错误记录
    void errorLog(std::ostream &out)
    {
        out << "Illegal Word in "
            << "Row :" << _row << ','
            << "Col :" << _col << ", "
            << '"' << _word << '"' << std::endl
            << std::endl;
    }

    // 全角字符
    void errorLog(std::wostream &out)
    {
        out << "Illegal Character in "
            << "Row :" << _row << ", "
            << "Col :" << _col << ", "
            << '"' << _wch << '"' << std::endl
            << std::endl;
    }

    // 词法记录
    void lexLog()
    {
        // _outLog << "< " << _type << ", "
        //         << _word << " >" << std::endl;
    }

    // 关闭文件
    void closeFile()
    {
        if (_inFile.is_open())
        {
            _inFile.close();
        }
        if (_outLog.is_open())
        {
            _outLog.close();
        }
        remove(_ofileName.c_str());
    }

public:
    LexAnalysis();
    ~LexAnalysis();

    // 返回行号
    int getRow() const
    {
        return _row;
    }

    // 返回列号
    int getCol() const
    {
        return _col;
    }

    bool isOver() const 
    {
        return _over;
    }

    // 返回当前单词
    std::string getWord() const
    {
        return _word;
    }

    // 返回当前类型
    std::string getType() const
    {
        return _type;
    }

    // tick函数
    bool tick();

    // 设置文件名
    bool setFileName(const char *fileName);

    // 设置文件名
    bool setFileName(std::string fileName);
};

#endif // LEX_ANALYSIS_H