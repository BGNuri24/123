#include "raylib.h"
#include "logic/Game.h"
#include "logic/Players.h"
#include "presentation/Input.h"
#include "presentation/Renderer.h"
#include "presentation/Screen.h"
#include "presentation/MainMenu.h"
#include "presentation/ControlsScreen.h"
#include "presentation/PlayerSelect.h"
#include "presentation/LevelSelect.h"
#include "presentation/PauseMenu.h"
#include "presentation/EndScreen.h"
#include "presentation/MazeGenerationScreen.h"
#include <cstdlib>
#include <ctime>

int main() {
    InitWindow(1000, 700, "Maze");
    SetExitKey(KEY_NULL);               // ESC no longer closes the window by itself
    SetTargetFPS(120);
    srand((unsigned)time(0));

    {   // everything inside this block is destroyed BEFORE the window closes (textures need the window)
        Players players;                // loads players.txt
        Renderer renderer;              // creates the textures
        MainMenu mainMenu;
        ControlsScreen controls;
        PlayerSelect playerSelect(players);
        LevelSelect levelSelect;
        PauseMenu pauseMenu;
        EndScreen endScreen;
        MazeGenerationScreen generationScreen;
        Game* game = nullptr;
        int currentSize = 0;            // remembered so "Restart" can recreate the same level
        float currentSeconds = 0;
        float bestTime = -1.0f;
        bool newBestTime = false;
        Screen screen = Screen::MAIN_MENU;

        while (screen != Screen::QUIT && !WindowShouldClose()) {
            Screen next = screen;

            if (screen == Screen::MAIN_MENU) {
                next = mainMenu.update();
                BeginDrawing();
                mainMenu.draw();
                EndDrawing();
            }
            else if (screen == Screen::CONTROLS) {
                next = controls.update();
                BeginDrawing();
                controls.draw();
                EndDrawing();
            }
            else if (screen == Screen::PLAYER_SELECT) {
                next = playerSelect.update();
                BeginDrawing();
                playerSelect.draw();
                EndDrawing();
            }
            else if (screen == Screen::LEVEL_SELECT) {
                next = levelSelect.update();
                if (next == Screen::PLAYING) {
                    currentSize = levelSelect.size();
                    currentSeconds = levelSelect.seconds();
                    game = new Game(currentSize, currentSeconds);
                    generationScreen.begin(*game);
                    next = Screen::GENERATING;
                }
                BeginDrawing();
                levelSelect.draw();
                EndDrawing();
            }
            else if (screen == Screen::GENERATING) {
                bool complete = generationScreen.update(*game);
                BeginDrawing();
                generationScreen.draw(*game);
                EndDrawing();
                if (complete) {
                    DisableCursor();
                    next = Screen::PLAYING;
                }
            }
            else if (screen == Screen::PLAYING) {
                game->update(readInput());
                bool pause = IsKeyPressed(KEY_ESCAPE);
                BeginDrawing();
                renderer.draw(*game);
                EndDrawing();
                if (game->finished()) {
                    EnableCursor();
                    newBestTime = game->won() && players.recordWin(currentSize, game->timeTaken());
                    bestTime = players.bestTime(currentSize);
                    next = Screen::END;
                } else if (pause) {
                    EnableCursor();
                    pauseMenu.reset();
                    next = Screen::PAUSED;
                }
            }
            else if (screen == Screen::PAUSED) {
                PauseAction action = pauseMenu.update();
                BeginDrawing();
                renderer.draw(*game);   // frozen maze (timer stopped) behind the menu
                pauseMenu.draw();
                EndDrawing();

                if (action == PauseAction::RESUME) {
                    DisableCursor();
                    next = Screen::PLAYING;
                }
                else if (action == PauseAction::RESTART) {
                    delete game;
                    game = new Game(currentSize, currentSeconds);   // new maze, same level
                    generationScreen.begin(*game);
                    next = Screen::GENERATING;
                }
                else if (action == PauseAction::CHANGE_PLAYER) {
                    delete game;
                    game = nullptr;
                    next = Screen::PLAYER_SELECT;
                }
                else if (action == PauseAction::MAIN_MENU) {
                    delete game;
                    game = nullptr;
                    next = Screen::MAIN_MENU;
                }
            }
            else if (screen == Screen::END) {
                next = endScreen.update();
                BeginDrawing();
                renderer.draw(*game);   // frozen maze behind the popup
                endScreen.draw(*game, players.activeName(), bestTime, newBestTime);
                EndDrawing();
                if (next != Screen::END) {
                    delete game;
                    game = nullptr;
                }
            }

            screen = next;              // switch screens only after a full frame is drawn
        }
        delete game;
    }
    CloseWindow();
}