#include <cstdlib>
#include <limits>
#include <unordered_map>
#include "../state/state.hpp"
#include "./alphabeta.hpp"

using TranspositionTable = std::unordered_map<std::string, int>;

int alphabeta::alphabeta_value(State* state, int depth, int alpha, int beta, bool maximizing_player, std::unordered_map<std::string, int>& transposition_table) {

  std::string state_key = state->encode_state();

  if (transposition_table.count(state_key)) {
    return transposition_table[state_key];
  }

  state->get_legal_actions();
  if (depth == 0 || !state->legal_actions.size()) {
    if(maximizing_player) {
      int eval = state->evaluate();
      transposition_table[state_key] = eval;
      return eval;
    }
    else {
      int eval = (-1) * state->evaluate();
      transposition_table[state_key] = eval;
      return eval;
    }
  }

  if (maximizing_player) {
    int val = std::numeric_limits<int>::min();

    for (const auto& action : state->legal_actions) {
      State* next_state = state->next_state(action);
      val = std::max(val, alphabeta_value(next_state, depth - 1, alpha, beta, false, transposition_table));
      alpha = std::max(alpha, val);

      delete next_state;
      if (beta <= alpha)
        break;
    }

    transposition_table[state_key] = val;
    return val;
  } else {
    int min_val = std::numeric_limits<int>::max();

    for (const auto& action : state->legal_actions) {
      State* next_state = state->next_state(action);
      min_val = std::min(min_val, alphabeta_value(next_state, depth - 1, alpha, beta, true, transposition_table));
      beta = std::min(beta, min_val);

      delete next_state;
      if (beta <= alpha)
        break;
    }

    transposition_table[state_key] = min_val;
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
  if(state->legal_actions.empty())
    state->get_legal_actions();
  
  TranspositionTable transposition_table;

  int max_val = std::numeric_limits<int>::min();
  Move best_move;

  for (const auto& action : state->legal_actions) {
    State* next_state = state->next_state(action);
    int val = alphabeta_value(next_state, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, transposition_table);

    if (val > max_val) {
      max_val = val;
      best_move = action;
    }

    delete next_state;
  }

  return best_move;  
}