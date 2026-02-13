#pragma once
#include <concepts>
#include <cstdint>

namespace Core {
enum class Player : char {
  X = 'X',
  O = 'O',
  None = ' ',
};

enum class GameStatus {
  InProgress,
  Win,
  Draw,
};

struct Coordinate {
  uint8_t row;
  uint8_t col;
};

enum class MoveError { outOfBounds, CellOccupied, GameNotActive };

struct Score {
  int player_x{0};
  int player_o{0};
};

// Table concepts: These are rules of how must be the table
template <typename T>
concept BoardConcept = requires(T b, Coordinate c) {
  { b.at(c) } -> std::convertible_to<Player>;
  { b.width() } -> std::convertible_to<int>;
  { b.height() } -> std::convertible_to<int>;
};

} // namespace Core
