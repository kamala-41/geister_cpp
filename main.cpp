#include "geister_board.h"
#include <iostream>

using namespace geister;

int main() {
  Board board;
  while (!board.IsTerminal()) {
    std::cout << "\nCurrent player: " << (board.CurrentPlayer()==Player::kP0?0:1) << '\n';
    std::cout << board.DebugString() << '\n';

    const auto& moves = board.LegalMoves();
    for (size_t i = 0; i < moves.size(); ++i)
      std::cout << i << ' ';
    std::cout << "\nchoose move id: ";

    size_t id;
    std::cin >> id;
    if (id >= moves.size()) { std::cout << "invalid id!\n"; continue; }
    board.ApplyMove(moves[id]);
  }

  std::cout << "\nGame over!  P0 result = "
            << board.Result(Player::kP0) << '\n';
}

