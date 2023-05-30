#include <usyscalls.h>
#include <colors.h>
#include <userio.h>

// Pong game

void drawBall(int x, int y, int radius, int color);
void drawPaddle(int x, int y, int width, int height, int color);
void drawScore(int score, int color);
void win(int color, int player);

void pong();

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

#define PLAYER1_MOVE_AMOUNT 30
#define PLAYER2_MOVE_AMOUNT 15

uint16_t width;
uint16_t height;

int ballXDirection = 1;
int ballYDirection = -1;

int ballRadius = 10;

int stop = 0;

void drawBall(int x, int y, int radius, int color) {
    int i, j;
    for (i = -radius; i <= radius; i++) {
        for (j = -radius; j <= radius; j++) {
            if (i*i + j*j <= radius*radius && x+i != 25 && x+i != width - 25 - PLAYER_WIDTH && x+i != width / 2) {
                sys_draw_rectangle(x + i, y + j, 1, 1, color);
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

int isValidKey(char c) {
    return c == PLAYER1_UP || c == PLAYER1_UP2 || c == PLAYER1_DOWN || c == PLAYER1_DOWN2 || c == PLAYER2_UP || c == PLAYER2_DOWN;
}

void handleMovement(Player * player1, Player * player2, char c) {
    // Erase player
    if (c == PLAYER1_UP || c == PLAYER1_UP2) {
        if (player1->y > 0) {
            drawPaddle(player1->x, player1->y + PLAYER_HEIGHT - PLAYER1_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, BLACK);
            player1->y -= PLAYER1_MOVE_AMOUNT;
            drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, WHITE);
        }
    } else if (c == PLAYER1_DOWN || c == PLAYER1_DOWN2) {
        if (player1->y < 600) {
            drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, BLACK);
            player1->y += PLAYER1_MOVE_AMOUNT;
            drawPaddle(player1->x, player1->y + PLAYER_HEIGHT - PLAYER1_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, WHITE);
        }
    } else if (c == PLAYER2_UP) {
        if (player2->y > 0) {
            drawPaddle(player2->x, player2->y + PLAYER_HEIGHT - PLAYER2_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, BLACK);
            player2->y -= PLAYER2_MOVE_AMOUNT;
            drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, WHITE);
        }
    } else if (c == PLAYER2_DOWN) {
        if (player2->y < 600) {
            drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, BLACK);
            player2->y += PLAYER2_MOVE_AMOUNT;
            drawPaddle(player2->x, player2->y + PLAYER_HEIGHT - PLAYER2_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, WHITE);
        }
    }
}

void moveBall(int * ballX, int * ballY, int ballRadius) {
    // Erase ball, but not players or middle line
    drawBall(*ballX, *ballY, ballRadius, BLACK);

    // Move ball
    *ballX += ballXDirection;
    *ballY += ballYDirection;

    // Check if ball is out of bounds
    if (*ballX < 0) {
        win(WHITE, 2);
    } else if (*ballX > width) {
        win(WHITE, 1);
    } else if (*ballY < 0) {
        ballYDirection = 1;
    } else if (*ballY > height) {
        ballYDirection = -1;
    }

    // Draw ball
    drawBall(*ballX, *ballY, ballRadius, WHITE);
}

void win(int color, int player) {
    sys_clear_screen();
    char * winner = "Player 1 wins!";
    if (player == 2) {
        winner = "Player 2 wins!";
    }
    sys_write_color(1, winner, 14, color);
    sys_write_color(1, "\n\nPress any key to continue...", 30, color);
    while (getChar() == 0xFF) {
        ;
    }
    sys_toggle_cursor();
    stop = 1;
}

void pong() {
    sys_clear_screen();
    sys_toggle_cursor();

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
    drawBall(ballX, ballY, ballRadius, WHITE);

    Player player1 = {25, (height - PLAYER_HEIGHT) / 2, 0};
    Player player2 = {width - 25 - PLAYER_WIDTH, (height - PLAYER_HEIGHT) / 2, 0};

    // Draw player 1
    drawPaddle(player1.x, player1.y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);
    // Draw player 2
    drawPaddle(player2.x, player2.y, PLAYER_WIDTH, PLAYER_HEIGHT, WHITE);


    while (!stop) {
        char c = getChar();
        if (isValidKey(c))
            handleMovement(&player1, &player2, c);
        moveBall(&ballX, &ballY, ballRadius);

        // Check if ball hit player
        if (ballX <= player1.x + PLAYER_WIDTH && ballX >= player1.x && ballY >= player1.y && ballY <= player1.y + PLAYER_HEIGHT) {
            ballXDirection = 1;
        } else if (ballX >= player2.x && ballX <= player2.x + PLAYER_WIDTH && ballY >= player2.y && ballY <= player2.y + PLAYER_HEIGHT) {
            ballXDirection = -1;
        }
    }
    stop = 0;
}
