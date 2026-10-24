#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "racket.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


//Padding of x
#define FIELD_X 10

//Padding of y
#define FIELD_Y 10

#define RACKET_MARGIN (2*FIELD_X)
#define RACKET_WIDTH 10

void game_update_score(Ball *ball,Racket *racket_1, Racket *racket_2 );

#endif