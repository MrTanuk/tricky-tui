#pragma once
#include "GameType.hpp"
#include <array>
#include <span>
#include <utility>

namespace Core {

class Board {
public:
  static constexpr int SIZE = 3;

  Board();
  void reset();

  [[nodiscard]] bool makeMove(Coordinate coord, Player player);
  [[nodiscard]] Player getCell(Coordinate coord) const;
  [[nodiscard]] GameStatus checkStatus() const;
  [[nodiscard]] std::span<const Player> getView() const { return m_grid; }

  template <typename Self> auto &&getRawData(this Self &&self) {
    return std::forward<Self>(self).m_grid;
  }

private:
  std::array<Player, SIZE * SIZE> m_grid;

  [[nodiscard]] int toIndex(Coordinate coord) const;
};

} // namespace Core
