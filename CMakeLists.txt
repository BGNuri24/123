cmake_minimum_required(VERSION 3.14)
project(MazeGame CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.5)
FetchContent_MakeAvailable(raylib)

add_executable(MazeGame
    src/main.cpp
    src/data/MazeData.cpp
    src/data/PlayerStore.cpp
    src/logic/Game.cpp
    src/logic/Players.cpp
    src/presentation/Input.cpp
    src/presentation/Renderer.cpp
    src/presentation/Textures.cpp
    src/presentation/Ui.cpp
    src/presentation/MainMenu.cpp
    src/presentation/ControlsScreen.cpp
    src/presentation/PlayerSelect.cpp
    src/presentation/LevelSelect.cpp
    src/presentation/PauseMenu.cpp
    src/presentation/EndScreen.cpp
    src/presentation/MazeGenerationScreen.cpp)
target_link_libraries(MazeGame raylib)