#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../cli-lib" // Biblioteca CLI-lib para manipulação no terminal

// Constantes
#define WIDTH 40
#define HEIGHT 20
#define MAX_TIME 60
#define NUM_TEAMS 10

// Estruturas
typedef struct {
    int x, y, dx, dy;
} Ball;

typedef struct {
    char name[50];
    char team[20];
    int score;
    int paddle_y;
} Player;

typedef struct {
    int mode;       // 0 = contra máquina, 1 = dois jogadores
    int difficulty; // 1 = Fácil, 2 = Médio, 3 = Difícil
    int time_remaining;
    Ball *ball;
    Player *player1;
    Player *player2;
    char *teams[NUM_TEAMS];
} Game;

// Protótipos de Funções
void init_game(Game *game);
void cleanup_game(Game *game);
void draw_game_area();
void draw(Game *game);
void update_ball(Game *game);
void move_paddle(Player *player, char input);
void move_paddle_ai(Game *game);
void menu(Game *game);
void choose_difficulty(Game *game);
int show_winner(Game *game);
void reset_game(Game *game);

// Função para inicializar o jogo
void init_game(Game *game) {
    game->ball = (Ball *)malloc(sizeof(Ball));
    game->player1 = (Player *)malloc(sizeof(Player));
    game->player2 = (Player *)malloc(sizeof(Player));

    // Inicializar times
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

    // Inicializar bola
    game->ball->x = WIDTH / 2;
    game->ball->y = HEIGHT / 2;
    game->ball->dx = 1;
    game->ball->dy = 1;

    // Inicializar jogadores
    game->player1->paddle_y = HEIGHT / 2;
    game->player1->score = 0;

    game->player2->paddle_y = HEIGHT / 2;
    game->player2->score = 0;

    game->time_remaining = MAX_TIME;
}

// Função para limpar a memória alocada
void cleanup_game(Game *game) {
    free(game->ball);
    free(game->player1);
    free(game->player2);
}

// Função para desenhar o campo de jogo
void draw_game_area() {
    cli_draw_border(0, 0, WIDTH, HEIGHT); // Bordas do campo
    cli_draw_line(WIDTH / 2, 0, WIDTH / 2, HEIGHT, '|'); // Linha central
}

// Função para desenhar o estado do jogo
void draw(Game *game) {
    cli_clear(); // Limpar o terminal
    cli_draw_text("CAMPEONATO PERNAMBUCANO DE PONG 2025", WIDTH / 4, 0);

    // Placar
    char score_text[100];
    snprintf(score_text, sizeof(score_text), "%s %d x %d %s",
             game->player1->team, game->player1->score,
             game->player2->score, game->player2->team);
    cli_draw_text(score_text, WIDTH / 4, 1);

    // Tempo restante
    char time_text[50];
    snprintf(time_text, sizeof(time_text), "Tempo Restante: %d segundos", game->time_remaining);
    cli_draw_text(time_text, WIDTH / 4, 2);

    draw_game_area();

    // Desenhar raquetes
    cli_draw_char(2, game->player1->paddle_y, '|');
    cli_draw_char(WIDTH - 3, game->player2->paddle_y, '|');

    // Desenhar bola
    cli_draw_char(game->ball->x, game->ball->y, 'O');
}

// Função para atualizar a posição da bola
void update_ball(Game *game) {
    game->ball->x += game->ball->dx;
    game->ball->y += game->ball->dy;

    // Colisões com bordas superiores e inferiores
    if (game->ball->y <= 0 || game->ball->y >= HEIGHT - 1) {
        game->ball->dy *= -1;
    }

    // Colisões com raquetes
    if (game->ball->x == 3 &&
        game->ball->y >= game->player1->paddle_y - 1 &&
        game->ball->y <= game->player1->paddle_y + 1) {
        game->ball->dx *= -1;
    } else if (game->ball->x == WIDTH - 4 &&
               game->ball->y >= game->player2->paddle_y - 1 &&
               game->ball->y <= game->player2->paddle_y + 1) {
        game->ball->dx *= -1;
    }

    // Pontuação
    if (game->ball->x <= 0) {
        game->player2->score++;
        reset_game(game);
    } else if (game->ball->x >= WIDTH - 1) {
        game->player1->score++;
        reset_game(game);
    }
}

// Função para mover a raquete de um jogador
void move_paddle(Player *player, char input) {
    if (input == 'w' && player->paddle_y > 1) {
        player->paddle_y--;
    } else if (input == 's' && player->paddle_y < HEIGHT - 2) {
        player->paddle_y++;
    }
}

// Função para movimentar a raquete do adversário (IA)
void move_paddle_ai(Game *game) {
    int target = game->ball->y;
    if (game->player2->paddle_y < target && game->player2->paddle_y < HEIGHT - 2) {
        game->player2->paddle_y++;
    } else if (game->player2->paddle_y > target && game->player2->paddle_y > 1) {
        game->player2->paddle_y--;
    }
}

// Menu principal do jogo
void menu(Game *game) {
    cli_clear();
    cli_draw_text("CAMPEONATO PERNAMBUCANO DE PONG 2025", WIDTH / 4, 2);
    cli_draw_text("1. Jogo Rápido (contra Máquina)", WIDTH / 4, 4);
    cli_draw_text("2. Dois Jogadores", WIDTH / 4, 5);
    game->mode = cli_get_option("Escolha uma opção: ", 1, 2);

    cli_draw_text("Escolha seu time:", WIDTH / 4, 7);
    for (int i = 0; i < NUM_TEAMS; i++) {
        char option[30];
        snprintf(option, sizeof(option), "%d. %s", i + 1, game->teams[i]);
        cli_draw_text(option, WIDTH / 4, 8 + i);
    }
    int choice = cli_get_option("Escolha um time (1-10): ", 1, 10);
    strcpy(game->player1->team, game->teams[choice - 1]);

    if (game->mode == 1) {
        strcpy(game->player2->team, "IA");
        choose_difficulty(game);
    } else {
        cli_draw_text("Jogador 2, escolha seu time:", WIDTH / 4, 18);
        for (int i = 0; i < NUM_TEAMS; i++) {
            char option[30];
            snprintf(option, sizeof(option), "%d. %s", i + 1, game->teams[i]);
            cli_draw_text(option, WIDTH / 4, 19 + i);
        }
        choice = cli_get_option("Escolha um time (1-10): ", 1, 10);
        strcpy(game->player2->team, game->teams[choice - 1]);
    }
}

// Função para escolher a dificuldade
void choose_difficulty(Game *game) {
    cli_draw_text("Escolha a dificuldade:", WIDTH / 4, 13);
    cli_draw_text("1. Fácil", WIDTH / 4, 14);
    cli_draw_text("2. Médio", WIDTH / 4, 15);
    cli_draw_text("3. Difícil", WIDTH / 4, 16);
    game->difficulty = cli_get_option("Escolha uma dificuldade (1-3): ", 1, 3);
}

// Função para resetar o jogo
void reset_game(Game *game) {
    game->ball->x = WIDTH / 2;
    game->ball->y = HEIGHT / 2;
    game->ball->dx *= -1;
    game->ball->dy *= -1;
}

// Função principal
int main() {
    Game game;
    init_game(&game);

    menu(&game);

    while (game.time_remaining > 0) {
        draw(&game);
        update_ball(&game);

        // Entrada do jogador
        char input = cli_get_key();
        move_paddle(game.player1, input);
        if (game.mode == 2) {
            move_paddle(game.player2, input);
        } else {
            move_paddle_ai(&game);
        }

        sleep(1);
        game.time_remaining--;
    }

    show_winner(&game);
    cleanup_game(&game);
    return 0;
}
