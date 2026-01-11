#include "trickyLib/tricky.hpp"
#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace Tricky {

Player::Player() = default;
Player::Player(const std::string &name)
    : m_name{name},
      m_wins{0},
      m_lose{0} {}

int Player::getWins() const { return m_wins; }
int Player::getLosses() const { return m_lose; }
std::string Player::getName() const { return m_name; }

void Player::setWin() { ++m_wins; }
void Player::setLose() { ++m_lose; }

bool Tablero::start() {
    if (!checkGameReady()) {
        return false;
    }
    rendererUI();
    return true;
}

bool Tablero::checkGameReady() {
    if (m_players.size() != 2) {
        std::cerr << "[ERROR] Require 2 players to start.\n";
        return false;
    }
    return true;
}

void Tablero::setPlayer(Player player) {
    if (m_players.size() < 2) {
        m_players.push_back(player);
    }
}

void Tablero::setPlayer(std::initializer_list<Player> players) {
    for (const auto player : players) {
        if (m_players.size() < 2) {
            m_players.push_back(player);
        }
    }
}

Tablero::Tablero()
    : m_screen(ftxui::ScreenInteractive::TerminalOutput()),
      m_row{0},
      m_col{0},
      logic{},
      is_victory{false},
      is_turn_player(true),
      is_game_over(false),
      is_draw(false),
      m_moves_count(0),
      m_menu_selected(0),
      m_tab_selector(0) {

    m_players.reserve(2);
    createTable(3, 3);

    // Menu configuration
    m_menu_options = {"Play again", "Exit"};
    auto menu_option = ftxui::MenuOption();
    menu_option.on_enter = [this] {
        if (m_menu_selected == 0) {
            resetGame();
        } else {
            m_screen.ExitLoopClosure()();
        }
    };
    m_menu = ftxui::Menu(&m_menu_options, &m_menu_selected, menu_option);

    m_container_main =
        ftxui::Container::Tab({m_layout, m_menu}, &m_tab_selector);
}

void Tablero::onClick(int index) {
    if (is_game_over || m_board_state[index] != CellState::Empty)
        return;

    int value_player{(is_turn_player) ? 1 : -1};

    m_board_state[index] =
        (is_turn_player) ? CellState::PlayerX : CellState::PlayerO;

    m_moves_count++;

    int row{index / 3};
    int col{index % 3};

    logic.row[row] += value_player;
    logic.column[col] += value_player;
    if (row == col)
        logic.diagonal += value_player;
    if (row + col == 2)
        logic.anti_diagonal += value_player;

    if (std::abs(logic.row[row]) == 3 || std::abs(logic.column[col]) == 3 ||
        std::abs(logic.diagonal) == 3 || std::abs(logic.anti_diagonal) == 3) {

        is_victory = true;
        is_game_over = true;

        if (is_turn_player) {
            m_players[0].setWin();
            m_players[1].setLose();
        } else {
            m_players[1].setWin();
            m_players[0].setLose();
        }

        m_tab_selector = 1;
        return;
    }

    if (m_moves_count == 9) {
        is_draw = true;
        is_game_over = true;
        m_tab_selector = 1;
        return;
    }

    is_turn_player = !is_turn_player;
}

ftxui::Component Tablero::createSquare(int index) {
    ftxui::ButtonOption opt{ftxui::ButtonOption::Simple()};

    opt.transform = [this, index](const ftxui::EntryState &s) {
        std::string label = "";
        ftxui::Color color_text = ftxui::Color::White;

        if (m_board_state[index] == CellState::PlayerX) {
            label = "X";
            color_text = ftxui::Color::Red;
        } else if (m_board_state[index] == CellState::PlayerO) {
            label = "O";
            color_text = ftxui::Color::Blue;
        }

        auto element = ftxui::text(label) | ftxui::center | ftxui::bold |
                       ftxui::color(color_text);

        if (s.focused && !is_game_over) {
            element |= ftxui::inverted;
        }

        return element | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5) |
               ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 3);
    };

    return ftxui::Button("", [this, index] { onClick(index); }, opt);
}

void Tablero::createTable(int row, int col) {
    m_board_state.assign(row * col, CellState::Empty);

    std::vector<ftxui::Component> layout_row;
    for (int i{}; i < row; ++i) {
        std::vector<ftxui::Component> row_buttons;
        for (int j{}; j < col; ++j) {
            int index = (i * col) + j;
            row_buttons.push_back(createSquare(index));
        }
        auto tmp_layout_row{ftxui::Container::Horizontal(row_buttons, &m_col)};

        auto renderer_fila = ftxui::Renderer(tmp_layout_row, [row_buttons] {
            std::vector<ftxui::Element> elementos_visuales;
            for (size_t k = 0; k < row_buttons.size(); ++k) {
                elementos_visuales.push_back(row_buttons[k]->Render());
                if (k < row_buttons.size() - 1) {
                    elementos_visuales.push_back(ftxui::separator());
                }
            }
            return ftxui::hbox(elementos_visuales);
        });

        auto renderer_fila_manual =
            ftxui::Renderer(tmp_layout_row, [row_buttons] {
                std::vector<ftxui::Element> elementos_visuales;
                for (size_t k = 0; k < row_buttons.size(); ++k) {
                    elementos_visuales.push_back(row_buttons[k]->Render());
                    if (k < row_buttons.size() - 1)
                        elementos_visuales.push_back(ftxui::separator());
                }
                return ftxui::hbox(elementos_visuales);
            });

        layout_row.push_back(renderer_fila_manual);
    }

    ftxui::Component container_vertical =
        ftxui::Container::Vertical(layout_row, &m_row);

    m_layout = ftxui::Renderer(container_vertical, [layout_row] {
        std::vector<ftxui::Element> filas_visuales;
        for (size_t k = 0; k < layout_row.size(); ++k) {
            filas_visuales.push_back(layout_row[k]->Render());
            if (k < layout_row.size() - 1)
                filas_visuales.push_back(ftxui::separator());
        }
        return ftxui::vbox(filas_visuales);
    });
}

void Tablero::rendererUI() {

    auto section_player = [](const Player &p, const std::string &l) {
        auto element_letra = ftxui::text(l) | ftxui::bold;
        if (l == "X")
            element_letra |= ftxui::color(ftxui::Color::Red);
        else if (l == "O")
            element_letra |= ftxui::color(ftxui::Color::Blue);

        return ftxui::vbox(
            {ftxui::hbox(
                 {ftxui::text("Playre: " + p.getName()) | ftxui::bold |
                      ftxui::center | ftxui::flex,
                  ftxui::separator(),
                  ftxui::hbox({ftxui::text("Letter: "), element_letra}) |
                      ftxui::center | ftxui::flex}),
             ftxui::separator(),
             ftxui::hbox(
                 {ftxui::hbox({ftxui::text("Wins: "),
                               ftxui::text(std::to_string(p.getWins())) |
                                   ftxui::color(ftxui::Color::Green)}) |
                      ftxui::center | ftxui::flex,
                  ftxui::separator(),
                  ftxui::hbox({ftxui::text("Loose: "),
                               ftxui::text(std::to_string(p.getLosses())) |
                                   ftxui::color(ftxui::Color::Red)}) |
                      ftxui::center | ftxui::flex})});
    };

    ftxui::Component renderer =
        ftxui::Renderer(m_container_main, [this, section_player] {
            ftxui::Element status_element = ftxui::text("");

            if (is_game_over) {
                if (is_victory) {
                    std::string winner_name = (is_turn_player)
                                                  ? m_players[0].getName()
                                                  : m_players[1].getName();
                    status_element =
                        ftxui::text("¡Winner: " + winner_name + "!") |
                        ftxui::bold | ftxui::color(ftxui::Color::Gold1) |
                        ftxui::blink;
                } else if (is_draw) {
                    status_element = ftxui::text("Draw!") | ftxui::bold |
                                     ftxui::color(ftxui::Color::GrayDark);
                }
            }

            return ftxui::vbox(
                       {ftxui::text("Tricky") | ftxui::bold | ftxui::center,
                        ftxui::separator(), section_player(m_players[0], "X"),
                        ftxui::separator(), section_player(m_players[1], "O"),
                        ftxui::separator(), m_layout->Render() | ftxui::center,
                        (is_game_over)
                            ? (ftxui::vbox({ftxui::separator(),
                                            status_element | ftxui::center}))
                            : ftxui::text(""),
                        (is_game_over)
                            ? (ftxui::vbox({ftxui::separator(),
                                            m_menu->Render() | ftxui::center}))
                            : ftxui::text("")}) |
                   ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 30) |
                   ftxui::border | ftxui::center;
        });

    m_screen.Loop(renderer);
}

void Tablero::resetGame() {
    std::fill(m_board_state.begin(), m_board_state.end(), CellState::Empty);

    logic.row.fill(0);
    logic.column.fill(0);
    logic.diagonal = 0;
    logic.anti_diagonal = 0;

    is_victory = false;
    is_draw = false;
    is_game_over = false;
    m_moves_count = 0;
    is_turn_player = true;

    m_tab_selector = 0;
    m_menu_selected = 0;
};

}; // namespace Tricky
