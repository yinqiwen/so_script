/*
 * test_func.h
 *
 *  Created on: 2018Äê3ÔÂ28ÈÕ
 *      Author: qiyingwang
 */

#ifndef TEST_TEST_FUNC_H_
#define TEST_TEST_FUNC_H_

#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
#endif


    typedef int (*EntryFunc)(int, void*);

    void test_func1();
    void test_func2();

#ifdef __cplusplus
}
#endif



#endif /* TEST_TEST_FUNC_H_ */
