// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"
#include "curve.h"
#include <random>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#define DSS_CURVE_HEIGHT 6
#define DSS_CURVE_WIDTH 160

#define DSS_DEBUG_STRATEGY 0

dss::state dss::make_state(const f32 balance, const f32 initial_bet){
        dss::state state = {};
        state.balance = balance;
        state.max_reached_balance = balance;
        state.min_reached_balance = balance;
        state.bet = initial_bet;
        state.initial_bet = initial_bet;
        state.win_chance = 0.495f;
        return state;
}

b8 dss::run_strategy(dss::state &state, dss::strategy &strategy, std::vector<f32> &variables){
    srand (time(NULL));
    if (dss::get_random() < state.win_chance){
        state.win_streak++;
        state.lose_streak = 0;
        state.balance += state.bet * calculate_multiplier(state.win_chance);
    } else {
        state.lose_streak++;
        state.win_streak = 0;
        state.balance -= state.bet * calculate_multiplier(state.win_chance);
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

void dss::run_simulation(dss::simulation_output& sim_output, dss::state &state, dss::strategy &strategy, std::vector<f32> &variables, const u32 iterations){
    sim_output.balance_data.clear();
    for (u32 i = 0; i < iterations; i++){
        const b8 success = run_strategy(state, strategy, variables);
        sim_output.final_balance = state.balance;
        sim_output.max_reached_balance = state.max_reached_balance;
        sim_output.min_reached_balance = state.min_reached_balance;
        sim_output.balance_data.push_back(state.balance);
#if DSS_DEBUG_STRATEGY
        std::cout << "Iteration: " << i << " | " << dss::state_to_string(state) << std::endl;
#endif
        if (!success){
            break;
        }
    }
}

std::vector<f32> aggregate_balance_data(const std::vector<dss::simulation_output> &sim_outputs) {
    if (sim_outputs.empty()) {
        return {}; // Handle case where no simulation outputs are provided
    }
    size_t data_size = sim_outputs[0].balance_data.size();
    std::vector<f32> aggregated_data(data_size, 0.0f);
    for (const auto &sim_output : sim_outputs) {
        for (size_t i = 0; i < sim_output.balance_data.size(); ++i) {
            aggregated_data[i] += sim_output.balance_data[i];
        }
    }
    for (size_t i = 0; i < aggregated_data.size(); ++i) {
        aggregated_data[i] /= sim_outputs.size();
    }
    return aggregated_data;
}

std::vector<f32> downsample_data(const std::vector<f32> &data, size_t const target_size) {
    if (data.empty() || target_size == 0) {
        return {}; // Handle case where no data is provided
    }

    std::vector<f32> downsampled_data;
    if (data.empty()) {
        return downsampled_data;
    }

    size_t input_size = data.size();
    if (input_size <= target_size) {
        return data; // No need to downsample if input size is already smaller than or equal to target size
    }

    size_t step = input_size / target_size;
    size_t remainder = input_size % target_size;
    size_t start = 0;

    for (size_t i = 0; i < target_size; ++i) {
        size_t end = start + step + (i < remainder ? 1 : 0);
        f32 sum = 0.0f;
        size_t count = 0;

        for (size_t j = start; j < end; ++j) {
            sum += data[j];
            count++;
        }

        downsampled_data.push_back(sum / count);
        start = end;
    }

    return downsampled_data;
}

std::vector<f32> smooth_data(const std::vector<f32> &data, size_t window_size) {
    std::vector<f32> smoothed_data(data.size(), 0.0f);
    size_t half_window = window_size / 2;

    for (size_t i = 0; i < data.size(); ++i) {
        f32 sum = 0.0f;
        size_t count = 0;
        for (size_t j = i; j < data.size() && j < i + half_window; ++j) {
            sum += data[j];
            count++;
        }
        for (size_t j = i; j > 0 && j >= i - half_window; --j) {
            sum += data[j];
            count++;
        }
        smoothed_data[i] = sum / count;
    }
    return smoothed_data;
}

std::vector<f32> calculate_average_movement(const std::vector<dss::simulation_output> &sim_outputs) {
    if (sim_outputs.empty()) {
        return {}; // Handle case where no simulation outputs are provided
    }

    size_t data_size = sim_outputs[0].balance_data.size();
    std::vector<f32> average_movement(data_size, 0.0f);
    for (const auto &sim_output : sim_outputs) {
        for (size_t i = 0; i < sim_output.balance_data.size(); ++i) {
            average_movement[i] += sim_output.balance_data[i];
        }
    }
    for (size_t i = 0; i < average_movement.size(); ++i) {
        average_movement[i] /= sim_outputs.size();
    }
    return average_movement;
}

dss::simulations_output dss::run_simulations(dss::state &state, dss::strategy &strategy, std::vector<f32> &variables, const u32 iterations, const u32 threads_count, const b8 draw_curve){
    dss::simulations_output final_output = {};
    std::vector<dss::state> states(threads_count, state);
    std::vector<std::thread> thread_pool;
    thread_pool.reserve(threads_count); 
    std::vector<dss::simulation_output> sim_outputs = {};
    sim_outputs.reserve(threads_count);
    sim_outputs.resize(threads_count);
    const f32 initial_balance = state.balance;
    for (u32 i = 0; i < threads_count; i++){
        dss::state& state = states[i];
        dss::simulation_output& sim_output = sim_outputs[i];
        thread_pool.push_back(std::thread([&sim_output, &state, &strategy, &variables, iterations = iterations, &initial_balance](){
            dss::run_simulation(sim_output, state, strategy, variables, iterations);
#if DSS_DEBUG_STRATEGY
            std::cout << "Thread: " << std::this_thread::get_id() << "| PNL: " << state.balance - initial_balance << std::endl;
#endif
        }));
    }
    for (u32 i = 0; i < threads_count; i++){
        thread_pool[i].join();
    }

    final_output.PNL = 0.f;
    final_output.breaking_count = 0;
    final_output.max_reached_balance = initial_balance;
    final_output.min_reached_balance = initial_balance;
    final_output.average_final_balance = 0.f;
    final_output.average_max_reached_balance = 0.f;
    final_output.average_min_reached_balance = 0.f;
    
    for (u32 i = 0; i < threads_count; i++){
        final_output.PNL += sim_outputs[i].final_balance - initial_balance;
        final_output.average_final_balance += sim_outputs[i].final_balance;
        final_output.average_max_reached_balance += sim_outputs[i].max_reached_balance;
        final_output.average_min_reached_balance += sim_outputs[i].min_reached_balance;
        if (sim_outputs[i].max_reached_balance > final_output.max_reached_balance){
            final_output.max_reached_balance = sim_outputs[i].max_reached_balance;
        }
        if (sim_outputs[i].min_reached_balance < final_output.min_reached_balance){
            final_output.min_reached_balance = sim_outputs[i].min_reached_balance;
        }
        if (sim_outputs[i].final_balance == 0){
            final_output.breaking_count++;
        }
    }
    final_output.average_final_balance /= threads_count;
    final_output.average_max_reached_balance /= threads_count;
    final_output.average_min_reached_balance /= threads_count;

    std::vector<f32> balance_data;
    for (u32 i = 0; i < threads_count; i++) {
        balance_data.push_back(sim_outputs[i].final_balance);
    }

    if (draw_curve){
        std::vector<f32> average_movement = calculate_average_movement(sim_outputs);
        std::vector<f32> downsampled_data = downsample_data(average_movement, DSS_CURVE_WIDTH);
        dss::draw_curve(downsampled_data, DSS_CURVE_HEIGHT);
    }
    
    return final_output;
}

f32 dss::get_random(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<f32> dist(0.f, 1.f);
    return dist(gen);
}

f32 dss::calculate_multiplier(f32 win_chance, f32 house_edge){
    return (100.0f - house_edge) / win_chance;
}

void dss::set_bet(dss::state &state, const f32 bet){
    state.bet = bet;
}

void dss::reset_bet_to_initial(dss::state &state){
    state.bet = state.initial_bet;
}

std::string dss::balance_to_string(const dss::state &state){
    return  "Balance: " + std::to_string(state.balance) + 
            "| Max: " + std::to_string(state.max_reached_balance) +
            "| Min: " + std::to_string(state.min_reached_balance);
}

void dss::set_win_chance(dss::state &state, const f32 win_chance){
    state.win_chance = win_chance;
}

void dss::reset_win_chance(dss::state &state){
    state.win_chance = 0.495f;
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
    return  "PNL: " + std::to_string(output.PNL) +
            "| Average Final Balance: " + std::to_string(output.average_final_balance) + 
            "| Max Reached Balance: " + std::to_string(output.max_reached_balance) +
            "| Min Reached Balance: " + std::to_string(output.min_reached_balance) +
            "| Average Max Reached Balance: " + std::to_string(output.average_max_reached_balance) +
            "| Average Min Reached Balance: " + std::to_string(output.average_min_reached_balance) +
            "| Breaking Count: " + std::to_string(output.breaking_count);
}
