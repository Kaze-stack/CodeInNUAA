#ifndef CONST_H
#define CONST_H

#include <iostream>
#include <string>

typedef char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

namespace TypeMap {
    const char syskey[] = "SysKey";
    const char assign[] = "assign"; // :=
    const char lop[] = "lop"; // =, <>, <, <=, >, >=
    const char aop[] = "aop"; // +, -
    const char mop[] = "mop"; // *, /
    const char id[] = "id";
    const char integer[] = "integer";
    const char error[] = "ERROR";
};

const std::string SysKey[19] = {
    "program", "procedure",
    "(", ")",
    "const", "var",
    "begin", "end",
    "if", "then", "else", 
    "while", "do", 
    "call", 
    "read", "write", 
    "odd", 
    ",", ";"
};

namespace IDType {
    enum Type {
        MAIN,
        PROCEDURE, 
        CONST, 
        VAR
    };
};

namespace Operation {
    enum Opr {
        LIT, 
        LOD, 
        STO, 
        CAL,
        INT, 
        JMP, 
        JPC, 
        WRT, 
        OPR, 
        RED
    };
    const char Type[][4] = {
        "lit", "lod", "sto", "cal", 
        "int", "jmp", "jpc", "wrt", 
        "opr", "red"
    };
};

#endif // CONST_H