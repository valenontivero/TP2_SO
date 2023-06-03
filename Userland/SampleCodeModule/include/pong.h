#ifndef PONG_H
#define PONG_H

#define PLAYER1_UP 'w'
#define PLAYER1_UP2 'W'
#define PLAYER1_DOWN 's'
#define PLAYER1_DOWN2 'S'

#define PLAYER2_UP 17
#define PLAYER2_DOWN 20

#define PLAYER_WIDTH 15
#define PLAYER_HEIGHT 150

#define PLAYER1_MOVE_AMOUNT 30
#define PLAYER2_MOVE_AMOUNT 30

typedef struct Player {
    int x;
    int y;
    int score;
} Player;

void drawBall(int x, int y, int radius, int color);
void drawPaddle(int x, int y, int width, int height, int color);
void drawScore(int score, int color);
void win(int color, int player);
void pong();
int isValidKey(char c);
void moveBall(int * ballX, int * ballY, int ballRadius);
short tick() ;
void handleKey(char key, char moves[]) ;

void handleMovement(Player * player1, Player * player2, char moves[2]) ;

#endif