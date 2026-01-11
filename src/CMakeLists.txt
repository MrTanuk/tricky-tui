add_library(tricky STATIC tricky.cpp)

add_library(tricky::core ALIAS tricky)

target_include_directories(
  tricky
  PUBLIC "${PROJECT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(tricky PRIVATE ftxui::screen ftxui::component ftxui::dom)
