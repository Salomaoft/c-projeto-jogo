#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// Constantes
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define WIDTH 40
#define HEIGHT 20
#define MAX_TIME 60

// Protótipos de Funções
void gotoxy(int x, int y);
void clear_screen();
void draw_game_area(int offset_x, int offset_y);
void draw(int time_remaining, int offset_x, int offset_y);
void update_ball();
void move_paddle1(char input);
void move_paddle2(char input);
void move_paddle_ai();
void menu();
void choose_difficulty();
int show_winner();
int ask_for_rematch();

// Times do Campeonato Pernambucano
char *teams[] = {"Sport", "Nautico", "Santa Cruz", "Retro", "Afogados", "Central", "Decisao", "Jaguar", "Maguary", "Petrolina"};

// Variáveis globais
int ball_x, ball_y, ball_dx = 1, ball_dy = 1;
int paddle1_y = HEIGHT / 2, paddle2_y = HEIGHT / 2;
int player1_score = 0, player2_score = 0;
int mode = 0; // 0 = Jogo rápido contra a máquina, 1 = Dois jogadores
int difficulty = 1; // 1 = Fácil, 2 = Médio, 3 = Difícil
char *player1_team, *player2_team;
char player2_name[50];
int previous_winner = 0; // 1 = Player 1 venceu, 2 = Player 2 venceu, 0 = Empate
int replay = 0; // Controla o reinício do jogo com as mesmas configurações

// Função para posicionar o cursor no terminal
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Função para limpar a tela do console
void clear_screen() {
    system("cls");
}

// Função para desenhar o campo de jogo, delimitando suas bordas
void draw_game_area(int offset_x, int offset_y) {
    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(offset_x, offset_y + i);
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1) {
                printf("-"); // Limites superior e inferior
            } else if (j == 0 || j == WIDTH - 1) {
                printf("|"); // Limites laterais
            } else if (j == WIDTH / 2) {
                printf("|"); // Linha do meio do campo
            } else {
                printf(" ");
            }
        }
    }
}

// Função para exibir o placar e desenhar o campo de jogo
void draw(int time_remaining, int offset_x, int offset_y) {
    clear_screen();
    gotoxy((SCREEN_WIDTH - 36) / 2, offset_y - 3);
    printf("CAMPEONATO PERNAMBUCANO DE PONG 2025");
    gotoxy((SCREEN_WIDTH - 50) / 2, offset_y - 1);
    printf("%-15s - %2d  X  %2d - %-15s (%s)", player1_team, player1_score, player2_score, player2_name, player2_team);
    gotoxy((SCREEN_WIDTH - 28) / 2, offset_y);
    printf("TEMPO RESTANTE: %d SEGUNDOS", time_remaining);
    draw_game_area(offset_x, offset_y + 1);

    // Desenhar as raquetes e a bola
    gotoxy(offset_x + 2, offset_y + paddle1_y - 1);
    printf("|");
    gotoxy(offset_x + 2, offset_y + paddle1_y);
    printf("|");
    gotoxy(offset_x + 2, offset_y + paddle1_y + 1);
    printf("|");

    gotoxy(offset_x + WIDTH - 3, offset_y + paddle2_y - 1);
    printf("|");
    gotoxy(offset_x + WIDTH - 3, offset_y + paddle2_y);
    printf("|");
    gotoxy(offset_x + WIDTH - 3, offset_y + paddle2_y + 1);
    printf("|");

    gotoxy(offset_x + ball_x, offset_y + ball_y);
    printf("O");
}

// Função para atualizar a posição da bola e verificar colisões
void update_ball() {
    ball_x += ball_dx;
    ball_y += ball_dy;

    // Garantir que a bola fique dentro da área delimitada
    if (ball_y <= 1 || ball_y >= HEIGHT - 2) {
        ball_dy *= -1;
    }

    // Verificar colisão com as raquetes dos jogadores
    if (ball_x == 3 && ball_y >= paddle1_y - 1 && ball_y <= paddle1_y + 1) {
        ball_dx *= -1;
    } else if (ball_x == WIDTH - 4 && ball_y >= paddle2_y - 1 && ball_y <= paddle2_y + 1) {
        ball_dx *= -1;
    }

    // Verificar se a bola saiu pelas laterais (marcação de pontos)
    if (ball_x <= 1) { // Ponto para Player 2
        player2_score++;
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
        ball_dx = 1;
    } else if (ball_x >= WIDTH - 2) { // Ponto para Player 1
        player1_score++;
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
        ball_dx = -1;
    }
}

// Função para movimentar a raquete do jogador 1 (Player 1)
void move_paddle1(char input) {
    if (input == 'w' && paddle1_y > 2) {
        paddle1_y--;
    } else if (input == 's' && paddle1_y < HEIGHT - 3) {
        paddle1_y++;
    }
}

// Função para movimentar a raquete do jogador 2 (Player 2 manual, modo 2 jogadores)
void move_paddle2(char input) {
    if (input == 'o' && paddle2_y > 2) {
        paddle2_y--;
    } else if (input == 'l' && paddle2_y < HEIGHT - 3) {
        paddle2_y++;
    }
}

// Função para movimentar a raquete do jogador 2 (automático, modo contra a máquina)
void move_paddle_ai() {
    int target_y = ball_y; // A posição da bola define o alvo
    int step = 1; // Velocidade padrão da máquina

    if (difficulty == 2) { // Dificuldade média
        step = 1;
    } else if (difficulty == 3) { // Dificuldade difícil
        step = 2;
    }

    // Movimentar a raquete do Player 2 automaticamente para seguir a bola
    if (paddle2_y < target_y && paddle2_y < HEIGHT - 3) {
        paddle2_y += step;
    } else if (paddle2_y > target_y && paddle2_y > 2) {
        paddle2_y -= step;
    }
}

// Função para mostrar o menu principal do jogo
void menu() {
    clear_screen();
    gotoxy((SCREEN_WIDTH - 36) / 2, 3);
    printf("CAMPEONATO PERNAMBUCANO DE PONG 2025");
    gotoxy((SCREEN_WIDTH - 28) / 2, 5);
    printf("1. Jogo Rapido com a Maquina");
    gotoxy((SCREEN_WIDTH - 20) / 2, 6);
    printf("2. Jogo contra o Player");
    gotoxy((SCREEN_WIDTH - 8) / 2, 8);
    printf("Escolha: ");
    scanf("%d", &mode);

    clear_screen();
    gotoxy((SCREEN_WIDTH - 20) / 2, 5);
    printf("Escolha seu time:");
    for (int i = 0; i < 10; i++) {
        gotoxy((SCREEN_WIDTH - 20) / 2, 7 + i);
        printf("%d. %s", i + 1, teams[i]);
    }
    gotoxy((SCREEN_WIDTH - 8) / 2, 18);
    printf("Escolha (1-10): ");
    int choice;
    scanf("%d", &choice);
    player1_team = teams[choice - 1];
    clear_screen();

    if (mode == 1) {
        gotoxy((SCREEN_WIDTH - 28) / 2, 5);
        printf("Escolha o time da Maquina:");
        for (int i = 0; i < 10; i++) {
            gotoxy((SCREEN_WIDTH - 20) / 2, 7 + i);
            printf("%d. %s", i + 1, teams[i]);
        }
        gotoxy((SCREEN_WIDTH - 8) / 2, 18);
        printf("Escolha (1-10): ");
        scanf("%d", &choice);
        player2_team = teams[choice - 1];
        strcpy(player2_name, "Maquina");
        clear_screen();
        choose_difficulty(); // Chamar escolha de dificuldade
    } else {
        gotoxy((SCREEN_WIDTH - 28) / 2, 5);
        printf("Jogador 2, escolha seu time:");
        for (int i = 0; i < 10; i++) {
            gotoxy((SCREEN_WIDTH - 20) / 2, 7 + i);
            printf("%d. %s", i + 1, teams[i]);
        }
        gotoxy((SCREEN_WIDTH - 8) / 2, 18);
        printf("Escolha (1-10): ");
        scanf("%d", &choice);
        player2_team = teams[choice - 1];
        strcpy(player2_name, "Player 2");
        clear_screen();
    }
}

// Função para escolher dificuldade do jogo contra a máquina
void choose_difficulty() {
    gotoxy((SCREEN_WIDTH - 28) / 2, 5);
    printf("Escolha a dificuldade:");
    gotoxy((SCREEN_WIDTH - 20) / 2, 7);
    printf("1. Facil");
    gotoxy((SCREEN_WIDTH - 20) / 2, 8);
    printf("2. Medio");
    gotoxy((SCREEN_WIDTH - 20) / 2, 9);
    printf("3. Dificil");
    gotoxy((SCREEN_WIDTH - 8) / 2, 11);
    printf("Escolha: ");
    scanf("%d", &difficulty);
    clear_screen();
}

// Função para exibir o vencedor e pedir próxima ação
int show_winner() {
    clear_screen();
    gotoxy((SCREEN_WIDTH - 20) / 2, 5);
    printf("Resultado Final:");
    gotoxy((SCREEN_WIDTH - 40) / 2, 7);
    printf("%s - %d  x  %d - %s", player1_team, player1_score, player2_score, player2_team);

    if (player1_score > player2_score) {
        gotoxy((SCREEN_WIDTH - 30) / 2, 9);
        printf("Vencedor: Player 1 (%s)!", player1_team);
        previous_winner = 1;
    } else if (player2_score > player1_score) {
        gotoxy((SCREEN_WIDTH - 30) / 2, 9);
        printf("Vencedor: %s (%s)!", player2_name, player2_team);
        previous_winner = 2;
    } else {
        gotoxy((SCREEN_WIDTH - 15) / 2, 9);
        printf("Empate!");
        previous_winner = 0;
    }

    gotoxy((SCREEN_WIDTH - 30) / 2, 11);
    printf("1. Jogar novamente");
    gotoxy((SCREEN_WIDTH - 30) / 2, 12);
    printf("2. Voltar ao menu principal");
    gotoxy((SCREEN_WIDTH - 30) / 2, 13);
    printf("3. Encerrar o programa");
    gotoxy((SCREEN_WIDTH - 20) / 2, 15);
    printf("Escolha: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

// Função para perguntar sobre o reinício do jogo
int ask_for_rematch() {
    if (previous_winner == 2) {
        clear_screen();
        gotoxy((SCREEN_WIDTH - 50) / 2, 5);
        printf("Deseja perder novamente? HAHAHAHA");
    } else if (previous_winner == 1) {
        clear_screen();
        gotoxy((SCREEN_WIDTH - 30) / 2, 5);
        printf("Vamos para esse revanche!");
    } else {
        return 1; // Empate, sem provocação
    }

    gotoxy((SCREEN_WIDTH - 20) / 2, 7);
    printf("Sim (1) ou Nao (0): ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

// Função principal com loop corrigido
int main() {
    ball_x = WIDTH / 2;
    ball_y = HEIGHT / 2;

    while (1) {
        if (!replay) {
            menu();
        }

        int time_remaining = MAX_TIME;
        time_t start_time = time(NULL);
        int offset_x = (SCREEN_WIDTH - WIDTH) / 2;
        int offset_y = (SCREEN_HEIGHT - HEIGHT) / 2;

        while (time_remaining > 0) {
            draw(time_remaining, offset_x, offset_y);
            update_ball();

            // Movimentar a raquete do Player 1 conforme o usuário pressiona teclas
            if (_kbhit()) {
                char input = _getch();
                move_paddle1(input);
                if (mode == 2) {
                    move_paddle2(input);
                }
            }

            // Movimentar a máquina automaticamente no modo contra a máquina
            if (mode == 1) {
                move_paddle_ai();
            }

            Sleep(20);
            time_remaining = MAX_TIME - (time(NULL) - start_time);
        }

        int option = show_winner();
        if (option == 1) {
            player1_score = 0;
            player2_score = 0;
            if (!ask_for_rematch()) {
                replay = 0;
                continue;
            }
            replay = 1;
        } else if (option == 2) {
            replay = 0;
        } else if (option == 3) {
            break;
        }
    }

    return 0;
}
