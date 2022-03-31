#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>

using Pieces = std::vector<std::vector<Piece>>;

class Bot {
public:
  Bot(int difficulty, char player_side)
      : difficulty(difficulty), player_side(player_side),
        bot_side(player_side == 'w' ? 'b' : 'w') {}

  void MakeTurn(Board &cur_board) {
    cur_side = bot_side;
    std::cout << "AI turn. Please wait." << std::endl;
    root =
        std::make_shared<Node>(cur_board, cur_board.GetScore(), Board::Move());
    std::queue<std::shared_ptr<Node>> queue;
    queue.push(root);
    for (int k = 0; k < difficulty; ++k) {
      std::queue<std::shared_ptr<Node>> new_queue;
      for (size_t m = 0; m < queue.size(); ++m) {
        auto cur = queue.front();
        queue.pop();
        auto pieces = cur_board.GetCopyPieces();
        for (int i = 0; i < 8; ++i) {
          for (int j = 0; j < 8; ++j) {
            if (pieces[i][j].value == cur_side) {
              std::vector<Board::Move> moves = FindPossibleTurns(i, j, pieces);
              std::vector<Board> possible_boards;
              for (auto &move : moves) {
                Board new_board = cur->cur_board;
                new_board.MakeMove(move);
                cur->children.emplace_back(std::make_shared<Node>(
                    new_board, new_board.GetScore(), move));
                new_queue.push(cur->children.back());
              }
            }
          }
        }
      }
      cur_side = (cur_side == bot_side ? player_side : bot_side);
      queue = std::move(new_queue);
    }
    DFS(root, 0);
    int64_t target = -10000;
    for (auto child : root->children) {
      if (target <= child->score && !(killed && child->move.killed.empty())) {
        root->move = child->move;
        target = child->score;
      }
    }
    killed = false;
    cur_board.MakeMove(root->move);
  }

private:
  struct Node {
    Node(const Board &board, int64_t score, Board::Move move)
        : cur_board(board), score(score), move(move) {}

    Board cur_board;
    int64_t score;
    Board::Move move;
    std::vector<std::shared_ptr<Node>> children;
  };

  std::vector<Board::Move> FindPossibleTurns(int i, int j, Pieces &pieces) {
    std::vector<Board::Move> possible_moves;
    if (!pieces[i][j].king) {
      std::vector<std::pair<int, int>> vecs;
      if (pieces[i][j].value == bot_side) {
        vecs = {{1, -1}, {1, 1}};
      } else {
        vecs = {{-1, -1}, {-1, 1}};
      }
      for (const auto &vec : vecs) {
        if (i < 7 && j > 0 && i > 0 && j < 7 &&
            pieces[i + vec.first][j + vec.second].value == ' ') {
          Board::Move move;
          move.to_move = pieces[i][j];
          move.dest = {i + vec.first, j + vec.second};
          possible_moves.push_back(std::move(move));
        }
      }
    } else {
      std::vector<std::pair<int, int>> vecs{{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
      for (auto &vec : vecs) {
        for (int cur_i = i + vec.first, cur_j = j + vec.second;
             cur_i < 8 && cur_j < 8 && cur_i >= 0 && cur_j >= 0;
             cur_i += vec.first, cur_j += vec.second) {
          if (pieces[cur_i][cur_j].value != ' ') {
            break;
          }
        }
      }
    }
    std::vector<Board::Move> kill_moves;
    std::vector<Piece> to_kill =
        KillLookup(i, j, pieces, false, pieces[i][j].value == 'w' ? 'b' : 'w');
    if (!to_kill.empty()) {
      killed = true;
    }
    for (auto &killed : to_kill) {
      kill_moves.push_back(
          {pieces[i][j],
           std::pair<int, int>(killed.pos_i +
                                   ((pieces[i][j].value == bot_side) ? 1 : -1),
                               killed.pos_j + (killed.pos_j - j)),
           std::unordered_set<Piece, Piece::PieceHash>{killed}, 1,
           killed.king});
    }
    while (!kill_moves.empty()) {
      std::vector<Board::Move> new_kill_moves;
      for (auto &move : kill_moves) {
        to_kill = KillLookup(move.dest.first, move.dest.second, pieces, true,
                             pieces[i][j].value == 'w' ? 'b' : 'w');
        for (auto &killed : to_kill) {
          Board::Move new_move = move;
          if (new_move.killed.count(killed) == 1) {
            continue;
          }
          new_move.killed.emplace(killed);
          new_move.dest = {killed.pos_i + (killed.pos_i - move.dest.first),
                           killed.pos_j + (killed.pos_j - move.dest.second)};
          ++new_move.killed_pieces_amt;
          new_move.killed_kings_amt += killed.king;
          new_kill_moves.push_back(new_move);
        }
      }
      if (new_kill_moves.empty()) {
        for (auto &move : kill_moves) {
          possible_moves.push_back(move);
        }
        kill_moves.clear();
      } else {
        kill_moves = std::move(new_kill_moves);
        new_kill_moves.clear();
      }
    }
    return possible_moves;
  }

  std::vector<Piece> KillLookup(int i, int j, Pieces &pieces, bool back,
                                char side) {
    std::vector<Piece> to_kill;
    if (pieces[i][j].king) {
      std::vector<std::pair<int, int>> vecs{{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
      for (const auto &vec : vecs) {
        for (int cur_i = i + vec.first, cur_j = j + vec.second;
             cur_i < 7 && cur_j < 7 && cur_j > 0 && cur_i > 0;
             cur_i += vec.first, cur_j += vec.second) {
          if (pieces[cur_i][cur_j].value == side &&
              pieces[cur_i + vec.first][cur_j + vec.second].value == ' ') {
            to_kill.push_back(pieces[cur_i][cur_j]);
            break;
          }
        }
      }
    } else {
      if (i < 6 && j < 6 && pieces[i + 1][j + 1].value == side &&
          pieces[i + 2][j + 2].value == ' ') {
        to_kill.push_back(pieces[i + 1][j + 1]);
      }
      if (i < 6 && j > 1 && pieces[i + 1][j - 1].value == side &&
          pieces[i + 2][j - 2].value == ' ') {
        to_kill.push_back(pieces[i + 1][j - 1]);
      }
      if (back) {
        if (i > 1 && j > 1 && pieces[i - 1][j - 1].value == side &&
            pieces[i - 2][j - 2].value == ' ') {
          to_kill.push_back(pieces[i - 1][j - 1]);
        }
        if (i > 1 && j < 6 && pieces[i - 1][j + 1].value == side &&
            pieces[i - 2][j + 2].value == ' ') {
          to_kill.push_back(pieces[i - 1][j + 1]);
        }
      }
    }
    return to_kill;
  }

  void DFS(std::shared_ptr<Node> cur_node, int depth) {
    int64_t target = depth % 2 == 1 ? 10000 : -10000;
    for (auto child : cur_node->children) {
      DFS(child, depth + 1);
      target = depth % 2 == 1 ? std::min(child->score, target)
                              : std::max(child->score, target);
    }
    if (!cur_node->children.empty()) {
      cur_node->score = target;
    }
  }
  std::shared_ptr<Node> root;
  char player_side;
  char bot_side;
  char cur_side;
  int difficulty;
  bool killed = false;
};
