#include <iostream>

struct Piece {

  struct PieceHash {
    std::size_t operator()(const Piece &piece) const {
      auto h1 = std::hash<int>{}(piece.pos_i);
      auto h2 = std::hash<int>{}(piece.pos_j);
      return h1 ^ h2;
    }
  };

  Piece() = default;
  Piece(int i, int j, char side) : pos_i(i), pos_j(j), value(side) {}

  bool operator==(const Piece &other) const {
    return pos_j == other.pos_j && pos_i == other.pos_i;
  }

  int pos_i = 0;
  int pos_j = 0;
  char value = ' ';
  bool king = false;
};