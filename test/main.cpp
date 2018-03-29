/*
 *Copyright (c) 2017-2017, yinqiwen <yinqiwen@gmail.com>
 *All rights reserved.
 *
 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Redis nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "so_script.hpp"
#include "test_func.h"
#include "test_func.hpp"

void test_func1()
{
    printf("###test_func1\n");
}

void test_func2()
{
    printf("###test_func2\n");
}

void test_c()
{
    using namespace so_script;
    Script s;
    s.AddSource("./test.c");
    s.AddInclude("./");
    s.AddCompileFlag("-D__STDC_FORMAT_MACROS");
    s.SetWorkDir("/tmp/so_script"); // the dir where so saved

    if (0 != s.Build())
    {
        printf("####%s\n", s.GetBuildError().c_str());
    }
    else
    {
        CEntryFunc entry = (CEntryFunc) s.GetFunc("hello_func");
        entry(101);
    }
}

void test_cpp()
{
    using namespace so_script;
    Script s;
    s.AddSource("./test2.cpp");
    s.AddInclude("./");
    s.AddCompileFlag("-D__STDC_FORMAT_MACROS");
    s.SetWorkDir("/tmp/so_script"); // the dir where so saved

    if (0 != s.Build())
    {
        printf("####%s\n", s.GetBuildError().c_str());
    }
    else
    {
        CppEntryFunc entry = (CppEntryFunc) s.GetFunc("hello_func");
        std::string arg = "hello, world!";
        entry(102, arg);
    }
}

int main()
{
    test_c();
    test_cpp();
    return 0;
}

