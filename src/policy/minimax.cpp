#include <cstdlib>
#include <limits>
#include "../state/state.hpp"
#include "./minimax.hpp"



int minimax::minimax_value(State* state, int depth, bool maximizing_player, int me) {
  state->get_legal_actions();

  if (depth == 0 || !state->legal_actions.size()) {
    if(state->player == me)
    return state->evaluate();
    else return -state->evaluate();
  }

  if (maximizing_player) {
    int max_val = std::numeric_limits<int>::min();

    for (const auto& action : state->legal_actions) {
      State* next_state = state->next_state(action);
      int val = minimax_value(next_state, depth - 1, false, me);
      max_val = std::max(max_val, val);

      delete next_state;
    }

    return max_val;
  } else {
    int min_val = std::numeric_limits<int>::max();

    for (const auto& action : state->legal_actions) {
      State* next_state = state->next_state(action);
      int val = minimax_value(next_state, depth - 1, true, me);
      min_val = std::min(min_val, val);

      delete next_state;
    }

    return min_val;
  }
}


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
Move minimax::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  int max_val = std::numeric_limits<int>::min();
  Move best_move;

  for (const auto& action : state->legal_actions) {
    State* next_state = state->next_state(action);
    int val = minimax_value(next_state, depth - 1, false, state->player);

    if (val > max_val) {
      max_val = val;
      best_move = action;
    }

    delete next_state;
  }

  return best_move;  
}