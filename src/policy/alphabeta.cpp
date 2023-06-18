#include <cstdlib>
#include <limits>
#include "../state/state.hpp"
#include "./alphabeta.hpp"


int alphabeta::alphabeta_value(State* state, int depth, int alpha, int beta, bool maximizing_player, int me) {
  if (depth == 0) {
    if(state->player == me)
    return state->evaluate();
    else return -state->evaluate();
  }

  if (maximizing_player) {
    int max_val = std::numeric_limits<int>::min();
    state->get_legal_actions();

    for (const auto& action : state->legal_actions) {
      State* next_state = state->next_state(action);
      int val = alphabeta_value(next_state, depth - 1, alpha, beta, false, me);
      max_val = std::max(max_val, val);
      alpha = std::max(alpha, val);

      delete next_state;
      if (beta <= alpha)
        break;
    }

    return max_val;
  } else {
    int min_val = std::numeric_limits<int>::max();
    state->get_legal_actions();

    for (const auto& action : state->legal_actions) {
      State* next_state = state->next_state(action);
      int val = alphabeta_value(next_state, depth - 1, alpha, beta, true, me);
      min_val = std::min(min_val, val);
      beta = std::min(beta, val);

      delete next_state;
      if (beta <= alpha)
        break;
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
Move alphabeta::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;
  int max_val = std::numeric_limits<int>::min();
  Move best_move;

  for (const auto& action : state->legal_actions) {
    State* next_state = state->next_state(action);
    int val = alphabeta_value(next_state, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, state->player);

    if (val > max_val) {
      max_val = val;
      best_move = action;
    }

    delete next_state;
  }

  return best_move;  
}