#pragma once

#include <array>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <vector>

namespace Tricky {

enum class CellState { Empty, PlayerX, PlayerO };

class Player {
        std::string m_name;
        int m_wins, m_lose;

    public:
        Player();
        Player(const std::string &name);

        int getWins() const;
        int getLosses() const;
        void setWin();
        void setLose();
        std::string getName() const;
};

class Tablero {

        struct logic_game {
                std::array<int, 3> row;
                std::array<int, 3> column;
                int diagonal;
                int anti_diagonal;
        };
        logic_game logic;

        std::vector<Player> m_players;

        std::vector<CellState> m_board_state;

        bool is_turn_player;
        bool is_victory;
        bool is_game_over;
        bool is_draw;

        int m_moves_count;
        int m_row, m_col;

        // Layouts
        ftxui::Component m_layout;

        // Menu
        ftxui::Component m_menu;
        ftxui::Component m_container_main;
        std::vector<std::string> m_menu_options;
        int m_menu_selected;

        // Tab Selector (0: Game, 1: Menu)
        int m_tab_selector;

        ftxui::ScreenInteractive m_screen;

        void createTable(int, int);
        void rendererUI();

        bool checkGameReady();

        ftxui::Component createSquare(int);
        void onClick(int);
        void resetGame();

    public:
        Tablero();

        bool start();

        void setPlayer(Player);
        void setPlayer(std::initializer_list<Player>);
};

} // namespace Tricky
