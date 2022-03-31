#include <iostream>
#include <vector>

class Player {
public:
  Player(char side) : your_side(side), opponent_side(side == 'w' ? 'b' : 'w') {}

  void MakeTurn(Board &board) {
    std::cout << board;
    Piece chosen;
    Board::Move move;
    while (true) {
      std::cout
          << "Your turn! Choose piece and enter position in format: i j. i "
             "- row, j - column:\n";
      int i, j;
      std::cin >> i >> j;
      if (i < 1 || i > 8 || j < 1 || j > 8) {
        std::cout << "Incorrect input. Try again.\n";
        continue;
      }
      --i;
      --j;
      chosen = board.GetPieces()[i][j];
      if (chosen.value != your_side) {
        std::cout << "It's not your piece. Choose another one.\n";
        continue;
      }
      break;
    }
    //      std::cout << "Would you like to beat some pieces? "
    std::cout << "Enter move in format: i j. i - row, j - column.\n"
                 "If you beat one or more pieces, you should enter your move "
                 "step by step adding position of piece being beat and finish "
                 "move by entering -1 -1:\n";
    move.to_move = chosen;
    while (true) {
      int i, j;
      std::cin >> i >> j;
      if (i == -1 || j == -1) {
        break;
      }
      --i;
      --j;
      if (i < 0 || i > 7 || j < 0 || j > 7) {
        std::cout << "Incorrect input. Try again.\n";
        continue;
      }
      if ((chosen.pos_j == j && chosen.pos_i == i) ||
          board.GetPieces()[i][j].value != ' ') {
        std::cout
            << "You can move your pieces only to empty cells. Try again.\n";
        continue;
      }
      if (!chosen.king && i > chosen.pos_i) {
        std::cout << "You cannot move back non-king piece. Try again.\n";
        continue;
      }
      if (!chosen.king &&
          (std::abs(i - chosen.pos_i) > 2 && std::abs(j - chosen.pos_j) > 2)) {
        std::cout << "You can move non-king piece only to adjacent cells.\n";
        continue;
      }
      if (std::abs(i - chosen.pos_i) != std::abs(j - chosen.pos_j)) {
        std::cout << "You can move pieces only along diagonals.\n";
        continue;
      }
      if (std::abs(i - chosen.pos_i) == 1 && std::abs(j - chosen.pos_j) == 1) {
        move.dest = {i, j};
        break;
      }
      if (std::abs(i - chosen.pos_i) > 1 && std::abs(j - chosen.pos_j) > 1) {
        std::cout << "Would you like to beat some pieces? (Yes/No)\n";
        std::string ans;
        std::cin >> ans;
        if (ans == "Yes") {
          std::cout << "Choose piece to beat: \n";
          int beat_i, beat_j;
          if (beat_j != -1 || beat_i != -1) {
            std::cin >> beat_i >> beat_j;
            --beat_i;
            --beat_j;
            if (board.GetPieces()[beat_i][beat_j].value != opponent_side ||
                std::abs(beat_j - chosen.pos_j) > std::abs(beat_j - j) ||
                std::abs(beat_i - chosen.pos_i) > std::abs(beat_i - i)) {
              std::cout
                  << "You cannot beat your pieces or empty cells. Enter your "
                     "move again.\n";
              continue;
            }
            move.dest = {i, j};
            move.killed.emplace(beat_i, beat_j, opponent_side);
            ++move.killed_pieces_amt;
            move.killed_kings_amt += board.GetPieces()[beat_i][beat_j].king;
          }
        } else if (ans == "No") {
          move.dest = {i, j};
          break;
        }
      }
    }
    board.MakeMove(move);
    std::cout << board;
  }

private:
  char your_side;
  char opponent_side;
};
