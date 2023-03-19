#ifndef GRAM_ANALYSIS_H
#define GRAM_ANALYSIS_H

#include <stack>

#include "const.h"
#include "lex_analysis.h"
#include "lex_analysis.cpp"

#include "symbol_table.h"
#include "symbol_table.cpp"

#include "pcode_gen.h"
#include "pcode_gen.cpp"

class GramAnalysis{
private:
    bool _stall;// 是否阻塞/暂停获取
    bool _eof;// 是否结束
    bool _errorCnt;// 错误记录

    std::string _word;// 单词
    std::string _type;// 类型

    LexAnalysis *_lexModule;// 词法分析模块指针
    SymbolTable *_symbolTable;// 符号表指针
    PcodeGen *_pcodeGen;// p代码生成模块指针

    std::stack<std::string> _paraStack;// 过程参数栈
    std::pair<uint, int> _varLoc;// 变量/过程定位

    // 获取词法分析的结果
    void getWord();

    // 将<integer>转为数字
    short wordToInt();

    // 记录语法错误
    void errorLog(const char *p)
    {
        _errorCnt = true;
        cout << "Grammar Error in "
                << "Row :" << _lexModule->getRow() << ", "
                << "Col :" << _lexModule->getCol() << ", "
                << p << std::endl
                << std::endl;
    }

    // 记录语法错误
    void errorLog(std::string &&p)
    {
        _errorCnt = true;
        cout << "Grammar Error in "
                << "Row :" << _lexModule->getRow() << ", "
                << "Col :" << _lexModule->getCol() << ", "
                << p << std::endl
                << std::endl;
    }
    
    // 记录语法错误
    void errorLog(int row, int col, const char *p)
    {
        _errorCnt = true;
        cout << "Grammar Error in "
                << "Row :" << row << ", "
                << "Col :" << col << ", "
                << p << std::endl
                << std::endl;
    }

protected:
    // <prog> -> program <id>; <block>
    void Prog();

    // <block> -> [<condecl>][<vardecl>][<proc>]<body>
    void Block();

    // <condecl> -> const <const>{, <const>};
    //void Condecl();

    // <const> -> <id>:=<integer>
    void Const();

    // <vardecl> -> var <id>{, <id>};
    void Vardecl();

    // <proc> -> procedure <id>([<id>{, <id>}]);<block>{;<proc>}
    void Proc();

    // <body> -> begin <statement>{;<statement>}end
    void Body();

    /* <statement> -> <id> := <exp>
    // if <lexp> then <statement>[else <statement>]
    // while <lexp> do <statement>
    // call <id>([<exp>{, <exp>}])
    // <body>
    // read (<id>{, <id>})
    // write (<exp>{, <exp>})
    */
    void Statement();

    // <lexp> -> <exp> <lop> <exp>|odd <exp>
    void Lexp();

    // <exp> -> [<aop>]<term>{<aop><term>}
    void Exp();

    // <term> -> <factor>{<mop><factor>}
    void Term();

    // <factor> -> <id>|<integer>|(<exp>)
    void Factor();

public:
    GramAnalysis();
    ~GramAnalysis();

    // 设置词法分析模块
    void setLexModule(LexAnalysis *module)
    {
        _lexModule = module;
    }

    // 设置符号表
    void setSymbolTable(SymbolTable *table)
    {
        _symbolTable = table;
    }

    // 设置P代码生成模块
    void setPcodeGen(PcodeGen *gen)
    {
        _pcodeGen = gen;
        _pcodeGen->setStopPointer(&_errorCnt);
    }

    // 开始
    bool start()
    {
        Prog();
        if (_errorCnt == 0)
        {
            return true;
        }
        else return false;
    } 
};

#endif // GRAM_ANALYSIS_H