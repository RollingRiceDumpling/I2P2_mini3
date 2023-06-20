#pragma once
#include <unordered_map>
#include "../state/state.hpp"


/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */
class submission{
public:
  static int alphabeta_value(State* state, int depth, int alpha, int beta, bool maximizing_player, std::unordered_map<std::string, int>& transposition_table);
  static Move get_move(State *state, int depth);
};