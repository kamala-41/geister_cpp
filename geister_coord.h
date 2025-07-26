#pragma once
#include <array>
#include <cstdint>

namespace geister {

// 盤面大小
constexpr int kRows = 6;
constexpr int kCols = 6;
constexpr int kSquares = kRows * kCols;

// 2D <-> 1D
constexpr int RC(int r, int c) { return r * kCols + c; }
constexpr int Row(int idx)     { return idx / kCols; }
constexpr int Col(int idx)     { return idx % kCols; }

// 方向
enum class Dir : uint8_t { Up = 0, Down, Left, Right };
inline int Dr(Dir d) { return d == Dir::Up ? -1 : (d == Dir::Down ? +1 : 0); }
inline int Dc(Dir d) { return d == Dir::Left ? -1 : (d == Dir::Right ? +1 : 0); }

// 下個座標（越界回 -1）
inline int NextIdx(int idx, Dir d) {
  int r = Row(idx) + Dr(d);
  int c = Col(idx) + Dc(d);
  return (r < 0 || r >= kRows || c < 0 || c >= kCols) ? -1 : RC(r, c);
}

// 出口判定
inline bool IsEscapeIdx(bool is_p0, int idx) {
  int r = Row(idx), c = Col(idx);
  if (is_p0) return r == 0 && (c == 0 || c == kCols - 1);
  else       return r == kRows - 1 && (c == 0 || c == kCols - 1);
}

}  // namespace geister
