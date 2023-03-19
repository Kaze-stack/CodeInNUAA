#include "symbol_table.h"
#include "const.h"
#include <utility>

using namespace std;

SymbolTable::SymbolTable()
{
    _table.reserve(4);
    _entryMap.reserve(2);
    _curEntry = 0;

    _error.clear();
}

SymbolTable::~SymbolTable(){}

bool SymbolTable::push(std::string &name, IDType::Type curtype)
{
    for (int i = _curEntry; ; i++)
    {
        if (i >= _table.size() || _table[i].entry != _curEntry)
        {
            break;
        }
        if (_table[i].name == name && _table[i].type != IDType::PROCEDURE)
        {
            _error = "Multiple definition of : " + name;
            _table.push_back(SymbolNode(move(name), curtype, _curEntry));
            return false;
        }
    }
    _table.push_back(SymbolNode(move(name), curtype, _curEntry));
    return true;
}

void SymbolTable::setConstValue(int value)
{
    int n = _table.size() - 1;
    _table[n].constValue = value;
}

void SymbolTable::setParaCount(int Count)
{
    _table[_curEntry].constValue = Count;
}

void SymbolTable::setProcAddr(int addr)
{
    _table[_curEntry].addr = addr;
}

int SymbolTable::getVarCount()
{
    int cnt = 0;
    int n = _table.size();
    for (int i = _curEntry + 1; i < n; i++)
    {
        if (_curEntry != _table[i].entry)
        {
            break;
        }
        if (_table[i].type == IDType::VAR)
        {
            cnt++;
        }
    }
    return cnt;
}

bool SymbolTable::newLayer(std::string &name)
{
    uint16 father = _curEntry;
    _curEntry = _table.size();
    _entryMap.push_back(EntryNode(_curEntry, father));
    if (_curEntry == 0)
    {
        _table.push_back(SymbolNode(move(name), IDType::MAIN, 0));
        return true;
    }
    else  
    {
        int n = _entryMap.size() - 1;
        for (int i = 1; i < n; i++)
        {
            if (_entryMap[i].father == father)
            {
                if (_table[_entryMap[i].entry].name == name)
                {
                    _error = "Multiple definition of procedure: " + name;
                    _table.push_back(SymbolNode(move(name), IDType::PROCEDURE, father));
                    return false;
                }
            }
        }
        _table.push_back(SymbolNode(move(name), IDType::PROCEDURE, father));
    }
    return true;
}

void SymbolTable::backLayer()
{
    int n = _entryMap.size();
    for (int i = 0; i < n; i++)
    {
        if (_entryMap[i].entry == _curEntry)
        {
            _curEntry = _entryMap[i].father;
            break;
        }
    }
}

int SymbolTable::check_procedure(string &name)
{
    int n = _entryMap.size();
    // 同级检查
    for (int i = 0; i < n; i++)
    {
        if (_entryMap[i].father == _curEntry)
        {
            if (_table[_entryMap[i].entry].name == name)
            {
                return 0;
            }
        }
    }

    // 上级检查
    uint16 tmpEntry = _curEntry;
    uint16 ld = 0;
    while (tmpEntry != 0)
    {
        ld++;
        tmpEntry = _table[tmpEntry].entry;

        for (int i = 0; i < n; i++)
        {
            if (_entryMap[i].father == tmpEntry)
            {
                if (_table[_entryMap[i].entry].name == name)
                {
                    return ld;
                }
            }
        }
    }

    // 递归检查
    if (_table[_curEntry].name == name)
    {
        return 0;
    }
    _error = "Undefined procedure: " + name;
    return -1;
}

bool SymbolTable::check_lvalue(std::string &name)
{
    int n = _entryMap.size();
    int m = _table.size();
    uint16 tmpEntry = _curEntry;
    for (int i = tmpEntry + 1; i < m; i++)
    {
        if (_table[i].entry != tmpEntry)
        {
            break;
        }
        if (_table[i].name == name)
        {
            if (_table[i].type == IDType::VAR)
            {
                return true;
            }
            else  
            {
                _error = name + " is CONST, can't be lvalue";
                return false;
            }
        }
    }

    while (tmpEntry != 0)
    {
        for (int i = 0; i < n; i++)
        {
            if (_entryMap[i].entry == tmpEntry)
            {
                tmpEntry = _entryMap[i].father;
                break;
            }
        }

        for (int i = tmpEntry + 1; i < m; i++)
        {
            if (_table[i].entry != tmpEntry)
            {
                break;
            }
            if (_table[i].name == name)
            {
                if (_table[i].type == IDType::VAR)
                {
                    return true;
                }
                else  
                {
                    _error = name + "is CONST, can't be lvalue";
                    return false;
                }
            }
        }
    }
    _error = "Undefined identifier: " + name;
    return false;
}

bool SymbolTable::check_rvalue(std::string &name)
{
    int n = _entryMap.size();
    int m = _table.size();
    uint16 tmpEntry = _curEntry;
    for (int i = tmpEntry + 1; i < m; i++)
    {
        if (_table[i].entry != tmpEntry)
        {
            break;
        }
        if (_table[i].name == name)
        {
            return true;
        }
    }

    while (tmpEntry != 0)
    {
        for (int i = 1; i < n; i++)
        {
            if (_entryMap[i].entry == tmpEntry)
            {
                tmpEntry = _entryMap[i].father;
                break;
            }
        }

        for (int i = tmpEntry + 1; i < m; i++)
        {
            if (_table[i].entry != tmpEntry)
            {
                break;
            }
            if (_table[i].name == name)
            {
                return true;
            }
        }
    }
    _error = "Undefined identifier: " + name;
    return false;
}

pair<uint, int> SymbolTable::get_L_A(std::string &name)
{
    int layerDiff = 0, a = 0;
    int n = _entryMap.size();
    int m = _table.size();
    uint16 tmpEntry = _curEntry;
    for (int i = tmpEntry + 1; i < m; i++)
    {
        if (_table[i].entry != tmpEntry)
        {
            break;
        }
        if (_table[i].type == IDType::VAR)
        {
            a++;
            if (_table[i].name == name)
            {
                return make_pair(0, a + 2);
            }
        }
    }

    while (tmpEntry != 0)
    {
        a = 0;
        layerDiff++;
        for (int i = 0; i < n; i++)
        {
            if (_entryMap[i].entry == tmpEntry)
            {
                tmpEntry = _entryMap[i].father;
                break;
            }
        }

        for (int i = tmpEntry + 1; i < m; i++)
        {
            if (_table[i].entry != tmpEntry)
            {
                break;
            }
            if (_table[i].type == IDType::VAR)
            {
                a++;
                if (_table[i].name == name)
                {
                    return make_pair(layerDiff, a + 2);
                }
            }
        }
    }
    return make_pair(layerDiff, 0);
}

int SymbolTable::getConstValue(string &name)
{
    int n = _entryMap.size();
    int m = _table.size();
    uint16 tmpEntry = _curEntry;
    for (int i = tmpEntry + 1; i < m; i++)
    {
        if (_table[i].entry != tmpEntry)
        {
            break;
        }
        if (_table[i].name == name)
        {
            return _table[i].constValue;
        }
    }

    while (tmpEntry != 0)
    {
        for (int i = 1; i < n; i++)
        {
            if (_entryMap[i].entry == tmpEntry)
            {
                tmpEntry = _entryMap[i].father;
                break;
            }
        }

        for (int i = tmpEntry + 1; i < m; i++)
        {
            if (_table[i].entry != tmpEntry)
            {
                break;
            }
            if (_table[i].name == name)
            {
                return _table[i].constValue;
            }
        }
    }
    return _table[m - 1].constValue;
}

pair<uint, int> SymbolTable::getProcInfo(std::string &name)
{
    int n = _entryMap.size();
    // 同级检查
    for (int i = 0; i < n; i++)
    {
        if (_entryMap[i].father == _curEntry)
        {
            if (_table[_entryMap[i].entry].name == name)
            {
                return make_pair(_table[_entryMap[i].entry].constValue, _table[_entryMap[i].entry].addr);
            }
        }
    }

    // 上级检查
    uint16 tmpEntry = _curEntry;
    uint16 ld = 0;
    while (tmpEntry != 0)
    {
        ld++;
        tmpEntry = _table[tmpEntry].entry;

        for (int i = 0; i < n; i++)
        {
            if (_entryMap[i].father == tmpEntry)
            {
                if (_table[_entryMap[i].entry].name == name)
                {
                    return make_pair(_table[_entryMap[i].entry].constValue, _table[_entryMap[i].entry].addr);
                }
            }
        }
    }

    // 递归检查
    if (_table[_curEntry].name == name)
    {
        return make_pair(_table[_curEntry].constValue, -_table[_curEntry].addr);
    }
    return make_pair(0, 0);
}

string&& SymbolTable::error()
{
    return move(_error);
}

void SymbolTable::printAll()
{
    int n = _table.size();
    cout << "Symbol Table: " << endl;
    cout << "index\tname\t\t\ttype\tentry\t" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i << "\t";
        cout << _table[i].name << "\t\t\t";
        cout << _table[i].type << "\t";
        cout << _table[i].entry << "\t\n";
    }

    n = _entryMap.size();
    cout << "Entry Map" << endl;
    cout << "index\tentry\t\tfather\t" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i << "\t";
        cout << _entryMap[i].entry << "\t\t";
        cout << _entryMap[i].father << "\t\n";
    }
}