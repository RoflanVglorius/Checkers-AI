#include "player.h"

#include <iostream>

class Game {
public:
  Game(int difficulty, char side)
      : player_side(side), board(side), ai(difficulty, side), player(side) {
    if (player_side == 'w') {
      cur_turn = PLAYER;
    } else {
      cur_turn = AI;
    }
  }

  void Play() {
    while (board.GetStatus() == Board::Status::NONE) {
      if (cur_turn == PLAYER) {
        player.MakeTurn(board);
        cur_turn = AI;
      } else if (cur_turn == AI) {
        ai.MakeTurn(board);
        cur_turn = PLAYER;
      }
    }
  }

private:
  enum Turn { PLAYER, AI };

  char player_side;
  Board board;
  Bot ai;
  Player player;
  Turn cur_turn;
};
