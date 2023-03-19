#include "pcode_gen.h"
#include "const.h"

using namespace std;

PcodeGen::PcodeGen()
{
    _codeStack.clear();
    _codeStack.reserve(4);
    _codeStack.push_back(Pcode(Operation::JMP, 0, 0));

    _isStop = nullptr;
}

PcodeGen::~PcodeGen()
{
    _isStop = nullptr;
}

void PcodeGen::Gen(Operation::Opr opr, int layerDiff, int a)
{
    if (*_isStop) return ;
    _codeStack.push_back(Pcode(opr, layerDiff, a));
}

void PcodeGen::programGen()
{
    if (*_isStop) return ;
    // 回填Program入口
    _codeStack[0].A = _codeStack.size();
}

uint PcodeGen::backpatchGen(Operation::Opr opr, int layerDiff)
{
    if (*_isStop) return 0;
    _codeStack.push_back(Pcode(opr, layerDiff, 0));
    return _codeStack.size() - 1;
}

void PcodeGen::backPatch(uint addr)
{
    if (*_isStop) return ;
    _codeStack[addr].A = _codeStack.size();
}

void PcodeGen::printAll()
{
    if (*_isStop) return ;
    cout << "Pcode:" << endl;
    int n = _codeStack.size();
    for (int i = 0; i < n; i++)
    {
        cout << i << "\t";
        cout << Operation::Type[_codeStack[i].F] << "\t";
        cout << _codeStack[i].L << "\t";
        cout << _codeStack[i].A << endl;
    }
}