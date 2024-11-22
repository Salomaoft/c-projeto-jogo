#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <keyboard.h>
#include <screen.h>
#include <timer.h>

// Definições de constantes
#define WIDTH 40
#define HEIGHT 20
#define MAX_TIME 60
#define NUM_TEAMS 10

// Estruturas de dados
typedef struct {
    int x, y, dx, dy;
} Ball;

typedef struct {
    char team[20];
    int score;
    int paddle_y;
} Player;

typedef struct {
    int mode;       // 0 = contra máquina, 1 = dois jogadores
    int difficulty; // 1 = Fácil, 2 = Médio, 3 = Difícil
    Ball ball;
    Player player1;
    Player player2;
    char *teams[NUM_TEAMS];
    int timeRemaining;
} Game;


// Resetar a posição da bola
void resetBall(Ball *ball) {
    ball->x = WIDTH / 2;
    ball->y = HEIGHT / 2;
    ball->dx *= -1;
}

// Escolha de dificuldade
void chooseDifficulty(Game *game) {
    printf("Escolha a dificuldade:\n1. Fácil\n2. Médio\n3. Difícil\n");
    scanf("%d", &game->difficulty);
}

// Escolha de times e modo de jogo
void chooseTeams(Game *game) {
    screenClear();
    printf("Escolha o modo de jogo:\n");
    printf("1. Contra Máquina\n2. Dois Jogadores\n");
    scanf("%d", &game->mode);

    // Escolher time do jogador 1
    printf("Escolha seu time (1-10):\n");
    for (int i = 0; i < NUM_TEAMS; i++) {
        printf("%d. %s\n", i + 1, game->teams[i]);
    }
    int choice;
    scanf("%d", &choice);
    strcpy(game->player1.team, game->teams[choice - 1]);

    if (game->mode == 1) {
        strcpy(game->player2.team, "IA");
        chooseDifficulty(game);
    } else {
        printf("Jogador 2, escolha seu time (1-10):\n");
        scanf("%d", &choice);
        strcpy(game->player2.team, game->teams[choice - 1]);
    }
}

// Inicializa o jogo
void initGame(Game *game) {
    // Configuração inicial do jogo
    game->timeRemaining = MAX_TIME;
    game->ball.x = WIDTH / 2;
    game->ball.y = HEIGHT / 2;
    game->ball.dx = 1;
    game->ball.dy = 1;

    // Configuração dos jogadores
    game->player1.paddle_y = HEIGHT / 2;
    game->player1.score = 0;

    game->player2.paddle_y = HEIGHT / 2;
    game->player2.score = 0;

    // Times disponíveis
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

    // Escolher times e modo de jogo
    chooseTeams(game);
}


// Desenho do jogo
void drawGame(Game *game) {
    // Placar
    char score[50];
    snprintf(score, sizeof(score), "%s %d x %d %s",
             game->player1.team, game->player1.score,
             game->player2.score, game->player2.team);
    screenGotoxy(1, 1);
    printf("%s", score);

    // Tempo restante
    screenGotoxy(1, 2);
    printf("Tempo Restante: %d", game->timeRemaining);

    // Bordas
    screenBoxEnable();
    screenClear();

    // Bola
    screenGotoxy(game->ball.x, game->ball.y);
    printf("O");

    // Raquetes
    screenGotoxy(2, game->player1.paddle_y);
    printf("|");
    screenGotoxy(WIDTH - 3, game->player2.paddle_y);
    printf("|");
    screenUpdate();
}

// Atualização da bola
void updateBall(Game *game) {
    game->ball.x += game->ball.dx;
    game->ball.y += game->ball.dy;

    // Colisão com bordas
    if (game->ball.y <= 1 || game->ball.y >= HEIGHT - 1) {
        game->ball.dy *= -1;
    }

    // Colisão com as raquetes
    if ((game->ball.x == 3 && game->ball.y == game->player1.paddle_y) ||
        (game->ball.x == WIDTH - 4 && game->ball.y == game->player2.paddle_y)) {
        game->ball.dx *= -1;
    }

    // Pontuação
    if (game->ball.x <= 0) {
        game->player2.score++;
        resetBall(&game->ball);
    } else if (game->ball.x >= WIDTH) {
        game->player1.score++;
        resetBall(&game->ball);
    }
}

// Movimento da raquete do jogador
void movePaddle(Player *player, char input) {
    if (input == 'w' && player->paddle_y > 1) {
        player->paddle_y--;
    } else if (input == 's' && player->paddle_y < HEIGHT - 1) {
        player->paddle_y++;
    }
}

// Movimento da IA
void movePaddleAI(Game *game) {
    int target = game->ball.y;
    if (game->player2.paddle_y < target) {
        game->player2.paddle_y++;
    } else if (game->player2.paddle_y > target) {
        game->player2.paddle_y--;
    }
}

// Finalizar o jogo
void endGame(Game *game) {
    screenClear();
    printf("Fim de Jogo!\n");
    printf("Placar Final: %s %d x %d %s\n",
           game->player1.team, game->player1.score,
           game->player2.score, game->player2.team);
    screenUpdate();
}

int main() {
    Game game;
    initGame(&game);

    keyboardInit();
    screenInit(1);
    timerInit(1000); // Intervalo de 1 segundo

    while (game.timeRemaining > 0) {
        screenClear();
        drawGame(&game);

        // Processar entrada do jogador 1
        int key = keyhit() ? readch() : 0;
        if (key == 'w' || key == 's') {
            movePaddle(&game.player1, key);
        }

        // Processar entrada do jogador 2 ou IA
        if (game.mode == 1 && (key == 'i' || key == 'k')) {
            movePaddle(&game.player2, key);
        } else if (game.mode == 0) {
            movePaddleAI(&game);
        }

        // Atualizar bola e verificar condições
        updateBall(&game);

        // Atualizar tempo
        if (timerTimeOver()) {
            game.timeRemaining--;
            timerUpdateTimer(1000);
        }
    }

    endGame(&game);

    timerDestroy();
    keyboardDestroy();
    screenDestroy();
    return 0;
}

