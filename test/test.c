/*
 * test.c
 *
 *  Created on: 2018Äê3ÔÂ28ÈÕ
 *      Author: qiyingwang
 */

#include "test_func.h"

int hello_func(int v, void* arg)
{
    printf("###hello_func:%d\n", v);
    test_func1();
    test_func2();
    return -1;
}

