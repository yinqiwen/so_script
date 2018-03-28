/*
 * test.c
 *
 *  Created on: 2018Äê3ÔÂ28ÈÕ
 *      Author: qiyingwang
 */

#include "test_func.h"
#include <string>

extern "C"
{
    int hello_func(int v, void* arg)
    {
        std::string* arg_str = (std::string*)arg;
        printf("###hello_func:%d %s\n", v, arg_str->c_str());
        test_func1();
        test_func2();
        return -1;
    }
}

