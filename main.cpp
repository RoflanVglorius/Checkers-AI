#include "include/board.h"
#include "include/bot.h"
#include "include/game.h"

#include <iostream>

int main() {
  std::cout << "Choose difficulty (1/2/3): \n";
  int difficulty;
  std::cin >> difficulty;
  std::cout << "Choose your side (w/b):\n";
  char side;
  std::cin >> side;
  Game game(difficulty, side);
  game.Play();
  return 0;
}
