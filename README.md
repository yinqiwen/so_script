so_script
======

A C++ library to run c/c++ source code on the fly like script. 


## Limit
- Only works with LINUX/UNIX
- The running system must have GCC compiler.

## Example

```cpp
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
```

```shell
   cd test
   g++ main.cpp ../so_script.cpp -I../ -ldl -rdynamic
   ./a.out

```

## Embedding so_script

Just copy the source files without test into your project. 
