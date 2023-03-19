#include "gram_analysis.h"
#include "const.h"

using namespace std;

GramAnalysis::GramAnalysis()
{
    _eof = false;
    _stall = false;
    _errorCnt = false;

    _word.reserve(30);
    _type.reserve(10);

    _lexModule = nullptr;
    _symbolTable = nullptr;
    _pcodeGen = nullptr;
}

GramAnalysis::~GramAnalysis()
{
    _lexModule = nullptr;
    _symbolTable = nullptr;
    _pcodeGen = nullptr;
}

void GramAnalysis::getWord()
{
    if (_stall)
    {
        _stall = false;
    }
    else  
    {
        if (!_lexModule->tick())
        {
            _errorCnt = true;
        }
        _eof = _lexModule->isOver();

        _word = _lexModule->getWord();
        _type = _lexModule->getType();
    }
}

short GramAnalysis::wordToInt()
{
    int n = _word.size();
    short res = 0;
    for (int i = 0; i < n; i++)
    {
        res = (res << 3) + (res << 1);
        res += (_word[i] - '0');
    }
    return res;
}

void GramAnalysis::Prog()
{
    // program
    getWord();
    if (_eof)
    {
        errorLog("Expected 'program'.");
        return ;
    }
    if (_word != "program")
    {
        _stall = true;
        errorLog("Expected 'program' before '" + _word + "'.");
    }

    // <id>
    getWord();
    if (_eof) 
    {
        errorLog("Expected identifier after 'program'.");
        return ;
    }
    if (_type != TypeMap::id)
    {
        errorLog("'" + _word + "' is not a valid identifier.");
    }
    _symbolTable->newLayer(_word);

    // ;
    getWord();
    if (_eof) 
    {
        errorLog("Expected ';'.");
        return ;
    }
    if (_word != ";")
    {
        errorLog("Expected ';' instead of '" + _word + "'.");
        _stall = true;
    }

    // <block>
    Block();
}

void GramAnalysis::Block()
{
    getWord();
    if (_eof) 
    {
        errorLog("Expected Part of <block>.");
        return ;
    }

    // [<condecl>]
    if (_word == "const")
    {
        Const();
    }

    // [<vardecl>]
    if (_word == "var")
    {
        Vardecl();
    }

    // [<proc>]
    if (_word == "procedure")
    {
        _stall = true;
        Proc();
    }

    _symbolTable->setProcAddr(_pcodeGen->getLength());
    // <body>
    if (_word == "begin")
    {
        _stall = true;
    }

    // 最后一次执行, 即回填program入口
    _pcodeGen->programGen();
    // 开辟n + 3 块空间
    _pcodeGen->Gen(Operation::INT, 0, _symbolTable->getVarCount() + 3);
    Body();
}

void GramAnalysis::Const()
{
    while (1)
    {
        // <id>
        getWord();
        if (_eof)
        {
            errorLog("Expected identifier.");
            return ;
        }
        if ( _type != TypeMap::id)
        {
            if (_word == ",")
            {
                errorLog("Expected identifier.");
                continue;
            }
            if (_word == ";")
            {
                errorLog("Expected identifier.");
                break;
            }
            errorLog("'" + _word + "' is not a valid identifier.");
        }
        if (!_symbolTable->push(_word, IDType::CONST))
        {
            errorLog(_symbolTable->error());
        }

        // :=
        getWord();
        if (_eof)
        {
            errorLog("Expected ':='.");
            return ;
        }
        if (_type != TypeMap::assign)
        {
            if (_word == ",")
            {
                errorLog("Expected initialization for the identifier.");
                continue;
            }
            if (_word == ";")
            {
                errorLog("Expected initialization for the identifier.");
                break;
            }
            errorLog("Expected ':=' instead of '" + _word + "'.");
        }

        // <integer>
        getWord();
        if (_eof)
        {
            errorLog("Expected <integer>");
            return ;
        }
        if (_type != TypeMap::integer)
        {
            if (_word == ",")
            {
                errorLog("Expected <integer>.");
                continue;
            }
            if (_word == ";")
            {
                errorLog("Expected <integer>.");
                break;
            }
            errorLog("Expected integer instead of '" + _word + "'.");
        }
        _symbolTable->setConstValue(wordToInt());

        // ; or ,
        getWord();
        if (_eof)
        {
            errorLog("Expected ';'.");
            return ;
        }
        else if (_word == ",")
        {
            int row = _lexModule->getRow();
            int col = _lexModule->getCol();

            getWord();
            _stall = true;
            if (_type == TypeMap::id)
            {
                continue;
            }
            else  
            {
                errorLog(row, col, "Expected ';'.");
                break;
            }
        }
        else if (_word == ";")
        {
            break;
        }
        else  
        {
            _stall = true;
            if (_type == TypeMap::id)
            {
                errorLog("Expected ',' before '" + _word + "'.");
                continue;
            }
            else
            {
                int row = _lexModule->getRow();
                int col = _lexModule->getCol();

                errorLog(row, col - _word.size(), "Expected ';'.");
                break;
            }
        }
    }

    getWord();
    if (_eof) 
    {
        errorLog("Expected Part of <block>.");
        return ;
    }
}

void GramAnalysis::Vardecl()
{
    while (1)
    {
        // <id>
        getWord();
        if (_eof)
        {
            errorLog("Expected identifier.");
            return ;
        }
        if (_type != TypeMap::id)
        {
            if (_word == ",")
            {
                errorLog("Expected identifier.");
                continue;
            }
            if (_word == ";")
            {
                errorLog("Expected identifier.");
                break;
            }
            errorLog("'" + _word + "' is not a valid identifier.");
        }
        if (!_symbolTable->push(_word, IDType::VAR))
        {
            errorLog(_symbolTable->error());
        }

        // ; or ,
        getWord();
        if (_eof)
        {
            errorLog("Expected ';'.");
            return ;
        }
        else if (_word == ",")
        {
            int row = _lexModule->getRow();
            int col = _lexModule->getCol();

            getWord();
            _stall = true;
            if (_type == TypeMap::id)
            {
                continue;
            }
            else  
            {
                errorLog(row, col, "Expected ';'.");
                break;
            }
        }
        else if (_word == ";")
        {
            break;
        }
        else  
        {
            _stall = true;
            if (_type == TypeMap::id)
            {
                errorLog("Expected ',' before '" + _word + "'.");
                continue;
            }
            else
            {
                int row = _lexModule->getRow();
                int col = _lexModule->getCol();

                errorLog(row, col - _word.size(), "Expected ';'.");
                break;
            }
        }
    }

    getWord();
    if (_eof) 
    {
        errorLog("Expected Part of <block>.");
        return ;
    }
}

void GramAnalysis::Proc()
{
    // procedure
    getWord();
    if (_eof)
    {
        errorLog("Expected 'procedure'.");
        return ;
    }
    if (_word == "begin")
    {
        _stall = true;
        errorLog("Unexpected ';' before 'begin'.");
        return ;
    }
    if (_word != "procedure")
    {
        errorLog("Expected 'procedure' instead of '" + _word + "'.");
    }

    // <id>
    getWord();
    if (_eof)
    {
        errorLog("Expected indentifier. after 'procedure'.");
        return ;
    }
    if (_type != TypeMap::id)
    {
        errorLog("'" + _word + "' is not a valid identifier.");
    }
    if (!_symbolTable->newLayer(_word))
    {
        errorLog(_symbolTable->error());
    }

    // (
    getWord();
    if (_eof)
    {
        errorLog("Expected '(' after 'procedure'.");
        return ;
    }
    if (_word != "(")
    {
        _stall = true;
        errorLog("Expected '(' after 'procedure'.");
    }

    // <id>{, <id>}
    while (1)
    {
        // <id>
        getWord();
        if (_eof)
        {
            errorLog("Expected ')' after ')'.");
            return ;
        }
        if (_type != TypeMap::id)
        {
            if (_word == ",")
            {
                errorLog("Expected identifier after '('.");
                continue;
            }
            else if (_type == TypeMap::integer)
            {
                errorLog("Expected identifier instead of integer.");
            }
            else if (_word == ")")
            {
                _stall = true;
                break;
            }
            else  
            {
                errorLog("'" + _word + "' is not a valid identifier.");
            }
        }
        _paraStack.push(_word);
        if (!_symbolTable->push(_word, IDType::VAR))
        {
            errorLog(_symbolTable->error());
            _paraStack.pop();
        }

        // ,
        getWord();
        if (_eof)
        {
            errorLog("Expected ')'.");
            return ;
        }
        if (_word != ",")
        {
            _stall = true;
            if (_type == TypeMap::id || _type == TypeMap::integer)
            {
                errorLog("Expected ','.");
                continue;
            }
            else if (_word.size() > 1)
            {
                errorLog("Expected ','.");
                continue;
            }
            else
            {
                _stall = true;
                break;
            }
        }
    }

    // )
    getWord();
    if (_eof)
    {
        errorLog("Expected ')'.");
        return ;
    }
    if (_word != ")")
    {
        errorLog("Expected ')'.");
        if (_word == ";")
        {
            _stall =true;
        }
    }
    int n = _paraStack.size();
    _symbolTable->setParaCount(n);
    while (!_paraStack.empty())
    {
        _paraStack.pop();
    }

    // ;
    getWord();
    if (_eof)
    {
        errorLog("Expected ';'.");
        return ;
    }
    if (_word != ";")
    {
        errorLog("Expected ';'.");
        if (_word.size() > 1)
        {
            _stall = true;
        }
    }


    // <block>
    Block();
    _symbolTable->backLayer();
    _pcodeGen->Gen(Operation::OPR, 0, 0);

    // {; <proc>}
    // ;
    getWord();
    if (_eof)
    {
        errorLog("Expected <body>.");
        return ;
    }
    if (_word != ";")
    {
        if (_word == "begin")
        {
            _stall = true;
            return ;
        }
        else if (_word == "procedure")
        {
            _stall = true;
            errorLog("Expected ';' before 'procedure'.");
        }
        else  
        {
            errorLog("Expected ';'.");
        }
    }

    // procedure
    Proc();
}

void GramAnalysis::Body()
{
    // begin
    getWord();
    if (_eof)
    {
        errorLog("Expected <body>.");
        return ;
    }
    if (_word != "begin")
    {
        if (_type == TypeMap::id || _word == "if" || _word == "while" ||
            _word == "call" || _word == "read" || _word == "write")
        {
            _stall = true;
        }
    }

    // <statement>
    Statement();

    // {; <statement>}
    while (1)
    {
        // ;
        getWord();
        if (_eof)
        {
            errorLog("Expected 'end'.");
            return ;
        }
        if (_word != ";")
        {
            _stall = true;
            if (_type == TypeMap::id || _word == "if" || _word == "while" ||
            _word == "call" || _word == "read" || _word == "write")
            {
                errorLog("Expected ';' before '" + _word + ".");
            }
            else 
            {
                break;
            }
        }

        // <statement>
        Statement();
    }

    // end
    getWord();
    if (_eof)
    {
        errorLog("Expected 'end'.");
        return ;
    }
}

void GramAnalysis::Statement()
{
    getWord();
    if (_eof)
    {
        errorLog("Expected expression of <statement>.");
        return ;
    }
    //<ERROR> ;end
    if (_word == "end")
    {
        errorLog("Unexpected ';' before 'end'.");
        _stall = true;
        return ;
    }
    // <body>
    else if (_word == "begin")
    {
        _stall = true;
        Body();
    }
    // if
    else if (_word == "if")
    {
        // <lexp>
        Lexp();
        // if not跳转
        int nif = _pcodeGen->backpatchGen(Operation::JPC, 0);

        // then
        getWord();
        if (_eof)
        {
            errorLog("Expected 'then'.");
            return ;
        }
        if (_word != "then")
        {
            _stall = true;
            errorLog("Expected 'then' before '" + _word + "'.");
        }

        // <statement>
        Statement();

        // [else <statement>]
        // else 
        getWord();
        if (_eof)
        {
            errorLog("Expected 'end'.");
            return ;
        }
        if (_word != "else")
        {
            // 回填not if
            _pcodeGen->backPatch(nif);
            _stall = true;
            return ;
        }

        // end if跳转
        int endif = _pcodeGen->backpatchGen(Operation::JMP, 0);
        // if not回填
        _pcodeGen->backPatch(nif);
        // <statement>
        Statement();
        // end if回填
        _pcodeGen->backPatch(endif);
    }
    // while
    else if (_word == "while")
    {
        int inwhile = _pcodeGen->getLength();
        // <lexp>
        Lexp();
        // if not跳转
        int nif = _pcodeGen->backpatchGen(Operation::JPC, 0);

        // do
        getWord();
        if (_eof)
        {
            errorLog("Expected 'do'.");
            return ;
        }
        if (_word != "do")
        {
            errorLog("Expected 'do' instead of '" + _word + "'.");
        }

        // <statement>
        Statement();
        _pcodeGen->Gen(Operation::JMP, 0, inwhile);
        _pcodeGen->backPatch(nif);
    }
    // call
    else if (_word == "call")
    {
        // <id>
        getWord();
        if (_eof)
        {
            errorLog("Expected identifier after 'call'.");
            return ;
        }
        if (_type != TypeMap::id)
        {
            errorLog("'" + _word + "' is not a valid identifier.");
        }
        int layerdiff = _symbolTable->check_procedure(_word);
        if (layerdiff == -1)
        {
            errorLog(_symbolTable->error());
        }
        _varLoc = _symbolTable->getProcInfo(_word);
        int recursion = false;
        int cnt = _varLoc.first;
        int addr = _varLoc.second;
        if (addr < 0)
        {
            recursion = true;
            addr = -addr;
        }
        else if(addr > 0)
        {
            recursion = layerdiff;
        }
        else  
        {
            errorLog("Undefined Procedure " + _word);
        }

        // (
        getWord();
        if (_eof)
        {
            errorLog("Expected '('.");
            return ;
        }
        if (_word != "(")
        {
            errorLog("Expected '('.");
            if (_word.size() > 1)
            {
                _stall = true;
            }
        }

        // [<exp>{, <exp>}]
        // <exp>
        getWord();
        if (_eof)
        {
            errorLog("Expected ')' after '('.");
            return ;
        }
        if (_word != ")")
        {
            int i = 0;
            _stall = true;
            Exp();
            _pcodeGen->Gen(Operation::STO, -1, 3 + i);
            i++;
            cnt--;

            // {, <exp>}
            while (1)
            {
                // ,
                getWord();
                if (_eof)
                {
                    errorLog("Expected ')'.");
                    return ;
                }
                else if (_word == ")")
                {
                    _stall = true;
                    break;
                }
                else if (_word == "+" || _word == "-" || _word == "(" ||
                        _type == TypeMap::id || _type == TypeMap::integer)
                {
                    _stall = true;
                    errorLog("Expected ','.");
                }
                else if (_word != ",")
                {
                    _stall = true;
                    errorLog("Expected ')'.");
                    break;
                }

                // <exp>
                Exp();
                _pcodeGen->Gen(Operation::STO, -1, 3 + i);
                i++;
                cnt--;
            }
        }
        // 参数个数不符合定义
        if (cnt != 0)
        {
            errorLog("the Count of Parameter is not correct.");
        }
        _pcodeGen->Gen(Operation::CAL, recursion, addr);

        // )
        getWord();
        if (_eof)
        {
            errorLog("Expected ')'.");
            return ;
        }
        if (_word != ")")
        {
            errorLog("Expected ')'.");
        }
    }
    // read
    else if (_word == "read")
    {
        // (
        getWord();
        if (_eof)
        {
            errorLog("Expected '(' after 'read'.");
            return ;
        }
        if (_word != "(")
        {
            _stall = true;
            errorLog("Expected '(' after 'read'.");
        }

        // <id>
        getWord();
        if (_eof)
        {
            errorLog("Expected identifier after '('.");
        }
        if (_type != TypeMap::id)
        {
            errorLog("'" + _word + "' is not a valid identifier.");
        }
        if (!_symbolTable->check_lvalue(_word))
        {
            errorLog(_symbolTable->error());
        }
        _varLoc = _symbolTable->get_L_A(_word);
        _pcodeGen->Gen(Operation::RED, 0, 0);
        _pcodeGen->Gen(Operation::STO, _varLoc.first, _varLoc.second);

        // {, <id>}
        while (1)
        {
            // ,
            getWord();
            if (_eof)
            {
                errorLog("Expected ')'.");
                return ;
            }
            if (_word == ")")
            {
                _stall = true;
                break;
            }
            else if (_type == TypeMap::id || _type == TypeMap::integer)
            {
                _stall = true;
                errorLog("Expected ','.");
            }
            else if (_word != ",")
            {
                _stall = true;
                break;
            }

            // <id>
            getWord();
            if (_eof)
            {
                errorLog("Expected identifier after ','.");
            }
            if (_type != TypeMap::id)
            {
                errorLog("'" + _word + "' is not a valid identifier.");
            }
            if (!_symbolTable->check_lvalue(_word))
            {
                errorLog(_symbolTable->error());
            }
            _varLoc = _symbolTable->get_L_A(_word);
            _pcodeGen->Gen(Operation::RED, 0, 0);
            _pcodeGen->Gen(Operation::STO, _varLoc.first, _varLoc.second);
        }

        // )
        getWord();
        if (_eof)
        {
            errorLog("Expected ')'.");
            return ;
        }
        if (_word != ")")
        {
            _stall = true;
            errorLog("Expected ')' before '" + _word + "'.");
        }
    }
    // write
    else if (_word == "write")
    {
        // (
        getWord();
        if (_eof)
        {
            errorLog("Expected '(' after 'write'.");
            return ;
        }
        if (_word != "(")
        {
            errorLog("Expected '(' after 'write'.");
            if (_word == "+" || _word == "-" || _word == "(" ||
                _type == TypeMap::id || _type == TypeMap::integer)
            {
                _stall = true;
            }
        }
        
        // <exp>
        Exp();
        _pcodeGen->Gen(Operation::WRT, 0, 0);

        // {, <exp>}
        while (1)
        {
            // ,
            getWord();
            if (_eof)
            {
                errorLog("Expected ')'.");
                return ;
            }
            else if (_word == ")")
            {
                _stall = true;
                break;
            }
            else if (_word == "+" || _word == "-" || _word == "(" ||
                    _type == TypeMap::id || _type == TypeMap::integer)
            {
                _stall = true;
                errorLog("Expected ','.");
            }
            else if (_word != ",")
            {
                _stall = true;
                break;
            }

            // <exp>
            Exp();
            _pcodeGen->Gen(Operation::WRT, 0, 0);
        }

        // )
        getWord();
        if (_eof)
        {
            errorLog("Expected ')'.");
            return ;
        }
        if (_word != ")")
        {
            _stall = true;
            errorLog("Expected ')' before '" + _word + "'.");
        }
    }
    // <id>:=<exp>
    else 
    {
        if (_type != TypeMap::id)
        {
            errorLog("'" + _word + "' is not a valid identifier.");
        }
        if (!_symbolTable->check_lvalue(_word))
        {
            errorLog(_symbolTable->error());
        }
        pair<uint, int> varLoc = _symbolTable->get_L_A(_word);
        // cout << _word << ": " << _varLoc.first << ", " << _varLoc.second << endl;

        // :=
        getWord();
        if (_eof)
        {
            errorLog("Expected ':=' after identifier.");
            return ;
        }
        if (_word != ":=")
        {
            errorLog("Expected ':=' instead of '" + _word + "'.");
        }

        // <exp>
        Exp();
        _pcodeGen->Gen(Operation::STO, varLoc.first, varLoc.second);
    }
}

void GramAnalysis::Lexp()
{
    // <exp> | odd
    getWord();
    if (_eof)
    {
        errorLog("Expected part of <lexp>.");
        return ;
    }
    // <exp>
    if (_type == TypeMap::aop || _word == "(" ||
        _type == TypeMap::id || _type == TypeMap::integer)
    {
        _stall = true;
        Exp();

        // <lop>
        getWord();
        if (_eof)
        {
            errorLog("Expected part of <lop>.");
            return ;
        }
        string lop;
        if (_type != TypeMap::lop)
        {
            if (_type == TypeMap::aop || _word == "(" ||
                _type == TypeMap::id || _type == TypeMap::integer)
            {
                errorLog("Expected <lop> before '" + _word + "'.");
                _stall = true;
            }
        }
        else  
        {
            lop = move(_word);
        }

        // <exp>
        Exp();
        if (lop == "=")
        {
            _pcodeGen->Gen(Operation::OPR, 0, 7);
        }
        else if (lop == "<>")
        {
            _pcodeGen->Gen(Operation::OPR, 0, 8);
        }
        else if (lop == "<")
        {
            _pcodeGen->Gen(Operation::OPR, 0, 9);
        }
        else if (lop == ">=")
        {
            _pcodeGen->Gen(Operation::OPR, 0, 10);
        }
        else if (lop == ">")
        {
            _pcodeGen->Gen(Operation::OPR, 0, 11);
        }
        else
        {
            _pcodeGen->Gen(Operation::OPR, 0, 12);
        }
    }
    // odd
    else if (_word == "odd")
    {
        // <exp>
        Exp();
        _pcodeGen->Gen(Operation::OPR, 0, 6);
    }
    else  
    {
        errorLog("Expected <exp> or 'odd'.");
        return ;
    }
}

void GramAnalysis::Exp()
{
    // [<aop>]
    getWord();
    if (_eof)
    {
        errorLog("Expected part of <exp>.");
        return ;
    }
    string aop;
    if (_type != TypeMap::aop)
    {
        if (_type == TypeMap::id || _type == TypeMap::integer || 
            _word == "(")
        {
            _stall = true;
        }
        else  
        {
            _stall = true;
            return ;
        }
    }
    else aop = move(_word);

    // <term>
    Term();
    if (aop == "-")
    {
        _pcodeGen->Gen(Operation::OPR, 0, 1);
    }

    // {<aop><term>}
    while (1)
    {
        // <aop>
        getWord();
        if (_eof)
        {
            return ;
        }
        if (_type != TypeMap::aop)
        {
            _stall = true;
            break;
        }
        else aop = move(_word);

        // <term>
        getWord();
        if (_type == TypeMap::id || _type == TypeMap::integer || 
            _word == "(")
        {
            _stall = true;
        }
        else  
        {
            break;
        }
        Term();
        if (aop == "+")
        {
            _pcodeGen->Gen(Operation::OPR, 0, 2);
        }
        else  
        {
            _pcodeGen->Gen(Operation::OPR, 0, 3);
        }
    }
}

void GramAnalysis::Term()
{
    // <factor>
    Factor();

    // {<mop><factor>}
    while (1)
    {
        // <mop>
        getWord();
        if (_eof)
        {
            return ;
        }
        string mop;
        if (_type != TypeMap::mop)
        {
            _stall = true;
            break;
        }
        else mop = move(_word);

        // <factor>
        getWord();
        if (_type == TypeMap::id || _type == TypeMap::integer || 
            _word == "(")
        {
            _stall = true;
        }
        else  
        {
            break;
        }
        Factor();
        if (mop == "*")
        {
            _pcodeGen->Gen(Operation::OPR, 0, 4);
        }
        else  
        {
            _pcodeGen->Gen(Operation::OPR, 0, 5);
        }
    }
}

void GramAnalysis::Factor()
{
    // <id> | <integer> | (
    getWord();
    if (_eof)
    {
        errorLog("Expected identifier or integer or '('.");
        return ;
    }
    if (_type != TypeMap::id && _type != TypeMap::integer && 
        _word != "(")
    {
        errorLog("Expected identifier or integer or '('.");
    }
    if (_type == TypeMap::id)
    {
        if (!_symbolTable->check_rvalue(_word))
        {
            errorLog(_symbolTable->error());
        }
        else  
        {
            _varLoc = _symbolTable->get_L_A(_word);
            _pcodeGen->Gen(Operation::LOD, _varLoc.first, _varLoc.second);
        }
    }
    else if (_type == TypeMap::integer)
    {
        _pcodeGen->Gen(Operation::LIT, 0, wordToInt());
    }

    // (
    if (_word == "(")
    {
        // <exp>
        Exp();

        // )
        getWord();
        if (_eof)
        {
            errorLog("Expected ')'.");
            return ;
        }
        if (_word != ")")
        {
            if (_type == TypeMap::mop)
            {
                _stall = true;
            }
            errorLog("Expected ')'.");
        }
    }
}