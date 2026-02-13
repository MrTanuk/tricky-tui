#pragma once
#include "GameType.hpp"
#include "Board.hpp"
#include <expected>
#include <span>

namespace Core {
    class GameController {
        public:
            GameController();

            [[nodiscard]]
                std::expected<GameStatus, MoveError> playTurn(Coordinate coord);

            void resetGame();

            [[nodiscard]] Player getCurrentPlayer() const {return m_current_player;}
            [[nodiscard]] Score getScore() const {return m_score;}

            [[nodiscard]] std::span<const Player> getBoardView() const;

        private:
            Board m_board;
            Player m_current_player;
            GameStatus m_current_status;
            Score m_score;

            void switchTurn();
            void updateScore(GameStatus result);
    };
}
