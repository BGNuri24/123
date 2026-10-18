#include "EndScreen.h"
#include "Ui.h"

static const char* formatTime(float t) {
    int minutes = (int)t / 60;
    int seconds = (int)t % 60;
    int tenths = (int)(t * 10) % 10;
    return TextFormat("%02d:%02d.%d", minutes, seconds, tenths);
}

Screen EndScreen::update() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) return Screen::MAIN_MENU;
    return Screen::END;
}

void EndScreen::draw(const Game& game, const std::string& playerName,
                     float bestTime, bool newBestTime) const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));   // dim the maze

    int w = 600, h = 400;
    int x = (GetScreenWidth() - w) / 2;
    int y = (GetScreenHeight() - h) / 2;
    DrawRectangle(x, y, w, h, Color{25, 28, 38, 245});
    DrawRectangleLines(x, y, w, h, game.won() ? GOLD : RED);

    if (game.won()) {
        drawCentered("FINISH LINE REACHED!", y + 25, 48, GOLD);
        drawCentered(TextFormat("Player: %s", playerName.c_str()), y + 95, 30, SKYBLUE);
        drawCentered(TextFormat("Time: %s", formatTime(game.timeTaken())), y + 150, 36, WHITE);
        drawCentered(TextFormat("Time left: %s", formatTime(game.timeLeft())), y + 200, 30, LIGHTGRAY);
    } else {
        drawCentered("TIME'S UP!", y + 25, 56, RED);
        drawCentered(TextFormat("Player: %s", playerName.c_str()), y + 95, 30, SKYBLUE);
        drawCentered("You did not reach the finish line", y + 160, 30, LIGHTGRAY);
    }
    const char* recordText = bestTime < 0.0f
        ? "Best time: --"
        : TextFormat("%s%s", newBestTime ? "NEW BEST: " : "Best time: ", formatTime(bestTime));
    drawCentered(recordText, y + 245, 28, newBestTime ? GOLD : LIGHTGRAY);
    drawCentered(TextFormat("Points: %d", game.score()), y + 290, 36, GOLD);
    drawCentered("Press ENTER for the menu", y + 350, 24, GRAY);
}