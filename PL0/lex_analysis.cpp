#include "lex_analysis.h"

using namespace std;

LexAnalysis::LexAnalysis()
{
    wcout.imbue(locale());
    _row = 1;
    _col = 0;

    _wch = L'0';
    _ch = '0';
    _lex = false;
    _eof = false;
    _over = false;

    _word.reserve(30);
    _type.reserve(10);
}

LexAnalysis::~LexAnalysis()
{
    closeFile();
}

bool LexAnalysis::isSysKey() const
{
    for (int i = 0; i < 19; i++)
    {
        if (_word == SysKey[i])
        {
            return true;
        }
    }
    return false;
}

bool LexAnalysis::isID() const 
{
    if (!isCharacter(_word[0]))
    {
        return false;
    }
    for (int i = 1; i < _word.size(); i++)
    {
        if (!isCharacter(_word[0]) && !isNumber(_word[0]))
        {
            return false;
        }
    }
    return true;
}

bool LexAnalysis::isInteger() const 
{
    if (_word.size() == 1)
    {
        if (_word[0] >= '0' && _word[0] <= '9')
        {
            return true;
        }
        else return false;
    }
    if (_word[0] <= '0' || _word[0] > '9')
    {
        return false;
    }
    for (int i = 1; i < _word.size(); i++)
    {
        if (!isNumber(_word[i]))
        {
            return false;
        }
    }
    return true;
}

void LexAnalysis::typeAnalysis()
{
    if (isSysKey())
    {
        _type = TypeMap::syskey;
    }
    else if (isID())
    {
        _type = TypeMap::id;
    }
    else if (isInteger())
    {
        _type = TypeMap::integer;
    }
    else 
    {
        if (_word == ":=")
        {
            _type = TypeMap::assign;
        }
        else if (
            _word == "=" || _word == "<>" ||
            _word == "<" || _word == "<=" ||
            _word == ">" || _word == ">="
        )
        {
            _type = TypeMap::lop;
        }
        else if (_word == "+" || _word == "-")
        {
            _type = TypeMap::aop;
        }
        else if (_word == "*" || _word == "/")
        {
            _type = TypeMap::mop;
        }
        else _type = TypeMap::error;
    }
}

bool LexAnalysis::tick()
{
    _word.clear();
    while (!_eof && !_inFile.eof())
    {
        if (_ch == '\n')
        {
            _row++;
            _col = 0;
        }

        _inFile.get(_wch);
        if (!_inFile)
        {
            _eof = true;
            break;
        }
        _col++;
        if (_wch > 0x7F)
        {
            errorLog(wcout);
            _word.clear();
            continue;
        }
        _ch = (uint8_t)_wch;

        if (_ch == '\n' || _ch == '\r' || _ch == ' ' )
        {
            if (_word.size() != 0)
            {
                typeAnalysis();
                _lex = true;
            }
        }
        else if (_ch == ':')
        {
            if (_word.size() != 0)
            {
                typeAnalysis();
                _lex = true;
                _inFile.seekg(-1, ios::cur);
                _col--;
            }
            else  
            {
                _word += _ch;

                _inFile.get(_wch);
                if (!_inFile)
                {
                    _eof = true;
                    break;
                }
                _col++;
                if (_wch > 0x7F)
                {
                    errorLog(wcout);
                    _ch = ' ';
                }
                else _ch = (uint8_t)_wch;

                if (_ch == '=')
                {
                    _word += _ch;
                    typeAnalysis();
                    _lex = true;
                }
                else  
                {
                    typeAnalysis();
                    _lex = true;
                    _inFile.seekg(-1, ios::cur);
                    _col--;
                }
            }
        }
        else if (_ch == '=' || _ch == '+' || _ch == '-' || 
                _ch == '*' || _ch == '/' || _ch == ',' || 
                _ch == ';' || _ch == '(' || _ch == ')')
        {
            if (_word.size() != 0)
            {
                typeAnalysis();
                _lex = true;
                _inFile.seekg(-1, ios::cur);
                _col--;
            }
            else  
            {
                _word += _ch;
                typeAnalysis();
                _lex = true;
            }
        }
        else if (_ch == '<')
        {
            if (_word.size() != 0)
            {
                typeAnalysis();
                _lex = true;
                _inFile.seekg(-1, ios::cur);
                _col--;
            }
            else  
            {
                _word += _ch;

                _inFile.get(_wch);
                if (!_inFile)
                {
                    _eof = true;
                    break;
                }
                _col++;
                if (_wch > 0x7F)
                {
                    errorLog(wcout);
                    _ch = ' ';
                }
                else _ch = (uint8_t)_wch;

                if (_ch == '=' || _ch == '>')
                {
                    _word += _ch;
                    typeAnalysis();
                    _lex = true;
                }
                else  
                {
                    typeAnalysis();
                    _lex = true;
                    _inFile.seekg(-1, ios::cur);
                    _col--;
                }
            }
        }
        else if (_ch == '>')
        {
            if (_word.size() != 0)
            {
                typeAnalysis();
                _lex = true;
                _inFile.seekg(-1, ios::cur);
                _col--;
            }
            else  
            {
                _word += _ch;

                _inFile.get(_wch);
                if (!_inFile)
                {
                    _eof = true;
                    break;
                }
                _col++;
                if (_wch > 0x7F)
                {
                    errorLog(wcout);
                    _ch = ' ';
                }
                else _ch = (uint8_t)_wch;

                if (_ch == '=')
                {
                    _word += _ch;
                    typeAnalysis();
                    _lex = true;
                }
                else  
                {
                    typeAnalysis();
                    _lex = true;
                    _inFile.seekg(-1, ios::cur);
                    _col--;
                }
            }
        }
        else  
        {
            _word += _ch;
        }

        if (_lex)
        {
            _lex = false;

            if (_type == TypeMap::error)
            {
                errorLog(cout);
                return false;
            }
            else  
            {
                lexLog();
                return true;
            }
        }
    }

    if (_eof)
    {
        if (_word.size() != 0)
        {
            typeAnalysis();
            if (_type == TypeMap::error)
            {
                errorLog(cout);
                return false;;
            }
            else  
            {
                lexLog();
                return true;
            }
        }
        else  
        {
            _over = true;
            _word.clear();
            return false;
        }
    }
    return false;
}

bool LexAnalysis::setFileName(const char *fileName)
{
    _inFile.open(fileName, ios::in);
    if (!_inFile)
    {
        cout << "Can't Open source file." << endl;
        return false;
    }

    int n = strlen(fileName);
    _ofileName.reserve(n);

    for (int i = 0; i < n; i++)
    {
        if (fileName[i] == '.')
        {
            break;
        }
        else
        {
            _ofileName += fileName[i];
        }
    }
    _ofileName += ".o";

    _outLog.open(_ofileName, ios::out);

    return true;
}

bool LexAnalysis::setFileName(string fileName)
{
    _inFile.open(fileName, ios::in);
    if (!_inFile)
    {
        cout << "Can't Open source file." << endl;
        return false;
    }

    int n = fileName.size();
    _ofileName.reserve(n);

    for (int i = 0; i < n; i++)
    {
        if (fileName[i] == '.')
        {
            break;
        }
        else
        {
            _ofileName += fileName[i];
        }
    }
    _ofileName += ".o";

    _outLog.open(_ofileName, ios::out);

    return true;
}