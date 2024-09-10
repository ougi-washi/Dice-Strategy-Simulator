// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi
#pragma once

#include "types.h"
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <functional>
#include <thread>
#include <vector>
#include <ctime>
#include <string>
#include <mutex>

namespace dss{
    struct state {
        f32 balance;
        f32 max_reached_balance;
        f32 min_reached_balance;
        f32 bet;
        f32 initial_bet;
        f32 win_chance;
        u32 win_streak;
        u32 lose_streak;
    };
    
    struct simulation_output {
        f32 final_balance;
        f32 max_reached_balance;
        f32 min_reached_balance;
        std::vector<f32> balance_data;
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
    void run_simulation(dss::simulation_output& output, dss::state& state, dss::strategy& strategy, std::vector<f32>& variables, const u32 iterations);
    dss::simulations_output run_simulations(dss::state& state, dss::strategy& strategy, std::vector<f32>& variables, const u32 iterations, const u32 threads_count, const b8 draw_curve = true);

    f32 get_random();
    f32 calculate_multiplier(f32 win_chance, f32 house_edge = 1.0);

    void set_bet(dss::state& state, const f32 bet);
    void reset_bet_to_initial(dss::state& state);
    void set_win_chance(dss::state& state, const f32 win_chance);
    void reset_win_chance(dss::state& state);
    std::string balance_to_string(const dss::state& state);
    std::string state_to_string(const dss::state& state);
    std::string simulations_output_to_string(const dss::simulations_output& output);
};
