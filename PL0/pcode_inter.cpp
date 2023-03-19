#include "pcode_inter.h"
#include "const.h"
#include "pcode_gen.h"

using namespace std;

void PcodeInterpreter::_lit()
{
    _stack[_T] = _code[_I].A;
    _T++;
}

void PcodeInterpreter::_lod()
{
    int layerdiff = _code[_I].L;
    uint oBase = _B;
    while (layerdiff > 0)
    {
        oBase = _stack[oBase + 1];
        layerdiff--;
    }
    _stack[_T] = _stack[oBase + _code[_I].A];
    _T++;
}

void PcodeInterpreter::_sto()
{
    _T--;
    if (_code[_I].L == -1)
    {
        _stack[_code[_I].A + _T] = _stack[_T];
    }
    else  
    {
        int layerdiff = _code[_I].L;
        uint oBase = _B;
        while (layerdiff > 0)
        {
            oBase = _stack[oBase + 1];
            layerdiff--;
        }
        _stack[oBase + _code[_I].A] = _stack[_T];
    }
}

void PcodeInterpreter::_cal()
{
    _stack[_T + 0] = _P;// RA

    int layerdiff = _code[_I].L;
    uint oBase = _B;
    while (layerdiff > 0)
    {
        oBase = _stack[oBase + 1];
        layerdiff--;
    }
    _stack[_T + 1] = _code[_I].L == -1 ? _stack[_B + 1] : _stack[oBase + 1];// DL
    _stack[_T + 2] = _B;// SL

    // cout << "RA\tDL\tSL" << endl;
    // cout << _stack[_T + 0] << "\t";
    // cout << _stack[_T + 1] << "\t";
    // cout << _stack[_T + 2] << "\n";
    
    _B = _T;
    _P = _code[_I].A;
}

void PcodeInterpreter::_int()
{
    _T = _T + _code[_I].A;
}

void PcodeInterpreter::_jmp()
{
    _P = _code[_I].A;
}

void PcodeInterpreter::_jpc()
{
    _T--;
    if (_stack[_T] == 0)
    {
        _P = _code[_I].A;
    }
}

void PcodeInterpreter::_wrt()
{
    _T--;
    cout << _stack[_T] << endl;
}

void PcodeInterpreter::_opr()
{
    if (_code[_I].A == 0)
    {
        _P = _stack[_B + 0];
        _T = _B;
        _B = _stack[_B + 2];
    }
    else if (_code[_I].A == 1)
    {
        _T--;
        _stack[_T] = -_stack[_T];
        _T++;
    }
    else if (_code[_I].A == 2)
    {
        _T--;
        _stack[_T - 1] = _stack[_T - 1] + _stack[_T];
    }
    else if (_code[_I].A == 3)
    {
        _T--;
        _stack[_T - 1] = _stack[_T - 1] - _stack[_T];
    }
    else if (_code[_I].A == 4)
    {
        _T--;
        _stack[_T - 1] = _stack[_T - 1] * _stack[_T];
    }
    else if (_code[_I].A == 5)
    {
        _T--;
        if (_stack[_T] == 0)
        {
            cout << "NaN Error!" << endl;
            cout << "Fail to Execute." << endl;
            exit(-1);
        }
        _stack[_T - 1] = _stack[_T - 1] / _stack[_T];
    }
    else if (_code[_I].A == 6)
    {
        _T--;
        if (_stack[_T] % 2 == 0)
        {
            _stack[_T] = 0;
        }
        else  
        {
            _stack[_T] = 1;
        }
        _T++;
    }
    else if (_code[_I].A == 7)
    {
        _T--;
        _stack[_T - 1] = (_stack[_T - 1] == _stack[_T]);
    }
    else if (_code[_I].A == 8)
    {
        _T--;
        _stack[_T - 1] = (_stack[_T - 1] != _stack[_T]);
    }
    else if (_code[_I].A == 9)
    {
        _T--;
        _stack[_T - 1] = (_stack[_T - 1] < _stack[_T]);
    }
    else if (_code[_I].A == 10)
    {
        _T--;
        _stack[_T - 1] = (_stack[_T - 1] >= _stack[_T]);
    }
    else if (_code[_I].A == 11)
    {
        _T--;
        _stack[_T - 1] = (_stack[_T - 1] > _stack[_T]);
    }
    else if (_code[_I].A == 12)
    {
        _T--;
        _stack[_T - 1] = (_stack[_T - 1] <= _stack[_T]);
    }
}

void PcodeInterpreter::_red()
{
    int temp;
    cin >> temp;
    _stack[_T] = temp;
    _T++;
}

void PcodeInterpreter::execute()
{
    int n = _code.size();
    while (_P < n)
    {
        _I = _P;
        _P++;
        switch (_code[_I].F)
        {
            case Operation::LIT: _lit();break;
            case Operation::LOD: _lod();break;
            case Operation::STO: _sto();break;
            case Operation::CAL: _cal();break;
            case Operation::INT: _int();break;
            case Operation::JMP: _jmp();break;
            case Operation::JPC: _jpc();break;
            case Operation::WRT: _wrt();break;
            case Operation::OPR: _opr();break;
            case Operation::RED: _red();break;
        }
    }
    cout << "execute over." << endl;
}