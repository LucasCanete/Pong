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



void racket_init(Racket *racket, Player player);


void racket_move(Racket *racket);


void racket_detect_collision(Racket *racket, Ball *ball);





#endif