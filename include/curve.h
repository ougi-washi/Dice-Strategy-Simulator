// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi
#pragma once

#include "types.h"
#include <vector>

namespace dss{
    std::vector<i32> normalize_data(const std::vector<f32>& data, i32 height);
    void draw_curve(const std::vector<f32>& data, i32 height = 20);
};