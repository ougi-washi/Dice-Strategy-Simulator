// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"
#include <random>
#include <stdio.h>
#include <iostream>

#define DSS_DEBUG_STRATEGY 0

dss::state dss::make_state(const f32 balance, const f32 initial_bet){
        dss::state state = {};
        state.balance = balance;
        state.max_reached_balance = balance;
        state.min_reached_balance = balance;
        state.bet = initial_bet;
        return state;
}

b8 dss::run_strategy(dss::state &state, dss::strategy &strategy, std::vector<f32> &variables){
    if (dss::get_random() > .495f){
        state.win_streak++;
        state.lose_streak = 0;
        state.balance += state.bet;
    } else {
        state.lose_streak++;
        state.win_streak = 0;
        state.balance -= state.bet;
    }
    if (state.balance < 0){
        state.balance = 0;
        return false; // break
    }
    if (state.balance > state.max_reached_balance){
        state.max_reached_balance = state.balance;
    }
    if (state.balance < state.min_reached_balance){
        state.min_reached_balance = state.balance;
    }
    strategy(state, variables);
    return true;
}

void dss::run_simulation(dss::state &state, dss::strategy &strategy, std::vector<f32> &variables, const u32 iterations){
    for (u32 i = 0; i < iterations; i++){
        const b8 output = run_strategy(state, strategy, variables);
#if DSS_DEBUG_STRATEGY
        std::cout << "Iteration: " << i << " | " << dss::state_to_string(state) << std::endl;
#endif
        if (!output){
            return; // no more balance
        }
    }
}

dss::simulations_output dss::run_simulations(dss::state &state, dss::strategy &strategy, std::vector<f32> &variables, const u32 iterations, const u32 threads){
    dss::simulations_output sim_output = {};
    std::mutex output_mutex = {};
    std::vector<dss::state> states(threads, state);
    std::vector<std::thread> thread_pool;
    for (u32 i = 0; i < threads; i++){
        dss::state& state = states[i];
        thread_pool.push_back(std::thread([&sim_output, &state, &strategy, &variables, iterations = iterations, &output_mutex](){
            dss::run_simulation(state, strategy, variables, iterations);
            std::lock_guard<std::mutex> guard(output_mutex);
            sim_output.average_final_balance += state.balance;
            sim_output.average_max_reached_balance += state.max_reached_balance;
            sim_output.average_min_reached_balance += state.min_reached_balance;
            if (state.balance == 0){
                sim_output.breaking_count++;
            }
            if (state.max_reached_balance > sim_output.max_reached_balance){
                sim_output.max_reached_balance = state.max_reached_balance;
            }
            if (state.min_reached_balance < sim_output.min_reached_balance){
                sim_output.min_reached_balance = state.min_reached_balance;
            }
        }));
    }
    for (u32 i = 0; i < threads; i++){
        thread_pool[i].join();
    }
    sim_output.average_final_balance /= threads;
    sim_output.average_max_reached_balance /= threads;
    sim_output.average_min_reached_balance /= threads;
    return sim_output;
}

f32 dss::get_random(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<f32> dist(0.f, 1.f);
    return dist(gen);
}

std::string dss::balance_to_string(const dss::state &state){
    return  "Balance: " + std::to_string(state.balance) + 
            "| Max: " + std::to_string(state.max_reached_balance) +
            "| Min: " + std::to_string(state.min_reached_balance);
}

std::string dss::state_to_string(const dss::state &state){
    return  "Balance: " + std::to_string(state.balance) + 
            "| Max: " + std::to_string(state.max_reached_balance) +
            "| Min: " + std::to_string(state.min_reached_balance) +
            "| Bet: " + std::to_string(state.bet) +
            "| Win Streak: " + std::to_string(state.win_streak) +
            "| Lose Streak: " + std::to_string(state.lose_streak);
}

std::string dss::simulations_output_to_string(const dss::simulations_output &output){
    return  "Average Final Balance: " + std::to_string(output.average_final_balance) + 
            "| Max Reached Balance: " + std::to_string(output.max_reached_balance) +
            "| Min Reached Balance: " + std::to_string(output.min_reached_balance) +
            "| Average Max Reached Balance: " + std::to_string(output.average_max_reached_balance) +
            "| Average Min Reached Balance: " + std::to_string(output.average_min_reached_balance) +
            "| Breaking Count: " + std::to_string(output.breaking_count);
}
