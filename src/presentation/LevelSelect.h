#pragma once
#include <string>
#include "Screen.h"

class LevelSelect {
public:
    LevelSelect();
    Screen update();          // PLAYING = level chosen, MAIN_MENU = back, LEVEL_SELECT = stay
    void draw() const;
    int size() const;         // chosen maze size
    float seconds() const;    // chosen time limit
private:
    int selected;
    bool typing;              // true while the player types a custom size
    std::string text;         // the typed digits
    bool showError;
    int chosenSize;
    float chosenSeconds;
};