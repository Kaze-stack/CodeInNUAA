#ifndef PCODE_INTER_H
#define PCODE_INTER_H

#include "const.h"
#include "pcode_gen.h"
#include "data_stack.h"
#include <vector>

class PcodeInterpreter {
private:
    std::vector<Pcode> _code;// 代码存储器
    DataStack _stack;// 数据栈
    uint _I;// 当前指令
    uint _P;// 下一条指令
    uint _B;// 基地址
    uint _T;// 栈顶

    void _lit();
    void _lod();
    void _sto();
    void _cal();
    void _int();
    void _jmp();
    void _jpc();
    void _wrt();
    void _opr();
    void _red();

public:
    PcodeInterpreter(std::vector<Pcode> &&codeStack)
    {
        _code = std::forward<std::vector<Pcode> &&>(codeStack);
        _I = 0;
        _P = 0;
        _B = 0;
        _T = 0;
    }
    ~PcodeInterpreter()
    {
    }

    // 执行解释
    void execute();
};

#endif// PCODE_INTER_H