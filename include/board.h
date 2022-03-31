#include "piece.h"

#include <iostream>
#include <unordered_set>
#include <vector>

class Board {

public:
  enum Status { WIN, LOSE, NONE };

  struct Move {
    Piece to_move;
    std::pair<int, int> dest;
    std::unordered_set<Piece, Piece::PieceHash> killed;
    int killed_pieces_amt = 0;
    int killed_kings_amt = 0;
  };

  Board()
      : pieces(std::vector<std::vector<Piece>>(8, std::vector<Piece>(8))),
        player_side('w') {
    bot_side = player_side == 'w' ? 'b' : 'w';
    FillBoard();
  };

  Board(char side)
      : pieces(std::vector<std::vector<Piece>>(8, std::vector<Piece>(8))),
        player_side(side) {
    bot_side = player_side == 'w' ? 'b' : 'w';
    FillBoard();
  }

  Board(const Board &board) = default;

  int64_t GetScore() const {
    return ai_pieces - player_pieces + ai_kings * 5 - player_kings * 5;
  }

  const std::vector<std::vector<Piece>> &GetPieces() const { return pieces; };

  std::vector<std::vector<Piece>> GetCopyPieces() const { return pieces; }

  friend std::ostream &operator<<(std::ostream &stream, const Board &board) {
    std::cout << "   ";
    for (size_t i = 0; i < 8; ++i) {
      std::cout << i + 1 << " ";
    }
    std::cout << '\n';
    for (size_t i = 0; i < 8; ++i) {
      std::cout << i + 1 << " ";
      for (size_t j = 0; j < 8; ++j) {
        std::cout << "|"
                  << (board.pieces[i][j].king ? static_cast<char>(std::toupper(
                                                    board.pieces[i][j].value))
                                              : board.pieces[i][j].value);
      }
      std::cout << "|\n";
    }
    return stream;
  }

  void MakeMove(const Move &move) {
    for (auto &kill : move.killed) {
      pieces[kill.pos_i][kill.pos_j] = Piece(kill.pos_i, kill.pos_j, ' ');
    }
    if (move.to_move.value == player_side) {
      ai_pieces -= move.killed_pieces_amt;
      ai_kings -= move.killed_kings_amt;
    } else if (move.to_move.value == bot_side) {
      player_pieces -= move.killed_pieces_amt;
      player_kings -= move.killed_kings_amt;
    }
    pieces[move.to_move.pos_i][move.to_move.pos_j] =
        Piece(move.to_move.pos_i, move.to_move.pos_j, ' ');
    pieces[move.dest.first][move.dest.second] =
        Piece(move.dest.first, move.dest.second, move.to_move.value);
    if (move.dest.first == 0 && move.to_move.value == player_side ||
        move.dest.first == 7 && move.to_move.value == bot_side) {
      pieces[move.dest.first][move.dest.second].king = true;
    }
  }

  Status GetStatus() const {
    if (ai_pieces <= 0) {
      return Status::WIN;
    } else if (player_pieces <= 0) {
      return Status::LOSE;
    } else {
      return Status::NONE;
    }
  }

private:
  void FillBoard() {
    for (size_t j = 0; j < 3; ++j) {
      for (size_t i = (j + 1) % 2; i < 8; i += 2) {
        pieces[j][i] = Piece(j, i, bot_side);
      }
    }
    for (size_t j = 7; j > 4; --j) {
      for (size_t i = (j + 1) % 2; i < 8; i += 2) {
        pieces[j][i] = Piece(j, i, player_side);
      }
    }
    for (size_t j = 7; j > 4; --j) {
      for (size_t i = j % 2; i < 8; i += 2) {
        pieces[j][i] = Piece(j, i, ' ');
      }
    }
    for (size_t j = 0; j < 3; ++j) {
      for (size_t i = j % 2; i < 8; i += 2) {
        pieces[j][i] = Piece(j, i, ' ');
      }
    }
    for (size_t j = 3; j < 5; ++j) {
      for (size_t i = 0; i < 8; ++i) {
        pieces[j][i] = Piece(j, i, ' ');
      }
    }
  }

  std::vector<std::vector<Piece>> pieces;
  int64_t ai_pieces = 12;
  int64_t player_pieces = 12;
  int64_t ai_kings = 0;
  int64_t player_kings = 0;
  char player_side;
  char bot_side;
};
