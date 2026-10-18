#include "Ui.h"

void drawCentered(const char* text, int y, int size, Color color) {
    DrawText(text, (GetScreenWidth() - MeasureText(text, size)) / 2, y, size, color);
}

void drawOptions(const char* const* labels, int count, int selected, int startY) {
    for (int i = 0; i < count; i++) {
        const char* line = TextFormat("%s%s", i == selected ? "> " : "  ", labels[i]);
        drawCentered(line, startY + i * 70, 40, i == selected ? GOLD : GRAY);
    }
}