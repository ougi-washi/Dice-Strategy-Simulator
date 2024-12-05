// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"
#include <iostream>

#define INITIAL_BALANCE 10000.f
#define INITAL_BET INITIAL_BALANCE/1000000.f
#define RUNS_COUNT 100000
#define SIMULATIONS_COUNT 100

i32 main(i32 argc, const c8 *argv[]){
    
    dss::state state = dss::make_state(INITIAL_BALANCE, INITAL_BET);
    std::vector<f32> martingale_variables = {
        2.f, // bet multiplier
    };
    dss::strategy martingale_strategy = [](dss::state& state, std::vector<f32>& variables){
        const f32& bet_multiplier = variables[0];
        if (state.lose_streak > 0){
            state.bet *= bet_multiplier;
        }
        else {
            state.bet = state.initial_bet;
        }
    };
    dss::strategy delayed_martingale = [](dss::state& state, std::vector<f32>& variables){
        const f32& bet_multiplier = variables[0];
        if (state.lose_streak > 2){
            state.bet *= bet_multiplier;
        }
        else {
            state.bet = state.initial_bet;
        }
    };
    
    std::cout << "Runs: " << RUNS_COUNT << std::endl;
    std::cout << "Simulations: " << SIMULATIONS_COUNT << std::endl;
    std::cout << "Initial Balance: " << INITIAL_BALANCE << std::endl; 
    std::cout << "Initial Bet: " << INITAL_BET << std::endl;
    
    dss::simulations_output output = dss::run_simulations(state, delayed_martingale, martingale_variables, RUNS_COUNT, SIMULATIONS_COUNT, false);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}
