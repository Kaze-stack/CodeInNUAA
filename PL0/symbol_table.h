#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <map>
#include <vector>

#include "const.h"

struct SymbolNode {
    std::string name;// 名称
    IDType::Type type;// 类型
    uint16 entry;// 入口
    uint16 addr;// 过程地址
    int constValue;// 常量值/过程参数个数

    SymbolNode(std::string &&newName, IDType::Type curtype, uint8 curEntry)
    {
        name = newName;
        type = curtype;
        entry = curEntry;
        constValue = 0;
        addr = 0;
    }
};

struct EntryNode {
    uint16 entry;// 入口
    uint16 father;// 父级入口

    EntryNode(uint16 curEntry, uint16 fatherEntry)
    {
        entry = curEntry;
        father = fatherEntry;
    }
};

class SymbolTable {
private:
    std::vector<SymbolNode> _table;// 符号表
    std::vector<EntryNode> _entryMap;// 入口层次表
    uint16 _curEntry; // 当前入口

    std::string _error;

public:
    SymbolTable();
    ~SymbolTable();

    // 入表
    bool push(std::string &name, IDType::Type curtype);

    // 设置常量值
    void setConstValue(int value);

    // 设置过程参数个数
    void setParaCount(int count);

    // 设置过程地址
    void setProcAddr(int addr);

    // 获取变量个数
    int getVarCount();

    // 进入新入口
    bool newLayer(std::string &name);

    // 退出当前入口
    void backLayer();

    // 检查过程入口
    int check_procedure(std::string &name);

    // 检查左值
    bool check_lvalue(std::string &name);

    // 检查右值
    bool check_rvalue(std::string &name);

    // 获取变量层差和偏移量
    std::pair<uint, int> get_L_A(std::string &name);

    // 获取常量值
    int getConstValue(std::string &name);

    // 获取过程参数个数和地址
    std::pair<uint, int> getProcInfo(std::string &name);

    // 传出错误
    std::string&& error();

    // 打印所有符号
    void printAll();
};

#endif // SYMBOL_TABLE_H