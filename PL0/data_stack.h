#ifndef DATA_STACK_H
#define DATA_STACK_H

#include <iostream>
#include <cstring>
#include <cstdlib>
typedef unsigned int uint;

class DataStack {
private:
    int *_data;// 数据
    int *_tdata;// 数据缓冲
    uint _capacity;// 内存大小

public:
    DataStack()
    {
        _data = new int[32 + 1];
        memset(_data, 0, (sizeof(int) << 5));
        _tdata = nullptr;
        _capacity = 32;
    }
    ~DataStack()
    {
        if (_data != nullptr) 
        {
            delete[] _data;
        }
        if (_tdata != nullptr)
        {
            delete[] _tdata;
        }
        _capacity = 0;
    }
    int& operator[](uint index)
    {
        if (index > _capacity)
        {
            // std::cout << "扩栈开始" << std::endl;
            // std::cout << "target: " << index << std::endl;
            // std::cout << "capacity: " << _capacity << std::endl;
            uint ocapacity = _capacity;
            while (_capacity < index)
            {
                _capacity = _capacity >> 1;
                _capacity = (_capacity << 1) + _capacity;
            }
            // std::cout << "new capacity: " << _capacity << std::endl;
            _tdata = new int[_capacity + 1];
            if (_tdata == nullptr)
            {
                // std::cout << "Bad allocate." << std::endl;
                delete[] _data;
                exit(-1);
            }
            // std::cout << "old *data = " << (void *)_data << std::endl;
            // std::cout << "new *data = " << (void *)_tdata << std::endl;
            memset(_tdata, 0, sizeof(int) * _capacity);
            // std::cout << "栈内置零" << std::endl;
            memcpy(_tdata, _data, sizeof(int) * ocapacity);
            // std::cout << "复制数据" << std::endl;
            delete[] _data;
            // std::cout << "删除数据" << std::endl;
            _data = _tdata;
            _tdata = nullptr;
            // std::cout << "扩栈结束" << std::endl;
        }
        return _data[index];
    }
};

#endif// DATA_STACK_H