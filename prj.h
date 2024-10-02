#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "file_helper.h"

using json = nlohmann::json;

namespace prj
{
    bool init(fs::path KPM, fs::path PWD);
    void load_configs(fs::path PWD);
    void update_configs(fs::path PWD);
}
