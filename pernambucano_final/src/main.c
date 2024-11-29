#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <keyboard.h>
#include <screen.h>
#include <timer.h>

#define WIDTH 40 
#define HEIGHT 20
#define MAX_TIME 60
#define NUM_TEAMS 10
#define PADDLE_SIZE 3 

typedef struct {
    int x, y, dx, dy;
} Ball;

typedef struct {
    char team[20];
    int score;
    int paddle_y;
} Player;

typedef struct {
    int mode;       
    int difficulty; 
    Ball ball;
    Player player1;
    Player player2;
    char *teams[NUM_TEAMS];
    int timeRemaining;
} Game;


void centerText(const char *text, int totalWidth) {
    int len = strlen(text);
    int padding = (totalWidth - len) / 2;
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s", text);
}


void drawGameField() {
    for (int y = 0; y <= HEIGHT; y++) {
        for (int x = 0; x <= WIDTH; x++) {
            if (y == 0 || y == HEIGHT) {
                printf("-");
            } else if (x == 0 || x == WIDTH) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    
    for (int i = 0; i < 5; i++) {
        printf("\n");
    }
}


void drawGame(Game *game) {
    screenClear();
    drawGameField();

    
    printf("\033[%d;%dHo", game->ball.y + 1, game->ball.x + 1);

    
    for (int i = -PADDLE_SIZE / 2; i <= PADDLE_SIZE / 2; i++) {
        if (game->player1.paddle_y + i >= 1 && game->player1.paddle_y + i <= HEIGHT - 1) {
            printf("\033[%d;%dH|", game->player1.paddle_y + i, 2);
        }
        if (game->player2.paddle_y + i >= 1 && game->player2.paddle_y + i <= HEIGHT - 1) {
            printf("\033[%d;%dH|", game->player2.paddle_y + i, WIDTH - 1);
        }
    }

    
    printf("\033[%d;0H==================================================================\n", HEIGHT + 6);

    char player1Info[50], player2Info[50];
    sprintf(player1Info, "%s (Player 1)", game->player1.team);
    if (game->mode == 1) {
        sprintf(player2Info, "%s (Máquina)", game->player2.team);
    } else {
        sprintf(player2Info, "%s (Player 2)", game->player2.team);
    }

    printf(" ");
    centerText(player1Info, WIDTH / 2 + 10);
    printf(" (%d)   X   (%d) ", game->player1.score, game->player2.score);
    centerText(player2Info, WIDTH / 2 + 10);
    printf("\n");

    if (game->mode == 1) {
        char difficultyInfo[30];
        sprintf(difficultyInfo, "Dificuldade: %s",
                game->difficulty == 1 ? "Fácil" :
                game->difficulty == 2 ? "Médio" : "Difícil");
        printf(" ");
        centerText(difficultyInfo, WIDTH + 10);
        printf("\n");
    } else {
        printf("\n");
    }

    char timeText[30];
    sprintf(timeText, "Tempo restante: %d segundos", game->timeRemaining);
    printf(" ");
    centerText(timeText, WIDTH + 10);
    printf("\n");

    printf("==================================================================\n");
    fflush(stdout);
}


void resetBall(Ball *ball) {
    ball->x = WIDTH / 2;
    ball->y = HEIGHT / 2;
    ball->dx = (rand() % 2 == 0 ? 1 : -1) * 2;
    ball->dy = (rand() % 2 == 0 ? 1 : -1) * 1;
}


void movePaddle(Player *player, int key) {
    if (key == 'w' && player->paddle_y > PADDLE_SIZE / 2) {
        player->paddle_y--;
    } else if (key == 's' && player->paddle_y < HEIGHT - PADDLE_SIZE / 2) {
        player->paddle_y++;
    }
}


void movePaddleAI(Game *game) {
    int target = game->ball.y;

    if (game->difficulty == 1) {
        target += (rand() % 3) - 1;
    } else if (game->difficulty == 2) {
        target += (rand() % 2) - 1;
    }

    if (game->player2.paddle_y < target && game->player2.paddle_y < HEIGHT - PADDLE_SIZE / 2) {
        game->player2.paddle_y++;
    } else if (game->player2.paddle_y > target && game->player2.paddle_y > PADDLE_SIZE / 2) {
        game->player2.paddle_y--;
    }
}


void updateBall(Game *game) {
    game->ball.x += game->ball.dx;
    game->ball.y += game->ball.dy;

    
    if (game->ball.y <= 1 || game->ball.y >= HEIGHT - 1) {
        game->ball.dy *= -1;
    }

    
    if (game->ball.x == 2 && (game->ball.y >= game->player1.paddle_y - PADDLE_SIZE / 2 && game->ball.y <= game->player1.paddle_y + PADDLE_SIZE / 2)) {
        game->ball.dx *= -1; 
    } else if (game->ball.x == WIDTH - 2 && (game->ball.y >= game->player2.paddle_y - PADDLE_SIZE / 2 && game->ball.y <= game->player2.paddle_y + PADDLE_SIZE / 2)) {
        game->ball.dx *= -1; 
    }

    
    if (game->ball.x <= 0) { 
        game->player2.score++;
        resetBall(&game->ball);
    } else if (game->ball.x >= WIDTH) { 
        game->player1.score++;
        resetBall(&game->ball);
    }
}


void chooseTeams(Game *game) {
    screenClear();
    printf("=========================================\n");
    printf("  CAMPEONATO PERNAMBUCANO DE PONG 2025  \n");
    printf("=========================================\n");

    printf("Escolha o modo de jogo:\n1. Contra Máquina\n2. Dois Jogadores\n");
    scanf("%d", &game->mode);
    screenClear();

    printf("Escolha seu time (1-10):\n");
    for (int i = 0; i < NUM_TEAMS; i++) {
        printf("%d. %s\n", i + 1, game->teams[i]);
    }
    int choice;
    scanf("%d", &choice);
    strcpy(game->player1.team, game->teams[choice - 1]);
    screenClear();

    if (game->mode == 1) {
        printf("Escolha o time da máquina (1-10):\n");
        for (int i = 0; i < NUM_TEAMS; i++) {
            printf("%d. %s\n", i + 1, game->teams[i]);
        }
        scanf("%d", &choice);
        strcpy(game->player2.team, game->teams[choice - 1]);
        screenClear();

        printf("Escolha a dificuldade:\n1. Fácil\n2. Médio\n3. Difícil\n");
        scanf("%d", &game->difficulty);
        screenClear();
    } else {
        printf("Jogador 2, escolha seu time (1-10):\n");
        for (int i = 0; i < NUM_TEAMS; i++) {
            printf("%d. %s\n", i + 1, game->teams[i]);  
        }
        scanf("%d", &choice);
        strcpy(game->player2.team, game->teams[choice - 1]);
        screenClear();
    }
}


void showEndScreen(Game *game) {
    screenClear();
    printf("=========================================\n");
    printf("                FIM DO JOGO              \n");
    printf("=========================================\n");
    printf("Placar Final:\n");
    printf("%s: %d pontos\n", game->player1.team, game->player1.score);
    printf("%s: %d pontos\n", game->player2.team, game->player2.score);

    if (game->player1.score > game->player2.score) {
        printf("Vencedor: %s\n", game->player1.team);
    } else if (game->player1.score < game->player2.score) {
        printf("Vencedor: %s\n", game->player2.team);
    } else {
        printf("Empate!\n");
    }
        exit(0);
}


void initGame(Game *game) {
    game->timeRemaining = MAX_TIME;
    resetBall(&game->ball);

    game->player1.paddle_y = HEIGHT / 2;
    game->player1.score = 0;

    game->player2.paddle_y = HEIGHT / 2;
    game->player2.score = 0;

    game->teams[0] = "Sport";
    game->teams[1] = "Náutico";
    game->teams[2] = "Santa Cruz";
    game->teams[3] = "Retrô";
    game->teams[4] = "Afogados";
    game->teams[5] = "Central";
    game->teams[6] = "Decisão";
    game->teams[7] = "Jaguar";
    game->teams[8] = "Maguary";
    game->teams[9] = "Petrolina";

    chooseTeams(game);
}


int main() {
    Game game;
    initGame(&game);

    keyboardInit();
    screenInit(1);
    timerInit(75);

    while (1) {
        if (timerTimeOver()) {
            updateBall(&game);
            game.timeRemaining--;
            timerUpdateTimer(1000);
        }

        if (game.timeRemaining <= 0) {
            showEndScreen(&game);
        }

        drawGame(&game);

        int key = keyhit() ? readch() : 0;
        if (key == 'w' || key == 's') {
            movePaddle(&game.player1, key);
        }
        if (game.mode == 1) {
            movePaddleAI(&game);
        } else if (game.mode == 2 && (key == 'i' || key == 'k')) {
            movePaddle(&game.player2, key);
        }
    }

    timerDestroy();
    keyboardDestroy();
    screenDestroy();
    return 0;
}
