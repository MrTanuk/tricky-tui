# 🎮 Tricky (Tic-Tac-Toe)

> A modern, terminal-based Tic-Tac-Toe game built with C++ and FTXUI.

**Tricky** is a console application that reimagines the classic "Tres en Raya" game. Unlike standard command-line games, Tricky uses **FTXUI** to provide a fully interactive interface with mouse support, dynamic layouts, and a responsive design within your terminal.

## ✨ Features

- **Interactive UI:** Clickable board buttons using the mouse.
- **Score Tracking:** Real-time updates of Wins and Losses for both players.
- **Game States:** Automatic detection of Win, Loss, and Draw (Empate) conditions.
- **Smart Menus:** Context-aware menus that appear only when the game ends.
- **Modern C++:** Written in C++17 using clean code practices (Enums, separating logic/UI).
- **Cross-Platform:** Runs on Linux, macOS, and Windows (via terminal).

## 📸 Preview

https://github.com/user-attachments/assets/524aa424-f85c-428d-80fa-47a123781063

## 🛠️ Prerequisites

To build this project, you need:

- **C++ Compiler:** Supporting C++17 or later (GCC, Clang, MSVC).
- **CMake:** Version 3.20 or higher.
- **Git:** To fetch the repository and dependencies.

## 🚀 Building and Running

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/MrTanuk/tricky-tui.git
    cd tricky-tui
    ```

2.  **Create a build directory:**

    ```bash
    mkdir build && cd build
    ```

3.  **Configure and Compile:**

    ```bash
    cmake ..
    cmake --build .
    ```

4.  **Run the game:**
    ```bash
    ./app/app
    ```

## 🕹️ Controls

- **Mouse:** Click on the grid cells to place your mark (X or O).
- **Keyboard (Menu):**
  - Use `UP` / `DOWN` arrows to navigate the "Game Over" menu.
  - Press `ENTER` to select an option.

## 📂 Project Structure

The project follows a modular architecture separating logic from the interface.

```
.
├── app/
│   └── main.cpp       # Entry point
├── include/
│   └── trickyLib/
│       └── tricky.hpp # Class definitions (Player, Tablero)
├── src/
│   └── tricky.cpp     # Implementation of logic and UI rendering
└── CMakeLists.txt     # Build configuration
```

## 🗺️ Roadmap

- [x] Basic Game Logic (Win/Draw detection).
- [x] Interactive UI with FTXUI.
- [x] Scoreboard system.
- [x] Replay/Exit Menu.
- [ ] **Upcoming:** Input screens for Player Names (Login UI).
- [ ] **Upcoming:** Multiplayer local
- [ ] **Upcoming:** AI implementation for single-player mode.

## 🤝 Contributing

Contributions are welcome! Feel free to open issues or submit pull requests to improve the game logic or UI.

## 📝 License

This project is open-source. Feel free to use it for educational purposes.
