// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"
#include <iostream>

i32 main(i32 argc, const c8 *argv[]){
    
    dss::state state = dss::make_state(10.f, 0.001f);
    std::vector<f32> strat50x_variables = {
        0.001f // initial bet
    };
    dss::strategy strat50x_strategy = [](dss::state& state, std::vector<f32>& variables){
    };
    
    dss::simulations_output output = dss::run_simulations(state, strat50x_strategy, strat50x_variables, 1000, 100);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}