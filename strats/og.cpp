// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi
// Strategy inspired by AurumAeternus https://www.publish0x.com/@AngelCasas

#include "dss.h"
#include <iostream>

#define INITIAL_BALANCE 10.f
#define INITAL_BET 0.f
#define RUNS_COUNT 1000
#define SIMULATIONS_COUNT 1000

i32 main(i32 argc, const c8 *argv[]){
    
    dss::state state = dss::make_state(INITIAL_BALANCE, INITAL_BET);
    std::vector<f32> og_variables = {
        100000, // Balance divisor, on 2 losses | index 0
        318, // bet multiplier, on 3 losses | index 1
        315, // bet multiplier, on 4 losses | index 2
        312, // bet multiplier, on 5 losses | index 3
        309, // bet multiplier, on 6 losses | index 4
        49.5, // win chance, on 7 losses    | index 5
        101, // bet multiplier, on 7 losses | index 6
        100, // bet multiplier, on 8 losses | index 7
        99, // bet multiplier, on 9 losses  | index 8
        40, // win chance, on 10 losses     | index 9
    };

    dss::strategy og_strategy = [](dss::state& state, std::vector<f32>& variables){
        const f32& balance_divisor = variables[0];
        const f32& bet_multiplier_3 = variables[1];
        const f32& bet_multiplier_4 = variables[2];
        const f32& bet_multiplier_5 = variables[3];
        const f32& bet_multiplier_6 = variables[4];
        const f32& win_chance_7 = variables[5];
        const f32& bet_multiplier_7 = variables[6];
        const f32& bet_multiplier_8 = variables[7];
        const f32& bet_multiplier_9 = variables[8];
        const f32& win_chance_10 = variables[9];

        if (state.win_streak > 0){
            state.bet = state.initial_bet;
        }
        if (state.lose_streak == 2){
            state.bet = state.balance/balance_divisor;
        }
        if (state.lose_streak == 3){
            state.bet = state.bet * (bet_multiplier_3 / 100.f);
        }
        if (state.lose_streak == 4){
            state.bet = state.bet * (bet_multiplier_4 / 100.f);
        }
        if (state.lose_streak == 5){
            state.bet = state.bet * (bet_multiplier_5 / 100.f);
        }
        if (state.lose_streak == 6){
            state.bet = state.bet * (bet_multiplier_6 / 100.f);
        }
        if (state.lose_streak == 7){
            state.win_chance = win_chance_7 / 100.f;
            state.bet = state.bet * (bet_multiplier_7 / 100.f);
        }
        if (state.lose_streak == 8){
            state.bet = state.bet * (bet_multiplier_8 / 100.f);
        }
        if (state.lose_streak == 9){
            state.bet = state.bet * (bet_multiplier_9 / 100.f);
        }
        if (state.lose_streak == 10){
            state.win_chance = win_chance_10 / 100.f;
        }
    };
    
    std::cout << "Running OG Strategy with " << RUNS_COUNT << " runs and " << SIMULATIONS_COUNT << " simulations" << std::endl;
    std::cout << "Initial Balance: " << INITIAL_BALANCE << " | Initial Bet: " << INITAL_BET << std::endl;
    dss::simulations_output output = dss::run_simulations(state, og_strategy, og_variables, RUNS_COUNT, SIMULATIONS_COUNT);
    std::cout << dss::simulations_output_to_string(output) << std::endl;
    return 0;
}