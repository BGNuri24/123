#pragma once
#include <string>
#include "../data/PlayerStore.h"

const int MAX_NAME_LENGTH = 12;

class Players {
public:
    enum AddResult { ADDED, INVALID, DUPLICATE, FULL, SAVE_FAILED };

    Players();
    int count() const;
    const std::string& name(int i) const;
    const std::string& activeName() const;   // the player who plays now
    void setActive(int i);
    AddResult add(const std::string& name);  // validates, then saves
    bool remove(int i);
    float bestTime(int mazeSize) const;
    bool recordWin(int mazeSize, float seconds);
private:
    static bool isValidName(const std::string& name);

    PlayerStore store;
    int activeIndex;
};