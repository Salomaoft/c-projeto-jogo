# Jogo de Ping-Pong em C

Este projeto é uma implementação de um jogo de ping-pong em console, inspirado no clássico arcade "Pong". O jogo permite ao jogador escolher entre o modo multijogador (Player vs. Player) ou enfrentar a máquina (Player vs. CPU) com diferentes níveis de dificuldade.

## Como Funciona o Jogo

O objetivo do jogo é rebater a bola com a barra do jogador, impedindo que ela ultrapasse o limite da arena. Quando a bola passa pela barra de um jogador, o adversário ganha um ponto. O jogo termina quando um dos jogadores atinge a pontuação alvo (geralmente 2 pontos), determinando o vencedor.

O jogador pode escolher entre:

- *Modo Multijogador*: Dois jogadores competem entre si, cada um controlando uma barra para rebater a bola.
- *Modo Player vs. Máquina*: O jogador enfrenta a máquina, que controla a barra adversária. É possível escolher o nível de dificuldade, que ajusta a velocidade da bola e a capacidade de resposta da máquina.

## Ideia Principal

A ideia principal do jogo é proporcionar uma experiência de competição simples e divertida. Cada jogador controla uma barra que se movimenta verticalmente, e o objetivo é manter a bola em jogo, rebater o máximo possível e evitar que o adversário marque pontos.

## Níveis de Dificuldade

No modo contra a máquina, o jogador pode escolher entre três níveis de dificuldade:

1. *Fácil* - Velocidade da bola e da barra controlada pela máquina são reduzidas.
2. *Médio* - Velocidade moderada para um desafio equilibrado.
3. *Difícil* - Velocidade da bola e resposta da máquina aumentam, tornando o jogo mais desafiador.

Cada nível aumenta a dificuldade, ajustando a velocidade da bola e a capacidade de reação da barra controlada pela máquina.

## Implementação da Mecânica

### Funções Principais

1. *Função gotoxy*: Define a posição do cursor para imprimir elementos na tela do console.
2. *Função paredes*: Desenha as bordas da arena, criando os limites superior, inferior e laterais do jogo.
3. *Função menu*: Exibe o menu inicial com opções para o jogador escolher o modo de jogo e nível de dificuldade.
4. *Funções de Vitória (vence1, vence2, vencecomp)*: Exibem a tela de vitória e oferecem opções para voltar ao menu ou encerrar o jogo.
5. *Função multiplayer*: Gerencia o modo Player vs. Player, incluindo a movimentação das barras, controle de colisão da bola e contagem de pontos.
6. *Função contramaquina*: Implementa o modo Player vs. CPU, ajustando a velocidade e a resposta da barra da máquina com base no nível de dificuldade.

### Lógica do Jogo

- *Movimento da Bola*: A bola se move em uma direção (x, y) e rebate nas bordas laterais, superior e inferior da arena. Quando a bola ultrapassa a área da barra de um jogador, o adversário marca um ponto.
- *Movimento das Barras*: No modo multijogador, cada jogador controla sua barra com teclas diferentes (ex.: ‘w’ e ‘s’ para o jogador 1, e teclas direcionais para o jogador 2). No modo contra a máquina, a barra da CPU se movimenta automaticamente conforme o nível de dificuldade.
- *Pontuação e Condições de Vitória*: Cada jogador marca um ponto ao fazer com que a bola ultrapasse a área da barra do oponente. O primeiro a atingir o limite de pontos vence.