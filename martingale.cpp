// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"
#include <iostream>

i32 main(i32 argc, const c8 *argv[]){
    
    dss::state state = dss::make_state(10.f, 0.0001f);
    
    std::vector<f32> martingale_variables = {
        2.f, // bet multiplier
        0.0001f // initial bet
    };
    dss::strategy martingale_strategy = [](dss::state& state, std::vector<f32>& variables){
        const f32& bet_multiplier = variables[0];
        const f32& initial_bet = variables[1];
        if (state.lose_streak > 0){
            state.bet *= bet_multiplier;
        }
        else {
            state.bet = initial_bet;
        }
    };
    
    dss::simulations_output output = dss::run_simulations(state, martingale_strategy, martingale_variables, 10000, 1000);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}
