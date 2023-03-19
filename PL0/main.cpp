// 打印符号表
// #define PRINT_SYMBOL
// 打印P代码
// #define PRINT_PCODE

#include "gram_analysis.h"
#include "gram_analysis.cpp"

#include "pcode_inter.h"
#include "pcode_inter.cpp"

extern "C" {
    #include <time.h>
}

using namespace std;

const char filename[] = "test4.txt";
time_t Tstart, Tend;

int main()
{
    LexAnalysis lex;
    if (!lex.setFileName(filename))
    {
        return 1;
    }

    SymbolTable symbolTable;
    PcodeGen pcodeGen;

    GramAnalysis grammar;
    grammar.setLexModule(&lex);
    grammar.setSymbolTable(&symbolTable);
    grammar.setPcodeGen(&pcodeGen);

    Tstart = clock();
    bool res = grammar.start();

#ifdef PRINT_SYMBOL
    symbolTable.printAll();
#endif

#ifdef PRINT_PCODE
    pcodeGen.printAll();
#endif

    if (res)
    {
        PcodeInterpreter interpreter(pcodeGen.getCodeStack());
        interpreter.execute();
    }
    
    Tend = clock();

    cout << "Serial Time: " << (Tend - Tstart) * 1.0 / CLOCKS_PER_SEC << " s" << endl;
    cout << endl;

    return 0;
}