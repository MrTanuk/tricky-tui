#include "core/GameController.hpp"
#include "ui/GameUI.hpp"

int main() {
  // 1. Instanciar el Core (Lógica)
  Core::GameController game;

  // 2. Instanciar la UI (Presentación) inyectándole el Core
  UI::GameUI ui(game);

  // 3. Correr la aplicación
  ui.run();

  return 0;
}
