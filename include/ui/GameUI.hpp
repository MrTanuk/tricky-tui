#pragma once
#include "core/GameController.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace UI {

class GameUI {
public:
  explicit GameUI(Core::GameController &controller);
  void run();

private:
  Core::GameController &m_controller;
  ftxui::ScreenInteractive m_screen;

  // Components
  ftxui::Component m_board_component;
  ftxui::Component m_menu_component;
  ftxui::Component m_main_container; // Main component (Tab)

  // Navigation state
  int m_selected_row{0};
  int m_selected_col{0};

  // Menu state and flow
  int m_tab_selector{0}; // 0: Game, 1: Menu Game Over
  int m_menu_selected{0};

  // Helpers
  ftxui::Component createBoard();
  ftxui::Component createGameOverMenu();

  // Rendering
  ftxui::Element renderScore();
  ftxui::Element renderBoardPanel(); // Visual logic of separators
  void onCellClick(int row, int col);
};

} // namespace UI
