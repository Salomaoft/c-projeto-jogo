#include "keyboard.h"

void initKeyboard() {
    keyboardInit();
}

void destroyKeyboard() {
    keyboardDestroy();
}

int getKeyInput() {
    if (keyhit()) {
        return readch();
    }
    return 0;
}
