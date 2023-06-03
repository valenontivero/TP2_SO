#include <usyscalls.h>
#include <colors.h>
#include <userio.h>
#include <pong.h>
#include <sounds.h>

uint16_t width;
uint16_t height;

int ballXDirection = 1;
int ballYDirection = -1;

int ballRadius = 10;

int goalMade = 0;

static int checkBorder(x, y, i){
    if(y+i > height/10+6 && y-i < height-16) {
        return 1;
    }
    return 0;
}

void drawBall(int x, int y, int radius, int color) {
    int i, j;
    for (i = -radius; i <= radius; i++) {
        for (j = -radius; j <= radius; j++) {
            if (i*i + j*j <= radius*radius && !(x+i >= 25 && x+i <= 25 + PLAYER_WIDTH) && !(x+i >= width - 25 - PLAYER_WIDTH && x+i <= width - 25 ) && x+i != width / 2 && checkBorder(x, y, i )) {
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

void handleMovement(Player * player1, Player * player2, char moves[2]) {
    for (int i = 0; i < 2 ; i++) {
        if (moves[i] == PLAYER1_UP) {
            if (player1->y > height/10) {
                drawPaddle(player1->x, player1->y + PLAYER_HEIGHT - PLAYER1_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, BLACK);
                player1->y -= PLAYER1_MOVE_AMOUNT;
                drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, WHITE);
            }
        } else if (moves[i] == PLAYER1_DOWN) {
            if (player1->y < 580) {
                drawPaddle(player1->x, player1->y, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, BLACK);
                player1->y += PLAYER1_MOVE_AMOUNT;
                drawPaddle(player1->x, player1->y + PLAYER_HEIGHT - PLAYER1_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER1_MOVE_AMOUNT, WHITE);
            }
        } else if (moves[i] == PLAYER2_UP) {
            if (player2->y > height/10) {
                drawPaddle(player2->x, player2->y + PLAYER_HEIGHT - PLAYER2_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, BLACK);
                player2->y -= PLAYER2_MOVE_AMOUNT;
                drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, WHITE);
            }
        } else if (moves[i] == PLAYER2_DOWN) {
            if (player2->y < 580) {
                drawPaddle(player2->x, player2->y, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, BLACK);
                player2->y += PLAYER2_MOVE_AMOUNT;
                drawPaddle(player2->x, player2->y + PLAYER_HEIGHT - PLAYER2_MOVE_AMOUNT, PLAYER_WIDTH, PLAYER2_MOVE_AMOUNT, WHITE);
            }
        }
    }
}

void handleGoal(int playerNumber, Player * player) {
    goalSound();
    if (playerNumber == 1 ) {
        player->score++;
        if (player->score == 3) {
            win(WHITE, 1);
        }
    } else if (playerNumber == 2) {
        player->score++;
        if (player->score == 3) {
            win(WHITE, 2);
        }
    }
    printf("\n\n\n\t\t\tPlayer %d scored!!\n", playerNumber);
    goalMade = 1;
}

void moveBall(int * ballX, int * ballY, int ballRadius, Player * player1, Player * player2) {
    // Erase ball, but not players or middle line
    drawBall(*ballX, *ballY, ballRadius, BLACK);

    // Move ball
    *ballX += ballXDirection * 10;
    *ballY += ballYDirection * 10;

    // Check if ball is out of bounds
    if (*ballX < 0) {
        handleGoal(2, player2);
        // win(WHITE, 2);
    } else if (*ballX > width) {
        handleGoal(1, player1);
        // win(WHITE, 1);
    } else if (*ballY < height/10) {
        ballYDirection = 1;
        wallSound();
    } else if (*ballY > height-20) {
        ballYDirection = -1;
        wallSound();
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
    goalMade = 1;
}

short tick() {
    static uint32_t lastTick = 0;
    uint32_t currentTicks;
    sys_get_ticks(&currentTicks);
    if (currentTicks != lastTick) {
        lastTick = currentTicks;
        return 1;
    }
    return 0;
}

void handleKey(char key, char moves[]) {
    if (key == PLAYER1_UP || key == PLAYER1_UP2) {
        moves[0] = PLAYER1_UP;
    } else if (key == PLAYER1_DOWN || key == PLAYER1_DOWN2) {
        moves[0] = PLAYER1_DOWN;
    } else if (key == PLAYER2_UP) {
        moves[1] = PLAYER2_UP;
    } else if (key == PLAYER2_DOWN) {
        moves[1] = PLAYER2_DOWN;
    }
}

void pong() {
    sys_clear_screen();
    sys_toggle_cursor();

    sys_get_screen_size(&width, &height);

    // Draw middle line, left and right line
    for (int i = height/12; i < height-10; i++) {
        sys_draw_rectangle(10, i, 5, 5, WHITE);
        sys_draw_rectangle(width-10, i, 5, 5, WHITE);
        if (i % 10 == 0) {
            sys_draw_rectangle(width / 2, i, 1, 5, WHITE);
        }
    }

    //Draw Top line and Bottom line
    for (int i = 10; i < width-10; i++) {
        sys_draw_rectangle(i, height-10, 1, 5, WHITE);
        sys_draw_rectangle(i, height/12, 1, 5, WHITE);
    }

    sys_write_color(1, "Player 1: W and S\nPlayer 2: Up and Down\n", 41, WHITE);
    sys_write_place(1, "Press any key to start...", 26, 50, 50);

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

    char moves[2] = { 0 };

    while (player1.score < 3 && player2.score < 3) {
        ballX = width / 2;
        ballY = height / 2;
        drawBall(ballX, ballY, ballRadius, WHITE);

        while (!goalMade) {
            char c = getChar();
            if (isValidKey(c))
                handleKey(c, moves);

            if (tick()) {
                handleMovement(&player1, &player2, moves);
                moveBall(&ballX, &ballY, ballRadius, &player1, &player2);

                // Check if ball hit player
                if (ballX - ballRadius/2 <= player1.x + PLAYER_WIDTH && ballX >= player1.x && ballY >= player1.y && ballY <= player1.y + PLAYER_HEIGHT) {
                    ballXDirection = 1;
                    paddleHitSound();
                } else if (ballX + ballRadius/2 >= player2.x && ballX <= player2.x + PLAYER_WIDTH && ballY >= player2.y && ballY <= player2.y + PLAYER_HEIGHT) {
                    ballXDirection = -1;
                    paddleHitSound();
                }
                moves[0] = 0; moves[1] = 0;
            }

            // If ESC pressed, exit
            if (c == 0x01) {
                return;
            }
            // If P pressed, pause
            if (c == 0x19) {
                sys_write_place(1, "PAUSED", 6, 50, 50);
                while (getChar() != 0x19) {
                    ;
                }
                sys_write_place(1, "      ", 6, 50, 50);
            }
        }
        goalMade = 0;
    }
}
