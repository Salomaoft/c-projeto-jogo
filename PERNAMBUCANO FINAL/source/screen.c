#include "screen.h"

void initScreen() {
    screenInit(1); // Inicializa a tela com bordas
    screenHideCursor();
}

void destroyScreen() {
    screenShowCursor();
    screenDestroy();
}

void drawBorders() {
    screenInit(1); // Redesenha as bordas, se necessário
}

void drawText(const char *text, int x, int y) {
    screenGotoxy(x, y);
    printf("%s", text);
    screenUpdate();
}

void drawChar(char ch, int x, int y) {
    screenGotoxy(x, y);
    printf("%c", ch);
    screenUpdate();
}

void clearScreen() {
    screenClear();
}
