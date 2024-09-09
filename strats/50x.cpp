// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi
// Strategy inspired by AurumAeternus https://www.publish0x.com/@AngelCasas

#include "dss.h"
#include <iostream>

#define INITIAL_BALANCE 10.f
#define INITAL_BET INITIAL_BALANCE/100.f
#define RUNS_COUNT 1000
#define SIMULATIONS_COUNT 1000

i32 main(i32 argc, const c8 *argv[]){
    
    dss::state state = dss::make_state(INITIAL_BALANCE, INITAL_BET);
    std::vector<f32> strat50x_variables = {
        100, // first loss threshold
        150, // second loss threshold
    };
    dss::strategy strat50x_strategy = [](dss::state& state, std::vector<f32>& variables){
        const f32& first_loss_threshold = variables[0];
        const f32& second_loss_threshold = variables[1];
        if (state.win_streak == 1){
            state.bet = state.initial_bet;
        }
        if (state.lose_streak == first_loss_threshold){
            // reset seed ?? (I believe we are generating a new seed every time we call get_random in the run_strategy function)
        }
        if (state.lose_streak == second_loss_threshold){
            state.bet = INITIAL_BALANCE/200.f;
            // increase payout ?
        }
        if (state.lose_streak < second_loss_threshold){
            state.bet += state.bet * 0.021f; // increase bet by 2.1%
        }
        if (state.lose_streak > second_loss_threshold){
            state.bet += state.bet; // double bet 
        }
    };
    
    std::cout << "Running 50x Strategy with " << RUNS_COUNT << " runs and " << SIMULATIONS_COUNT << " simulations" << std::endl;
    std::cout << "Initial Balance: " << INITIAL_BALANCE << " | Initial Bet: " << INITAL_BET << std::endl;
    dss::simulations_output output = dss::run_simulations(state, strat50x_strategy, strat50x_variables, RUNS_COUNT, SIMULATIONS_COUNT);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}