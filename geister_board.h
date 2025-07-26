#pragma once
#include "geister_coord.h"
#include <vector>
#include <string>

namespace geister {

enum class Player : uint8_t { kP0 = 0, kP1 = 1 };

enum class Piece : uint8_t {
  kEmpty,
  kP0Blue, kP0Red,
  kP1Blue, kP1Red
};

// 行動（脫出=true 代表從出口移除該子）
struct Move { int src; Dir dir; bool escape; };

// ─────────── Board ───────────
class Board {
 public:
  Board();                                // 固定開局
  Player CurrentPlayer() const { return cur_; }

  const std::vector<Move>& LegalMoves() const { return moves_cache_; }
  void ApplyMove(const Move& m);

  bool IsTerminal() const { return terminal_; }
  // P0: +1/-1/0 ；P1 則相反
  int Result(Player p) const {
    return p == Player::kP0 ? result_ : -result_;
  }

  std::string DebugString(bool hide_pieces = false) const;

 private:
  // internal helpers
  bool IsMyPiece(Player p, int idx) const;
  void GenerateMoves();
  void CheckTerminal();

  std::array<Piece, kSquares> sq_;
  Player cur_ = Player::kP0;
  bool terminal_ = false;
  int result_ = 0;                       // +1 P0 win / -1 P1 win /0 draw
  std::vector<Move> moves_cache_;
};

}  // namespace geister

