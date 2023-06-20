#include <iostream>
#include <sstream>
#include <cstdint>

#include "./state.hpp"
#include "../config.hpp"


const int wKingTableMid[6][5] = {
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-10,-20,-20,-20,-10},
     { 0,  0,  0,  0,  0},
     {20, 30,  0, 30, 20}
};
const int wQueenTable[6][5] = {
    {-20,-10, -5,-10,-20},
    {-10,  0,  0,  0,-10},
    {-10,  0,  0,  0,-10},
    {-10,  0,  0,  0,-10},
    {-10,  0,  0,  0,-10},
    {-20,-10, -5,-10,-20}
};
const int wRookTable[6][5] = {
    {  0,  0,  0,  0,  0},
    {  5, 10, 10, 10,  5},
    { -5,  0,  0,  0, -5},
    { -5,  0,  0,  0, -5},
    { -5,  0,  0,  0, -5},
    {  0,  0,  5,  0,  0}
};
const int wBishopTable[6][5] = {
    {-20,-10,-10,-10,-20},
    {-10,  0,  0,  0,-10},
    {-10,  0, 10,  0,-10},
    {-10, 10, 10, 10,-10},
    {-10,  5,  0,  5,-10},
    {-20,-10,-10,-10,-20}
};
const int wKnightTable[6][5] = {
    {-50,-40,-30,-40,-50},
    {-40,-20,  0,-20,-40},
    {-30,  0, 15,  0,-30},
    {-30,  5, 15,  5,-30},
    {-40,-20,  5,-20,-40},
    {-50,-40,-30,-40,-50}
};
const int wPawnTable[6][5] = {
    {  0,  0,  0,  0,  0},
    { 50, 50, 50, 50, 50},
    {  5, 10, 25, 10,  5},
    {  0,  0, 20,  0,  0},
    { 10, 10,-20, 10, 10},
    {  0,  0,  0,  0,  0},
};

// Black's Piece-Square Tables
const int bKingTableMid[6][5] = {
    {20, 30,  0, 30, 20},
    {  0,  0,  0,  0, 20},
    {-10,-20,-20,-20,-10},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30}
};

const int bQueenTable[6][5] = {
  {  -20,-10, -5,-10,-20},
  {  -10,  0,  0,  0,-10},
  {  -10,  0,  0,  0,-10},
  {  -10,  0,  0,  0,-10},
  {  -10,  0,  0,  0,-10},
  {  -20,-10, -5,-10,-20}
};
const int bRookTable[6][5] = {
    {  0,  0,  5,  0,  0},
    { -5,  0,  0,  0, -5},
    { -5, 10, 10, 10, -5},
    { -5,  0, 20,  0, -5},
    {  5, 10, 10, 10,  5},
    {  0,  0,  0,  0,  0}
};
const int bBishopTable[6][5] = {
    {-20,-10,-10,-10,-20},
    {-10,  5,  5,  5,-10},
    {-10, 10, 10, 10,-10},
    {-10,  0, 10,  0,-10},
    {-10,  0,  0,  0,-10},
    {-20,-10,-10,-10,-20}
};
const int bKnightTable[6][5] = {
    {-50,-40,-30,-40,-50},
    {-40,-20,  5,-20,-40},
    {-30,  5, 15,  5,-30},
    {-30,  0, 15,  0,-30},
    {-40,-20,  0,-20,-40},
    {-50,-40,-30,-40,-50}
};
const int bPawnTable[6][5] = {
    {  0,  0,  0,  0,  0},
    {  5, 10,-20, 10,  5},
    {  5, -5,  0, -5,  5},
    { 10, 10, 30, 10, 10},
    { 50, 50, 50, 50, 50},
    { 50, 50, 50, 50, 50}
};


/**
 * @brief evaluate the state
 * 
 * @return int 
 */
int State::evaluate(){
  static const int chess_value[7] = {0, 100, 300, 350, 400, 1000, 5000};
  int value = 0; 
  for(int i=0 ; i< BOARD_H ; i++) {
    for(int j=0 ; j<BOARD_W ; j++) {
      int my_chess = this->board.board[this->player][i][j];
      int op_chess = this->board.board[1 - this->player][i][j];
      value += chess_value[my_chess];   
      value -= chess_value[op_chess];   
      switch (my_chess)
      {
        case 1:
          if(this->player == 0) value += wPawnTable[i][j];
          else value += bPawnTable[i][j];
          break;
        case 2:
          if(this->player == 0) value += wRookTable[i][j];
          else value += bRookTable[i][j];
          break;
        case 3:
          if(this->player == 0) value += wKnightTable[i][j];
          else value += bKnightTable[i][j];
          break;
        case 4:
          if(this->player == 0) value += wBishopTable[i][j];
          else value += bBishopTable[i][j];
          break;
        case 5:
          if(this->player == 0) value += wQueenTable[i][j];
          else value += bQueenTable[i][j];
          break;
        case 6:
          if(this->player == 0) value += wKingTableMid[i][j];
          else value += bKingTableMid[i][j];
          break;
        default:
          break;
      }
    }
  }
  return value;
}


/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  //promotion for pawn
  if(moved == 1 && (to.first==BOARD_H-1 || to.first==0)){
    moved = 5;
  }
  if(next.board[1-this->player][to.first][to.second]){
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player);
  
  if(this->game_state != WIN)
    next_state->get_legal_actions();
  return next_state;
}


static const int move_table_rook_bishop[8][7][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};


/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece=self_board[i][j])){
        // std::cout << this->player << "," << now_piece << ' ';
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<BOARD_H-1){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j])
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i+1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i-1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<std::max(BOARD_H, BOARD_W); k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>=BOARD_H || x<0 || y>=BOARD_W || y<0) continue;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) continue;
              now_piece = self_board[p[0]][p[1]];
              if(now_piece) continue;
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  std::cout << "\n";
  this->legal_actions = all_actions;
}


//const char piece_table[2][7][5] = {
//  {" ", "♙", "♖", "♘", "♗", "♕", "♔"},
//  {" ", "♟", "♜", "♞", "♝", "♛", "♚"}
//};
/**
 * @brief encode the output for command line output
 * 
 * @return std::string 
 */
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << std::string(PIECE_TABLE[0][now_piece]);
      }else if((now_piece = this->board.board[1][i][j])){
        ss << std::string(PIECE_TABLE[1][now_piece]);
      }else{
        ss << " ";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}


/**
 * @brief encode the state to the format for player
 * 
 * @return std::string 
 */
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<BOARD_H; i+=1){
      for(int j=0; j<BOARD_W; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}