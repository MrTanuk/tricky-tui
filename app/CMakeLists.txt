add_executable(app main.cpp)

target_link_libraries(app PRIVATE tricky::core ftxui::screen ftxui::component
                                  ftxui::dom)
