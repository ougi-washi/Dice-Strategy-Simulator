// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"
#include <iostream>

#define INITIAL_BALANCE 1.f
#define INITAL_BET INITIAL_BALANCE/1000
#define RUNS_COUNT 1000000
#define SIMULATIONS_COUNT 100

i32 main(i32 argc, const c8 *argv[]){
    
    dss::state state = dss::make_state(INITIAL_BALANCE, INITAL_BET);
    std::vector<f32> paroli_var = {
        0, 
    };
    dss::strategy paroli_strategy = [](dss::state& state, std::vector<f32>& variables){
        if (state.win_streak > 0 && state.win_streak < 2){
            state.bet *= 2;
        }
        else {
            state.bet = state.initial_bet;
        }
    };
    
    std::cout << "Paroli - strat" << std::endl;
    std::cout << "Runs: " << RUNS_COUNT << std::endl;
    std::cout << "Simulations: " << SIMULATIONS_COUNT << std::endl;
    std::cout << "Initial Balance: " << INITIAL_BALANCE << std::endl; 
    std::cout << "Initial Bet: " << INITAL_BET << std::endl;
    
    dss::simulations_output output = dss::run_simulations(state, paroli_strategy, paroli_var, RUNS_COUNT, SIMULATIONS_COUNT, false);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}
