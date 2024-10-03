#pragma once

#include "includes.h"

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
