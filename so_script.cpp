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
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <strings.h>
#include <time.h>
namespace so_script
{
    static bool has_suffix(const std::string& str, const std::string& suffix)
    {
        if (str.size() < suffix.size())
        {
            return false;
        }
        return str.rfind(suffix) == str.size() - suffix.size();
    }
    Script::Script()
            : work_dir("/tmp/so_script"), so_handler(NULL)
    {
    }
    void Script::SetBuildError(const std::string& err)
    {
        build_error = err;
    }
    const std::string& Script::GetBuildError()
    {
        return build_error;
    }
    void* Script::GetFunc(const std::string& func)
    {
        if (NULL != so_handler)
        {
            return dlsym(so_handler, func.c_str());
        }
        return NULL;
    }
    void Script::SetWorkDir(const std::string& dir)
    {
        work_dir = dir;
    }
    int Script::AddInclude(const std::string& inc)
    {
        incs.push_back(inc);
        return 0;
    }
    int Script::AddSource(const std::string& src)
    {
        srcs.push_back(src);
        return 0;
    }
    int Script::AddCompileFlag(const std::string& flag)
    {
        compile_flags.push_back(flag);
        return 0;
    }

    int Script::Build()
    {
        std::string compile_cmd;
        bool have_cpp = false;
        for (size_t i = 0; i < compile_flags.size(); i++)
        {
            compile_cmd.append(compile_flags[i]).append(" ");
        }
        for (size_t i = 0; i < incs.size(); i++)
        {
            compile_cmd.append("-I").append(incs[i]).append(" ");
        }
        std::string so_base_name;
        for (size_t i = 0; i < srcs.size(); i++)
        {
            compile_cmd.append(srcs[i]).append(" ");
            if(has_suffix(srcs[i], ".cpp") || has_suffix(srcs[i], ".c"))
            {
                have_cpp = true;
            }
            char* fname = strdup(srcs[i].c_str());
            char* base_fname = basename(fname);
            std::string base_src_name = base_fname;
            free(fname);
            base_src_name = base_src_name.substr(0, base_src_name.find("."));
            if(!so_base_name.empty())
            {
                so_base_name.append("_");
            }
            so_base_name.append(base_src_name);
        }
        time_t now = time(NULL);
        char time_suffix[1024];
        snprintf(time_suffix, sizeof(time_suffix), "%u", now);
        so_base_name.append("_").append(time_suffix).append(".so");
        so_base_name = "lib"  + so_base_name;

        if (have_cpp)
        {
            compile_cmd = "gcc " + compile_cmd;
        }
        else
        {
            compile_cmd = "g++ " + compile_cmd;
        }
        compile_cmd.append(" -fPIC -shared -rdynamic ");
        so_path = work_dir + "/" +  so_base_name;
        compile_cmd.append("-o ").append(so_path);
        compile_cmd.append(" 2>&1 >/dev/null");
        //printf("compile_cmd: %s\n", compile_cmd.c_str());
        build_error.clear();
        FILE *compile = popen(compile_cmd.c_str(), "r");
        char buf[1024];
        while (fgets(buf, sizeof(buf), compile) != 0)
        {
            build_error.append(buf);
        }
        pclose(compile);
        if (!build_error.empty())
        {
            return -1;
        }

        so_handler = dlopen(so_path.c_str(), RTLD_NOW);
        if (NULL == so_handler)
        {
            SetBuildError(dlerror());
        }
        return 0;
    }

    Script::~Script()
    {
        if (NULL != so_handler)
        {
            if (0 != dlclose(so_handler))
            {
                SetBuildError(dlerror());
            }

        }
        if(!so_path.empty())
        {
            remove(so_path.c_str());
        }
    }
}

