#ifndef PCODE_GEN_H
#define PCODE_GEN_H

#include "const.h"
#include <vector>

struct Pcode {
    Operation::Opr F;// 伪操作码
    int L;// 层差(Cur - Target)
    int A;// 偏移量/常数
    Pcode(){}

    Pcode(Operation::Opr opr, int layerDiff, int a)
    {
        F = opr;
        L = layerDiff;
        A = a;
    }
};

class PcodeGen {
private:
    std::vector<Pcode> _codeStack;// 指令栈

    bool *_isStop;// 是否停止生成

public:
    PcodeGen();
    ~PcodeGen();

    // 绑定布尔变量指针
    void setStopPointer(bool *p)
    {
        _isStop = p;
    }

    // 生成指令
    void Gen(Operation::Opr opr, int layerDiff, int a);

    // 回填第一条指令
    void programGen();

    // 需要回填的指令, 返回指令编号
    uint backpatchGen(Operation::Opr opr, int layerDiff);

    // 回填指令
    void backPatch(uint addr);

    // 获取指令栈长度
    int getLength()
    {
        return _codeStack.size();
    }

    // 打印所有指令
    void printAll();

    // 移交指令栈
    std::vector<Pcode>&& getCodeStack()
    {
        if (*_isStop)
        {
            _codeStack.clear();
        }
        return std::move(_codeStack);
    }
};

#endif// PCODE_GEN_H