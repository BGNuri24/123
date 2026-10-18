#include "PlayerSelect.h"
#include "Ui.h"

PlayerSelect::PlayerSelect(Players& p) : players(p), selected(0), typing(false), error(0) {}

int PlayerSelect::optionCount() const {
    // all players, plus "+ New player" while there is room
    return players.count() < MAX_PLAYERS ? players.count() + 1 : players.count();
}

Screen PlayerSelect::update() {
    if (typing) {
        int ch = GetCharPressed();
        while (ch > 0) {
            bool ok = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                      (ch >= '0' && ch <= '9') || ch == '_';
            if (ok && (int)text.size() < MAX_NAME_LENGTH) {
                text += (char)ch;
                error = 0;
            }
            ch = GetCharPressed();
        }
        if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && !text.empty()) {
            text.pop_back();
            error = 0;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            Players::AddResult r = players.add(text);
            if (r == Players::ADDED) {
                players.setActive(players.count() - 1);   // the new player plays
                typing = false;
                return Screen::LEVEL_SELECT;
            }
            if (r == Players::DUPLICATE) error = 2;
            else if (r == Players::SAVE_FAILED) error = 3;
            else error = 1;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            typing = false;
            error = 0;
        }
        return Screen::PLAYER_SELECT;
    }

    int n = optionCount();
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) selected = (selected + 1) % n;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) selected = (selected + n - 1) % n;
    if (IsKeyPressed(KEY_ESCAPE)) return Screen::MAIN_MENU;

    if (IsKeyPressed(KEY_DELETE) && selected < players.count()) {
        if (players.remove(selected)) {
            error = 0;
            if (selected >= optionCount()) selected = optionCount() - 1;
        } else {
            error = 3;
        }
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (selected < players.count()) {
            players.setActive(selected);
            return Screen::LEVEL_SELECT;
        }
        typing = true;                              // "+ New player"
        text.clear();
        error = 0;
        while (GetCharPressed() > 0) {}             // drop leftover typed characters
    }
    return Screen::PLAYER_SELECT;
}

void PlayerSelect::draw() const {
    ClearBackground(Color{20, 22, 30, 255});
    drawCentered("SELECT PLAYER", 80, 70, GOLD);

    int n = optionCount();
    for (int i = 0; i < n; i++) {
        const char* mark = (i == selected) ? "> " : "  ";
        const char* line = (i < players.count())
            ? TextFormat("%s%s", mark, players.name(i).c_str())
            : TextFormat("%s+ New player", mark);
        drawCentered(line, 200 + i * 65, 40, i == selected ? GOLD : GRAY);
    }
    drawCentered("UP/DOWN choose    ENTER select    DEL remove    ESC back", 630, 22, LIGHTGRAY);

    if (typing) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        int w = 640, h = 260;
        int x = (GetScreenWidth() - w) / 2;
        int y = (GetScreenHeight() - h) / 2;
        DrawRectangle(x, y, w, h, Color{25, 28, 38, 255});
        DrawRectangleLines(x, y, w, h, GOLD);

        drawCentered("New player name", y + 30, 36, WHITE);

        int fs = 50;
        int tw = MeasureText(text.c_str(), fs);
        int tx = (GetScreenWidth() - tw) / 2;
        DrawText(text.c_str(), tx, y + 100, fs, GOLD);
        if (((int)(GetTime() * 2)) % 2 == 0) DrawText("_", tx + tw + 4, y + 100, fs, GOLD);   // blinking cursor

        if (error == 1)
            drawCentered(TextFormat("Use 1-%d letters, digits or _", MAX_NAME_LENGTH), y + 190, 26, RED);
        else if (error == 2)
            drawCentered("That name already exists", y + 190, 26, RED);
        else if (error == 3)
            drawCentered("Could not save player data", y + 190, 26, RED);
        else
            drawCentered("ENTER confirm    ESC cancel", y + 200, 24, GRAY);
    }
    if (!typing && error == 3)
        drawCentered("Could not save player data", 670, 22, RED);
}