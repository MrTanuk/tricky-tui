#include "core/Board.hpp"
#include <algorithm>

namespace Core {

Board::Board() { reset(); }

void Board::reset() { m_grid.fill(Player::None); }

int Board::toIndex(Coordinate c) const { return c.row * SIZE + c.col; }

Player Board::getCell(Coordinate coord) const {
  if (coord.row >= SIZE || coord.col >= SIZE)
    return Player::None;
  return m_grid[toIndex(coord)];
}

bool Board::makeMove(Coordinate coord, Player player) {
  if (coord.row >= SIZE || coord.col >= SIZE)
    return false;

  int idx{toIndex(coord)};
  if (m_grid[idx] != Player::None)
    return false;

  m_grid[idx] = player;
  return true;
}

GameStatus Board::checkStatus() const {

  // 1. Row and columns
  for (int i = 0; i < SIZE; ++i) {
    // Check row
    if (m_grid[i * SIZE] != Player::None &&
        m_grid[i * SIZE] == m_grid[i * SIZE + 1] &&
        m_grid[i * SIZE] == m_grid[i * SIZE + 2]) {
      return GameStatus::Win;
    }
    // Check columns
    if (m_grid[i] != Player::None && m_grid[i] == m_grid[i + SIZE] &&
        m_grid[i] == m_grid[i + (SIZE * 2)]) {
      return GameStatus::Win;
    }
  }

  // 2. Diagonales
  if (m_grid[0] != Player::None && m_grid[0] == m_grid[4] &&
      m_grid[0] == m_grid[8])
    return GameStatus::Win;
  if (m_grid[2] != Player::None && m_grid[2] == m_grid[4] &&
      m_grid[2] == m_grid[6])
    return GameStatus::Win;

  bool has_empty{
      std::ranges::any_of(m_grid, [](Player p) { return p == Player::None; })};

  if (!has_empty)
    return GameStatus::Draw;

  return GameStatus::InProgress;
}

} // namespace Core
