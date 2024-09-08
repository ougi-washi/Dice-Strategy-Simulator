#pragma once

#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <functional>
#include <thread>
#include <vector>
#include <ctime>
#include <string>

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
        u32 win_streak;
        u32 lose_streak;
    };
    
    typedef std::function<void(dss::state& state, std::vector<f32>& variables)> strategy;

    dss::state make_state(const f32 balance, const f32 initial_bet);
    b8 run_strategy(dss::state& state, dss::strategy& strategy, std::vector<f32>& variables);
    void run_simulation(dss::state& state, dss::strategy& strategy, std::vector<f32>& variables, const u32 iterations);
    std::string balance_to_string(dss::state& state);
};
