add_library(tricky_core STATIC core/Board.cpp core/GameController.cpp)

target_include_directories(tricky_core PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_library(tricky_ui STATIC ui/GameUI.cpp)

target_link_libraries(tricky_ui PUBLIC tricky_core ftxui::screen
                                       ftxui::component ftxui::dom)

target_include_directories(tricky_ui PUBLIC "${PROJECT_SOURCE_DIR}/include")
