#pragma once
#include <string>
#include "Screen.h"
#include "../logic/Players.h"

class PlayerSelect {
public:
    explicit PlayerSelect(Players& players);
    Screen update();      // LEVEL_SELECT = player chosen, MAIN_MENU = back, PLAYER_SELECT = stay
    void draw() const;
private:
    int optionCount() const;

    Players& players;
    int selected;
    bool typing;          // true while typing a new name
    std::string text;
    int error;            // 0 none, 1 invalid name, 2 duplicate
};