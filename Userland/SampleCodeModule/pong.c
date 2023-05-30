#include <usyscalls.h>
#include <colors.h>
#include <userio.h>

// Pong game

void drawBall(int x, int y, int radius, int color);
void drawPaddle(int x, int y, int width, int height, int color);
void drawScore(int score, int color);
void win(int color, int player);


void drawBall(int x, int y, int radius, int color) {
    int i, j;
    for (i = -radius; i <= radius; i++) {
        for (j = -radius; j <= radius; j++) {
            if (i*i + j*j <= radius*radius) {
                sys_draw_rectangle(x + i, y + j, 1, 1, WHITE);
            }
        }
    }
}

void drawPaddle(int x, int y, int width, int height, int color) {
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++){
            sys_draw_rectangle(x + i, y + j, 1, 1, color);
        }
    }
}

typedef struct Player {
    int x;
    int y;
    int score;
} Player;

#define PLAYER1_UP 'w'
#define PLAYER1_UP2 'W'
#define PLAYER1_DOWN 's'
#define PLAYER1_DOWN2 'S'

#define PLAYER2_UP 17
#define PLAYER2_DOWN 20

#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 150

int isValidKey(char c) {
    return c == PLAYER1_UP || c == PLAYER1_UP2 || c == PLAYER1_DOWN || c == PLAYER1_DOWN2 || c == PLAYER2_UP || c == PLAYER2_DOWN;
}

void handleMovement(Player * player1, Player * player2, char c) {
    // Erase player
    if (c == PLAYER1_UP || c == PLAYER1_UP2) {
        drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER_HEIGHT, BLACK);
        if (player1->y > 0) {
            player1->y -= PLAYER_WIDTH;
        }
        drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);
    } else if (c == PLAYER1_DOWN || c == PLAYER1_DOWN2) {
        drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER_HEIGHT, BLACK);
        if (player1->y < 600) {
            player1->y += PLAYER_WIDTH;
        }
        drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);
    } else if (c == PLAYER2_UP) {
        drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER_HEIGHT, BLACK);
        if (player2->y > 0) {
            player2->y -= PLAYER_WIDTH;
        }
        drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);
    } else if (c == PLAYER2_DOWN) {
        drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER_HEIGHT, BLACK);
        if (player2->y < 600) {
            player2->y += PLAYER_WIDTH;
        }
        drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);
    }
}

void pong() {
    sys_clear_screen();
    sys_toggle_cursor();

    uint16_t width;
    uint16_t height;
    sys_get_screen_size(&width, &height);

    // Draw middle line
    for (int i = 0; i < height; i++) {
        if (i % 10 == 0) {
            sys_draw_rectangle(width / 2, i, 1, 5, WHITE);
        }
    }

    // Draw ball
    int ballX = width / 2;
    int ballY = height / 2;
    int ballRadius = 10;
    drawBall(ballX, ballY, ballRadius, WHITE);

    Player player1 = {25, (height - PLAYER_HEIGHT) / 2, 0};
    Player player2 = {width - 25 - PLAYER_WIDTH, (height - PLAYER_HEIGHT) / 2, 0};

    // Draw player 1
    drawPaddle(player1.x, player1.y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);
    // Draw player 2
    drawPaddle(player2.x, player2.y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);

    while (1) {
        char c = getChar();
        if (isValidKey(c))
            handleMovement(&player1, &player2, c);
    }
}
