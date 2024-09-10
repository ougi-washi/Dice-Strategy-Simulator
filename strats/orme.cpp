// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi
// Strategy inspired by AurumAeternus https://www.publish0x.com/@AngelCasas

#include "dss.h"
#include <iostream>

#define INITIAL_BALANCE 10.f
#define INITAL_BET 0.00001f
#define RUNS_COUNT 10000
#define SIMULATIONS_COUNT 100

i32 main(i32 argc, const c8 *argv[]){
    
    dss::state state = dss::make_state(INITIAL_BALANCE, INITAL_BET);
    std::vector<f32> orme_variables = {
        146, // win streak threshold    | index 0
        3.1, // win chance on threshold | index 1
        100000, // Balance divisor, on win streak | index 2
        
        5, // loss streak threshold | index 3
        3.25, // bet multiplier, on loss streak | index 4
    };

    dss::strategy orme_strategy = [](dss::state& state, std::vector<f32>& variables){
        const f32& win_streak_threshold = variables[0];
        const f32& win_chance_on_threshold = variables[1];
        const f32& balance_divisor = variables[2];
        
        const f32& loss_streak_threshold = variables[3];
        const f32& bet_multiplier_on_loss_streak = variables[4];
        
        if (state.win_streak > 0){
            dss::reset_bet_to_initial(state);
            dss::reset_win_chance(state);
        }
        if (state.win_streak == win_streak_threshold){
            state.win_chance = win_chance_on_threshold / 100.f;
            state.bet = state.balance / balance_divisor;
        }
        if (state.lose_streak > loss_streak_threshold){
            state.bet += state.bet * (bet_multiplier_on_loss_streak / 100.f);
        }
    };
    
    std::cout << "Running ORME Strategy with " << RUNS_COUNT << " runs and " << SIMULATIONS_COUNT << " simulations" << std::endl;
    std::cout << "Initial Balance: " << INITIAL_BALANCE << " | Initial Bet: " << INITAL_BET << std::endl;
    dss::simulations_output output = dss::run_simulations(state, orme_strategy, orme_variables, RUNS_COUNT, SIMULATIONS_COUNT);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}