cmake_minimum_required(VERSION 3.14)
project(cpp_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Baixa e prepara o FTXUI localmente
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# Cria o executável a partir do seu arquivo .cpp
add_executable(cpp_project main.cpp)

# Linka o FTXUI ao programa
target_link_libraries(cpp_project PRIVATE
    # Chose a submodule
    ftxui::component
    ftxui::dom
    ftxui::screen
)
