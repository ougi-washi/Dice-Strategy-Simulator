// Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

#include "dss.h"

dss::state dss::make_state(const f32 balance, const f32 initial_bet){
        dss::state state = {};
        state.balance = balance;
        state.max_reached_balance = balance;
        state.min_reached_balance = balance;
        state.bet = initial_bet;
        return state;
}

b8 dss::run_strategy(dss::state &state, dss::strategy &strategy, std::vector<f32> &variables){
    std::srand(std::time(nullptr));
    const f32 random_val = (f32)std::rand() / (f32)RAND_MAX;
    if (random_val > .495f){
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
        if (!run_strategy(state, strategy, variables)){
            return; // no more balance
        }
    }
}

std::string dss::balance_to_string(dss::state &state){
    return  "Balance: " + std::to_string(state.balance) + 
            "| Max: " + std::to_string(state.max_reached_balance) +
            "| Min: " + std::to_string(state.min_reached_balance);
}
