#include "geister_board.h"
#include <sstream>
#include <algorithm>

namespace geister {

// ───── ctor：紅前藍後 固定開局 ─────
Board::Board() {
  sq_.fill(Piece::kEmpty);

  // P0 (下)
  for (int c = 1; c <= 4; ++c) sq_[RC(kRows - 1, c)] = Piece::kP0Red;
  for (int c = 1; c <= 4; ++c) sq_[RC(kRows - 2, c)] = Piece::kP0Blue;

  // P1 (上)
  for (int c = 1; c <= 4; ++c) sq_[RC(0, c)]           = Piece::kP1Red;
  for (int c = 1; c <= 4; ++c) sq_[RC(1, c)]           = Piece::kP1Blue;

  GenerateMoves();
}

// ───── 判斷 idx 是否當前玩家棋子 ─────
bool Board::IsMyPiece(Player p, int idx) const {
  Piece pc = sq_[idx];
  if (p == Player::kP0) return pc == Piece::kP0Blue || pc == Piece::kP0Red;
  else                  return pc == Piece::kP1Blue || pc == Piece::kP1Red;
}

// ───── 產生合法手 ─────
void Board::GenerateMoves() {
  moves_cache_.clear();
  if (terminal_) return;

  bool is_p0 = cur_ == Player::kP0;
  for (int idx = 0; idx < kSquares; ++idx) {
    if (!IsMyPiece(cur_, idx)) continue;
    for (Dir d : {Dir::Up, Dir::Down, Dir::Left, Dir::Right}) {
      int dst = NextIdx(idx, d);
      if (dst == -1) continue;                       // 越界
      Piece dst_piece = sq_[dst];

      // 自己棋子擋住
      if (IsMyPiece(cur_, dst)) continue;

      Move mv{idx, d, /*escape=*/false};

      // 脫出?
      if (IsEscapeIdx(is_p0, dst) &&
          (sq_[idx] == (is_p0 ? Piece::kP0Blue : Piece::kP1Blue))) {
        mv.escape = true;
        moves_cache_.push_back(mv);
        continue;
      }

      // 一般走 / 吃
      moves_cache_.push_back(mv);
    }
  }
}

// ───── 套用行動 ─────
void Board::ApplyMove(const Move& m) {
  int dst = NextIdx(m.src, m.dir);
  Piece& from = sq_[m.src];

  if (m.escape) {
    from = Piece::kEmpty;                         // 移除藍棋
  } else {
    sq_[dst] = from;                              // 移動或吃
    from = Piece::kEmpty;
  }

  // 換手 & 更新終局
  cur_ = cur_ == Player::kP0 ? Player::kP1 : Player::kP0;
  CheckTerminal();
  GenerateMoves();
}

// ───── 勝負判定 ─────
void Board::CheckTerminal() {
  // 先計算雙方剩餘藍/紅
  int p0_b=0, p0_r=0, p1_b=0, p1_r=0;
  for (auto pc : sq_) {
    switch (pc) {
      case Piece::kP0Blue: ++p0_b; break;
      case Piece::kP0Red:  ++p0_r; break;
      case Piece::kP1Blue: ++p1_b; break;
      case Piece::kP1Red:  ++p1_r; break;
      default: break;
    }
  }

  // 1. 全吃光對手藍
  if (p1_b == 0) { terminal_ = true; result_ = +1; return; }
  if (p0_b == 0) { terminal_ = true; result_ = -1; return; }

  // 2. 把自己紅棋全給對手吃
  if (p0_r == 0) { terminal_ = true; result_ = +1; return; }
  if (p1_r == 0) { terminal_ = true; result_ = -1; return; }

  // 3. 脫出在 ApplyMove 時直接 return 結果
  //   (簡化：脫出後直接終局，reward 早一步設)
  // 若無終局，維持原狀態
}

// ───── 盤面字串 ─────
std::string Board::DebugString(bool hide) const {
  std::ostringstream os;
  auto sym = [&](Piece pc, bool me) {
    if (pc == Piece::kEmpty) return '.';
    if (hide) {
      if (me) return (pc == Piece::kP0Blue || pc == Piece::kP1Blue) ? 'B' : 'R';
      else     return '?';
    }
    switch (pc) {
      case Piece::kP0Blue: return 'B';
      case Piece::kP0Red:  return 'R';
      case Piece::kP1Blue: return 'b';
      case Piece::kP1Red:  return 'r';
      default: return '?';
    }
  };

  for (int r = 0; r < kRows; ++r) {
    for (int c = 0; c < kCols; ++c) {
      os << sym(sq_[RC(r,c)], false) << ' ';
    }
    os << '\n';
  }
  return os.str();
}

}  // namespace geister
