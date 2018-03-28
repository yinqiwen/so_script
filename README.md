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

    s.Build();
    if (!s.GetBuildError().empty())
    {
        printf("####%s\n", s.GetBuildError().c_str());
    }
    else
    {
        EntryFunc entry = (EntryFunc) s.GetFunc("hello_func");
        entry(101, NULL);
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

    s.Build();
    if (!s.GetBuildError().empty())
    {
        printf("####%s\n", s.GetBuildError().c_str());
    }
    else
    {
        EntryFunc entry = (EntryFunc) s.GetFunc("hello_func");
        std::string arg = "hello, world!";
        entry(102, &arg);
    }
}
```

## Embedding so_script

Just copy the source files into your project. 