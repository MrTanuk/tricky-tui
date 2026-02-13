#include "ui/GameUI.hpp"
#include <format>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

namespace UI {

GameUI::GameUI(Core::GameController &controller)
    : m_controller(controller), m_screen(ScreenInteractive::TerminalOutput()) {

  m_board_component = createBoard();
  m_menu_component = createGameOverMenu();

  m_main_container =
      Container::Tab({m_board_component, m_menu_component}, &m_tab_selector);

  auto component_renderer = Renderer(m_main_container, [this] {
    auto game_view =
        vbox({text("TRICKY") | bold | center | color(Color::Cyan),
              separator(), renderScore() | center, separator(),
              m_board_component->Render() | center,
              text(std::format(
                  "Turn: Player {}",
                  m_controller.getCurrentPlayer() == Core::Player::X ? "X"
                                                                     : "O")) |
                  dim | center}) |
        border | center;

    if (m_tab_selector == 1) {
      return dbox({game_view, m_menu_component->Render() | center |
                                  size(WIDTH, GREATER_THAN, 30)});
    }

    return game_view;
  });

  m_screen.Loop(component_renderer);
}

void GameUI::run() {}

ftxui::Element GameUI::renderScore() {
  auto score = m_controller.getScore();
  return hbox({text(std::format("Player X: {} ", score.player_x)) | bold |
                   color(Color::Red),
               separator(),
               text(std::format(" Player O: {}", score.player_o)) | bold |
                   color(Color::Blue)});
}

ftxui::Component GameUI::createBoard() {

  std::vector<Component> layout_rows;

  for (int i = 0; i < 3; ++i) {
    std::vector<Component> row_buttons;
    for (int j = 0; j < 3; ++j) {

      ButtonOption opt = ButtonOption::Simple();

      opt.transform = [this, i, j](const EntryState &s) {
        auto boardView = m_controller.getBoardView();
        int index = i * 3 + j;
        Core::Player p = boardView[index];

        std::string label = "   ";
        if (p == Core::Player::X)
          label = " X ";
        if (p == Core::Player::O)
          label = " O ";

        auto el = text(label);

        if (p == Core::Player::X)
          el |= color(Color::Red) | bold;
        if (p == Core::Player::O)
          el |= color(Color::Blue) | bold;

        if (s.focused)
          el |= inverted;

        return el;
      };

      row_buttons.push_back(
          Button("", [this, i, j] { onCellClick(i, j); }, opt));
    }

    auto row_comp = Container::Horizontal(row_buttons, &m_selected_col);

    auto row_renderer = Renderer(row_comp, [row_comp] {
      return hbox({row_comp->ChildAt(0)->Render(), separator(),
                   row_comp->ChildAt(1)->Render(), separator(),
                   row_comp->ChildAt(2)->Render()});
    });

    layout_rows.push_back(row_renderer);
  }

  auto col_container = Container::Vertical(layout_rows, &m_selected_row);

  return Renderer(col_container, [col_container] {
    return vbox({col_container->ChildAt(0)->Render(), separator(),
                 col_container->ChildAt(1)->Render(), separator(),
                 col_container->ChildAt(2)->Render()}) |
           border;
  });
}

ftxui::Component GameUI::createGameOverMenu() {
  auto btn_play = Button(
      "Play Again",
      [this] {
        m_controller.resetGame();
        m_tab_selector = 0;
        m_menu_selected = 0;
      },
      ButtonOption::Ascii());

  auto btn_exit = Button(
      "Exit", [this] { m_screen.ExitLoopClosure()(); }, ButtonOption::Ascii());

  auto container = Container::Vertical({btn_play, btn_exit}, &m_menu_selected);

  return Renderer(container, [this, container] {
    auto winner =
        m_controller.getCurrentPlayer() == Core::Player::X ? "X" : "O";

    return window(text(" GAME OVER ") | center | bold | color(Color::Gold1),
                  vbox({text("Match Finished!") | center, separator(),
                        container->Render() | center})) |
           clear_under;
  });
}

void GameUI::onCellClick(int row, int col) {
  if (m_tab_selector == 1)
    return;

  Core::Coordinate coord{static_cast<uint8_t>(row), static_cast<uint8_t>(col)};
  auto result = m_controller.playTurn(coord);

  if (result.has_value()) {
    auto status = result.value();
    if (status == Core::GameStatus::Win || status == Core::GameStatus::Draw) {
      m_tab_selector = 1;
    }
  }
}

} // namespace UI
