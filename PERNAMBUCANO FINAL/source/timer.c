#include "timer.h"

void initTimer(int intervalMs) {
    timerInit(intervalMs);
}

void destroyTimer() {
    timerDestroy();
}

void resetTimer(int intervalMs) {
    timerUpdateTimer(intervalMs);
}

int isTimeOver() {
    return timerTimeOver();
}

void printTime() {
    timerPrint();
}
