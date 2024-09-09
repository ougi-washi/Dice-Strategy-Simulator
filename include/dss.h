// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi
#pragma once

#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <functional>
#include <thread>
#include <vector>
#include <ctime>
#include <string>
#include <mutex>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef char c8;
typedef wchar_t c16;
typedef bool b8;
typedef size_t sz;

namespace dss{
    struct state {
        f32 balance;
        f32 max_reached_balance;
        f32 min_reached_balance;
        f32 bet;
        f32 initial_bet;
        u32 win_streak;
        u32 lose_streak;
    };
    
    struct simulation_output {
        f32 final_balance;
        f32 max_reached_balance;
        f32 min_reached_balance;
    };

    struct simulations_output {
        f32 PNL;
        f32 average_final_balance;
        f32 max_reached_balance;
        f32 min_reached_balance;
        f32 average_max_reached_balance;
        f32 average_min_reached_balance;
        u32 breaking_count;
    };

    typedef std::function<void(dss::state& state, std::vector<f32>& variables)> strategy;

    dss::state make_state(const f32 balance, const f32 initial_bet);
    b8 run_strategy(dss::state& state, dss::strategy& strategy, std::vector<f32>& variables);
    dss::simulation_output run_simulation(dss::state& state, dss::strategy& strategy, std::vector<f32>& variables, const u32 iterations);
    dss::simulations_output run_simulations(dss::state& state, dss::strategy& strategy, std::vector<f32>& variables, const u32 iterations, const u32 threads_count);

    f32 get_random();
    std::string balance_to_string(const dss::state& state);
    std::string state_to_string(const dss::state& state);
    std::string simulations_output_to_string(const dss::simulations_output& output);
};
