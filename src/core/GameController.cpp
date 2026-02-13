#include "core/GameController.hpp"
#include <expected>

namespace Core {
GameController::GameController()
    : m_current_player(Player::X), m_current_status(GameStatus::InProgress) {}

void GameController::resetGame() {
  m_board.reset();
  m_current_player = Player::X;
  m_current_status = GameStatus::InProgress;
}

std::expected<GameStatus, MoveError>
GameController::playTurn(Coordinate coord) {
  if (m_current_status != GameStatus::InProgress)
    return std::unexpected(MoveError::GameNotActive);

  if (!m_board.makeMove(coord, m_current_player)) {
    if (coord.row >= Board::SIZE || coord.col >= Board::SIZE)
      return std::unexpected(MoveError::outOfBounds);
    else
      return std::unexpected(MoveError::CellOccupied);
  }

  m_current_status = m_board.checkStatus();

  if (m_current_status == GameStatus::InProgress)
    switchTurn();
  else {
    updateScore(m_current_status);
  }

  return m_current_status;
}

void GameController::switchTurn() {
  m_current_player = (m_current_player == Player::X) ? Player::O : Player::X;
}

void GameController::updateScore(GameStatus result) {
  if (result == GameStatus::Win) {
    if (m_current_player == Player::X)
      m_score.player_x++;
    else
      m_score.player_o++;
  }
}

std::span<const Player> GameController::getBoardView() const {
  return m_board.getView();
}

} // namespace Core
