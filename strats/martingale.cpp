// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"
#include <iostream>

#define INITIAL_BALANCE 10.f
#define INITAL_BET INITIAL_BALANCE/1000.f
#define RUNS_COUNT 1000
#define SIMULATIONS_COUNT 1000

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
    
    std::cout << "Running Martingale Strategy with " << RUNS_COUNT << " runs and " << SIMULATIONS_COUNT << " simulations" << std::endl;
    std::cout << "Initial Balance: " << INITIAL_BALANCE << " | Initial Bet: " << INITAL_BET << std::endl;
    dss::simulations_output output = dss::run_simulations(state, martingale_strategy, martingale_variables, RUNS_COUNT, SIMULATIONS_COUNT);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}
