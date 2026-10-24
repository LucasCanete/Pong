#ifndef RACKET_H
#define RACKET_H

#include <raylib.h>
#include "ball.h"


typedef enum {
    PLAYER_1,
    PLAYER_2,
} Player;

typedef struct {
    Vector2 position;
    int width;
    int height;
    float velocity;
    Player player;
    int score;
} Racket;


void racket_move(Racket *racket);

//Bug: When ball collides with upper or lower horizontal part of the racket then it either slides up or down lagging several times
void racket_detect_collision(Racket *racket, Ball *ball);



#endif