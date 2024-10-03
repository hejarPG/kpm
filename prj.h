#pragma once

#include "includes.h"
#include "file_helper.h"
#include "console_color.h"
#include "compiler.h"
#include "assembler.h"
#include "linker.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace prj
{
    bool init(fs::path KPM, fs::path PWD);
    bool out(fs::path KPM, fs::path PWD);
    bool integrate(fs::path KPM, fs::path PWD);
    void load_configs(fs::path PWD);
    void update_configs(fs::path PWD);
    void run(fs::path PWD);
    json get_configs();
}
