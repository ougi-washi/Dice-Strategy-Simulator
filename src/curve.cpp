// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi
#include "curve.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

std::vector<i32> dss::normalize_data(const std::vector<f32> &data, i32 height) {
    std::vector<i32> normalized_data;
    if (data.empty()) return normalized_data;

    f32 min_value = *std::min_element(data.begin(), data.end());
    f32 max_value = *std::max_element(data.begin(), data.end());

    if (max_value == min_value) {
        normalized_data.assign(data.size(), height / 2);
        return normalized_data;
    }

    for (f32 value : data) {
        i32 normalized = static_cast<i32>(((value - min_value) / (max_value - min_value)) * (height - 1));
        normalized_data.push_back(normalized);
    }
    return normalized_data;
}

void connect_points(std::vector<std::string> &graph, i32 prev_row, i32 curr_row, i32 col) {
    if (prev_row == curr_row) {
        graph[prev_row][col] = '-'; // Horizontal line
    } else if (prev_row > curr_row) {
        for (i32 r = curr_row; r <= prev_row; ++r) {
            graph[r][col] = '/'; // Vertical/Diagonal line up
        }
    } else {
        for (i32 r = prev_row; r <= curr_row; ++r) {
            graph[r][col] = '|'; // Vertical/Diagonal line down
        }
    }
}

void dss::draw_curve(const std::vector<f32> &data, i32 height) {
    if (data.empty()) return;
    if (height < 2) height = 2;
    std::vector<i32> normalized_data = normalize_data(data, height);
    std::vector<std::string> graph(height, std::string(data.size(), ' '));
    i32 prev_row = height - 1 - normalized_data[0]; // Start at the first point
    for (size_t i = 0; i < normalized_data.size(); ++i) {
        i32 curr_row = height - 1 - normalized_data[i];
        connect_points(graph, prev_row, curr_row, i);
        prev_row = curr_row;
    }
    f32 min_value = *std::min_element(data.begin(), data.end());
    f32 max_value = *std::max_element(data.begin(), data.end());
    for (i32 i = 0; i < height; ++i) {
        f32 value = min_value + (max_value - min_value) * (static_cast<f32>(height - 1 - i) / (height - 1));
        std::cout << std::setw(8) << std::fixed << std::setprecision(8) << value << " | " << graph[i] << std::endl;
    }
}